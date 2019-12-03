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

    // panel refresh loop
    while (!interrupt_received)
    {
        // refresh the current mode (renders to current frame)
        _panelModes[_currentMode]->Update();

        // swap current frame on VSync
        Render::FinalizeFrame();

        /* limit actual content refresh to a fixed framerate to prevent unecessary 
        memory churning. The panel still scans at a much higher frequency, but constantly
        ticking this update loop is avoided to keep panel scan rate optimal. See
        REFRESH_DELAY_MS in SYSTEMGLOBALS for more info. */
#if __linux__
        usleep(REFRESH_DELAY_MS);
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
	_currentMode = Mode::BasicClock;

	/* Panel Test */
	_panelModes[Mode::PanelTest] = new MPanelTest();

	/* Basic Clock */
    _panelModes[Mode::BasicClock] = new MClock(
        std::tuple<int, int>(Render::GetDisplayWidth() / 2, ((Render::GetDisplayHeight() / 2) - (ResourceManager::GetFont("StarTrek_20.dmdf")->GetFontHeight() / 2))),
        STime::HH_MM_SS_12_PERIOD,
        Render::TextJustification::Center,
        0,
        ResourceManager::GetSystemColorPalette()->GetColor(15),
        ResourceManager::GetFont("StarTrek_20.dmdf")
    );
}

////////////////////////////////////////////////////////////////////////////////