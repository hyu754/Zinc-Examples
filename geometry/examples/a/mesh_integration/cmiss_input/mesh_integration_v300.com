# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at http://mozilla.org/MPL/2.0/.
#
# Cmgui example: Mesh integration
#
# Calculate volume and surface area of a mesh using Gaussian quadrature.
#
# Old example for Cmgui v3.0.0, which integrates using Gauss point
# generation, and lists outside faces explicitly.
# Note it can be converted to run on Cmgui 2.8 and 2.9 by changing all
# occurrences of the text:
# mesh3d -> cmiss_mesh_3d
# mesh2d -> cmiss_mesh_2d
# datapoints -> cmiss_data
#
# Read mesh containing all possible 3-D element shapes
gfx read nodes $example/allshapes.exnode;
gfx read elements $example/allshapes.exelem;
#
# Create an element group containing just the exterior faces, and clear graphics for it
gfx create egroup exterior region allshapes;
gfx modify egroup allshapes/exterior add faces 2,4..5,8..10,12..14,16..24;
gfx modify g_element allshapes/exterior general clear;
#
# create order 2 Gauss points on both the volume and exterior surface to integrate with
gfx define field allshapes/gauss_location finite_element number_of_components 1 field element_xi;
gfx define field allshapes/gauss_weight finite_element number_of_components 1 field real;
gfx create dgroup gausspoints region allshapes;
gfx create gauss_points region allshapes mesh mesh3d order 2 gauss_location_field gauss_location gauss_point_nodeset gausspoints.datapoints gauss_weight_field gauss_weight first_identifier 1;
gfx create dgroup surface_gausspoints region allshapes;
gfx create gauss_points region allshapes mesh exterior.mesh2d order 2 gauss_location_field gauss_location gauss_point_nodeset surface_gausspoints.datapoints gauss_weight_field gauss_weight first_identifier 10001;
#
# get derivatives of coordinate field w.r.t. element chart 'xi'
# (Be aware that the 'xi' is for the element you evaluate it at, so differs for 3-D and 2-D)
gfx define field allshapes/dX_dxi1 basis_derivative fe_field coordinates order 1 xi_indices 1;
gfx define field allshapes/dX_dxi2 basis_derivative fe_field coordinates order 1 xi_indices 2;
gfx define field allshapes/dX_dxi3 basis_derivative fe_field coordinates order 1 xi_indices 3;
# pack these into a 3x3 matrix, and get differential volume dV from its determinant, in 3-D elements
gfx define field allshapes/dX_dxi composite dX_dxi1 dX_dxi2 dX_dxi3;
gfx define field allshapes/dV determinant field dX_dxi;
# magnitude of dX_dxi1 (x) dX_dxi2 gives dA when evaluated on 2-D elements
gfx define field allshapes/dX_dxi_cross cross_product fields dX_dxi1 dX_dxi2;
gfx define field allshapes/dA magnitude field dX_dxi_cross;
#
# obtain fields at gauss points:
gfx define field allshapes/gauss_coordinates embedded element_xi gauss_location field coordinates;
gfx define field allshapes/gauss_dV embedded element_xi gauss_location field dV;
gfx define field allshapes/gauss_dA embedded element_xi gauss_location field dA;
#
# sum weighted dV over gauss points
gfx define field allshapes/weighted_gauss_dV multiply fields gauss_weight gauss_dV;
gfx define field allshapes/volume nodeset_sum field weighted_gauss_dV nodeset gausspoints.datapoints;
#
# sum weighted dA over surface gauss points
gfx define field allshapes/weighted_gauss_dA multiply fields gauss_weight gauss_dA;
gfx define field allshapes/surface_area nodeset_sum field weighted_gauss_dA nodeset surface_gausspoints.datapoints;
#
# visualise mesh and both sets of Gauss points
gfx modify g_element allshapes general clear;
gfx modify g_element allshapes/gausspoints general clear;
gfx modify g_element allshapes/gausspoints data_points coordinate gauss_coordinates LOCAL glyph cross general size "0.05*0.05*0.05" centre 0,0,0 font default select_on material silver selected_material default_selected;
gfx modify g_element allshapes/surface_gausspoints general clear;
gfx modify g_element allshapes/surface_gausspoints data_points coordinate gauss_coordinates LOCAL glyph cross general size "0.05*0.05*0.05" centre 0,0,0 font default select_on material green selected_material default_selected;
gfx modify g_element allshapes lines coordinate coordinates tessellation default LOCAL native_discretization NONE select_on material default selected_material default_selected;
gfx modify g_element allshapes node_points coordinate coordinates LOCAL glyph sphere general size "0.07*0.07*0.07" centre 0,0,0 font default select_on material gold selected_material default_selected;
#
# show mesh volume in silver in the window corner 
gfx modify g_element allshapes point NORMALISED_WINDOW_FIT_LEFT glyph none general size "1*1*1" centre 0.95,-0.9,0 font default label volume select_on material silver selected_material default_selected;
# show mesh surface area in silver in the window corner
gfx modify g_element allshapes point NORMALISED_WINDOW_FIT_LEFT glyph none general size "1*1*1" centre 0.95,-0.8,0 font default label surface_area select_on material green selected_material default_selected;

if (!$TESTING) {
	gfx create window 1;
}

# Note if you enable the 'edit' option for the node tool in the graphics window,
# and move nodes, the volume and surface area are dynamically recalculated.

# Clear all the gauss points and recreate for order 3 quadrature:
gfx destroy data all group allshapes;
gfx create gauss_points region allshapes mesh mesh3d order 3 gauss_location_field gauss_location gauss_point_nodeset gausspoints.datapoints gauss_weight_field gauss_weight first_identifier 1;
gfx create gauss_points region allshapes mesh exterior.mesh2d order 3 gauss_location_field gauss_location gauss_point_nodeset surface_gausspoints.datapoints gauss_weight_field gauss_weight first_identifier 10001;

# It is instructive to inspect the fields that are summed here.
# Show field gauss_weight, gauss_location, gauss_dA and gauss_dV as labels on the data_points
# Also try editing the nodes using the node tool in the graphics window
# to see the volume and area change

if ($TESTING) {
    # Test all quadrature orders produce the same volume and area, including when coordinates scaled
    my $scale;
    for ($scale = 1; $scale <= 2; $scale++) {
        my $prefix = "";
        if ($scale == 2) {
            $prefix = "scaled_";
            gfx define field allshapes/scaled_coordinates scale field coordinates scale_factors 1.5 0.75 2.0;
            gfx evaluate ngroup allshapes source scaled_coordinates destination coordinates;
        }
        my $order;
        for ($order = 1; $order <= 4; $order++) {
            gfx destroy data all group allshapes;
            gfx create gauss_points region allshapes mesh mesh3d order $order gauss_location_field gauss_location gauss_point_nodeset gausspoints.datapoints gauss_weight_field gauss_weight first_identifier 1;
            gfx create gauss_points region allshapes mesh exterior.mesh2d order $order gauss_location_field gauss_location gauss_point_nodeset surface_gausspoints.datapoints gauss_weight_field gauss_weight first_identifier 10001;

            # for testing answers use trick of evaluating into constant field, wastefully for all nodes
            my $volume_name = $prefix . "volume_order$order";
            gfx define field "allshapes/$volume_name" constant 1;
            gfx evaluate ngroup allshapes source volume destination $volume_name;
            gfx list field "allshapes/$volume_name" commands;

            my $area_name = $prefix . "area_order$order";
            gfx define field "allshapes/$area_name" constant 1;
            gfx evaluate ngroup allshapes source surface_area destination $area_name;
            gfx list field "allshapes/$area_name" commands;
        }
    }
    # restore original nodal coordinates
    gfx read nodes $example/allshapes.exnode;
}

