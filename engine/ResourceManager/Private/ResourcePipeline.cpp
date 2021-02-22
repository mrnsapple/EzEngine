#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include "FileIO.hpp"
#include <utility>
#include "../Public/Resource/Resource.hpp"
#include "../Public/Resource/OBJResource.hpp"
#include "../Public/Resource/ImageResource.hpp"

#include "log.hpp"
#include "../Public/ResourcePipeline.hpp"
#ifndef RESOURCE_PIPELINE_CPP
#define RESOURCE_PIPELINE_CPP

#include "Resource/ResourceData.hpp"

namespace ez {
ResourcePipeline::ResourcePipeline()
{
    _checker_map["wav"] = std::bind(&Checker::checkWavFile, _checker,std::placeholders::_1);
    _checker_map["obj"] = std::bind(&Checker::checkOBJFile, _checker,std::placeholders::_1);
    _checker_map["jpeg"] = std::bind(&Checker::checkIMGFile, _checker,std::placeholders::_1);
    _checker_map["png"] = std::bind(&Checker::checkIMGFile, _checker,std::placeholders::_1);
    _checker_map["jpg"] = std::bind(&Checker::checkIMGFile, _checker,std::placeholders::_1);

    _resource_map["obj"] = &OBJResource::Create;
    _resource_map_path["jpeg"] = &ImageResource::CreateFromPath;
    _resource_map_path["jpg"] = &ImageResource::CreateFromPath;
    _resource_map_path["png"] = &ImageResource::CreateFromPath;

    _resource_map_path["wav"] = &AudioOutputResource::CreateFromPath;

}
    

std::tuple<std::string, size_t, std::string> ResourcePipeline::saveResourceData(std::string path, std::string binarydatapath)
{
    LOG_DETAIL(EZ_TRACE, "Saving resource in ResourcePipeline ", __FILE__, __LINE__);
    std::string file_type = getFileType(path);
    auto checker_func = _checker_map.find(file_type);

    if (checker_func == end(_checker_map) || !((checker_func->second))(path)) {
       LOG_DETAIL(EZ_TRACE, "Checker FAIL for file:" + path + " ", __FILE__, __LINE__);

        return std::make_tuple("",-1,"");
    }
    LOG_DETAIL(EZ_TRACE, "Checker SUCCESS for file:" + path + " ", __FILE__, __LINE__);

    std::string filecontent = FileIO::loadFile(path);
    std::string xmlpath = ( binarydatapath.empty()) ? this->generateBinaryDataPath(path): binarydatapath;
    if (FileIO::saveBinary(xmlpath, filecontent ))
        return std::make_tuple(xmlpath,filecontent.size(), file_type);
    return std::make_tuple("",-1,"");
}

void* ResourcePipeline::loadResourceData(std::string path, size_t size)
{
    void *a = FileIO::loadBinary(path, size);
    return a;
}

Resource *ResourcePipeline::loadResource(const std::string &path, const std::string type)
{
    LOG_DETAIL(EZ_TRACE, "In loadResource ", __FILE__, __LINE__);
    auto resource_createfunc = _resource_map_path.find(type);
    if (resource_createfunc == end(_resource_map_path))
        return nullptr;
    Resource *res = resource_createfunc->second(path);
    return res;
}


Resource *ResourcePipeline::loadResource(ResourceData &data,const std::string type)
{
    LOG_DETAIL(EZ_TRACE, "In loadResource ", __FILE__, __LINE__);

    auto resource_createfunc = _resource_map.find(type);
    if (resource_createfunc == end(_resource_map))
        return nullptr;
    Resource *res = resource_createfunc->second(data);
    return res;
}


std::string ResourcePipeline::generateBinaryDataPath(std::string path)
{
    std::string finalpath;
    vector<string> strings;
    istringstream f(path);
    string s;    
    while (getline(f, s, '.')) {
        strings.push_back(s);
    }
    
    finalpath = strings[0] + ".xml";
    return finalpath;
}

std::string ResourcePipeline::getFileType(std::string path)
{
    vector<string> strings;
    istringstream f(path);
    string s;    
    while (getline(f, s, '.'))
        strings.push_back(s);
    return strings.back();
}
}
#endif