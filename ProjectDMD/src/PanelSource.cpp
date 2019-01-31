#include "../include/PanelSource.h"


void Demo::Restart() 
{ 
    return; 
}


void Demo::Update(Canvas *canvas)
{
    rVal++;
    if (rVal > 255) { rVal = 0; }
    canvas->Fill(255, 0, 0);
    usleep(1 * 3000);  // wait a little to slow down things.
}
