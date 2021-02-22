#include "log.hpp"
#include "Init.hpp"
#include "../Public/PlayerComponent.hpp"
#include "AudioComponent.hpp"
#include <filesystem>
#include "Settings.hpp"
#include "PrimitiveComponent.hpp"
#include "../Public/RotaryComponent.hpp"
using namespace ez;
namespace fs = std::filesystem;

void createScene()
{
    std::shared_ptr<Init> i = Init::getPtr();
    i->getSceneManager().CreateScene(true);
}

SceneComponent * createAudioComponent()
{
    std::string  path = fs::current_path().parent_path().parent_path().parent_path().u8string();
	path += "\\engine\\GameDemo\\assets\\LRMonoPhase4.wav";
    LOG(EZ_TRACE, "Audio Resource PATH: " + path);
	ResourceManager &_manager = Init::getPtr()->getResourceManager();
	std::pair<std::shared_ptr<UniqueID> , std::string> id = _manager.saveResource(path);
    std::string binarypath = std::get<1>(id);
    AudioOutputResource *audioOutputResource = (AudioOutputResource *)_manager.loadResource(binarypath);
    std::cout << "AFTER ADIO OUTPUT RESOURCE" << std::endl;

    if (!audioOutputResource) {
        std::cout << "EMPTY ADIO OUTPUT RESOURCE" << std::endl;
        //exit(9);
    }
    AudioComponent *audiocomponent = new AudioComponent;
    Vector3<float> position = audiocomponent->GetLocation();
    position +=  Vector3<float>(0, 10, 0);;
    audiocomponent->SetLocation(position);

    Sphere s;
	s.setRadius(2);
	audiocomponent->setSphereColl(s);
    audiocomponent->setAudioResource(audioOutputResource);
    return (SceneComponent *)audiocomponent;

}



SceneComponent * CreateRotaryComponent(std::string const &imagepath, std::string const &objpath)
{
    LOG(EZ_TRACE, " Resource ImagePATH: " + imagepath);
    LOG(EZ_TRACE, " Resource MeshPATH: " + objpath);
	ResourceManager &_manager = Init::getPtr()->getResourceManager();

    std::pair<std::shared_ptr<UniqueID> , std::string> id = _manager.saveResource(imagepath);
    std::string binarypath = std::get<1>(id);
    ImageResource *imageResource =  (ImageResource *)_manager.loadResource(binarypath);

    id = _manager.saveResource(objpath);
    binarypath = std::get<1>(id);
    OBJResource *objResource = (OBJResource *)_manager.loadResource(binarypath);

    RotaryComponent *primitivecomponent = new RotaryComponent;
    primitivecomponent->setImageResource(imageResource);
    primitivecomponent->setOBJResource(objResource);


    return ((SceneComponent *)primitivecomponent);

}

void createComponents()
{
    std::string  path = fs::current_path().parent_path().parent_path().parent_path().u8string();

    SceneManager &manager =  Init::getPtr()->getSceneManager();
    SceneComponent *component = new SceneComponent;
    SceneComponent *childcomponent = new SceneComponent;
    SceneComponent *player = new PlayerComponent;
    component->SetupAttachment(childcomponent);
    manager._activeScene->LoadGameObject(createAudioComponent());
    manager._activeScene->LoadGameObject(CreateRotaryComponent(path + "\\engine\\GameDemo\\assets\\viking_room.png", path + "\\engine\\GameDemo\\assets\\viking_room.obj"));

     // //manager._activeScene->CreatePrimitiveComponent(path + "\\engine\\GameDemo\\assets\\viking_room.png", path + "\\engine\\GameDemo\\assets\\viking_room.obj");
    manager._activeScene->CreateCubeComponent(path + "\\engine\\GameDemo\\assets\\viking_room.png");
    manager._activeScene->LoadGameObject(component);
    manager._activeScene->LoadGameObject(player);
 }

int  startGame()
{
    std::shared_ptr<Init> i = Init::create();
    createScene();
    createComponents();
    App &app = i->getApp();
    app.Run();
    return 0;
}


int main ()
{
    LOG(EZ_TRACE, "IN MAIN ");
    return startGame();
}