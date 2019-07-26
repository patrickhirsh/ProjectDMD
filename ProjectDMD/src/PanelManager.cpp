////////////////////////////////////////////////////////////////////////////////
/* ProjectDMD Written by: Patrick Hirsh, 2019 */
////////////////////////////////////////////////////////////////////////////////

#include "../include/PanelManager.h"

////////////////////////////////////////////////////////////////////////////////
// PanelManager

/* statics */
PanelManager::Mode PanelManager::_currentMode;
std::map<PanelManager::Mode, PanelMode*> PanelManager::_panelModes;

/* interupt management */
volatile bool interrupt_received = false;
static void InterruptHandler(int signo)
{
    interrupt_received = true;
}


int PanelManager::Run(int argc, char* argv[])
{
    printf("\n\nrunning...\n\n");

    // listen for interupts
    signal(SIGTERM, InterruptHandler);
    signal(SIGINT, InterruptHandler);

    // initialize
    initializeSystems(argc, argv);
    initializeModes();

    Render::GlobalBrightness = 1.0f;

    // panel refresh loop
    while (!interrupt_received)
    {
        // clear canvas each frame update
        Render::Clear();

        // refresh the current mode
        _panelModes[_currentMode]->Update();

        // lock frames at 20/s
#if __linux__
        usleep(50000);
#endif 
    }

    // shutdown
    Render::Clear();
    printf("\n\nshutting down...\n\n");
    return 0;
}

void PanelManager::initializeSystems(int argc, char *argv[])
{
    Render::Initialize(argc, argv);
    ResourceManager::Initialize();
}

void PanelManager::initializeModes()
{
    _panelModes.empty();

    _panelModes[Mode::BasicClock] = new MClock(
        std::tuple<int, int>(DISPLAY_WIDTH / 2, ((DISPLAY_HEIGHT / 2) - (ResourceManager::GetFont("StarTrek_20.dmdf")->GetFontHeight() / 2))),
        STime::HH_MM_SS_12_PERIOD,
        Render::TextJustification::Center,
        0,
        ResourceManager::GetSystemColorPalette()->GetColor(15),
        ResourceManager::GetFont("StarTrek_20.dmdf")
    );

    _currentMode = Mode::BasicClock;
}

////////////////////////////////////////////////////////////////////////////////