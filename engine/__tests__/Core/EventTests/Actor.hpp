/**
 * @file "Core/EventTests/Actor.hpp"
 *
 * @author Oriol Manzano
 */

#include "Event.hpp"
#include <bitset>
#include <mutex>
#include "MouseEvent.hpp"
#include <iostream>
namespace ez{

class BaseActor
{
public:
    virtual bool OnEvent(Event& event) {
        return false;
    }
    virtual void SetRegisteTypes() = 0;



};

class SomeActor : public BaseActor
{
    public:
        SomeActor()
        {
            SetRegisteTypes();
        }
        bool OnEvent(Event& event) override {
            std::lock_guard<std::mutex> guard(_mutex);
            //Check class does ot use event or base class does not return true
            if ( ((int)event._type) == 0 || BaseActor::OnEvent(event) || event.Handled)
                return true;
            EventDispatcher dispatcher(event);
            std::function<bool( SomeActor &, Event &)> func;
            switch (event._type){
                case EventType::KeyPressed:
                    func = &SomeActor::RespondToAtach;
                    dispatcher.Dispatch(func, this);
                    return false;// ok forward event to others
                case EventType::MouseButtonPressed:
                    func = &SomeActor::SetDamage;

                    dispatcher.Dispatch(func, this);
                    return true;//I consume event, don't forward
                case EventType::MouseMoved:
                    func = &SomeActor::SetDamageConsumeEvent;
                    dispatcher.Dispatch(func, this);

                    if(!event.Handled)
                        dispatcher.Dispatch(func, this);
                    return true;
                default:
                    return false;//I do not consume event. do not forward
            }
        }

        bool RespondToAtach(Event& event)
        {  
            EventDispatcher dispatcher(event);
            _atack += 33;
            std::function<bool( SomeActor &, Event &)> funce = &SomeActor::SetDamage;

            dispatcher.Dispatch(funce, this);
            return true;
        }
       
        
        bool SetDamageConsumeEvent(Event &event) 
        {
            _atack += 44;
            return true;
        }
        bool SetDamage(Event &event) 
        {
            _atack += 44;
            return false;
        }

        void SetRegisteTypes()
        {
            _registerTypes[(int)EventType::KeyPressed] = 1;
            _registerTypes[(int)EventType::MouseButtonPressed] = 1;
        }
        int getAtack()
        {
            return _atack;
        }
    private:
        int _atack = 0;
        std::mutex _mutex;

        //array of 0/1 values to determine to which events this Actor can register.
        std::bitset<(int)EventType::MAX> _registerTypes;
};
}