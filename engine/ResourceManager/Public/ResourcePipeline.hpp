#include "ResourceRegistry.hpp"
#include "FileIO.hpp"
#include "Resource/Resource.hpp"
#include "Checker/Checker.hpp"
#include "Resource/AudioOutputResource.hpp"
#include "Resource/OBJResource.hpp"
#include <functional>
#include "log.hpp"
#ifndef ResourcePipeline_hpp
#define ResourcePipeline_hpp

using namespace std;
namespace ez {

typedef std::map<std::string, std::function<Resource *( ResourceData &)>> resource_map_t;
typedef std::map<std::string, std::function<bool(const std::string &)>> checker_map_t;
typedef std::map<std::string, std::function<Resource *( const std::string &)>> resource_map_path_t;

class ResourcePipeline
{

public:
    ResourcePipeline();
    /**
     * @brief This method will save a resource
     * @tparam T The resource, it would usually be Resource class
     * @param path the path to the resource file(the  .obj,.png,.jpeg,.mp3)
     * @param binarydatapath the path to save the binary resource data, if empty it will be saved as an xml at the same location of path 
     * @return the binary path and the resource
     */
    std::tuple<std::string, size_t,std::string> saveResourceData(std::string path, std::string binarydatapath="");
    
    /**
     * @brief This method will create a resource from the data stored in path
     * @tparam T The resource, it would usually be Resource class
     * @param path the path to binary resource data
     * @return the resource
     */
    void* loadResourceData(std::string path, size_t size);


    /**
     * @brief This method will create a resource
     * @param resource the resource binary data
     * @param size the resource size
     * @param type the resource type to be created
     * @return the resource created
     */
    Resource *loadResource(ResourceData &data, const std::string type);
    
     /**
     * @brief This method will create a resource
     * @param path resource path
     * @return the resource created
     */
    Resource *loadResource(const std::string &path, const std::string type);


    /**
     * @brief This method will create a resource
     * @param path resource path
     * @return the resource type, "/geo/myobj.obj" will return "obj"
     */
    std::string getFileType(std::string path);

private:

    /**
     * @brief This method will generate the location of the binarydatapath of the resource given as parameters
     * it would be an xml in the same location as path.
     * For instance if path is /Game/Resources/image.png the result will be /Game/Resources/image.xml
     * @param path the path to the resource
     * @return the binary resource data path
     */
    std::string generateBinaryDataPath(std::string path);

    checker_map_t _checker_map;
    resource_map_t _resource_map;
    resource_map_path_t _resource_map_path;

    Checker _checker;
};
}
#endif /* ResourceAllocator_hpp */
