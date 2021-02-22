#include "../Public/ResourceManager.hpp"
#include "Resource/ResourceData.hpp"
#include "log.hpp"

using namespace ez;



ResourceManager::ResourceManager()
{
    LOG(EZ_TRACE, "ResourceManager Constructor");
    _resourceRegistry = new ResourceRegistry;
    _resourcePipeline = new ResourcePipeline;
}


std::pair<std::shared_ptr<UniqueID>, std::string> ResourceManager::saveResource(std::string path, std::string binarydatapath)
{
    LOG_DETAIL(EZ_TRACE, "Saving resource in resourcemanager", __FILE__, __LINE__);

    //save resource in file
    std::tuple<std::string, size_t, std::string>  res = _resourcePipeline->saveResourceData(path,  binarydatapath);
    ifstream f(std::get<0>(res).c_str());
    if (f.good() && std::get<1>(res) > 0) {
       return std::make_pair(_resourceRegistry->Add(path, std::get<0>(res), std::get<1>(res),std::get<2>(res)), std::get<0>(res));
    }
    return std::make_pair(std::shared_ptr<UniqueID>(),"");
}

Resource *ResourceManager::loadResource(std::string binaryPath)
{
    LOG_DETAIL(EZ_TRACE, "Loading resource in resourcemanager, binarypath:" +  binaryPath, __FILE__, __LINE__);

    if (binaryPath.empty())
        return nullptr;
    std::shared_ptr<UniqueID>id(_resourceRegistry->getIdfromBinaryPath(binaryPath));
    if (!id.get()) 
        return nullptr;
    std::size_t size = _resourceRegistry->GetSize(id);
    if (size < 0) 
        return nullptr;
    void *filecontent = _resourcePipeline->loadResourceData(binaryPath, size);
    ResourceData data(filecontent, size);
    std::string resource_type = _resourceRegistry->GetType(id);
    LOG_DETAIL(EZ_TRACE, "Resource type:" +  resource_type + " ", __FILE__, __LINE__);
    Resource *a = _resourcePipeline->loadResource(data, resource_type);
    if (!a) {
        a = _resourcePipeline->loadResource(_resourceRegistry->GetPath(id), resource_type);
    }
    return a;
}

Resource *ResourceManager::loadResource(std::shared_ptr<UniqueID>id)
{
    std::string binaryPath = _resourceRegistry->GetBinaryDataPath(id);
    return loadResource(binaryPath);
}


bool ResourceManager::changeResourcePath(std::shared_ptr<UniqueID>id, std::string path)
{
    return _resourceRegistry->changeResourcePath(id, path);
}   

bool ResourceManager::changeResourceBinaryDataPath(std::shared_ptr<UniqueID>id, std::string path)
{
    return _resourceRegistry->changeResourceBinaryDataPath(id, path);
}

bool ResourceManager::deleteResource(std::shared_ptr<UniqueID>id)
{
    std::string path = _resourceRegistry->GetBinaryDataPath(id);
    FileIO::RemoveFile(path);
    return _resourceRegistry->Remove(id);
}

bool ResourceManager::updateAllResources()
{
    return false;
}

bool ResourceManager::updateResources(std::vector<std::shared_ptr<UniqueID>> ids)
{
    return false;
}

bool ResourceManager::updateResource(std::vector<std::shared_ptr<UniqueID>> ids)
{
    return false;
}