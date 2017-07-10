#include <GL/freeglut.h>
#include <GL/gl.h>
#include <GL/glu.h>

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

#include "zinc/stream.h"
#include "opencv2\highgui.hpp"
#include "opencv2\opencv.hpp"

#include <iostream>

#include <ctime>

#include "zinc_wrapper.hpp"

//Global video capture pointer
cv::VideoCapture *g_p_cap;

//Global zinc_wrapper pointer
zinc_wrapper *g_ZW_ptr;


using namespace OpenCMISS::Zinc;
using namespace std;
Context context("example");
Sceneviewer global_scene;
double eye1[3] = { 100.0, 10.0, 0.0 };
Sceneviewer global_scene2;
Fieldmodule global_field_module;
Field global_coordinates;
FieldImage gl_field_image;
double i = 0;

StreaminformationImage *global_imagestream;
Scene *g_p_scene;
Materialmodule *g_p_materialmodule;
cv::Mat in_image = cv::Mat(cv::Size(640, 480), CV_8UC3);// = cv::Mat(cv::Size(640, 480), CV_8UC4, buff.data, cv::Mat::AUTO_STEP);
//cv::Mat in_image_rgb = cv::Mat(cv::Size(640, 480), CV_8UC3);
double time_global = 0;
IplImage *imgO = cvCreateImage(cvSize(640, 480), IPL_DEPTH_8U, 3);


void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT);
	//glutSolidTeapot(1.5);

	//global_scene.viewAll();
	//global_scene.renderScene();
	g_ZW_ptr->render_scene(true);
	

	glFlush();

}




void run_view(void){
	global_field_module.beginChange();
	double eye2[3] = { 0, 0, 0.0 };
	//glClear(GL_COLOR_BUFFER_BIT);
	//glutSolidTeapot(.5);
	//global_scene.setEyePosition(eye2);
	Nodeset node = global_field_module.findNodesetByFieldDomainType(Field::DOMAIN_TYPE_NODES);

	if (i == 0)
	{
		double eye3[3] = { i, i, i };

		double eye31[3] = { 0, 0, 1300 };

		global_scene.setEyePosition(eye31);
		double eye321[3] = { 0, 0, -1.0 };
		global_scene.setLookatPosition(eye321);
		//global_scene.viewAll();
		global_scene.getEyePosition(eye3);
		global_scene.setViewAngle(0.69);

		std::cout << "eye ; " << eye3[0] << "," << eye3[1] << "," << eye3[2] << endl;
		global_scene.getLookatPosition(eye3);
		std::cout << "look at ; " << eye3[0] << "," << eye3[1] << "," << eye3[2] << endl;

		global_scene.getUpVector(eye3);
		std::cout << "getUpVector ; " << eye3[0] << "," << eye3[1] << "," << eye3[2] << endl;


		std::cout << "view angle: " << global_scene.getViewAngle() << endl;
		std::wcout << " getNearClippingPlane: " << global_scene.getNearClippingPlane() << endl; //72.5
		std::wcout << " getFarClippingPlane: " << global_scene.getFarClippingPlane() << endl; //2945.57
		////////////////////////

		global_scene.setNearClippingPlane(72.5);
		global_scene.setFarClippingPlane(2945.57);
	}

#if 0
	FieldImage field_image = global_field_module.createFieldImage();
#endif // 0

	//unsigned char * buff;
	//cv::Mat buff;

	*g_p_cap >> in_image;
	//int    nSize = 640*480*4*sizeof(unsigned char); //...       // Size of buffer
	//unsigned char * pcBuffer;// = ...    // Raw buffer data
	//pcBuffer = in_image.data;

	//cv::imshow("image", in_image);
	cv::imwrite("capture2.JPG", in_image);

	//cv::Mat im_out = cv::Mat(cv::Size(640, 480), CV_8UC4, pcBuffer);
	//cv::imshow("imout:", im_out);
	//std::vector<uchar> buffer;
	//
	/*cv::imencode(".JPG", in_image, buffer);
	*/

	// Create a Size(1, nSize) Mat object of 8-bit, single-byte elements
	//Mat rawData = Mat(1, size, CV_8UC1, pcBuffer);

	/*cv::imdecode(buff, CV_8UC4, &im_out);
	cv::imshow("im_out", im_out);*/
	StreaminformationImage stream_information = gl_field_image.createStreaminformationImage();


	/*img_buffer[0][0] = 'P';
	img_buffer[0][1] = 'N';
	img_buffer[0][2] = 'G';*/

	StreamresourceFile stream_image = stream_information.createStreamresourceFile("capture2.JPG");
	//StreamresourceMemory stream_image = stream_information.createStreamresourceMemory();


	/*buffer.at(0) = 'P';
	buffer.at(1) = 'N';
	buffer.at(2) = 'G';*/
#if 0
	unsigned char *imagedata = NULL;
	int framesize = 640 * 480;
	size_t nbytes = in_image.total() * in_image.elemSize();
	imagedata = (unsigned char*)malloc(nbytes);

	//cv::cvtColor(in_image, in_image_rgb, cv::COLOR_RGBA2RGB);
	cv::imshow("rgb", in_image);

	std::memcpy(imagedata, in_image.data, nbytes);

#endif // 0

	//cv::Mat lll = cv::Mat(cv::Size(640, 480), CV_8UC3);
	/*lll.data = (uchar)imagedata;
	cv::imshow("lll", lll);*/

	//StreamresourceMemory mm = stream_image.castMemory(); 
	//unsigned int memory_buffer_sizes_;
	//bool llllll=mm.isValid();
	//mm.getBuffer((void**)imagedata, &memory_buffer_sizes_);
	//bool asdf =stream_information.isValid();


	//StreamresourceMemory memememe = stream_information.createStreamresourceMemoryBuffer(reinterpret_cast<void*>(&imagedata), nbytes);

	//stream_information.setFileFormat()

	//stream_information.setAttributeReal(StreaminformationImage::Attribute::ATTRIBUTE_BITS_PER_COMPONENT, 24);
	//stream_information.setDataCompressionType(StreaminformationImage::DataCompressionType::DATA_COMPRESSION_TYPE_INVALID);
	///stream_information.setPixelFormat(StreaminformationImage::PixelFormat::PIXEL_FORMAT_RGB);

	//stream_information.setFileFormat(StreaminformationImage::FileFormat::FILE_FORMAT_JPG);

	//stream_image.getBuffer(buffer)
	/*stream_information.setFileFormat(StreaminformationImage::FileFormat::FILE_FORMAT_PNG);
	stream_information.setAttributeInteger(StreaminformationImage::Attribute::ATTRIBUTE_BITS_PER_COMPONENT, 8);
	stream_information.setAttributeInteger(StreaminformationImage::Attribute::ATTRIBUTE_BITS_PER_COMPONENT, 8);*/



	//unsigned char buffer_test[10] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	//stream_information.createStreamresourceMemoryBuffer(void_pointer, 640*480*3);

	//stream_information.setPixelFormat(StreaminformationImage::PixelFormat::PIXEL_FORMAT_RGB);
	/*stream_information.setPixelFormat(StreaminformationImage::PixelFormat::PIXEL_FORMAT_RGBA);
	stream_information.setFileFormat(StreaminformationImage::FileFormat::FILE_FORMAT_PNG);*/
	//stream_information.setResourceDataCompressionType()
	//global_imagestream = new StreaminformationImage();
	//stream_information.setPixelFormat(StreaminformationImage::PixelFormat::PIXEL_FORMAT_RGB);


	//
	//field_image.setTextureCoordinateHeight(480);
	//field_image.setTextureCoordinateWidth(640);
	gl_field_image.read(stream_information);
	//field_image.setFilterMode(FieldImage::FilterMode::FILTER_MODE_LINEAR);
	/*unsigned int size_return;
	StreamresourceMemory mm=stream_image.castMemory();*/
	/*cv::Mat hhh = cv::Mat(cv::Size(640, 480), CV_8UC4);
	mm.getBuffer((void**)img_buffer, &size_return);
	cv::imshow("ffff", hhh);*/

	Graphics surfaces = g_p_scene->findGraphicsByName("image_plane");


	Field plane_field = global_field_module.findFieldByName("heart");

	//Nodeset plane_node = plane_field.
	FieldGroup plane_group = plane_field.castGroup();
	Nodeset masternodeset = global_field_module.findNodesetByFieldDomainType(Field::DomainType::DOMAIN_TYPE_NODES);


	FieldNodeGroup plane_node_group = plane_group.getFieldNodeGroup(masternodeset);
	NodesetGroup plane_node_set = plane_node_group.getNodesetGroup();

	//FieldNodeGroup plane_node_group=plane_element.castNodeGroup();
	Fieldmodule plane_field_module = plane_field.getFieldmodule();
	Fieldcache cache = plane_field_module.createFieldcache();



#if 0
	double rotation_vector[9] = { i, i, 0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.50 };


	FieldConstant rotation_field = global_field_module.createFieldConstant(9, rotation_vector);
	FieldFiniteElement plane_fem = plane_group.castFiniteElement();



	Fieldmodule fm = global_coordinates.getFieldmodule();
	FieldNodeValue coordinate_node_value = fm.createFieldNodeValue(global_coordinates, OpenCMISS::Zinc::Node::ValueLabel::VALUE_LABEL_VALUE, 1);

	FieldMatrixMultiply multiplied_field = global_field_module.createFieldMatrixMultiply(3, coordinate_node_value, rotation_field);



	//multiplied_field.setName("mult_field");
#endif // 0




	//Field node_field = plane_field.


	//Field new_coordinates = global_field_module.createFieldMultiply(rotation_field, global_coordinates);
	//Field plane_rotation = global_field_module.
	/*for (int j = 0; j < 100; j++){
	node_sel = plane_node.findNodeByIdentifier(j);

	cache.setNode(node_sel);

	double currentpos[3];
	global_coordinates.evaluateReal(cache, 3, currentpos);
	currentpos[0] += i / 1000;
	global_coordinates.assignReal(cache, 3, currentpos);
	}*/
	//plane_node.c
	//	NodesetGroup plane_node_set=plane_node_group.getNodesetGroup();
	Nodeiterator node_iter = plane_node_set.createNodeiterator();



	Node node_sel = node_iter.next();



	double angle = 0.1;
	int num_points = 0;
	while (node_sel.isValid()){
		cache.setNode(node_sel);
		double currentpos[3];
		global_coordinates.evaluateReal(cache, 3, currentpos);
		//double newx = currentpos[0] * cos(angle) - currentpos[2] * sin(angle);
		//double newy = currentpos[0] * sin(angle) + currentpos[1] * cos(angle);
		//double newz = currentpos[0] * sin(angle) + currentpos[2] * cos(angle);
		//currentpos[0] = newx;
		////currentpos[1] = newy;
		//currentpos[2] = newz;



		//	double newx = currentpos[0] * cos(angle) - currentpos[2] * sin(angle);
		//	double newy = currentpos[0] * sin(angle) + currentpos[1] * cos(angle);
		double newz = currentpos[2] - 1.0;
		//currentpos[0] = newx;
		//currentpos[1] = newy;
		currentpos[2] = newz;
		//currentpos[2] = currentpos[2] + 0.01;

		//currentpos[0] += 1.01;
		//currentpos[1] += 1.01;
		//currentpos[2] += 5.01;
		//cout << currentpos[0] << currentpos[1] << endl;
		global_coordinates.assignReal(cache, 3, currentpos);
		//cmzn_nodeset_assign_field_from_source
		//FieldStoredMeshLocation plane_stored=multiplied_field.castStoredMeshLocation();
		/*global_coordinates.assignMeshLocation(cache, element, xi);*/
		/*if element.isValid() :
		node.merge(nodeTemplate)
		self._stored_location.assignMeshLocation(cache, element, xi)

		node = node_iter.next()*/
		node_sel = node_iter.next();

		//num_points++;
	}
	//cout << " num points : " << num_points << endl;
	////looping through some nodes
	//Nodeset node2 = global_field_module.findNodesetByFieldDomainType(Field::DOMAIN_TYPE_NODES);
	//Nodeset masternodeset2 = global_field_module.findNodesetByName("plane");

	//Nodeiterator node_it2 = node2.createNodeiterator();

	//Node node_selected = node_it2.next();
	//cache = global_field_module.createFieldcache();
	//double mesh_pos[3] = { 0, 0, 0 };
	//double outvalues[3];
	//while (node_selected.isValid()){
	//	cache.setNode(node_selected);
	//	global_coordinates.evaluateReal(cache, 3, outvalues);
	//	mesh_pos[0] = outvalues[1];
	//	mesh_pos[2] = outvalues[2];
	//	mesh_pos[1] = outvalues[3];
	//	/*coordinates.assignReal(cache, 3, mesh_pos);
	//	std::cout
	//	<<"x: "<< outvalues[0]<< " "
	//	<<"y: "<< outvalues[1] << " "
	//	<<"z: "<< outvalues[2]
	//	<< std::endl;*/
	//	node_selected = node_it2.next();

	//}





	Material image_material = g_p_materialmodule->createMaterial();
	if (i >= 0.0){
		image_material.setTextureField(1, gl_field_image);
	}
	i += 0.01;
#if 0
	//heart.setCoordinateField(global_coordinates);

	Material green = g_p_materialmodule->findMaterialByName("green");

	////image_material.setName("texture");
	//double dummy[3] = { 0.9, 0.9, 0.0 };
	//
	//Field xi_field = global_field_module.findFieldByName("xi");
	////surfaces.setCoordinateField(new_coordinates);
	//surfaces.setTextureCoordinateField(xi_field);

	heart.setMaterial(green);
#endif // 0

	surfaces.setMaterial(image_material);
	Field xi_field = global_field_module.findFieldByName("xi");
	surfaces.setTextureCoordinateField(xi_field);
	////heart.setTextureCoordinateField(xi_field);
	////heart.setMaterial(green);
	////heart.setVisibilityFlag(true);
	//surfaces.setVisibilityFlag(true);
	////////////////////////////

	//Scene temp_scene= global_scene.getScene();
	//
	//StreaminformationScene scene_stream = temp_scene.createStreaminformationScene();

	//

	//StreamresourceMemory scene_mem= scene_stream.createStreamresourceMemory();
	//std::cout << scene_mem.isValid();
	////cv::Mat outbuffer = cv::Mat(1000, 1000, CV_8U);
	//unsigned int size_buffer;

	//int success=scene_mem.getBuffer((void**)&outbuffer.data, &size_buffer);
	//cv::imshow("buffer", outbuffer);

	//global_scene.viewAll();
	global_field_module.endChange();



	global_scene.beginChange();
	Graphics heart = g_p_scene->findGraphicsByName("heart_surface");
	global_scene.endChange();
	time_global = std::clock();
	global_scene.renderScene();
	//	global_scene.setProjectionMode(Sceneviewer::ProjectionMode::)
	//glutSolidTeapot(1.5);

#if 0
	getScreenShot(imgO);
	cvShowImage("image from OpenGL to OpenCV", imgO);
#endif // 0

	//global_scene.writeImageToFile("outputss.JPG", 10, 0, 0, 0, 0);

	//std::cout << i << std::endl;
	//glFlush();










	//cv::waitKey(1);
	////Sleep(100);
	//glClear(GL_COLOR_BUFFER_BIT);
	////glutSolidTeapot(.5);
	//global_scene.setEyePosition(eye2);
	////global_scene2.viewAll();
	//global_scene.renderScene();

	////std::cout << i << std::endl;
	//glFlush();

	double t_now = std::clock();

	double dt = (t_now - time_global) / CLOCKS_PER_SEC;
	std::cout << "fps : " << 1.0 / dt << endl;

}
int main(int argc, char** argv)
{
#if 0
	//Opening default camera can getting its width and height
	g_p_cap = new cv::VideoCapture(0);

	if (!g_p_cap->isOpened()){
		std::cout << "Camera not initialized" << std::endl;
		return 0;
	}
	double cam_width = g_p_cap->get(cv::CAP_PROP_FRAME_WIDTH);
	double cam_height = g_p_cap->get(cv::CAP_PROP_FRAME_HEIGHT);
#endif // 0

	int width, height;
	width = 1000;
	height = 1000;
	
	//Initalizing glut
	glutInit(&argc, argv);

	//glutInitDisplayMode(GLUT_DOUBLE);
	glutInitDisplayMode(GLUT_RGB);

	
	glutInitWindowSize(width, height);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("Zinc Visualiser");
	
	zinc_wrapper ZW("region_1");
	
	ZW.read_exfile("plane.exfile"); //Field name is heart
	ZW.add_geometry_to_scene("plane", "coordinates");
	ZW.set_scene_viewer_size(width,height);



	glutSolidTeapot(1.5);






	////Assign global zinc_wrapper pointer so glut can access it
	g_ZW_ptr = &ZW;
#if 0

	//
	Region region = context.getDefaultRegion();
	//region.readFile("good.exregion");
	region.readFile("example.exfile");
	region.readFile("plane.exfile");

	Fieldmodule fieldmodule = region.getFieldmodule();

	/*StreaminformationRegion sir=region.createStreaminformationRegion();
	*/

	FieldImage field_image = fieldmodule.createFieldImage();


	field_image.setName("image_texture");
	StreaminformationImage stream_information = field_image.createStreaminformationImage();

	/*stream_information.setAttributeReal(StreaminformationImage::Attribute::ATTRIBUTE_RAW_WIDTH_PIXELS, 1);
	stream_information.setAttributeReal(StreaminformationImage::Attribute::ATTRIBUTE_RAW_HEIGHT_PIXELS,1);
	stream_information.setAttributeInteger(StreaminformationImage::Attribute::ATTRIBUTE_BITS_PER_COMPONENT, 32);*/
	stream_information.setFileFormat(StreaminformationImage::FileFormat::FILE_FORMAT_PNG);
	stream_information.createStreamresourceFile("capture.PNG");

	//global_imagestream = new StreaminformationImage();
	//stream_information.setPixelFormat(StreaminformationImage::PixelFormat::PIXEL_FORMAT_RGB);


	//
	field_image.read(stream_information);
	gl_field_image = field_image;
	//field_image.setTextureCoordinateHeight(1);
	//ffield_image.setTextureCoordinateWidth(1);
	/*Scene scene = region.getScene();

	Graphics a = scene.getFirstGraphics();

	*/

	context.getMaterialmodule();
	Glyphmodule glyphmodule = context.getGlyphmodule();
	glyphmodule.defineStandardGlyphs();

	//----------------------------BEGIN CHANGING FIELD MODEL------------------//
	fieldmodule.beginChange();
	Field heart_field = fieldmodule.findFieldByName("heart");
	Field plane_field = fieldmodule.findFieldByName("plane");
	Field coordinates = fieldmodule.findFieldByName("coordinates");

	global_coordinates = coordinates;
	Mesh nodes = fieldmodule.findMeshByDimension(1);

	Nodeset dfdummy = fieldmodule.findNodesetByName("heart");

	FieldMagnitude mag = fieldmodule.createFieldMagnitude(coordinates);
	mag.setName("mag");
	mag.setManaged(true);
	fieldmodule.endChange();


	//----------------------------BEGIN CHANGING SCENE--------------------------//
	Scene scene = region.getScene();
	scene.beginChange();

	/*Glyphmodule glyphmodule =scene.getGlyphmodule();
	glyphmodule.getDefaultPointGlyph();*/
	Materialmodule materialmodule = scene.getMaterialmodule();
	materialmodule.defineStandardMaterials();
	//View the lines
	GraphicsLines lines = scene.createGraphicsLines();
	lines.setCoordinateField(coordinates);
	lines.setSubgroupField(plane_field);
	Material black = materialmodule.findMaterialByName("red");
	//Material image_material = materialmodule.createMaterial();

	Tessellationmodule tessmod = scene.getTessellationmodule();
	tessmod.getDefaultTessellation();
	int tes1_vec[3] = { 1, 1, 1 };
	Tessellation tes1 = tessmod.createTessellation();
	tes1.setRefinementFactors(3, tes1_vec);
	lines.setTessellation(tes1);
	/*Tessellationmodule tessmod;



	int tes1_vec[3] = { 300, 300, 300 };
	tes1.setRefinementFactors(3, tes1_vec);
	lines.setTessellation(tes1);*/


	lines.setRenderLineWidth(2.0);


	lines.setMaterial(black);
	lines.setVisibilityFlag(true);
	Material brown = materialmodule.findMaterialByName("red");
	brown.setAttributeReal(Material::ATTRIBUTE_ALPHA, 0.5);
	GraphicsSurfaces heart_surface = scene.createGraphicsSurfaces();
	heart_surface.setSubgroupField(heart_field);
	heart_surface.setCoordinateField(coordinates);
	heart_surface.setName("heart_surface");
	heart_surface.setExterior(true);

	heart_surface.setMaterial(brown);
	heart_surface.setVisibilityFlag(false);
	//View the surfaces
	GraphicsSurfaces surfaces = scene.createGraphicsSurfaces();
	surfaces.setSubgroupField(plane_field);
	surfaces.setCoordinateField(coordinates);
	surfaces.setName("image_plane");

	Field texture_surface = surfaces.getTextureCoordinateField();
	//Material brown = materialmodule.findMaterialByName("gray");
	Material image_material = materialmodule.createMaterial();

	//field_image.setDomainField(texture_surface);
	image_material.setTextureField(1, field_image);
	image_material.setName("texture");
	double dummy[3] = { 0.9, 0.9, 0.0 };
	//brown.setAttributeReal3(Material::ATTRIBUTE_DIFFUSE, dummy);
	//brown.setAttributeReal3(Material::ATTRIBUTE_AMBIENT, dummy);
	//lines.setRenderLineWidth(2.0);
	Field xi_field = fieldmodule.findFieldByName("xi");
	surfaces.setTextureCoordinateField(xi_field);
	surfaces.setMaterial(image_material);

	surfaces.setVisibilityFlag(true);

	//GraphicsSurfaces heart_surface = scene.createGraphicsSurfaces();

	//heart_surface.setCoordinateField(heart_field);
	//heart_surface.setName("heat_surface");
	//heart_surface.setMaterial(brown);

	////points
	double vec_size[3] = { 4.0, 4.0, 4.0 };
	GraphicsPoints points = scene.createGraphicsPoints();

	points.setCoordinateField(coordinates);
	points.setSubgroupField(plane_field);
	points.setFieldDomainType(Field::DOMAIN_TYPE_NODES);
	points.setName("plane_nodes");
	Material green = materialmodule.findMaterialByName("orange");
	points.setMaterial(green);
	Graphicspointattributes pointpoint = points.getGraphicspointattributes();
	pointpoint.setGlyphShapeType(Glyph::SHAPE_TYPE_CUBE_SOLID);
	double dummy1[3] = { 4.0, 4.0, 4.0 };
	pointpoint.setBaseSize(3, dummy1);
	//
	pointpoint.setScaleFactors(10, vec_size);
	//points.setVisibilityFlag(true);


	//axis
	/*graphics = scene.createGraphicsPoints() yes
	attributes = graphics.getGraphicspointattributes()
	attributes.setGlyphShapeType(Glyph.SHAPE_TYPE_AXES_SOLID)
	# Note: default base size of 0.0 would make axes invisible!
	attributes.setBaseSize([1.0])
	*/
	GraphicsPoints axes = scene.createGraphicsPoints();
	axes.setFieldDomainType(Field::DOMAIN_TYPE_POINT);

	Graphicspointattributes	pointAttr = axes.getGraphicspointattributes();
	pointAttr.setGlyphShapeType(Glyph::SHAPE_TYPE_AXES_SOLID);
	//double vec_size[3] = { 100.0, 100.0, 100.0 };
	pointAttr.setBaseSize(1.0, vec_size);
	Material	blue = materialmodule.findMaterialByName("orange");

	axes.setMaterial(blue);
	axes.setVisibilityFlag(true);




	Sceneviewermodule a = context.getSceneviewermodule();
	Sceneviewer sceneviewer = a.createSceneviewer(Sceneviewer::BUFFERING_MODE_DOUBLE, Sceneviewer::STEREO_MODE_DEFAULT);

	sceneviewer.setProjectionMode(Sceneviewer::PROJECTION_MODE_PERSPECTIVE);

	Scenefiltermodule filter_module = context.getScenefiltermodule();

	Scenefilter graphics_filter = filter_module.createScenefilterVisibilityFlags();

	scene.endChange();
	//# Set the graphics filter for the scene viewer otherwise nothing will be visible.
	sceneviewer.setScenefilter(graphics_filter);
	sceneviewer.setScene(scene);

	//double rgb[3] = { 0, 0, 255 };
	//sceneviewer.setBackgroundColourRGB(rgb);
	//sceneviewer.set
	sceneviewer.setViewportSize(1000, 1000);
	//sceneviewer.viewAll();

	//Sceneviewer global_scene2(sceneviewer);
	// Render now
	global_scene = sceneviewer;
	global_field_module = fieldmodule;

	//looping through some nodes
	Nodeset node = fieldmodule.findNodesetByFieldDomainType(Field::DOMAIN_TYPE_NODES);
	Nodeiterator node_it = node.createNodeiterator();

	Node node_selected = node_it.next();
	Fieldcache cache = fieldmodule.createFieldcache();
	double mesh_pos[3] = { 0, 0, 0 };
	double outvalues[3];
	/*while (node_selected.isValid()){
	cache.setNode(node_selected);
	coordinates.evaluateReal(cache, 3, outvalues);
	mesh_pos[0] = outvalues[1];
	mesh_pos[2] = outvalues[2];
	mesh_pos[1] = outvalues[3];
	coordinates.assignReal(cache, 3, mesh_pos);
	std::cout
	<<"x: "<< outvalues[0]<< " "
	<<"y: "<< outvalues[1] << " "
	<<"z: "<< outvalues[2]
	<< std::endl;
	node_selected=node_it.next();

	}*/
	g_p_materialmodule = &materialmodule;
	Nodeset node_set = fieldmodule.findNodesetByFieldDomainType(Field::DOMAIN_TYPE_NODES);
	Nodeiterator node_iterator = node_set.createNodeiterator();
	Node node_test = node_iterator.next();
	g_p_scene = &scene;
	int size_node = node_set.getSize();
	std::cout << "size of nodes: " << size_node << std::endl;

#endif // 0
	
	glutDisplayFunc(display);
	glutIdleFunc(display);
	glutMainLoop();

	return 0;
}