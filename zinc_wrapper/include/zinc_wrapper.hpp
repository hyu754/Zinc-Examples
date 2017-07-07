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

#include "zinc/stream.h"
using namespace OpenCMISS::Zinc;
class zinc_wrapper
{
public:
	zinc_wrapper();
	zinc_wrapper(std::string context_name);


	~zinc_wrapper();

private:

};

zinc_wrapper::zinc_wrapper()
{
	Context context("default_context");
	Region region = context.getDefaultRegion();

	std::cout << "Zinc wrapper class initialized" << std::endl;
}
zinc_wrapper::zinc_wrapper(std::string context_name)
{
	//change context_name to char
	const char *temp_char = context_name.c_str();
	Context context(temp_char);
	Region region = context.getDefaultRegion();

	std::cout << "Zinc wrapper class initialized" << std::endl;
}

zinc_wrapper::~zinc_wrapper()
{
	std::cout << "Zinc wrapper class destroyed" << std::endl;
}

#endif