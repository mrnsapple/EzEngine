
set(RESOURCE_MANAGER 
        ResourceManager/Private/ResourceManager.cpp
        ResourceManager/Private/ResourceRegistry.cpp
        ResourceManager/Private/ResourcePipeline.cpp
        ResourceManager/Private/Checker/Checker.cpp
        ResourceManager/Public/Checker/Checker.hpp
        ResourceManager/Private/Resource/OBJResource.cpp
        ResourceManager/Private/Resource/AudioOutputResource.cpp
        ResourceManager/Public/Resource/OBJResource.hpp
        ResourceManager/Private/Resource/ResourceData.cpp
        ResourceManager/Public/Resource/Resource.hpp
        ResourceManager/Private/Resource/ImageResource.cpp
        ResourceManager/Public/ResourceManager.hpp
        ResourceManager/Public/ResourcePipeline.hpp
        ResourceManager/Public/ResourceRegistry.hpp)

set(RESOURCE_MANAGER_INCLUDES ResourceManager/Public ResourceManager/Public/Resource)