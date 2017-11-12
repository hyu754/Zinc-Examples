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
#include "zinc/fieldsceneviewerprojection.hpp"

#include "zinc/stream.h"


#ifdef OPENCV
#include "opencv2\highgui.hpp"
#include "opencv2\opencv.hpp"
#endif

#include <iostream>

#include <ctime>

#include "zinc_wrapper.hpp"



//Global zinc_wrapper pointer
zinc_wrapper *g_ZW_ptr;


using namespace OpenCMISS::Zinc;
using namespace std;
////Global video capture pointer
//cv::VideoCapture *g_p_cap;
//Context context("example");
//Sceneviewer global_scene;
//double eye1[3] = { 100.0, 10.0, 0.0 };
//Sceneviewer global_scene2;
//Fieldmodule global_field_module;
//Field global_coordinates;
//FieldImage gl_field_image;
//double i = 0;
//
//StreaminformationImage *global_imagestream;
//Scene *g_p_scene;
//Materialmodule *g_p_materialmodule;
//cv::Mat in_image = cv::Mat(cv::Size(640, 480), CV_8UC3);// = cv::Mat(cv::Size(640, 480), CV_8UC4, buff.data, cv::Mat::AUTO_STEP);
////cv::Mat in_image_rgb = cv::Mat(cv::Size(640, 480), CV_8UC3);
//double time_global = 0;
//IplImage *imgO = cvCreateImage(cvSize(640, 480), IPL_DEPTH_8U, 3);


void display(void)
{
	float start_time = std::clock();
	//glutSolidTeapot(1.5);

	//global_scene.viewAll();
	//global_scene.renderScene();
	double angle = 0.00012; // rads
	double R[3][3] = { { cos(angle), 0.0, sin(angle) }, { 0.0, 1.0, 0.0 }, { -sin(angle), 0.0, cos(angle) } };
	double T[3] = {0,0,0 };
	g_ZW_ptr->rotate_translation_geometry("allshapes", R, T, true);
	
	g_ZW_ptr->render_scene(true);
	
	//std::cout << (start_time - std::clock()) / CLOCKS_PER_SEC << std::endl;


}

void intialize_glut(int argc, char** argv, int width, int height){
	//Initalizing glut
	glutInit(&argc, argv);

	//glutInitDisplayMode(GLUT_DOUBLE);
	glutInitDisplayMode(GLUT_RGB);


	glutInitWindowSize(width, height);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("Zinc Visualiser");
}



int main(int argc, char** argv)
{
	//Initialize glut
	int width = 500;
	int height = 500;
	intialize_glut(argc, argv, width, height);
	
	

	//Initialize zinc wrapper and set the viewer size
	zinc_wrapper ZW("region_1");
	ZW.set_scene_viewer_size(width, height);

	ZW.read_exnode_exelem("allshapes");
	
	ZW.add_line_to_scene("allshapes", "coordinates", 1, "yellow", 1);
	ZW.add_surface_to_scene("allshapes", "coordinates", 0.2, "blue");
	ZW.mesh_integrator("allshapes");

	//Assign global zinc_wrapper pointer so glut can access it
	g_ZW_ptr = &ZW;



	//Add display function will loop through 
	glutDisplayFunc(display);
	glutIdleFunc(display);
	glutMainLoop();

	return 0;
}




//
//#if 0
//
////
//Region region = context.getDefaultRegion();
////region.readFile("good.exregion");
//region.readFile("temp.exfile");
////region.readFile("plane.exfile");
//
//Fieldmodule fieldmodule = region.getFieldmodule();
//
///*StreaminformationRegion sir=region.createStreaminformationRegion();
//*/
//
//FieldImage field_image = fieldmodule.createFieldImage();
//
//
//field_image.setName("image_texture");
//StreaminformationImage stream_information = field_image.createStreaminformationImage();
//
///*stream_information.setAttributeReal(StreaminformationImage::Attribute::ATTRIBUTE_RAW_WIDTH_PIXELS, 1);
//stream_information.setAttributeReal(StreaminformationImage::Attribute::ATTRIBUTE_RAW_HEIGHT_PIXELS,1);
//stream_information.setAttributeInteger(StreaminformationImage::Attribute::ATTRIBUTE_BITS_PER_COMPONENT, 32);*/
//stream_information.setFileFormat(StreaminformationImage::FileFormat::FILE_FORMAT_PNG);
//stream_information.createStreamresourceFile("capture.PNG");
//
////global_imagestream = new StreaminformationImage();
////stream_information.setPixelFormat(StreaminformationImage::PixelFormat::PIXEL_FORMAT_RGB);
//
//
////
//field_image.read(stream_information);
//gl_field_image = field_image;
////field_image.setTextureCoordinateHeight(1);
////ffield_image.setTextureCoordinateWidth(1);
///*Scene scene = region.getScene();
//
//Graphics a = scene.getFirstGraphics();
//
//*/
//
//context.getMaterialmodule();
//Glyphmodule glyphmodule = context.getGlyphmodule();
//glyphmodule.defineStandardGlyphs();
//
////----------------------------BEGIN CHANGING FIELD MODEL------------------//
//fieldmodule.beginChange();
//Field heart_field = fieldmodule.findFieldByName("heart");
//Field plane_field = fieldmodule.findFieldByName("plane");
//Field coordinates = fieldmodule.findFieldByName("coordinates");
//
//global_coordinates = coordinates;
//Mesh nodes = fieldmodule.findMeshByDimension(1);
//
//Nodeset dfdummy = fieldmodule.findNodesetByName("heart");
//
//FieldMagnitude mag = fieldmodule.createFieldMagnitude(coordinates);
//mag.setName("mag");
//mag.setManaged(true);
//fieldmodule.endChange();
//
//
////----------------------------BEGIN CHANGING SCENE--------------------------//
//Scene scene = region.getScene();
//scene.beginChange();
//
///*Glyphmodule glyphmodule =scene.getGlyphmodule();
//glyphmodule.getDefaultPointGlyph();*/
//Materialmodule materialmodule = scene.getMaterialmodule();
//materialmodule.defineStandardMaterials();
////View the lines
//GraphicsLines lines = scene.createGraphicsLines();
//lines.setCoordinateField(coordinates);
//lines.setSubgroupField(heart_field);
//Material black = materialmodule.findMaterialByName("red");
////Material image_material = materialmodule.createMaterial();
//
//Tessellationmodule tessmod = scene.getTessellationmodule();
//tessmod.getDefaultTessellation();
//int tes1_vec[3] = { 1, 1, 1 };
//Tessellation tes1 = tessmod.createTessellation();
//tes1.setRefinementFactors(3, tes1_vec);
//lines.setTessellation(tes1);
///*Tessellationmodule tessmod;
//
//
//
//int tes1_vec[3] = { 300, 300, 300 };
//tes1.setRefinementFactors(3, tes1_vec);
//lines.setTessellation(tes1);*/
//
//
//lines.setRenderLineWidth(2.0);
//
//
//lines.setMaterial(black);
//lines.setVisibilityFlag(true);
//Material brown = materialmodule.findMaterialByName("red");
//brown.setAttributeReal(Material::ATTRIBUTE_ALPHA, 0.5);
//GraphicsSurfaces heart_surface = scene.createGraphicsSurfaces();
//heart_surface.setSubgroupField(heart_field);
//heart_surface.setCoordinateField(coordinates);
//heart_surface.setName("heart_surface");
////heart_surface.setExterior(true);
//
//heart_surface.setMaterial(brown);
//heart_surface.setVisibilityFlag(false);
////View the surfaces
//GraphicsSurfaces surfaces = scene.createGraphicsSurfaces();
//surfaces.setSubgroupField(plane_field);
//surfaces.setCoordinateField(coordinates);
//surfaces.setName("image_plane");
//
//Field texture_surface = surfaces.getTextureCoordinateField();
////Material brown = materialmodule.findMaterialByName("gray");
//Material image_material = materialmodule.createMaterial();
//
////field_image.setDomainField(texture_surface);
//image_material.setTextureField(1, field_image);
//image_material.setName("texture");
//double dummy[3] = { 0.9, 0.9, 0.0 };
////brown.setAttributeReal3(Material::ATTRIBUTE_DIFFUSE, dummy);
////brown.setAttributeReal3(Material::ATTRIBUTE_AMBIENT, dummy);
////lines.setRenderLineWidth(2.0);
//Field xi_field = fieldmodule.findFieldByName("xi");
//surfaces.setTextureCoordinateField(xi_field);
//surfaces.setMaterial(image_material);
//
//surfaces.setVisibilityFlag(false);
//
////GraphicsSurfaces heart_surface = scene.createGraphicsSurfaces();
//
////heart_surface.setCoordinateField(heart_field);
////heart_surface.setName("heat_surface");
////heart_surface.setMaterial(brown);
//
//////points
//double vec_size[3] = { 4.0, 4.0, 4.0 };
//GraphicsPoints points = scene.createGraphicsPoints();
//
//points.setCoordinateField(coordinates);
//points.setSubgroupField(heart_field);
//points.setFieldDomainType(Field::DOMAIN_TYPE_NODES);
//points.setName("plane_nodes");
//Material green = materialmodule.findMaterialByName("orange");
//points.setMaterial(green);
//Graphicspointattributes pointpoint = points.getGraphicspointattributes();
//pointpoint.setGlyphShapeType(Glyph::SHAPE_TYPE_CUBE_SOLID);
//double dummy1[3] = { 0.1, 0.1, 0.1 };
//pointpoint.setBaseSize(3, dummy1);
////
//pointpoint.setScaleFactors(10, vec_size);
////points.setVisibilityFlag(true);
//
//
////axis
///*graphics = scene.createGraphicsPoints() yes
//attributes = graphics.getGraphicspointattributes()
//attributes.setGlyphShapeType(Glyph.SHAPE_TYPE_AXES_SOLID)
//# Note: default base size of 0.0 would make axes invisible!
//attributes.setBaseSize([1.0])
//*/
//GraphicsPoints axes = scene.createGraphicsPoints();
//axes.setFieldDomainType(Field::DOMAIN_TYPE_POINT);
//
//Graphicspointattributes	pointAttr = axes.getGraphicspointattributes();
//pointAttr.setGlyphShapeType(Glyph::SHAPE_TYPE_AXES_SOLID);
////double vec_size[3] = { 100.0, 100.0, 100.0 };
//pointAttr.setBaseSize(1.0, vec_size);
//Material	blue = materialmodule.findMaterialByName("orange");
//
//axes.setMaterial(blue);
//axes.setVisibilityFlag(false);
//
//
//
//
//Sceneviewermodule a = context.getSceneviewermodule();
//Sceneviewer sceneviewer = a.createSceneviewer(Sceneviewer::BUFFERING_MODE_DOUBLE, Sceneviewer::STEREO_MODE_DEFAULT);
//
//sceneviewer.setProjectionMode(Sceneviewer::PROJECTION_MODE_PERSPECTIVE);
//
//Scenefiltermodule filter_module = context.getScenefiltermodule();
//
//Scenefilter graphics_filter = filter_module.createScenefilterVisibilityFlags();
//
//scene.endChange();
////# Set the graphics filter for the scene viewer otherwise nothing will be visible.
//sceneviewer.setScenefilter(graphics_filter);
//sceneviewer.setScene(scene);
//
////double rgb[3] = { 0, 0, 255 };
////sceneviewer.setBackgroundColourRGB(rgb);
////sceneviewer.set
//sceneviewer.setViewportSize(1000, 1000);
////sceneviewer.viewAll();
//
////Sceneviewer global_scene2(sceneviewer);
//// Render now
//global_scene = sceneviewer;
//global_field_module = fieldmodule;
//
////looping through some nodes
//Nodeset node = fieldmodule.findNodesetByFieldDomainType(Field::DOMAIN_TYPE_NODES);
//Nodeiterator node_it = node.createNodeiterator();
//
//Node node_selected = node_it.next();
//Fieldcache cache = fieldmodule.createFieldcache();
//double mesh_pos[3] = { 0, 0, 0 };
//double outvalues[3];
///*while (node_selected.isValid()){
//cache.setNode(node_selected);
//coordinates.evaluateReal(cache, 3, outvalues);
//mesh_pos[0] = outvalues[1];
//mesh_pos[2] = outvalues[2];
//mesh_pos[1] = outvalues[3];
//coordinates.assignReal(cache, 3, mesh_pos);
//std::cout
//<<"x: "<< outvalues[0]<< " "
//<<"y: "<< outvalues[1] << " "
//<<"z: "<< outvalues[2]
//<< std::endl;
//node_selected=node_it.next();
//
//}*/
//g_p_materialmodule = &materialmodule;
//Nodeset node_set = fieldmodule.findNodesetByFieldDomainType(Field::DOMAIN_TYPE_NODES);
//Nodeiterator node_iterator = node_set.createNodeiterator();
//Node node_test = node_iterator.next();
//g_p_scene = &scene;
//int size_node = node_set.getSize();
//std::cout << "size of nodes: " << size_node << std::endl;
//
//#endif // 0
