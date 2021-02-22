#include "../Public/BaseComponent.hpp"
namespace ez{



void BaseComponent::OnRegister()
{
	bRegistered = true;
	bIsActive = true;
}

void BaseComponent::OnUnregister()
{
	bRegistered = false;

}

bool BaseComponent::IsActivated()
{
	return bIsActive;
}


bool BaseComponent::isRegistered()
{
	return bRegistered;
}


}