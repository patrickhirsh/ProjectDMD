#include "../include/libraries.h"
#include "../include/PanelSource.h"


void Demo::Restart() 
{ 
    return; 
}


void Demo::Update(Canvas *canvas)
{
    rVal++;
    if (rVal > 255) { rVal = 0; }
    canvas->Fill(rVal, 0, 0);
    usleep(1 * 3000);  // wait a little to slow down things.
}
