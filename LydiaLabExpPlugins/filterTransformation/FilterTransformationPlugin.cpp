#include "FilterTransformationPlugin.h"

#include <iostream>

#include <parameters/RichParameterWidget.h>

/**
 * @brief FilterTransformationPlugin::initParameters
 * @param richParameterSet
 */
void FilterTransformationPlugin::initParameters(RichParameterSet *richParameterSet)
{
    richParameterSet->addParam(new RichInt("test"));
    std::cout << "initParameters:" << __FUNCTION__ << __LINE__ << std::endl;
}

/**
 * @brief FilterTransformationPlugin::applyFilter
 * @param richParameterSet
 */
void FilterTransformationPlugin::applyFilter(RichParameterSet *richParameterSet)
{
    std::cout << "applyFilter:" << __FUNCTION__ << __LINE__ << std::endl;
}
