#include "App.hpp"
#include "Settings.hpp"

namespace ez
{


const int App::Run()
{
    SceneManager &manager = Init::getPtr()->getSceneManager();
    VisualInstance &visual = Init::getPtr()->getVisual();
    _isRunning = true;

    std::vector<Mesh*> meshContainer = manager.getActiveScene()->GetMeshes();
    visual.OpenWindow(meshContainer);
    LOG(EZ_TRACE, "START RUN LOOP");
    while (_isRunning)
    {
        manager._activeScene->TickBeginFrame();
        // LOG(EZ_TRACE, "NEW FRAME");

        visual.SendEvents();
		manager._activeScene->TickDuringFrame();
        meshContainer = manager.getActiveScene()->GetMeshes();

        visual.DrawFrame(meshContainer);

		manager._activeScene->TickEndFrame();
    }
    LOG(EZ_TRACE, "OUT RUN LOOP");

    visual.CloseWindow(meshContainer);

    return 0;
}

}
