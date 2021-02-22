/**
 * @file "App/SceneManager/Public/SceneManager.hpp"
 *
 * @author Remy Bouvard
 */

#include <iostream>
#include <vector>
#include "Scene.hpp"

#ifndef SCENEMANAGER_HPP_
#define SCENEMANAGER_HPP_
namespace ez{
class SceneManager {
public:
    SceneManager();

    /**
     * Creates a scene
     * @param makeActive make the new scene the current active scene
    */
    void CreateScene(bool makeActive);
    
    /**
     * Get a scene
     * @param sceneID the id of the scene to retrieve
     * @return The scene asked
    */
    Scene *GetScene(std::shared_ptr<UniqueID> sceneID);
    
    /**
     * Add a scene
     * @param scene the new scene
     * @param makeActive make the new scene the current active scene
    */
    void AddScene(std::unique_ptr<Scene> scene, bool makeActive);

    /**
     * make scene of the id current scene
     * @param sceneID id of the scene to make current
    */
    void LoadScene(std::shared_ptr<UniqueID> sceneID);

    Scene *getActiveScene() { return (_activeScene); }
    Scene *_activeScene;

private:
    std::vector<std::unique_ptr<Scene>> _scenes;
};
}
#endif //SCENEMANAGER_HPP_
