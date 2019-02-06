#include "../../include/libraries.h"
#include "../../include/PanelSource.h"


void Clock::Start(Canvas *canvas)
{
    const char* fontPath = "lib/rpi-rgb-led-matrix/fonts/10x20.bdf";
    if (!font.LoadFont(fontPath))
    {
        printf("ERROR LOADING FONT");
    }
    time_format = "%r";
    Color testColor(255, 0, 0);
    color = testColor;  
    next_time.tv_sec = time(NULL);
    next_time.tv_nsec = 0;
}

void Clock::Restart() 
{ 
    return; 
}


void Clock::Update(Canvas *canvas)
{
    localtime_r(&next_time.tv_sec, &tm);
    strftime(text_buffer, sizeof(text_buffer), time_format, &tm);
    canvas->Fill(0, 0, 0);
    rgb_matrix::DrawText(canvas, font, 64 - 55, 6 + font.baseline(), color, NULL, text_buffer);
    clock_nanosleep(CLOCK_REALTIME, TIMER_ABSTIME, &next_time, NULL);
    next_time.tv_sec += 1;
}
