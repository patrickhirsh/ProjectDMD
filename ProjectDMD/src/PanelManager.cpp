#include "../include/libraries.h"
#include "../include/PanelManager.h"
#include "../include/PanelSource.h"
#include "../include/ResourceManager.h"

volatile bool interrupt_received = false;
static void InterruptHandler(int signo) {
    interrupt_received = true;
}

int PanelManager::Run(int argc, char* argv[])
{
    printf("\n\nrunning...\n\n");

    // listen for interupts
    signal(SIGTERM, InterruptHandler);
    signal(SIGINT, InterruptHandler);

    // define matrix options
    RGBMatrix::Options defaults;
    defaults.hardware_mapping = "adafruit-hat";
    defaults.rows = 32;
    defaults.cols = 64;
    defaults.chain_length = 2;
    defaults.show_refresh_rate = false;

    // create the canvas
    Canvas* canvas = rgb_matrix::CreateMatrixFromFlags(&argc, &argv, &defaults);
    if (canvas == NULL) { return 1; }
    
    // load resources
    ResourceManager::LoadFonts("../resources/fonts/dmdf");

    // initialize sources
    Clock clockSource;
    clockSource.Start(canvas);

    while(!interrupt_received)
    {
        // clear canvas each frame update
        canvas->Fill(0, 0, 0);

        // update sources...
        clockSource.Update(canvas);

        // arbitrary sleep period
        usleep(1000000);
    }

    // shutdown
    canvas->Clear();
    delete canvas;
    printf("\n\nshutting down...\n\n");
    return 0;
}