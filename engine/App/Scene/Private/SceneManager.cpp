#include "../Public/SceneManager.hpp"
namespace ez {
SceneManager::SceneManager()
{

}

void  SceneManager::CreateScene(bool makeActive)
{
    std::unique_ptr<Scene> new_scene(new Scene());
    AddScene(std::move(new_scene), makeActive);
}

void SceneManager::AddScene(std::unique_ptr<Scene> scene, bool makeActive)
{
    if (makeActive) {
        _activeScene = (scene.get());
    }
     
     _scenes.push_back(std::move(scene));
}

void SceneManager::LoadScene(std::shared_ptr<UniqueID> sceneID)
{
    if (!sceneID.get())
        return;
    for (auto it = begin(_scenes); it != end(_scenes); ++it)
        if ((*it)->getID().get() == sceneID.get()) {
            //_activeScene = std::move(*it);
            _scenes.erase(it);
        }
}


}