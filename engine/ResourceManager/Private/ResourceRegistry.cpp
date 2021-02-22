#include "UniqueID.hpp"
#include "../Public/ResourceRegistry.hpp"
#include <iostream>
#include <stdio.h>
//#include <Windows.h>
#include <exception>
namespace ez {
std::shared_ptr<UniqueID> ResourceRegistry::Add(const std::string& filePath, const std::string& binaryPath, size_t size,const std::string &type)
{    

    std::shared_ptr<UniqueID> existingPathId(getIdfromPath(filePath));

    std::shared_ptr<UniqueID> existingBinaryPathId(getIdfromBinaryPath(binaryPath));
    if (!existingPathId.get() && !existingBinaryPathId.get()){
        std::shared_ptr<UniqueID> id = std::shared_ptr<UniqueID>(new UniqueID);
        resources.insert(std::make_pair(id, std::make_tuple(filePath, binaryPath, size, type)));
        return id;
    }
    else if (existingPathId.get() == existingBinaryPathId.get() || existingPathId.get()) {
        //Extingbinarypathid is not null and existingbinarypathid could be
        Update(existingPathId, filePath, binaryPath, size, type);
        return existingPathId;
    }
    //only existinbinarypathid is not null
    Update(existingBinaryPathId, filePath, binaryPath, size, type);
    return existingBinaryPathId;
}

int ResourceRegistry::GetNumResources()
{
    return resources.size();
}

bool ResourceRegistry::Update(std::shared_ptr<UniqueID> const id,  std::string const &filePath,  std::string const &binaryPath, size_t const size,  std::string const &type)
{
    auto entry  = resources.find(id);
    if (entry != end(resources)) {
        entry->second = std::make_tuple(filePath, binaryPath,size, type);
        return true;
    }
    return false;
}

bool ResourceRegistry::Remove(std::shared_ptr<UniqueID> const id)
{
    for (auto it = resources.begin(); it != resources.end(); ++it) {
        if (it->first.get() == id.get()) {
            resources.erase(it->first);
            return true;
        }
    }
    return false;
}

bool ResourceRegistry::HasId(std::shared_ptr<UniqueID> const id)
{
    auto entry  = resources.find(id);
    if (entry != end(resources))
        return true;
    return false;
}

std::string ResourceRegistry::GetBinaryDataPath(std::shared_ptr<UniqueID> const id)
{
    auto it  = resources.find(id);
    if (it != end(resources)) 
        return std::get<1>(it->second);
    return "";
}

std::string ResourceRegistry::GetType(std::string const &binaryDataPath)
{
    std::shared_ptr<UniqueID> id(getIdfromBinaryPath(binaryDataPath));
    return GetType(id);
}

std::string ResourceRegistry::GetType(std::shared_ptr<UniqueID> const id)
{
    auto it  = resources.find(id);
    if (it != end(resources)) 
        return std::get<3>(it->second);
    return "";
}

std::string ResourceRegistry::GetPath(std::shared_ptr<UniqueID> const id)
{
    auto it  = resources.find(id);
    if (it != end(resources)) 
        return std::get<0>(it->second);
    return "";
}

std::size_t ResourceRegistry::GetSize(std::shared_ptr<UniqueID> const id)
{
    auto it  = resources.find(id);
    if (it != end(resources)) 
        return std::get<2>(it->second);
    return -1;
}

std::shared_ptr<UniqueID> ResourceRegistry::getIdfromPath(std::string const &Path)
{
    if (resources.empty()) {
        return std::shared_ptr<UniqueID>();
    }
    for (auto it = resources.begin(); it != resources.end(); ++it) {
        if (std::get<0>(it->second) == Path)
            return it->first;
    }
    return std::shared_ptr<UniqueID>();
}



std::shared_ptr<UniqueID> ResourceRegistry::getIdfromBinaryPath(std::string const &binaryDataPath)
{
     for (auto it = resources.begin(); it != resources.end(); ++it)
        if (std::get<1>(it->second) == binaryDataPath)
            return it->first;
    return std::shared_ptr<UniqueID>();
}


bool ResourceRegistry::changeResourcePath(std::shared_ptr<UniqueID> const id, std::string const &path)
{
    return false;
}   

bool ResourceRegistry::changeResourceBinaryDataPath(std::shared_ptr<UniqueID> const id, std::string const &path)
{
    return false;
}

}