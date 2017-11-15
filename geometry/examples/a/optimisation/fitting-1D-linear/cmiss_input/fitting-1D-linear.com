if (! defined $example) {
	$example = ".";
}

# read in the data to be fit to
gfx read data "$example/data.exnode";
# and the initial mesh
gfx read nodes "$example/mesh_upgrade.exnode";
gfx read nodes "$example/mesh_upgrade.exelem";

# draw the mesh
gfx modify g_element "/" general clear;
gfx modify g_element "/" lines tessellation default select_on material default selected_material default_selected;
gfx modify g_element "/" node_points glyph sphere general size "0.1*0.1*0.1" centre 0,0,0 font default select_on material green selected_material default_selected;
# and the data points
gfx modify g_element "/" data_points coordinate data_coordinates glyph cross general size "0.1*0.1*0.1" centre 0,0,0 font default select_on material gold selected_material default_selected;
gfx create window;
gfx modify window 1 layout 2d ortho_axes -y -z; # 2-D view
gfx modify window 1 node_tool edit; # enable node editing for trying new initial positions
gfx modify window 1 set node_tool; # switch to node tool for node editing

# create a field which dynamically finds the nearest location to the data_coordinates on the 1-D mesh
gfx define field found_location find_mesh_location find_nearest mesh mesh1d mesh_field coordinates source_field data_coordinates;
# Use of this field as part of the objective means the nearest mesh location is updated during
# optimisation which can be slow for big problems (which this is not), and is susceptible to
# diverging from bad initial positions without addition of some sort of smoothing factors. Refer to
# other optimisation examples e.g. fitting-2D-linear, smooth_volume_fitting for fitting with fixed
# data point projections, or try the 'variant' commands listed later. Example smooth_volume_fitting
# demonstrates how smoothing terms can be added to improve stability, which could be applied to
# this problem e.g. to limit allowable strains in each element during optimisation.

# define a field giving the coordinates at the found_location on the mesh:
gfx define field projected_coordinates embedded element_xi found_location field coordinates;
# define a field which calculates the error vector between the data point and its element project:
gfx define field error_vector add fields data_coordinates projected_coordinates scale_factors 1 -1;
# get the magnitude of that vector as a measure of error for each data point:
gfx define field error magnitude field error_vector;

if (0) {
	# variant: store fixed embedding locations so not recalculate during optimisation
	# create and define stored location field at the data points:
	gfx define field stored_location finite_element element_xi;
	gfx modify data all define stored_location;
	# evaluate the current found_location into the stored_location:
	gfx evaluate dgroup "/" destination stored_location source found_location;
	# change the projections to use the stored_location 
	gfx define field projected_coordinates embedded element_xi stored_location field coordinates;
}

#visualise the error bars:
gfx modify g_element "/" data_points coordinate data_coordinates LOCAL glyph line general size "0*0.1*0.1" centre 0,0,0 font default orientation error_vector scale_factors "-1*0*0" select_on material silver selected_material default_selected;

# define a scalar objective function to minimise which is the sum of each of the error terms over the data nodeset:
gfx define field objective_function nodeset_sum field error nodeset datapoints;

# define objective function for least-squares method as sum of squared error_vector over the data points
# (least-squares method works directly with individual terms from sum-of-squares field types like this)
gfx define field objective_function_lsq nodeset_sum_squares field error_vector nodeset datapoints;

# use the quasi-Newton Opt++ method to minimise the objective function field.
# [this method minimises a scalar valued field by altering the provided independent field(s)]
gfx minimise QUASI_NEWTON region "/" objective_fields objective_function independent_fields coordinates maximum_iterations 1;



#display tangent and normal vectors
gfx def field d_ds1 node_value fe_field coordinates d/ds1
gfx def field d_ds1_normal normal field d_ds1

# normal.y = 0
gfx define field n1 constant 1
gfx define field n2 constant 0

gfx define field v1_positive component d_ds1.x
gfx define field negative constant -1
gfx define field v1 multiply field negative v1_positive
gfx define field v3 component d_ds1.z

gfx define field n3 divide fields v1 v3
gfx define field normal composite n1 n2 n3
gfx define field normalnormal normal field normal


gfx modify g_element "/" general clear;
gfx modify g_element "/" lines domain_mesh1d coordinate coordinates tessellation default LOCAL line line_base_size 0 select_on material default selected_material default_selected render_shaded;
gfx modify g_element "/" points domain_nodes coordinate coordinates tessellation default_points LOCAL glyph arrow_solid size "0*0*0" offset 0,0,0 font default orientation normalnormal scale_factors "0.5*0.5*0.5" select_on material green selected_material default_selected render_shaded;
gfx modify g_element "/" points domain_datapoints coordinate data_coordinates tessellation default_points LOCAL glyph cross size "0.1*0.1*0.1" offset 0,0,0 font default select_on material gold selected_material default_selected render_shaded;
gfx modify g_element "/" points domain_datapoints coordinate data_coordinates tessellation default_points LOCAL glyph line size "0*0.1*0.1" offset 0,0,0 font default orientation error_vector scale_factors "-1*0*0" select_on material silver selected_material default_selected render_shaded;
gfx modify g_element "/" points domain_nodes coordinate coordinates tessellation default_points LOCAL glyph arrow_solid size "0*0*0" offset 0,0,0 font default orientation d_ds1_normal scale_factors "0.5*0.5*0.5" select_on material orange selected_material default_selected render_shaded;


if ($TESTING) {
   # write out the fitted nodes for testing
   gfx write node "fitted-qn.exnode"
   
	# perform the same fit using alternative methods
	
	# reset nodes to original locations
	gfx read node "$example/mesh.exnode";

	# Perform least-squares optimisation
	gfx minimise LEAST_SQUARES_QUASI_NEWTON region "/" objective_fields objective_function_lsq independent_fields coordinates hide_output;
	
   # write out the fitted nodes for testing
   gfx write node "fitted-lsq.exnode"

}
