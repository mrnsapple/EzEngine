#include <vector>
#include "UniqueID.hpp"
#include <bitset>
#include <mutex>
#include <iostream>

//#include "Scene.hpp"
#ifndef BASECOMPONENT_HPP
#define BASECOMPONENT_HPP
#include "Event.hpp"
namespace ez{

namespace EEndPlayReason {
	enum Type
	{
		/** When the Actor or Component is explicitly destroyed. */
		Destroyed,
		/** When the world is being unloaded for a level transition. */
		LevelTransition,
		/** When the world is being unloaded because PIE is ending. */
		EndPlayInEditor,
		/** When the level it is a member of is streamed out. */
		RemovedFromWorld,
		/** When the application is being exited. */
		Quit,
	};
}
class BaseComponent 
{
public:
	 ~BaseComponent(){ };

	bool isRegistered();
	bool IsActivated();
    //virtual bool isType() = 0;
    virtual void Update() =0;
	bool OnEvent(Event& event) { return false; }
    //virtual void SetRegisteTypes() = 0;
	std::shared_ptr<UniqueID> getId() { return _id; }
//protected:
  /**
	 * Called when a component is registered, after Scene is set, but before CreateRenderState_Concurrent or OnCreatePhysicsState are called.
	 */
	 void OnRegister();

	/**
	 * Called when a component is unregistered. Called after DestroyRenderState_Concurrent and OnDestroyPhysicsState are called.
	 */
	void OnUnregister();
	
protected:
	//Scene *_scene;
    std::bitset<(int)EventType::MAX> _registerTypes;

    std::shared_ptr<UniqueID> _id;
    //std::vector<IResources> _resources;
    /** 
	 *  Indicates if this ActorComponent is currently registered with a scene. 
	 */
	bool bRegistered=true;


    /** Whether the component is currently active. */
	bool bIsActive=true;
    


};
}
#endif