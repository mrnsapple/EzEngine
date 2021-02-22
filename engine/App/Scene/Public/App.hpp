
#include <iostream>
#include "Init.hpp"
#include "log.hpp"
#ifndef _EZ_ENGINE_APPLICATION_
#define _EZ_ENGINE_APPLICATION_

namespace ez
{

class App
{
    public:
        ~App(){}
        App(){}
        const int Run();
        const bool IsRunning() 	const noexcept { return _isRunning;	}

    private:
        bool _isRunning;

        App(const App &other) {}
        const App& operator=(const App& other) { return *this; }
};
}

#endif
