#ifndef _CALIBRATION_CLASS_H
#define _CALIBRATION_CLASS_H

#include <stdio.h>

#include <iostream>
#include <vector>

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
#include "zinc/fieldmatrixoperators.hpp"
#include "zinc/fieldfiniteelement.hpp"
#include "zinc/fieldsubobjectgroup.hpp"
#include "zinc/fieldgroup.hpp"
#include "zinc/scenepicker.hpp"

#include "zinc/stream.h"
using namespace OpenCMISS::Zinc;
class zinc_wrapper
{
public:

	zinc_wrapper();
	zinc_wrapper(std::string context_name);

	//Read a file with .exfile extension, and add to context
	void read_exfile(std::string file_name);

	//Set sceneviewer view port size
	void set_scene_viewer_size(int width, int height);

	//Add a geometry to scene
	//Variable geometry_name is the name of the object from exfile
	//Variable coordinate name is the name of the coordinate
	void add_geometry_to_scene(std::string geometry_name,std::string coordinate_name);

	//Sets and renders scene
	void render_scene(bool view_all);

	//Rotates the geometry 

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
};





#endif