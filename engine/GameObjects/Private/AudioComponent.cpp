#include "../Public/AudioComponent.hpp"
namespace ez {
void AudioComponent::TickComponent(float DeltaTime)
{
    if(!_scene)
        return;
    _spherecoll.setCenter(RelativeLocation);
    std::vector<SceneComponent*> neargObj = _scene->GetClosestGameObjects(_spherecoll);
    std::cout << "Near game obj:" << neargObj.size() << std::endl;
    AudioEvent event(EventType::AudioPlay);
    EventDispatcher dispatcher(event);
    
    for (auto it = begin(neargObj); it != end(neargObj); ++it) {
        if ((*it) != this) {
        std::cout << "PLAY AUDIO RESOURCE" << std::endl;
            playSound();
        }
        //(*it)->OnEvent(event);
        //std::function<bool( SceneComponent &, Event &)> func = &SceneComponent::OnEvent;
        //SceneComponent *comp = (*it);
        //dispatcher.Dispatch<SceneComponent>(func, comp);
    }
    if (_audioResource && (neargObj.size( ) <= 0 || (neargObj.size() == 1 && neargObj[0] == this))) {
        _audioResource->play(AudioOutputResource::State::PAUSE);
    }


}

void AudioComponent::playSound()
{
   
    if(!_audioResource || _audioResource->isPlaying()) 
        return;

    _audioResource->play(AudioOutputResource::State::PLAY);
}

bool AudioComponent::OnEvent(Event& event) 
{
    std::cout << "AN EVENT ENTER IN AUDIOCOMPONENT" <<std::endl;
    return false;
}

}