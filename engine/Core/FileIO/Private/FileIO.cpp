#include "FileIO.hpp"
#include "../../../config.hpp"
#include "log.hpp"
#include "Resource/ResourceData.hpp"

namespace ez {
	bool FileIO::RemoveFile(std::string const  &path) {
		if (std::remove(path.c_str()) != 0)
			return false;
		return true;
	}

	std::string FileIO::loadFile(std::string const  &path) {
		std::ifstream ifs(path);
		std::string content((std::istreambuf_iterator<char>(ifs)),
							(std::istreambuf_iterator<char>()));
		return content;
	}


	void *FileIO::loadBinary(std::string const  &path, size_t size) {

		std::ifstream rf(path, std::ios::out | std::ios::binary);

		if (!rf)
			throw fileIOFailed("can not open file for load, filepath:" + path, __FILE__, __LINE__);
		char *memblock = memblock = new char[size];

		rf.read(memblock, size);
		if (!rf.good()) 
			throw fileIOFailed("can not read file, filepath:" + path, __FILE__, __LINE__);
		rf.close();
		return memblock;
	}

	bool FileIO::saveBinary(std::string const  &path, std::string const  &filecontent) {
		std::mutex _mutex;

		std::unique_lock<std::mutex> _lock(_mutex);

		LOG(EZ_TRACE, "Saving to file: " + path + ", information:" + std::to_string(filecontent.size()));
	
		std::ofstream wf(path, std::ios::out | std::ios::binary);
		if (!wf)
        	throw fileIOFailed("can not open file for save it, filepath:" + path, __FILE__, __LINE__);
		wf.write(filecontent.c_str(), filecontent.size());
		if(!wf.good())
			throw fileIOFailed("can not write in file, filepath:" + path, __FILE__, __LINE__);
    	wf.close();
		return true;
	}
}

#if defined(EZ_OS_WINDOWS)
#	include <io.h>
#	include <fcntl.h>

namespace ez {

	struct fileWindowMemory {
		FileIO::memory_file _memory;
		HANDLE _handle;
	};

	FileIO::memory_file *FileIO::createFileFromMemory(const ResourceData &data) {
		fileWindowMemory _memory;
		char path[MAX_PATH + 1];
		char filepath[MAX_PATH + 5];

		if (!GetTempPath(MAX_PATH, path))
			throw std::logic_error(""); //TODO
		if (!GetTempFileName(path, "_ez_", 0, filepath))
			throw std::logic_error("");

		_memory._handle = CreateFile(filepath, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_ALWAYS,
									 FILE_ATTRIBUTE_TEMPORARY | FILE_FLAG_DELETE_ON_CLOSE, NULL);
		if (_memory._handle == INVALID_HANDLE_VALUE)
			throw std::logic_error("");

		if (WriteFile(_memory._handle, data.getData(), (DWORD) data.getSize(), 0, 0) == FALSE)
			throw std::logic_error("");

		if ((_memory._memory.fd = _open_osfhandle((intptr_t) _memory._handle, _O_RDONLY)) == -1)
			throw std::logic_error("");

		fileWindowMemory *mem = new fileWindowMemory;
		std::memcpy(mem, &_memory, sizeof(fileWindowMemory));
		return reinterpret_cast<memory_file *>(mem);
	}

	void FileIO::deleteFileFromMemory(memory_file **memory) {
		_close((*memory)->fd);
		delete reinterpret_cast<fileWindowMemory *>(*memory);
		*memory = nullptr;
	}
}
#elif defined(EZ_OS_LINUX)

#include <fcntl.h>
#include <unistd.h>

namespace ez
{
	struct fileLinuxMemory {
		FileIO::memory_file _memory;
		void* _handle;
	};

	FileIO::memory_file * FileIO::createFileFromMemory(const ResourceData &data)
	{
		memory_file	_memory;

		if ((_memory.fd = open("/tmp/", O_TMPFILE | O_RDWR)) == -1)
			throw std::logic_error("");
		if (write(_memory.fd, data.getData(), data.getSize()) == -1)
			throw std::logic_error("");

		auto *mem = new memory_file(_memory);
		return mem;
	}

	void FileIO::deleteFileFromMemory(memory_file **memory)
	{
		close((*memory)->fd);
		delete *memory;
		*memory = nullptr;
	}
}

#endif
