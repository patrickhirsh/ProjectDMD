////////////////////////////////////////////////////////////////////////////////
/* ProjectDMD Written by: Patrick Hirsh, 2019 */
////////////////////////////////////////////////////////////////////////////////

#include "../include/PanelManager.h"

////////////////////////////////////////////////////////////////////////////////
// PanelManager

volatile bool interrupt_received = false;
static void InterruptHandler(int signo)
{
    interrupt_received = true;
}


int PanelManager::Run(int argc, char* argv[])
{
#if !__linux__
    ErrorHandler::FatalError("ProjectDMD", "Unsupported operating system detected (expected linux)");
#endif

    printf("\n\nrunning...\n\n");

    // listen for interupts
    signal(SIGTERM, InterruptHandler);
    signal(SIGINT, InterruptHandler);

    // system init
    Render::Initialize(argc, argv);
    ResourceManager::Initialize();

    // source init
    Clock clockSource;

    Render::GlobalBrightness = 0.2f;

    // panel refresh loop
    while (!interrupt_received)
    {
        // clear canvas each frame update
        Render::Clear();

        // update sources...
        clockSource.Update();

        // temporary solution.. Render should do this
#if __linux__
        usleep(1000000);
#endif 
    }

    // shutdown
    Render::Clear();
    printf("\n\nshutting down...\n\n");
    return 0;
}

////////////////////////////////////////////////////////////////////////////////