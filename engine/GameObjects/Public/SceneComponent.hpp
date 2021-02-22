#include "BaseComponent.hpp"
#include "Vector.hpp"
//#include "SceneManager.hpp"
//#include "Scene.hpp"
//#include "Scene.hpp"
#ifndef SceneComponent_hpp
#define SceneComponent_hpp
namespace ez{
class Scene;
enum TickTime
	{
		/** When the Actor or Component is explicitly destroyed. */
		BeginFrame,
		/** When the world is being unloaded for a level transition. */
		DuringFrame,
		/** When the world is being unloaded because PIE is ending. */
		EndFrame,
};
enum ERelativeTransformSpace
{
	/** World space transform. */
	RTS_World,
	/** Actor space transform. */
	RTS_Actor,
	/** Component space transform. */
	RTS_Component,
	/** Parent bone space transform */
	RTS_ParentBoneSpace,
};

/***
* Scene Component is an Actor Component that exists at a specific physical position in the world. T
***/

class SceneComponent : public BaseComponent
{
public:
	SceneComponent(Scene *scene=nullptr);
	virtual ~SceneComponent(){ };
protected:
	Scene *_scene;
	TickTime _tickMethod=TickTime::BeginFrame;
	SceneComponent *_owner = nullptr;
	std::vector<SceneComponent*> _childs;
    /** Location of the component relative to its parent */
	Vector3<float> RelativeLocation= Vector3<float>(0, 0, 0);

	/** Rotation of the component relative to its parent */
	Vector3<float> RelativeRotation= Vector3<float>(0, 0, 0);

	/**
	*	Non-uniform scaling of the component relative to its parent.
	*	Note that scaling is always applied in local space (no shearing etc)
	*/
	Vector3<float> RelativeScale3D= Vector3<float>(0, 0, 0);

    /** If RelativeLocation should be considered relative to the world, rather than the parent */
	bool bAbsoluteLocation=true;
    
    /** If RelativeRotation should be considered relative to the world, rather than the parent */
	bool bAbsoluteRotation=true;

	/** If RelativeScale3D should be considered relative to the world, rather than the parent */
	bool bAbsoluteScale=true;

    /** Whether to completely draw the primitive; if false, the primitive is not drawn, does not cast a shadow. */
	bool bVisible=true;
	
	/** Indicates that BeginPlay has been called, but EndPlay has not yet */
	bool bHasBegunPlay=false;

	bool bIsBeingDestroyed=false;

	//Scene *_scene;

public:
	Scene *getComponentScene() { return _scene; }
	void SetComponentScene(Scene *scene) { _scene = scene; }
	SceneComponent *getOwner() { return _owner; }
	TickTime getTickTime() { return _tickMethod; }
	virtual bool OnEvent(Event& event) ; 

	void OnRegister() ;
	void Update() ;

	void DestroyComponent(bool bPromoteChildren);

	    /**
	 * Function called every frame on this ActorComponent. Override this function to implement custom logic to be executed every frame.
	 * Only executes if the component is registered, and also PrimaryComponentTick.bCanEverTick must be set to true.
	 *	
	 * @param DeltaTime - The time since the last tick.
	 */
	virtual void TickComponent(float DeltaTime);
	
	/** Returns true if this component is visible in the current context */
	virtual bool IsVisible() const;

    /** 
	* Initializes desired Attach Parent and SocketName to be attached to when the component is registered.
	* Generally intended to be called from its Owning Actor's constructor and should be preferred over AttachToComponent when
	* a component is not registered.
	* @param  InParent				Parent to attach to.
	*/

	bool SetupAttachment(SceneComponent* InChild);
    
	Vector3<float> GetLocation();
	Vector3<float> GetRotation();
	Vector3<float> GetScale();

	void SetLocation(Vector3<float> loc);
	void GetRotation(Vector3<float> rot);
	void GetScale(Vector3<float> scale);
	
	/**
	* Attach this component to another scene component, optionally at a named socket. It is valid to call this on components whether or not they have been Registered, however from
	* constructor or when not registered it is preferable to use SetupAttachment.
	* @param  Parent				Parent to attach to.
	* @return True if attachment is successful (or already attached to requested parent/socket), false if attachment is rejected and there is no change in AttachParent.
	*/
	bool AttachToComponent(SceneComponent* InParent);

	std::vector<SceneComponent*> getChilds() const;

	void setComponentVisibleOrUnvisible(bool isVisible);


    /** 
	 * Detach this component from whatever it is attached to. Automatically unwelds components that are welded together (See WeldTo)
	 */
	virtual bool DetachFromComponent();


/**
	 * Ends gameplay for this component.
	 * Called from AActor::EndPlay only if bHasBegunPlay is true
	 */
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason);

	/**
	 * Begins Play for the component. 
	 * Called when the owning Actor begins play or when the component is created if the Actor has already begun play.
	 * Actor BeginPlay normally happens right after PostInitializeComponents but can be delayed for networked or child actors.
	 * Requires component to be registered and initialized.
	 */
	virtual void BeginPlay();

  
    /** Called when a component is created (not loaded). This can happen in the editor or during gameplay */
	virtual void OnComponentCreated();

	/** 
	 * Called when a component is destroyed
	 * 
	 * @param	bDestroyingHierarchy  - True if the entire component hierarchy is being torn down, allows avoiding expensive operations
	 */
	virtual void OnComponentDestroyed(bool bDestroyingHierarchy);

	virtual bool RemoveOwnedComponent(SceneComponent *component);

};
}

#endif