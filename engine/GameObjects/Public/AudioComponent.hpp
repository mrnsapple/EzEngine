#include "SceneComponent.hpp"
#include "Scene.hpp"
#include "Event.hpp"
#include "AudioEvent.hpp"
#include "AudioOutputResource.hpp"
#include "Collision.hpp"
#ifndef AUDIOCOMPONENT_HPP
#define AUDIOCOMPONENT_HPP
namespace ez{
class AudioComponent : public SceneComponent
{
private:
    Sphere _spherecoll;
    AudioOutputResource *_audioResource;
public:
    void playSound();
    void setAudioResource(AudioOutputResource *audioResource) { _audioResource = audioResource; }
    void setSphereColl(Sphere &spherecoll) { _spherecoll = spherecoll; }
    Sphere &getSphereColl() { return _spherecoll; }
    void TickComponent(float DeltaTime) override;
    bool OnEvent(Event& event) override;
};
}
#endif
