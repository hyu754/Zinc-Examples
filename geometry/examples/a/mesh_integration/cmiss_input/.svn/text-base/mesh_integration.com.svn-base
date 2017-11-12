# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at http://mozilla.org/MPL/2.0/.
#
# Cmgui example: Mesh integration
#
# Calculate volume and surface area of a mesh using Gaussian quadrature.
#
# Note this example only runs on latest Cmgui developer releases v3.0.1
# since it relies on new field types: mesh_integral, is_exterior.
# For Cmgui v3.0 use the mesh_integration_v300.com, which integrates
# using generated Gauss point, and lists exterior faces explicitly.
#
# Read mesh containing all possible 3-D element shapes
gfx read nodes $example/allshapes.exnode;
gfx read elements $example/allshapes.exelem;
#
# Create an element group containing just the exterior faces (for surface area calculation)
gfx create egroup exterior region allshapes;
gfx define field allshapes/is_exterior is_exterior;
gfx modify egroup allshapes/exterior add faces conditional is_exterior;
gfx list faces region allshapes conditional exterior;
#
# Integrate constant 1.0 over 3-D or 2-D mesh to get result:
gfx define field allshapes/one constant 1.0;
gfx define field allshapes/volume mesh_integral coordinate_field coordinates integrand_field one mesh mesh3d gaussian_quadrature numbers_of_points "4";
gfx define field allshapes/surface_area mesh_integral coordinate_field coordinates integrand_field one mesh exterior.mesh2d gaussian_quadrature numbers_of_points "4";
#
# visualise mesh and both sets of Gauss points
gfx modify g_element allshapes lines coordinate coordinates tessellation default LOCAL native_discretization NONE select_on material default selected_material default_selected;
gfx modify g_element allshapes node_points coordinate coordinates LOCAL glyph sphere general size "0.07*0.07*0.07" centre 0,0,0 font default select_on material gold selected_material default_selected;

# show mesh volume in silver in the window corner 
gfx modify g_element allshapes point NORMALISED_WINDOW_FIT_LEFT glyph none general size "1*1*1" centre 0.95,-0.9,0 font default label_text "Volume: " label volume select_on material silver selected_material default_selected;
# show mesh surface area in green in the window corner
gfx modify g_element allshapes point NORMALISED_WINDOW_FIT_LEFT glyph none general size "1*1*1" centre 0.95,-0.8,0 font default label_text "Surface Area: " label surface_area select_on material green selected_material default_selected;

if (!$TESTING) {
	  gfx create window 1;
}

# Note if you enable the 'edit' option for the node tool in the graphics window,
# and move nodes, the volume and surface area are dynamically recalculated.

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
            gfx define field allshapes/volume mesh_integral coordinate_field coordinates integrand_field one mesh mesh3d gaussian_quadrature numbers_of_points "$order";
            gfx define field allshapes/surface_area mesh_integral coordinate_field coordinates integrand_field one mesh exterior.mesh2d gaussian_quadrature numbers_of_points "$order";

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
