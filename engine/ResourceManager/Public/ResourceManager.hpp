#include "ResourcePipeline.hpp"
#include "Resource/Resource.hpp"
#include "ResourceRegistry.hpp"
#include "UniqueID.hpp"
#include "log.hpp"

#ifndef ResourceManager_hpp
#define ResourceManager_hpp



namespace ez {

// enum class ResourceCopyType
// {
//     Unique,
//     Share
// }
class ResourceManager
{
public:
    ResourceManager();
    /**
     * @brief This method will save a resource
     * @param path the path to the resource file(the  .obj,.png,.jpeg,.mp3)
     * @param binarydatapath the path to save the binary resource data, if empty it will be saved as an xml at the same location of path 
     * @return 
     */
    std::pair<std::shared_ptr<UniqueID> , std::string> saveResource(std::string path, std::string binarydatapath="");
    
    /**
     * @brief This method will load a resource
     * @param binaryPath the path to the resource binary data
     * @return the resource
     */
    Resource *loadResource(std::string binaryPath);
    
    /**
     * @brief This method will load a resource
     * @param id the uniqueId of the reosurce
     * @return the resource
     */
    Resource *loadResource(std::shared_ptr<UniqueID> id);

    /**
     * @brief This method will change the resoure path
     * @param id the uniqueId of the reosurce
     * @return the new path
     */
    bool changeResourcePath(std::shared_ptr<UniqueID> id, std::string path);
    
    /**
     * @brief This method will change the resoure binary path
     * @param id the uniqueId of the reosurce
     * @return the new binary  path
     */
    bool changeResourceBinaryDataPath(std::shared_ptr<UniqueID> id, std::string path);
    
    /**
     * @brief This method will delete the resource
     * @param id the uniqueId of the reosurce
     * @return if the resource has been deleted
     */
    bool deleteResource(std::shared_ptr<UniqueID> id);
    
    /**
     * @brief This method updates all resources
     * @return if the resources have been updated
     */
    bool updateAllResources();

     /**
     * @brief This method updates specific resources
     * @return if the resources have been updated
     */
    bool updateResources(std::vector<std::shared_ptr<UniqueID> > ids);

    /**
     * @brief This method updates a specific resources
     * @return if the resource have been updated
     */
    bool updateResource(std::vector<std::shared_ptr<UniqueID> > ids);
    
    /**
     * @brief This method gets all resources
     * @return The resources
     */
    std::vector<Resource *> getAllResources();
   
  

private:
    ResourceRegistry *_resourceRegistry;
    ResourcePipeline *_resourcePipeline;
};
}
#endif /* ResourceAllocator_hpp */