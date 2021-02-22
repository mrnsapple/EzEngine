#include "UniqueID.hpp"
#ifndef ResourceRegistry_hpp
#define ResourceRegistry_hpp
#include <tuple>
#include <map>
#include <memory>
#include <string>
#include <vector>
namespace ez {
class ResourceRegistry
{
public:
    ResourceRegistry() {
    }
    std::shared_ptr<UniqueID> Add(const std::string& filePath, const std::string& binaryPath, size_t size, const std::string &type);
    bool Update(std::shared_ptr<UniqueID> id, const std::string& filePath, const std::string& binaryPath,size_t const size, const std::string &type);

    bool Remove(std::shared_ptr<UniqueID> const id);
    
    bool HasId(std::shared_ptr<UniqueID> const  id);
    int GetNumResources();

    std::string GetBinaryDataPath(std::shared_ptr<UniqueID> const id);
    std::string GetPath(std::shared_ptr<UniqueID> const id);
    std::size_t GetSize(std::shared_ptr<UniqueID> const id);
    std::string GetType(std::string const &binaryDataPath);
    std::string GetType(std::shared_ptr<UniqueID> const id);

    std::shared_ptr<UniqueID> getIdfromPath(std::string const &Path);
    std::shared_ptr<UniqueID> getIdfromBinaryPath(std::string const &binaryDataPath);

    bool changeResourcePath(std::shared_ptr<UniqueID> const id, std::string const &path);
    bool changeResourceBinaryDataPath(std::shared_ptr<UniqueID> const id, std::string const &path);

private:
    /**
     * Contain in touple:
     * Original Path, Binary Path, Size Binary, size and type
     */

    std::map<std::shared_ptr<UniqueID> , std::tuple<std::string, std::string, std::size_t, std::string>, std::owner_less<std::shared_ptr<UniqueID>>> resources;
};
}
#endif /* ResourceAllocator_hpp */