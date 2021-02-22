#include "../Public/SceneComponent.hpp"
#include "log.hpp"
namespace ez{

SceneComponent::SceneComponent(Scene *scene) :_scene(scene)
{
}

Vector3<float> SceneComponent::GetLocation()
{
	return RelativeLocation;
}

Vector3<float> SceneComponent::GetRotation()
{
	return RelativeRotation;
}

Vector3<float> SceneComponent::GetScale()
{
	return RelativeScale3D;
}

void SceneComponent::SetLocation(Vector3<float> loc)
{
	RelativeLocation = loc;
}

void SceneComponent::GetRotation(Vector3<float> rot)
{
	RelativeRotation = rot;
}

void SceneComponent::GetScale(Vector3<float> scale)
{
	RelativeScale3D = scale;
}

void SceneComponent::DestroyComponent(bool bPromoteChildren)
{
	if (bIsBeingDestroyed)
		return;
	bIsBeingDestroyed = true;
	if (bHasBegunPlay) {
		EndPlay(EEndPlayReason::Destroyed);
	}
	// Ensure that we call UninitializeComponent before we destroy this component
	if (isRegistered()) {
		OnUnregister();
	}
	if (_owner) {
		//_owner->RemoveOwnedComponent(std::make_shared<SceneComponent>(this));
	}
	//Send event to scene about component destroyed
	OnComponentDestroyed(false);

}



bool SceneComponent::IsVisible() const
{
	return bVisible;
}

bool SceneComponent::SetupAttachment(SceneComponent* InChild)
{
	if (!InChild || InChild == this)
		return false;
	_childs.push_back(InChild);
	InChild->AttachToComponent(this);
	return true;
}

bool SceneComponent::AttachToComponent(SceneComponent* InParent)
{
	if (InParent) {
		_owner = InParent;
		return true;
	}
	return false;
}


bool SceneComponent::DetachFromComponent()
{
	if (_owner)
		return _owner->RemoveOwnedComponent((this));
	return false;
}

void SceneComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	bHasBegunPlay=false;
}

void SceneComponent::BeginPlay()
{
	bHasBegunPlay = true;
}


bool SceneComponent::RemoveOwnedComponent(SceneComponent* component)
{
	for (auto it = begin (_childs); it != end (_childs); ++it) {
	 	if(*it == component) {
	 		_childs.erase(it);
	 	}
	}
	return true;
}


bool SceneComponent::OnEvent(Event& event)
{
	std::cout << "AN EVENT ENTER IN SCENECOMPONENT LUL" <<std::endl;

	return false;
}


void SceneComponent::Update() 
{

}

void SceneComponent::OnComponentCreated()
{

}

void SceneComponent::OnComponentDestroyed(bool bDestroyingHierarchy)
{

}

// void SceneComponent::OnRegister()
// {
    
// 	bRegistered = true;
// 	// if (_owner == nullptr || _owner->IsActivated())
// 	// 	bIsActive = true;
// }

std::vector<SceneComponent*> SceneComponent::getChilds() const
{
	return this->_childs;
}

void SceneComponent::setComponentVisibleOrUnvisible(bool isVisible)
{
	this->bVisible = isVisible;
}

void SceneComponent::TickComponent(float DeltaTime)
{
	// LOG(EZ_INFO, "TICK COMPONENT");

}

}