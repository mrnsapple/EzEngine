#include "../Public/Scene.hpp"
#include "log.hpp"
#include "Init.hpp"
#include "../../GameDemo/Public/RotaryComponent.hpp"
namespace ez {

Scene::Scene()
{
    
}

std::vector<SceneComponent*> Scene::GetClosestGameObjects(Sphere sphere)
{
    std::vector<SceneComponent*> closestGameObj;
    for (auto it = begin(_gObj); it != end(_gObj); ++it) {
        if (Collision::isPointInsideSphere(sphere, (*it)->GetLocation()))
            closestGameObj.push_back(*it);
    }
    return closestGameObj;
}

void Scene::CreateCubeComponent(std::string const &imagepath)
{
	ResourceManager &_manager = Init::getPtr()->getResourceManager();
	std::pair<std::shared_ptr<UniqueID> , std::string> id = _manager.saveResource(imagepath);
    std::string binarypath = std::get<1>(id);
    ImageResource *imageResource =  (ImageResource *)_manager.loadResource(binarypath);
    PrimitiveComponent *primitivecomponent = new PrimitiveComponent;
    primitivecomponent->setImageResource(imageResource);
    Cube myCube;
    primitivecomponent->setMesh(myCube);
    LoadGameObject((SceneComponent *)primitivecomponent);
}

void Scene::CreatePrimitiveComponent(std::string const &imagepath, std::string const &path)
{
    LOG(EZ_TRACE, " Resource ImagePATH: " + imagepath);
	ResourceManager &_manager = Init::getPtr()->getResourceManager();
	std::pair<std::shared_ptr<UniqueID> , std::string> id = _manager.saveResource(imagepath);
    std::string binarypath = std::get<1>(id);
    ImageResource *imageResource =  (ImageResource *)_manager.loadResource(binarypath);
    PrimitiveComponent *primitivecomponent = new PrimitiveComponent;
    primitivecomponent->setImageResource(imageResource);
    Mesh myMesh;
    primitivecomponent->setMesh(myMesh);
    LoadGameObject((SceneComponent *)primitivecomponent);

}


std::vector<Mesh*> Scene::GetMeshes()
{
    std::vector<Mesh*> meshes;
    for (auto it = begin(_allComponentsInScene); it != end(_allComponentsInScene); ++ it) {
        if (dynamic_cast<PrimitiveComponent *>(*it) != nullptr)// || dynamic_cast<RotaryComponent *>(*it) != nullptr)
            meshes.push_back(dynamic_cast<PrimitiveComponent *>(*it)->getMesh());
    }
    return meshes;
}

unsigned int Scene::getNumberSceneGameObjects()
{
    return static_cast<unsigned int>(_gObj.size());
}

unsigned int Scene::getNumberSceneComponents()
{
    return static_cast<unsigned int>(_allComponentsInScene.size());
}

void Scene::Update()
{
    for (auto it = begin(_gObj); it != end(_gObj); ++it)
       (*it)->Update();
    for (auto it = begin(_allComponentsInScene); it != end(_allComponentsInScene); ++ it) {
        this->LoadChildObject((*it)->getChilds());
    }
}

void Scene::LoadGameObject(SceneComponent *gobj)
{
    if (gobj->getOwner()) {
        return ;
    }
    _gObj.push_back(gobj);
    gobj->SetComponentScene(this);
    _allComponentsInScene.push_back(gobj);

    LoadChildObject(gobj->getChilds());

}

void Scene::LoadChildObject(std::vector<SceneComponent*> childs)
{
    // std::cout <<" CHILDs ET SCENE" <<std::endl;

    for (auto it = begin(childs); it != end(childs); ++ it) {
        _allComponentsInScene.push_back(*it);
        size_t size = (*it)->getChilds().size();
        if ((*it)->getChilds().size() > 0) {
            this->LoadChildObject((*it)->getChilds());
        }
    }
}

void Scene::UnloadGameObject(std::shared_ptr<UniqueID> gobj)
{
    for (auto it = begin(_gObj); it != end(_gObj); ++it)
        if ((*it)->getId().get() == gobj.get())
            _gObj.erase(it) ;
}

void Scene::UnloadChildsObject(std::vector<SceneComponent*> childs)
{
    for (auto it = begin(childs); it != end(childs); ++ it) {
        if ((*it)->getChilds().size() != 0)
            this->UnloadChildsObject((*it)->getChilds());
        _allComponentsInScene.erase(it);
    }
}

bool Scene::OnEvent(Event &event)
{
    LOG(EZ_INFO, "Scene retrieve event");
    for (auto it = begin(_gObj); it != end(_gObj); ++it)
       (*it)->OnEvent(event);
    return false;
}

void Scene::Tick(TickTime ticktime)
{
    for (auto it = begin(_allComponentsInScene); it != end(_allComponentsInScene); ++it)
        if ((*it)->getTickTime() == ticktime)
            (*it)->TickComponent(0);
}

void Scene::TickBeginFrame()
{
    Tick(TickTime::BeginFrame);
}

void Scene::TickEndFrame()
{
    Tick(TickTime::EndFrame);

}

void Scene::TickDuringFrame()
{
    Tick(TickTime::DuringFrame);
}

// std::vector<SceneComponent> Scene::GetGameObjects(Type)
// {

// }

// std::vector<IComponent *> Scene::GetGameObject(uniqueID id)
// {
//     for (int i = 0; i < _gObj.size(); i ++ ) {
//         if (_gObj.at(i)._id == id)
//             return _gObj.at(i);
//     }
// }

}