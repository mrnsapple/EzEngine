#include <iostream>
#include <fstream>
#include <mutex>
#include <fstream>
#include <cstdio>

#include "fileIOFailed.hpp"
#include "Resource/Resource.hpp"
#ifndef FileIO_hpp
#define FileIO_hpp

namespace ez {

	class ResourceData;

class FileIO
{
public:
    static bool saveBinary(std::string const  &path, std::string const  &filecontent);
    
    static void* loadBinary(std::string const  &path, size_t const size);
    
    static bool RemoveFile(std::string const  &path);
    
    static std::string loadFile(std::string const  &path);
    
    template<typename T>
    bool saveFile(std::string const  &path, T *t);

    struct memory_file {
    	int		fd;
    };

    static memory_file	*createFileFromMemory(const ResourceData &data);
    static void 		deleteFileFromMemory(memory_file **memory);

private:
    //std::mutex _mutex;
};
}
#include "../Private/FileIO.tpp"
#endif /* ResourceAllocator_hpp */