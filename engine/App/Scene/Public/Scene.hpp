/**
 * @file "App/SceneManager/Public/IScene.hpp"
 *
 * @author Remy Bouvard
 */


#include <iostream>
#include <string>
#include <vector>
#include <array>
#include <list>
#include "../../../Core/UniqueID/Public/UniqueID.hpp"
#include "../../../Core/Math/Public/Vector.hpp"
#include "UniqueID.hpp"
#include <math.h>       
#include "SceneComponent.hpp"
#include "PrimitiveComponent.hpp"
#include "Collision.hpp"
#ifndef SCENE_HPP_
#define SCENE_HPP_
#include "ThreadPool.hpp"
namespace ez {



class Scene
{

private:
    //std::unique_ptr<ThreadPool> _threadpool;
    std::vector<SceneComponent *> _gObj;
    std::vector<SceneComponent *> _allComponentsInScene;

    std::shared_ptr<UniqueID> _id;

public:
    Scene();
    ~Scene(){};//  _threadpool.release(); };
    unsigned int getNumberSceneGameObjects();

    unsigned int getNumberSceneComponents();
    /**
     * Update scene components
    */
    void Update();
    
    /**
     * Loads a new game object to the scene
     * @param gobj the root component that makes the game object
    */
    void LoadGameObject(SceneComponent *gobj);

    void LoadChildObject(std::vector<SceneComponent *> childs);

    /**
     * Loads a new game object to the scene
     * @param gobj the root component that makes the game object
    */
    void UnloadGameObject(std::shared_ptr<UniqueID> gobj);

    void UnloadChildsObject(std::vector<SceneComponent*> childs);

    /**
     * Send an event to game objects
     * @param event the event to be sent to game objects
     * @return if event should be sent to other objects
    */
    bool OnEvent(Event &event);

    /**
     * Get scene id
     * @return The scene id
    */
    std::shared_ptr<UniqueID> getID() { return _id; }

    void Tick(TickTime ticktime);
    void TickBeginFrame();
    void TickEndFrame();
    void TickDuringFrame();

    void CreatePrimitiveComponent(std::string const &imagepath, std::string const &path);

    void CreateCubeComponent(std::string const &imagepath);
    // std::vector<SceneComponent>  GetGameObjects(Type);
    // std::vector<SceneComponent *> GetGameObject(uniqueID id);
    std::vector<SceneComponent*> GetClosestGameObjects(Sphere sphere);
    std::vector<Mesh*> GetMeshes();


};
}
#endif //SCENE_HPP_