#include "PrimitiveComponent.hpp"
#include "log.hpp"
#ifndef ROTARYCOMPONENT_HPP
#define ROTARYCOMPONENT_HPP
namespace ez{
class RotaryComponent : public PrimitiveComponent
{
public:
    bool OnEvent(Event& event) override;
    bool Rotate(Event& event);
private:

};
}
#endif