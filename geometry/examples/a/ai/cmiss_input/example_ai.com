gfx create material brain diffuse 1 0 0
gfx create material bone ambient 1 1 1 diffuse 0 1 0 emission 0 0 0 specular 0 0 0 alpha 0.55 shininess 0
gfx create material skin ambient 1 1 1 diffuse 0 0 1 emission 0 0 0 specular 0 0 0 alpha 0.31 shininess 0
gfx read node example head_geometry
gfx read elem example brain_geometry
gfx read elem example bone_geometry
gfx read elem example skin_geometry
gfx modify g_element brain general clear;
gfx modify g_element brain lines coordinate coordinates material default;
gfx modify g_element bone general clear;
gfx modify g_element bone lines coordinate coordinates material default;
gfx modify g_element skin general clear;
gfx modify g_element skin lines coordinate coordinates material default;
gfx modify g_element brain surfaces coordinate coordinates material brain;
gfx modify g_element bone surfaces coordinate coordinates material bone;
gfx modify g_element skin surfaces coordinate coordinates material skin;
# Following commmands obtained from gfx (after rotating image) using
# "gfx list window 1 commands"
gfx create window 1 double_buffer
gfx modify window 1 image scene default infinite_viewer_lighting two_sided_lighting;
gfx modify window 1 image add_light default;
gfx modify window 1 image add_light default_ambient;
gfx modify window 1 layout simple ortho_axes z -y eye_spacing 0.25 width 400 height 400
gfx modify window 1 set current_pane 1
gfx modify window 1 background colour 0 0 0
gfx modify window 1 view parallel eye_point 340.627 1641.13 3.64701 interest_point 182.683 218.039 129.194 up_vector -0.0108309 0.0890675 0.995967 view_angle 24.2458 near_clipping_plane 14.3732 far_clipping_plane 5136.5 relative_viewport ndc_placement -1 1 2 2 viewport_coordinates 0 0 1 1
gfx modify window 1 set transform current_pane 1 std_view_angle 40 normal_lines no_antialias slow_transparency




