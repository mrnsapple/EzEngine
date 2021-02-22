/** 
 * @author Oriol Manzano
 */

#include <gtest/gtest.h>
#include "SceneManager.hpp"
#include "SceneComponent.hpp"
#include <memory>
#include "Scene.hpp"
#include "AudioComponent.hpp"
#include "Collision.hpp"
using namespace ez;
namespace {
	SceneManager manager;
}
// TEST(SceneTest, SceneManagerAddScene) {
// 	for (int i =0; i < 5; i++)
// 		std::cout <<" SCENE TESTTTTTTTTTTTTTTT" <<std::endl;
// 	manager.CreateScene(true);
// 	manager.CreateScene(true);
// 	manager.CreateScene(true);
// 	std::unique_ptr<Scene> scene(new Scene());
// 	manager.AddScene(std::move(scene), false);


// 	SceneComponent *component;
// 	 int i = 1;
// 	for (i = 1; i <20; i++) {
		
// 	 	component = (new SceneComponent());

// 		manager._activeScene->LoadGameObject(component);
// 		std::cout << "NUM:" << manager._activeScene->getNumberSceneGameObjects() << ",I:" << i << std::endl;
// 		EXPECT_EQ(i, manager._activeScene.get()->getNumberSceneGameObjects());
// 		EXPECT_EQ(i, manager._activeScene.get()->getNumberSceneComponents());

// 	}
// }

TEST(SceneTest, SceneCompoludnentsChilds) {

	manager.CreateScene(true);
	SceneComponent *componente[100];
	for(int i = 0; i< 100; i++) {
		componente[i] = new SceneComponent;
	}
	
	std::cout << "BEFORE ATACH\n";
	for(int i = 0; i< 100; i++) {
		componente[0]->SetupAttachment(componente[i]);
		EXPECT_EQ((componente[0]->getChilds()).size(), i);
	}
	std::cout << "AFTER ATCH\n";
	manager._activeScene->LoadGameObject(componente[0]);
	std::cout << "AFTER LOAD\n";

	EXPECT_EQ(1, manager._activeScene->getNumberSceneGameObjects());
	EXPECT_EQ(100, manager._activeScene->getNumberSceneComponents());

}

TEST(SceneTest, SceneLoop) {
	manager.CreateScene(true);
	SceneComponent *componente[100];

	for(int i = 0; i< 100; i++) {
		componente[i] = new SceneComponent;
	}
	for(int i = 0; i< 100; i++) {
		componente[0]->SetupAttachment(componente[i]);
		EXPECT_EQ((componente[0]->getChilds()).size(), i);
	}
	manager._activeScene->LoadGameObject(componente[0]);
	manager._activeScene->LoadGameObject(componente[1]);
	EXPECT_EQ(1, manager._activeScene->getNumberSceneGameObjects());
	manager._activeScene->LoadGameObject(new SceneComponent);

	///////
	// AudioComponent *audiocomp = new AudioComponent;
	// Sphere s;
	// s.setRadius(20);
	// audiocomp->setSphereColl(s);
	// manager._activeScene->LoadGameObject(audiocomp);

	
	for (int i = 0; i < 200; i++) {
		manager._activeScene->TickBeginFrame();
		manager._activeScene->TickDuringFrame();
		manager._activeScene->TickEndFrame();
	}

}


