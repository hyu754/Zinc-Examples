
#include "zinc_wrapper.hpp"


//Zinc wrapper constructor
zinc_wrapper::zinc_wrapper()
{
	Context context_("Default");
	//TO DO :
	//NEED TO ADD CONSTRUCTOR
	//
	std::cout << "Zinc wrapper class initialized" << std::endl;
}

//Zinc wrapper constructor with context name
zinc_wrapper::zinc_wrapper(std::string context_name)
{
	//change context_name to char
	const char *temp_char = context_name.c_str();

	//Create a context
	context = new Context(temp_char);

	//Create a region
	region = new Region;
	*region=context->getDefaultRegion();

	//Create a fieldmodule
	fieldmodule = new Fieldmodule;
	*fieldmodule = region->getFieldmodule();

	//Create a scene
	scene = new Scene;
	*scene = region->getScene();

	//Create sceneviewermodule
	sceneviewermodule = new Sceneviewermodule;
	*sceneviewermodule = context->getSceneviewermodule();

	//Create a sceneviewer
	sceneviewer = new Sceneviewer;
	*sceneviewer = sceneviewermodule->createSceneviewer(OpenCMISS::Zinc::Sceneviewer::BufferingMode::BUFFERING_MODE_DEFAULT,OpenCMISS::Zinc::Sceneviewer::StereoMode::STEREO_MODE_MONO);
	
	//Create a scenepicker
	scenepicker = new Scenepicker;
	*scenepicker = scene->createScenepicker();
	

	std::cout << "Zinc wrapper class initialized" << std::endl;
}

//Zinc wrapper destructor
zinc_wrapper::~zinc_wrapper()
{
	std::cout << "Zinc wrapper class destroyed" << std::endl;
	//Destroy memory
	delete context;
	delete region;
	delete fieldmodule;
	delete scene;
	delete sceneviewermodule;
	delete sceneviewer;
}


//Read a file with .exfile extension, and add to context
void zinc_wrapper::read_exfile(std::string file_name){
	const char *file_name_char= file_name.c_str();
	this->region->readFile(file_name_char);
	/* TESTING
	Fielditerator field_it = fieldmodule->createFielditerator();
	Field field_temp=field_it.next();
	while (field_temp.isValid()){
		std::cout << field_temp.getName() << std::endl;
		std::cout << field_temp.getValueType() << std::endl;

		//field_temp.getFieldmodule;
		field_temp = field_it.next();
	}
	*/
}

//Set sceneviewer view port size
void zinc_wrapper::set_scene_viewer_size(int width, int height){
	sceneviewer->setViewportSize(width, height);

}

//Add a geometry to scene
void zinc_wrapper::add_geometry_to_scene(std::string geometry_name,std::string coordinate_name){
	const char * geometry_name_char = geometry_name.c_str();
	const char * coordinate_name_char = coordinate_name.c_str();
	Field geo_field = fieldmodule->findFieldByName(geometry_name_char);
	Field coordinates = fieldmodule->findFieldByName(coordinate_name_char);

	//Create a surface 
	GraphicsSurfaces surface = scene->createGraphicsSurfaces();
	
	//Assign the geometry field to the graphics surface
	surface.setSubgroupField(geo_field);

	//Assign the coordinate to the graphics surface
	surface.setCoordinateField(coordinates);

	//Set the name to of the graphics surface, this will be set to be geometry_name_surface
	geometry_name = geometry_name + "surface";
	const char * surface_name = geometry_name.c_str();
	surface.setName(surface_name);

	//Create material module
	Materialmodule materialmodule = scene->getMaterialmodule();

	Material red = materialmodule.findMaterialByName("red");
	if (!surface.setMaterial(red)){
		std::cout << "Surface colour is not set " << std::endl;
	}
	
	//surface.setVisibilityFlag(false);
	
	
}


//Sets and renders scene
void zinc_wrapper::render_scene(bool view_all){
	//Sets the scene to the viewer	
	sceneviewer->setScene(*scene);

	//Assign back bround 0<rgb<1
	double rgb[] = { 0.12, 0.23, 1.0 };
	sceneviewer->setBackgroundColourRGB(rgb);

	//View all if the specified
	if (view_all)
		sceneviewer->viewAll();

	
	//Render to screen
	sceneviewer->renderScene();

	
}