#ifndef _CALIBRATION_CLASS_H
#define _CALIBRATION_CLASS_H

#include <stdio.h>

#include <iostream>
#include <vector>
#include <string>

#include "opencmiss/zinc/context.hpp"
#include "opencmiss/zinc/element.hpp"
#include "opencmiss/zinc/field.hpp"
#include "opencmiss/zinc/fieldcache.hpp"
#include "opencmiss/zinc/fieldmodule.hpp"
#include "opencmiss/zinc/fieldimage.hpp"

#include <opencmiss/zinc/fieldvectoroperators.hpp>
#include "opencmiss/zinc/region.hpp"
#include "opencmiss/zinc/sceneviewer.hpp"
#include "opencmiss/zinc/scene.hpp"
#include "opencmiss/zinc/streamimage.hpp"
#include "opencmiss/zinc/streamscene.hpp"
#include "opencmiss/zinc/fieldconstant.hpp"
#include "opencmiss/zinc/fieldlogicaloperators.hpp"
#include "opencmiss/zinc/fieldmatrixoperators.hpp"
#include "opencmiss/zinc/fieldmeshoperators.hpp"
#include "opencmiss/zinc/fieldfiniteelement.hpp"
#include "opencmiss/zinc/fieldsubobjectgroup.hpp"
#include "opencmiss/zinc/fieldgroup.hpp"
#include "opencmiss/zinc/scenepicker.hpp"
#include "opencmiss/zinc/fieldarithmeticoperators.hpp"
#include "opencmiss/zinc/optimisation.hpp"
#include "opencmiss/zinc/fieldnodesetoperators.hpp"
#include "opencmiss/zinc/fieldderivatives.hpp"
#include "opencmiss/zinc/fieldcomposite.hpp"
#include "opencmiss/zinc/stream.h"
using namespace OpenCMISS::Zinc;


class zinc_wrapper
{
public:
	

	//Common fields
	struct _common_fields_
	{
		//Common fields
		Field zero;
		Field one;
		Field negative_one;
		Field identity_3x3;//TODO
	} common_fields;
	
	//Common colours
	struct _common_materials_
	{

		Material blue;
		Material red;
		Material green;
		Material magenta;
		Material purple;
		Material gold;
	} common_materials;

	
	//Constructor
	zinc_wrapper();
	zinc_wrapper(std::string context_name);

	//Initialize some common fields
	void initialize_common_fields();

	//Initialize some common materials
	void initialize_common_materials();

	//Read a file with .exfile extension, and add to context
	void read_exfile(std::string file_name);

	//Read geometry with .exnode and .exelem files
	void read_exnode_exelem(std::string file_name);

	//Read .exelem file
	void read_exelement(std::string file_name);

	//read data
	void read_exdata(std::string file_name);

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
	
	//Add data to scene
	void add_data_to_scene(std::string geometry_name, std::string coordinate_name, double alpha_, std::string material, float node_size);

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
	//TODO: MAKE MODULAR
	void mesh_integrator(std::string mesh_name);

	//set viewer to 2d 
	void set_viewer_2d(double up_vector[3]){ sceneviewer->setUpVector( up_vector); }

	//Optimise 1d
	//TODO: MAKE MODULAR
	void optimise_1d();

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
