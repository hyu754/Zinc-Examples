
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
	*region = context->getDefaultRegion();

	//Create a fieldmodule
	fieldmodule = new Fieldmodule;
	*fieldmodule = region->getFieldmodule();

	//Create a scene
	scene = new Scene;
	*scene = region->getScene();

	//Define standard glyph 
	context->getGlyphmodule().defineStandardGlyphs();

	//Define standard materials
	context->getMaterialmodule().defineStandardMaterials();
	////Create a fieldmodule
	//fieldmodule = new Fieldmodule;
	//*fieldmodule = region->getFieldmodule();

	////Create a scene
	//scene = new Scene;
	//*scene = region->getScene();
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
	const char *file_name_char = file_name.c_str();
	region->readFile(file_name_char);
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

//Read a file with .exnode and .exelem extension, and add to context
void zinc_wrapper::read_exnode_exelem(std::string file_name){
	std::string exnode_string = file_name + ".exnode";
	std::string exelem_string = file_name + ".exelem";
	const char *file_name_char_node = exnode_string.c_str();
	const char *file_name_char_elem = exelem_string.c_str();
	region->readFile(file_name_char_node);
	region->readFile(file_name_char_elem);


}


void zinc_wrapper::read_exelement(std::string file_name){

	std::string exelem_string = file_name + ".exelem";

	const char *file_name_char_elem = exelem_string.c_str();

	region->readFile(file_name_char_elem);

}


//Set sceneviewer view port size
void zinc_wrapper::set_scene_viewer_size(int width, int height){
	//Create sceneviewermodule
	sceneviewermodule = new Sceneviewermodule;
	*sceneviewermodule = context->getSceneviewermodule();


	//Create a sceneviewer
	sceneviewer = new Sceneviewer;
	*sceneviewer = sceneviewermodule->createSceneviewer(OpenCMISS::Zinc::Sceneviewer::BufferingMode::BUFFERING_MODE_SINGLE, OpenCMISS::Zinc::Sceneviewer::StereoMode::STEREO_MODE_DEFAULT);
	//sceneviewer->setProjectionMode(Sceneviewer::PROJECTION_MODE_PERSPECTIVE);
	sceneviewer->setScene(*scene);
	//Create a scenepicker
	scenepicker = new Scenepicker;
	*scenepicker = scene->createScenepicker();

	Scenefiltermodule filter_module = context->getScenefiltermodule();
	Scenefilter graphics_filter = filter_module.createScenefilterVisibilityFlags();
	sceneviewer->setScenefilter(graphics_filter);

	sceneviewer->setScene(*scene);

	sceneviewer->setViewportSize(width, height);
	//sceneviewer->setLightingTwoSided(true);
}

//Add a geometry to scene
void zinc_wrapper::add_surface_to_scene(std::string geometry_name, std::string coordinate_name, double alpha_, std::string material){

	fieldmodule->beginChange();

	const char * geometry_name_char = geometry_name.c_str();
	const char * coordinate_name_char = coordinate_name.c_str();
	Field geo_field = fieldmodule->findFieldByName(geometry_name_char);
	global_coordinates = fieldmodule->findFieldByName(coordinate_name_char);
	//Create material module
	Materialmodule materialmodule = context->getMaterialmodule();
	fieldmodule->endChange();



	scene->beginChange();
#if 1
	//Create a surface 
	GraphicsSurfaces surface = scene->createGraphicsSurfaces();

	//Assign the geometry field to the graphics surface
	surface.setSubgroupField(geo_field);

	//Assign the coordinate to the graphics surface
	surface.setCoordinateField(global_coordinates);

	//Set the name to of the graphics surface, this will be set to be geometry_name_surface
	geometry_name = geometry_name + "surface";
	const char * surface_name = geometry_name.c_str();
	surface.setName(surface_name);

	//Create material module
	//Materialmodule materialmodule = context->getMaterialmodule();

	//Define standard materials, i.e., green, blue, etc.
	materialmodule.defineStandardMaterials();

	Material newMaterial = materialmodule.findMaterialByName(material.c_str());
	newMaterial.setAttributeReal(Material::ATTRIBUTE_ALPHA, alpha_);
	Field xi_field = fieldmodule->findFieldByName("xi");
	//surface.setTextureCoordinateField(xi_field);
	//newMaterial.
	//newMaterial.setAttributeReal3(OpenCMISS::Zinc::Material::)
	if (!surface.setMaterial(newMaterial)){
		std::cout << "Surface colour is not set " << std::endl;
	}

	surface.setVisibilityFlag(true);




#endif // 0

	//lines.setSubgroupField(geo_field);
	//Material black = materialmodule.findMaterialByName("green");
	////Material image_material = materialmodule.createMaterial();

	//Tessellationmodule tessmod = scene->getTessellationmodule();
	//tessmod.getDefaultTessellation();
	//int tes1_vec[3] = { 1, 1, 1 };
	//Tessellation tes1 = tessmod.createTessellation();
	//tes1.setRefinementFactors(3, tes1_vec);
	//lines.setTessellation(tes1);

	/*Tessellationmodule tessmod = scene->getTessellationmodule();
	tessmod.getDefaultTessellation();
	int tes1_vec[3] = { 0, 1, 1 };
	Tessellation tes1 = tessmod.createTessellation();
	tes1.setRefinementFactors(3, tes1_vec);
	surface.setTessellation(tes1);*/



	//Sets the scene to the viewer	



	//Find coordinates to evaluate the nodes on
	// fieldmodule->findFieldByName("coordinates");



	scene->endChange();

}


void zinc_wrapper::add_line_to_scene(std::string geometry_name, std::string coordinate_name, double alpha_, std::string material, float line_width){

	fieldmodule->beginChange();




	const char * geometry_name_char = geometry_name.c_str();
	const char * coordinate_name_char = coordinate_name.c_str();
	Field geo_field = fieldmodule->findFieldByName(geometry_name_char);
	geo_field.getId();
	global_coordinates = fieldmodule->findFieldByName(coordinate_name_char);
	//Create material module
	Materialmodule materialmodule = context->getMaterialmodule();
	fieldmodule->endChange();



	scene->beginChange();


#if 1
	//Create a surface 
	GraphicsLines lines = scene->createGraphicsLines();

	//Assign the geometry field to the graphics surface
	lines.setSubgroupField(geo_field);

	//Assign the coordinate to the graphics surface
	lines.setCoordinateField(global_coordinates);

	//Set the name to of the graphics surface, this will be set to be geometry_name_surface
	geometry_name = geometry_name + "surface";
	const char * surface_name = geometry_name.c_str();
	lines.setName(surface_name);
	lines.setRenderLineWidth(line_width);
	//Create material module
	//Materialmodule materialmodule = context->getMaterialmodule();

	//Define standard materials, i.e., green, blue, etc.



	Material newMaterial = materialmodule.findMaterialByName(material.c_str());
	newMaterial.setAttributeReal(Material::ATTRIBUTE_ALPHA, alpha_);

	Field xi_field = fieldmodule->findFieldByName("xi");
	//surface.setTextureCoordinateField(xi_field);
	//newMaterial.
	//newMaterial.setAttributeReal3(OpenCMISS::Zinc::Material::)
	if (!lines.setMaterial(newMaterial)){
		std::cout << "Surface colour is not set " << std::endl;
	}

	lines.setVisibilityFlag(true);



	//




#endif // 0

	//lines.setSubgroupField(geo_field);
	//Material black = materialmodule.findMaterialByName("green");
	////Material image_material = materialmodule.createMaterial();

	//Tessellationmodule tessmod = scene->getTessellationmodule();
	//tessmod.getDefaultTessellation();
	//int tes1_vec[3] = { 1, 1, 1 };
	//Tessellation tes1 = tessmod.createTessellation();
	//tes1.setRefinementFactors(3, tes1_vec);
	//lines.setTessellation(tes1);

	/*Tessellationmodule tessmod = scene->getTessellationmodule();
	tessmod.getDefaultTessellation();
	int tes1_vec[3] = { 0, 1, 1 };
	Tessellation tes1 = tessmod.createTessellation();
	tes1.setRefinementFactors(3, tes1_vec);
	surface.setTessellation(tes1);*/



	//Sets the scene to the viewer	



	//Find coordinates to evaluate the nodes on
	// fieldmodule->findFieldByName("coordinates");



	scene->endChange();
}

void zinc_wrapper::add_node_to_scene(std::string geometry_name, std::string coordinate_name, double alpha_, std::string material, float node_size){

	fieldmodule->beginChange();




	const char * geometry_name_char = geometry_name.c_str();
	const char * coordinate_name_char = coordinate_name.c_str();
	Field geo_field = fieldmodule->findFieldByName(geometry_name_char);
	geo_field.getId();
	global_coordinates = fieldmodule->findFieldByName(coordinate_name_char);
	//Create material module
	Materialmodule materialmodule = context->getMaterialmodule();
	fieldmodule->endChange();



	scene->beginChange();




	GraphicsPoints nodes = scene->createGraphicsPoints();



	nodes.setCoordinateField(global_coordinates);

	//Set the name to of the graphics surface, this will be set to be geometry_name_surface
	geometry_name = geometry_name + "node";
	const char * node_name = geometry_name.c_str();
	nodes.setName(node_name);
	nodes.setFieldDomainType(Field::DOMAIN_TYPE_NODES);

	Graphicspointattributes attributes = nodes.getGraphicspointattributes();
	attributes.setGlyphShapeType(Glyph::SHAPE_TYPE_SPHERE);
	double base_[3] = { node_size, node_size, node_size };
	attributes.setBaseSize(3, base_);

	Material newMaterial = materialmodule.findMaterialByName(material.c_str());
	newMaterial.setAttributeReal(Material::ATTRIBUTE_ALPHA, alpha_);

	if (!nodes.setMaterial(newMaterial)){
		std::cout << "Surface colour is not set " << std::endl;
	}

	nodes.setVisibilityFlag(true);
	scene->endChange();

}



//
void zinc_wrapper::set_background_colour(double r, double b, double g){
	const double _c[3] = { r, b, g };
	sceneviewer->setBackgroundColourRGB(_c);

}


//Sets and renders scene
void zinc_wrapper::render_scene(bool view_all){


	//sceneviewer->setScene(*scene);

	//View all if the specified
	if (view_all)
		sceneviewer->viewAll();

	//sceneviewer->endChange();
	//Render to screen


	//sceneviewer->endChange();
	//scene->endChange();



	sceneviewer->renderScene();

}

void zinc_wrapper::set_window_attributes(double eye_point[3], double look_at[3], double up_vector[3], double angle){


	sceneviewer->setEyePosition(eye_point);
	sceneviewer->setUpVector(up_vector);

	sceneviewer->setViewAngle(angle);
	sceneviewer->setLookatPosition(look_at);

}



//Rotates and translates a given geometry
void zinc_wrapper::rotate_translation_geometry(std::string geometry_name, double R[3][3], double T[3], bool update){
	//scene->beginChange();
	//sceneviewer->beginChange();
	//region->beginChange();

	//Find field of given by geometry
	//scene->beginChange();
	Fieldmodule tempfieldmodule = global_coordinates.getFieldmodule();
	tempfieldmodule.beginChange();


	Field geoField = tempfieldmodule.findFieldByName(static_cast<const char*>(geometry_name.c_str()));


	//Create a field group
	FieldGroup fieldgroup = geoField.castGroup();


	Nodeset masternodeset = tempfieldmodule.findNodesetByFieldDomainType(Field::DomainType::DOMAIN_TYPE_NODES);

	//fieldmodule->findNodesetByFieldDomainType(Field::DomainType::DOMAIN_TYPE_NODES);

	//Cast the node group
	FieldNodeGroup fieldnodegroup = fieldgroup.getFieldNodeGroup(masternodeset);

	//Create nodesetgroup
	NodesetGroup nodesetgroup = fieldnodegroup.getNodesetGroup();

	//Create node iterator
	Nodeiterator node_it = nodesetgroup.createNodeiterator();





	//sceneviewer->beginChange();



	////Create a geo field module
	//Fieldmodule geoFieldModule = geoField.getFieldmodule();
	//Create field cache
	Fieldcache cache = tempfieldmodule.createFieldcache();
	//std::cout << cache.getId() << std::endl;
	Node node_temp = node_it.next();

	//	Field new_global_coordinates;// = global_coordinates;
	////	lines.setVisibilityFlag(false);
	//	//fieldmodule->beginChange();
	//	FieldNodeValue nodevaluefield = fieldmodule->createFieldNodeValue(global_coordinates, OpenCMISS::Zinc::Node::ValueLabel::VALUE_LABEL_VALUE, 1);
	//
	//
	//	Mesh mesh = fieldmodule->findMeshByDimension(3);
	//	Elementiterator el_iter = mesh.createElementiterator();
	//	Element element = el_iter.next();
	//	
	//	double xi[3] = { 0.5, 0.5, 0.5 };
	//	while (element.isValid()){
	//		double temp;
	//		cache.setMeshLocation(element, 3, xi);
	//		global_coordinates.evaluateReal(cache, 3, xi);
	//		//xi[0] += 10.11;
	//		//xi[1] += 10.11;
	//		global_coordinates.assignReal(cache, 3, xi);
	//		element = el_iter.next();
	//	}
	//
	global_coordinates.setManaged(true);
	global_coordinates.setCoordinateSystemType(OpenCMISS::Zinc::Field::CoordinateSystemType::COORDINATE_SYSTEM_TYPE_RECTANGULAR_CARTESIAN);
	Field newfield = global_coordinates;

	while (node_temp.isValid()){
		for (int _node_type = 1; _node_type <= 8; _node_type++){
			FieldNodeValue nodevaluefield = fieldmodule->createFieldNodeValue(global_coordinates, static_cast<Node::ValueLabel>(_node_type), 1);
			cache.setNode(node_temp);
			//

			double position_current[3], position_new[3];
			//global_coordinates.evaluateReal(cache,6, position_current);
			nodevaluefield.evaluateReal(cache, 3, position_current);
			///Rotate the point

			for (int _i = 0; _i < 3; _i++){
				double _row = 0.0;
				for (int _j = 0; _j < 3; _j++){
					_row += position_current[_j] * R[_i][_j];
				}
				position_new[_i] = _row;

			}


			//Translate points
			//position_current[0] += T[0];
			//position_current[1] += T[1];
			//position_current[2] += T[2];


			nodevaluefield.assignReal(cache, 3, position_new);


		}
		node_temp = node_it.next();
	}

	//cache.clearLocation();
	//Graphics surfaces = scene->findGraphicsByName("image_plane");

	///surfaces.setVisibilityFlag(true);
	tempfieldmodule.endChange();
	//scene->endChange();
	//region->endChange();
	//sceneviewer->endChange();

}


//Performs mesh integration
void zinc_wrapper::mesh_integrator(std::string geometry_name){
	//fieldmodule->beginChange();
	//Field is exterior
	FieldIsExterior exterior_field = fieldmodule->createFieldIsExterior();

	//Find faces
	Mesh mesh = fieldmodule->findMeshByDimension(2);

	FieldGroup groupfield = fieldmodule->createFieldGroup();

	FieldElementGroup egroup = fieldmodule->createFieldElementGroup(mesh);
	egroup.setName("exterior_group");
	MeshGroup exterior_mesh = egroup.getMeshGroup();


	exterior_mesh.addElementsConditional(exterior_field);

	std::cout << "Exterior face size : " << exterior_mesh.getSize() << std::endl;


	//Find elements
	Mesh mesh3d = fieldmodule->findMeshByDimension(3);


	/*
	Integration gfx


	gfx define field allshapes / one constant 1.0;
	gfx define field allshapes / volume mesh_integral coordinate_field coordinates integrand_field one mesh mesh3d gaussian_quadrature numbers_of_points "4";
	gfx define field allshapes / surface_area mesh_integral coordinate_field coordinates integrand_field one mesh exterior.mesh2d gaussian_quadrature numbers_of_points "4";
	*/
	//Volume
	double one_ = 1;
	FieldConstant one_field = fieldmodule->createFieldConstant(1, &one_);
	Field coordinates = fieldmodule->findFieldByName("coordinates");
	FieldMeshIntegral volume_field = fieldmodule->createFieldMeshIntegral(one_field, coordinates, mesh3d);

	//Surface
	FieldMeshIntegral surface_field = fieldmodule->createFieldMeshIntegral(one_field, coordinates, exterior_mesh);


	//Evaluate fields
	Fieldcache cache = fieldmodule->createFieldcache();
	double volume_out, surface_out;
	volume_field.evaluateReal(cache, 1, &volume_out);
	surface_field.evaluateReal(cache, 1, &surface_out);

	std::cout << "Volume : " << volume_out << std::endl << "Surface area: " << surface_out << std::endl;




	//point_attribute.setLabelText(1, "hi");

	/*point_attribute.setGlyph(point_glyph);
	volume_point.setRenderPointSize(20);*/





	char volume_string[100];
	char volume_c[20];
	char volume_value_string[20];
	sprintf(volume_string, "%f", volume_out);
	strcpy(volume_c, "Volume : ");
	strcpy(volume_value_string, volume_string);
	strcat(volume_c, volume_value_string);

	/*
	Display volume
	*/
	GraphicsPoints volume_text_node = scene->createGraphicsPoints();
	volume_text_node.setFieldDomainType(Field::DomainType::DOMAIN_TYPE_POINT);
	volume_text_node.setScenecoordinatesystem(Scenecoordinatesystem::SCENECOORDINATESYSTEM_NORMALISED_WINDOW_FIT_LEFT);
	Graphicspointattributes	attributes = volume_text_node.getGraphicspointattributes();
	double offset_[3] = { -0.95, 0.9, 0 };
	attributes.setLabelOffset(3, offset_);
	attributes.setLabelText(1, volume_c);
	attributes.setGlyphShapeType(Glyph::SHAPE_TYPE_INVALID);
	double size_ = 1;
	attributes.setBaseSize(1, &size_);


	/*
	Display surface area
	*/
	std::string surface_string = "Surface area : " + std::to_string(surface_out);


	GraphicsPoints surface_text_node = scene->createGraphicsPoints();
	surface_text_node.setFieldDomainType(Field::DomainType::DOMAIN_TYPE_POINT);
	surface_text_node.setScenecoordinatesystem(Scenecoordinatesystem::SCENECOORDINATESYSTEM_NORMALISED_WINDOW_FIT_LEFT);
	Graphicspointattributes	surface_attributes = surface_text_node.getGraphicspointattributes();
	double surface_offset_[3] = { -0.95, 0.8, 0 };
	surface_attributes.setLabelOffset(3, surface_offset_);
	surface_attributes.setLabelText(1, surface_string.c_str());
	surface_attributes.setGlyphShapeType(Glyph::SHAPE_TYPE_INVALID);
	
	surface_attributes.setBaseSize(1, &size_);

	surface_text_node.setMaterial(context->getMaterialmodule().Materialmodule::findMaterialByName("green"));





}



//Element elem;

///*mesh_exterior.destroyElementsConditional(exterior_field);*/
//Elementiterator el_iterator = mesh.createElementiterator();
//Fieldcache cache = fieldmodule->createFieldcache();

//elem = el_iterator.next();
//
//while (elem.isValid()){
//	cache.setElement(elem);
//	double isexterior;
//	exterior_field.evaluateReal(cache, 1, &isexterior);
//	std::cout << isexterior << std::endl;

//	if (isexterior != 1){
//		//mesh_exterior.addElement(elem);
//		//elem.
//		///eee.addElement(elem);
//		int stat_ = exterior_mesh.addElement(elem);
//		std::cout << "stat : " << stat_ << std::endl;
//		//std::cout << "Elem size : " << mesh_exterior.getSize() << std::endl;
//	}
//	elem = el_iterator.next();
//}