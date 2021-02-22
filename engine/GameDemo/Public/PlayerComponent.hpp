#include "SceneComponent.hpp"
#include "log.hpp"
#ifndef PLAYERCOMPONENT_HPP
#define PLAYERCOMPONENT_HPP
namespace ez{
class PlayerComponent : public SceneComponent
{
public:
    bool OnEvent(Event& event) override;
    bool Move(Event& event);
private:

};
}
#endif