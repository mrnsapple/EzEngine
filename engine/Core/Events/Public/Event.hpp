/**
 * @file "Core/Events/Public/Event.hpp"
 *
 * @author Oriol Manzano
 */

#ifndef EVENT_H_
#define EVENT_H_
#define BIT(x) (1 << x)

#include "Variant.hpp"
#include <cstddef>
#include <iostream>
#include <array>
#include <list>
#include <functional>

namespace ez
{
    class SceneComponent;

    enum EventType
    {
        NONE = 0,
        EVENT_TYPE_LEVEL_STARTED, 
        EVENT_TYPE_PLAYER_SPAWNED,
        EVENT_TYPE_ENEMY_SPOTTED,
        EVENT_TYPE_EXPLOSION,
        EVENT_TYPE_BULLET_HIT,
        KeyPressed, KeyReleased, KeyTyped,
        MouseButtonPressed, MouseButtonReleased, MouseMoved, MouseScrolled,
        AudioPlay, AudioPause, AudioStop,
        MAX,
    };

    enum EventCategory
    {
        None                        = 0,
        EventCategoryApplication    = BIT(0),
        EventCategoryInput          = BIT(1),
        EventCategoryKeyboard       = BIT(2),
        EventCategoryMouse          = BIT(3),
        EventCategoryMouseButton    = BIT(4),
        EventAudio					= BIT(5),
        Max
    };

    struct Event
    {
        EventType _type = EventType::NONE;
        EventCategory _category =  EventCategory::None;

        bool Handled = false;
        std::list<Variant> _variants; 
    };

    

    class EventDispatcher
    {
    public:
        EventDispatcher(Event& event)
            : _event(event)
        {
        }
            
        bool Dispatch( std::function<bool(Event  &)> func)
        {
            //if (_event._type == T::GetStaticType()){
                _event.Handled = func(_event);
                return true;
            //}
            return false;
        }

        template<typename T>
        bool Dispatch( std::function<bool(T&, ez::Event&)>&f, T *c )
        {
            //if (_event._type == T::GetStaticType()){
                _event.Handled = f(*c, _event);
                return true;
            //}
            return false;
        }

    private:
        Event& _event;
    };
}
#endif