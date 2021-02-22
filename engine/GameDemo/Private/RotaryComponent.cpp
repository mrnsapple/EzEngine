#include "../Public/RotaryComponent.hpp"
#include "Event.hpp"
#include "KeyBoardEvent.hpp"
namespace ez{
bool RotaryComponent::OnEvent(Event& event)
{
    EventDispatcher dispatcher(event);
    std::function<bool( RotaryComponent &, Event &)> func;

    std::cout <<" EVENT IN Rotary" << std::endl;
    LOG(EZ_TRACE, "Rotary COMPONENT RECEIVE EVENT ");
    std::cout << static_cast<std::underlying_type<EventType>::type>(event._type)<< std::endl;
    switch (event._type){
        case EventType::NONE:
            LOG(EZ_TRACE, "None event type");
        case EventType::KeyPressed:
            LOG(EZ_TRACE, "KEY PRESED ");
            func = &RotaryComponent::Rotate;
            dispatcher.Dispatch(func, this);
            return false;// ok forward event to ot
        case EventType::KeyReleased:
            LOG(EZ_TRACE, "KEY Released");
        default:
            return false;
    }
    return false;
}

bool RotaryComponent::Rotate(Event& event)
{
    static auto startTime = std::chrono::high_resolution_clock::now();
    auto currentTime = std::chrono::high_resolution_clock::now();
float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();

    LOG(EZ_TRACE, "Rotate CHARACTER ");
    std::cout << time << std::endl;
    Vector3<float> position = GetLocation();
    std::cout << "Position:" + std::to_string(position.x()) + "," + std::to_string(position.y()) + "," + std::to_string(position.z()) << std::endl;
    KeyboardEvent &e = static_cast<KeyboardEvent&>(event);
    switch (e._key){
        case Keyboard::key::KEY_Y:
            LOG(EZ_TRACE, "ROTATE");
           
            _mesh.mvpObject.model = ez::rotate(makeIdentity<float, 4>(), ez::radians(45.0f) * time, Vector3f(0.0f, 0.0f, 1.0f));
            _mesh.mvpObject.view = ez::lookAt(Vector3f(2.0f, 2.0f, 2.0f), Vector3f(22.0f, 0.0f, 0.0f), Vector3f(0.0f, 0.0f, 1.0f));

            break;
        default:
            LOG(EZ_TRACE, "UNKNOWN KEY");
            break;

    }

    return false;
}


}