#include "../Public/PlayerComponent.hpp"
#include "Event.hpp"
#include "KeyBoardEvent.hpp"
namespace ez{
bool PlayerComponent::OnEvent(Event& event)
{
    EventDispatcher dispatcher(event);
    std::function<bool( PlayerComponent &, Event &)> func;

    std::cout <<" EVENT IN PLAYER" << std::endl;
    LOG(EZ_TRACE, "PLAYER COMPONENT RECEIVE EVENT ");
    std::cout << static_cast<std::underlying_type<EventType>::type>(event._type)<< std::endl;

    switch (event._type){
        case EventType::NONE:
            LOG(EZ_TRACE, "None event type");
        case EventType::KeyPressed:
            LOG(EZ_TRACE, "KEY PRESED ");
            func = &PlayerComponent::Move;
            dispatcher.Dispatch(func, this);
            return false;// ok forward event to ot
        case EventType::KeyReleased:
            LOG(EZ_TRACE, "KEY Released");
        default:
            return false;
    }
    return false;
}

bool PlayerComponent::Move(Event& event)
{
    LOG(EZ_TRACE, "MOVING CHARACTER ");
    Vector3<float> position = GetLocation();
    std::cout << "Position:" + std::to_string(position.x()) + "," + std::to_string(position.y()) + "," + std::to_string(position.z()) << std::endl;
    KeyboardEvent &e = static_cast<KeyboardEvent&>(event);
    switch (e._key){
        case Keyboard::key::KEY_W:
            position +=  Vector3<float>(0, 1, 0);;
            SetLocation(position);
            LOG(EZ_TRACE, "GO UP");
            break;
        case Keyboard::key::KEY_D:
            position -=  Vector3<float>(1, 0, 0);;
            SetLocation(position);
            LOG(EZ_TRACE, "GO RIGHT");
            break;
        case Keyboard::key::KEY_S:
            position +=  Vector3<float>(0, -1, 0);;
            SetLocation(position);
            LOG(EZ_TRACE, "GO DOWN");
            break;
        case Keyboard::key::KEY_A:
            position +=  Vector3<float>(-1, 0, 0);;
            SetLocation(position);
            LOG(EZ_TRACE, "GO LEFT");
            break;
        default:
            LOG(EZ_TRACE, "UNKNOWN KEY");
            break;

    }

    return false;
}


}