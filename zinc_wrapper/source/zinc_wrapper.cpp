
#include "zinc_wrapper.hpp"


//Zinc wrapper constructor
zinc_wrapper::zinc_wrapper()
{
	Context context_("Default");
	//TO DO :
	//NEED TO ADD CONSTRUCTOR
	//
	std::cout << "Zinc wrapper class initialized" << std::endl;

	//Initialize common fields
	initialize_common_fields();
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

	//Initialize common fields and materials
	initialize_common_fields();
	initialize_common_materials();

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

void zinc_wrapper::initialize_common_materials(){

	common_materials.blue = context->getMaterialmodule().findMaterialByName("blue");
	common_materials.gold = context->getMaterialmodule().findMaterialByName("gold");
	common_materials.green = context->getMaterialmodule().findMaterialByName("green");

}
void zinc_wrapper::initialize_common_fields(){
	double zero_ = 0.0;
	double one_ = 1.0;
	double negative1_ = -1.0;
	common_fields.zero = fieldmodule->createFieldConstant(1, &zero_);
	common_fields.one = fieldmodule->createFieldConstant(1, &one_);
	common_fields.negative_one = fieldmodule->createFieldConstant(1, &negative1_);

		
	
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


void zinc_wrapper::read_exdata(std::string file_name){
	std::string data_string = file_name + ".exnode";
	int read_status= region->readFile(data_string.c_str());
	if (read_status == 0){
		data_string = file_name + ".exdata";
		read_status = region->readFile(data_string.c_str());
		if (read_status == 0){
			std::cout << "Error reading data file. Check file name. " << std::endl;
		}
	}

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
	
	lines.setRenderLineWidth(4);
	auto tess_mod = context->getTessellationmodule();
	auto tess_line = tess_mod.createTessellation();
	int tess_ = 4;
	tess_line.setMinimumDivisions(1, &tess_);
	
	lines.setTessellation(tess_line);
	
	//auto attributes_line = lines.getGraphicslineattributes();
	//double line_size = 4;
	//attributes_line.setBaseSize(1, &line_size);
	//
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
		std::cout << "Node colour is not set " << std::endl;
	}

	nodes.setVisibilityFlag(true);
	scene->endChange();

}


void zinc_wrapper::add_data_to_scene(std::string geometry_name, std::string coordinate_name, double alpha_, std::string material, float node_size){

	fieldmodule->beginChange();




	const char * geometry_name_char = geometry_name.c_str();
	const char * coordinate_name_char = coordinate_name.c_str();
	Field geo_field = fieldmodule->findFieldByName(geometry_name_char);
	
	geo_field.isValid();
	Field coordinates_ = fieldmodule->findFieldByName(coordinate_name_char);
	//Create material module
	Materialmodule materialmodule = context->getMaterialmodule();
	fieldmodule->endChange();



	scene->beginChange();




	Graphics nodes = scene->createGraphics(Graphics::TYPE_POINTS);




	nodes.setCoordinateField(coordinates_);

	//Set the name to of the graphics surface, this will be set to be geometry_name_surface
	geometry_name = geometry_name + "data";
	const char * node_name = geometry_name.c_str();
	nodes.setName(node_name);
	nodes.setFieldDomainType(Field::DOMAIN_TYPE_NODES);

	Graphicspointattributes attributes = nodes.getGraphicspointattributes();
	attributes.setGlyphShapeType(Glyph::SHAPE_TYPE_CROSS);
	double base_[3] = { node_size, node_size, node_size };
	attributes.setBaseSize(3, base_);

	Material newMaterial = materialmodule.findMaterialByName(material.c_str());
	newMaterial.setAttributeReal(Material::ATTRIBUTE_ALPHA, alpha_);

	if (!nodes.setMaterial(newMaterial)){
		std::cout << "Data colour is not set " << std::endl;
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





	std::string volume_string = "Volume : " + std::to_string(volume_out);
	/*
	Display volume
	*/
	GraphicsPoints volume_text_node = scene->createGraphicsPoints();
	volume_text_node.setFieldDomainType(Field::DomainType::DOMAIN_TYPE_POINT);
	volume_text_node.setScenecoordinatesystem(Scenecoordinatesystem::SCENECOORDINATESYSTEM_NORMALISED_WINDOW_FIT_LEFT);
	Graphicspointattributes	attributes = volume_text_node.getGraphicspointattributes();
	double offset_[3] = { -0.95, 0.9, 0 };
	attributes.setLabelOffset(3, offset_);
	attributes.setLabelText(1, volume_string.c_str());
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


void zinc_wrapper::optimise_1d(){
	//gfx define field found_location find_mesh_location find_nearest mesh mesh1d mesh_field coordinates source_field data_coordinates;
	Field coordinate_field = fieldmodule->findFieldByName("coordinates");
	Field datacoordinate_field = fieldmodule->findFieldByName("data_coordinates");
	Mesh mesh1d = fieldmodule->findMeshByDimension(1);
	FieldFindMeshLocation found_location = fieldmodule->createFieldFindMeshLocation(datacoordinate_field, coordinate_field, mesh1d); 
	found_location.setSearchMode(FieldFindMeshLocation::SearchMode::SEARCH_MODE_NEAREST);

	/* define a field giving the coordinates at the found_location on the mesh:
	gfx define field projected_coordinates embedded element_xi found_location field coordinates;*/
	FieldEmbedded projected_coordinates = fieldmodule->createFieldEmbedded(coordinate_field, found_location);
	

    //# define a field which calculates the error vector between the data point and its element project:
	//gfx define field error_vector add fields data_coordinates projected_coordinates scale_factors 1 - 1;
	double n_one = -1.0f;
	Field negative1 = fieldmodule->createFieldConstant(1, &n_one);
	Field negative_projected_coordinates = fieldmodule->createFieldMultiply(projected_coordinates, negative1);
	FieldAdd error_vector = fieldmodule->createFieldAdd(datacoordinate_field,negative_projected_coordinates);

	FieldDerivative field_derivative= fieldmodule->createFieldDerivative(coordinate_field,1);


	
	
	//# get the magnitude of that vector as a measure of error for each data point :
	//gfx define field error magnitude field error_vector;
	Field error = fieldmodule->createFieldMagnitude(error_vector);

	//#visualise the error bars :
	//gfx modify g_element "/" data_points coordinate data_coordinates LOCAL glyph line general size "0*0.1*0.1" centre 0, 0, 0 font default orientation error_vector scale_factors "-1*0*0" select_on material silver selected_material default_selected;
	Graphics nodes = scene->createGraphics(Graphics::TYPE_POINTS);
	nodes.setCoordinateField(datacoordinate_field);
	nodes.setFieldDomainType(Field::DOMAIN_TYPE_NODES);

	Graphicspointattributes attributes = nodes.getGraphicspointattributes();
	attributes.setGlyphShapeType(Glyph::SHAPE_TYPE_LINE);
	double base_[3] = {0,0.1,0.1 };
	attributes.setBaseSize(3, base_);
	double scale_[3] = { -1, 0, 0 };
	attributes.setScaleFactors(3, scale_);
	attributes.setOrientationScaleField(error_vector);

	Material newMaterial = context->getMaterialmodule().findMaterialByName("magenta");
	newMaterial.setAttributeReal(Material::ATTRIBUTE_ALPHA, 1);

	if (!nodes.setMaterial(newMaterial)){
		std::cout << "Error optimisation material " << std::endl;
	}

	//Draw some normal and tangent vectors
	//NOTE: ASSUME normal.y = 0
	/*
	
	#display tangent and normal vectors
	gfx def field d_ds1 node_value fe_field coordinates d / ds1
		gfx def field d_ds1_normal normal field d_ds1

		# normal.y = 0
		gfx define field n1 constant 1
		gfx define field n2 constant 0

		gfx define field v1_positive component d_ds1.x
		gfx define field negative constant - 1
		gfx define field v1 multiply field negative v1_positive
		gfx define field v3 component d_ds1.z

		gfx define field n3 divide fields v1 v3
		gfx define field normal composite n1 n2 n3
		gfx define field normalnormal normal field normal
	*/
	
	FieldNodeValue d_ds1 =fieldmodule->createFieldNodeValue(coordinate_field, Node::ValueLabel::VALUE_LABEL_D_DS1, 1);

	Field d_ds1_normalise = fieldmodule->createFieldNormalise(d_ds1);
	FieldComponent v1_positive = fieldmodule->createFieldComponent(d_ds1_normalise, 3);
	Field v1_negative = fieldmodule->createFieldMultiply(v1_positive, common_fields.negative_one);
	FieldComponent v3 = fieldmodule->createFieldComponent(d_ds1_normalise, 1);
	Field n3 = fieldmodule->createFieldDivide(v1_negative, v3);

	Field n1 = common_fields.one;
	Field n2 = common_fields.zero;
	Field normal_vec_[3] = { n1, n2, n3 };
	Field normal_vector = fieldmodule->createFieldConcatenate(3, normal_vec_);
	Field norm_normal_vector = fieldmodule->createFieldNormalise(normal_vector);

	Graphics g_dds1 = scene->createGraphicsPoints();
	g_dds1.setCoordinateField(coordinate_field);
	g_dds1.setFieldDomainType(Field::DomainType::DOMAIN_TYPE_NODES);
	auto g_dds1_attributes = g_dds1.getGraphicspointattributes();
	
	g_dds1_attributes.setGlyphShapeType(Glyph::ShapeType::SHAPE_TYPE_ARROW_SOLID);
	g_dds1_attributes.setOrientationScaleField(d_ds1_normalise);
	double dds1_size[3] = { 0.1, 0.01, 0.01 };
	g_dds1_attributes.setBaseSize(3, dds1_size);
	g_dds1.setMaterial(common_materials.gold);
	g_dds1.setVisibilityFlag(false);
	Graphics g_normal = scene->createGraphicsPoints();
	g_normal.setCoordinateField(coordinate_field);
	g_normal.setFieldDomainType(Field::DOMAIN_TYPE_NODES);
	auto g_normal_attributes = g_normal.getGraphicspointattributes();

	g_normal_attributes.setGlyphShapeType(Glyph::ShapeType::SHAPE_TYPE_ARROW_SOLID);
	g_normal_attributes.setOrientationScaleField(norm_normal_vector);
	
	g_normal_attributes.setBaseSize(3, dds1_size);
	g_normal.setMaterial(context->getMaterialmodule().findMaterialByName("blue"));



	/*

# define a scalar objective function to minimise which is the sum of each of the error terms over the data nodeset:
	gfx define field objective_function nodeset_sum field error nodeset datapoints;

# define objective function for least-squares method as sum of squared error_vector over the data points
# (least-squares method works directly with individual terms from sum-of-squares field types like this)
	gfx define field objective_function_lsq nodeset_sum_squares field error_vector nodeset datapoints;

	# use the quasi - Newton Opt++ method to minimise the objective function field.
# [this method minimises a scalar valued field by altering the provided independent field(s)]
	gfx minimise QUASI_NEWTON region "/" objective_fields objective_function independent_fields coordinates hide_output;

		*/
	Nodeset datapoints = fieldmodule->findNodesetByFieldDomainType(Field::DomainType::DOMAIN_TYPE_NODES);
	std::cout << datapoints.getSize();
	FieldNodesetSum objective_function = fieldmodule->createFieldNodesetSum(error, datapoints);
	FieldNodesetSumSquares objective_function_Sqrt = fieldmodule->createFieldNodesetSumSquares(error_vector, datapoints);




	Optimisation optimisation = fieldmodule->createOptimisation();
	optimisation.addObjectiveField(objective_function);
	optimisation.addIndependentField(coordinate_field);
	optimisation.setMethod(Optimisation::Method::METHOD_QUASI_NEWTON);
	optimisation.setAttributeInteger(Optimisation::Attribute::ATTRIBUTE_MAXIMUM_ITERATIONS, 1);
	
	int max_it = 5;
	for (int i = 0; i < max_it; i++){

		optimisation.optimise();
		this->render_scene(true);
		std::string file_name_out_ = "iteration" + std::to_string(i)+".jpg";
		sceneviewer->writeImageToFile(file_name_out_.c_str(),1,1000,1000,1,1);

	}
	
	
	

	std::cout << optimisation.getSolutionReport() << std::endl;
}