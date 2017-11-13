#ifndef _CALIBRATION_CLASS_H
#define _CALIBRATION_CLASS_H

#include <stdio.h>

#include <iostream>
#include <vector>
#include <string>

#include "zinc/context.hpp"
#include "zinc/element.hpp"
#include "zinc/field.hpp"
#include "zinc/fieldcache.hpp"
#include "zinc/fieldmodule.hpp"
#include "zinc/fieldimage.hpp"

#include <zinc/fieldvectoroperators.hpp>
#include "zinc/region.hpp"
#include "zinc/sceneviewer.hpp"
#include "zinc/scene.hpp"
#include "zinc/streamimage.hpp"
#include "zinc/streamscene.hpp"
#include "zinc/fieldconstant.hpp"
#include "zinc/fieldlogicaloperators.hpp"
#include "zinc/fieldmatrixoperators.hpp"
#include "zinc/fieldmeshoperators.hpp"
#include "zinc/fieldfiniteelement.hpp"
#include "zinc/fieldsubobjectgroup.hpp"
#include "zinc/fieldgroup.hpp"
#include "zinc/scenepicker.hpp"

#include "zinc/stream.h"
using namespace OpenCMISS::Zinc;
class zinc_wrapper
{
public:
	enum material_wrapper{
		TRANSPARENT_BLUE
	};
	
	//Constructor
	zinc_wrapper();
	zinc_wrapper(std::string context_name);

	//Read a file with .exfile extension, and add to context
	void read_exfile(std::string file_name);

	//Read geometry with .exnode and .exelem files
	void read_exnode_exelem(std::string file_name);

	//Read .exelem file
	void read_exelement(std::string file_name);

	//Set sceneviewer view port size
	void set_scene_viewer_size(int width, int height);

	//Add a geometry to scene
	//Variable geometry_name is the name of the object from exfile
	//Variable coordinate name is the name of the coordinate
	void add_surface_to_scene(std::string geometry_name, std::string coordinate_name, double alpha_,std::string material);

	//Add line to scene
	void add_line_to_scene(std::string geometry_name, std::string coordinate_name, double alpha_, std::string material,float line_width);

	//Add node to scene
	void add_node_to_scene(std::string geometry_name, std::string coordinate_name, double alpha_, std::string material, float node_size);

	//Sets and renders scene
	void render_scene(bool view_all);

	//Rotates and translates a given geometry
	void rotate_translation_geometry(std::string geometry_name, double R[3][3], double T[3], bool update);

	//set background colour
	void set_background_colour(double r, double b, double g);

	void create_material();//TODO


	//sets the window attributes, more can be added
	void set_window_attributes(double eye_point[3],double look_at[3], double up_vector[3],double view_angle);

	//Integrator
	void mesh_integrator(std::string mesh_name);

	//Destructor
	~zinc_wrapper();

private:

	//Class variables pointers
	Context *context;
	Region *region;
	Fieldmodule *fieldmodule;
	Scene *scene;
	Sceneviewermodule *sceneviewermodule;
	Sceneviewer *sceneviewer;
	Scenepicker *scenepicker;

	//Testing var
	Field global_coordinates;

	//Width and height of the iamge
	float width, height;
};





#endif