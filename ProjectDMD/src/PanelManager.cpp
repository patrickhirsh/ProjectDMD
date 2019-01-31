#include "../include/PanelManager.h"

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
    defaults.show_refresh_rate = true;

    // create the canvas
    Canvas* canvas = rgb_matrix::CreateMatrixFromFlags(&argc, &argv, &defaults);
    if (canvas == NULL) { return 1; }
    
    Demo demoSource;

    while(!interrupt_received)
    {
        demoSource.Update(canvas);
    }

    // shutdown
    canvas->Clear();
    delete canvas;
    printf("\n\nshutting down...\n\n");
    return 0;
}