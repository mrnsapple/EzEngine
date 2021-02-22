

#include <iostream>
#include <fstream>
#include <mutex>
#include <fstream>
#ifndef FileIO_tpp
#define FileIO_tpp

namespace ez {



template<class T>
bool FileIO::saveFile(std::string const  &path, T *t)
{
    std::mutex _mutex;

    std::unique_lock<std::mutex>	_lock(_mutex);

    std::ofstream wf(path, std::ios::out);
    if(!wf) {
        throw fileIOFailed("can not open file for save it, filepath:" + path, __FILE__, __LINE__);
    }
    wf.write((char *) t, sizeof(T));
    if(!wf.good())
        throw fileIOFailed("can not write in file, filepath:" + path, __FILE__, __LINE__);
    wf.close();
    return true;
}


}
#endif /* ResourceAllocator_hpp */