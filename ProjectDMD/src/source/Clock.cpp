#include "../../include/PanelSource.h"


void Clock::Start(Canvas* canvas)
{
    const char* fontPath = "lib/rpi-rgb-led-matrix/fonts/10x20.bdf";
    if (!font.LoadFont(fontPath))
    {
        printf("ERROR LOADING FONT\n");
    }
    time_format = "%r";
    Color testColor(255, 0, 0);
    color = testColor;  
    t = (time_t*)malloc(sizeof(time_t));
}

void Clock::Restart() 
{ 
    return; 
}


void Clock::Update(Canvas* canvas)
{
    //time(t);
    //localtime_r(t, &tm);
    //strftime(text_buffer, sizeof(text_buffer), time_format, &tm);
    //rgb_matrix::DrawText(canvas, font, 64 - 55, 6 + font.baseline(), color, NULL, text_buffer);
				Render::Text(canvas, "HELLO", ResourceManager::GetSystemFont(), std::tuple<int, int>(3, 3), *ResourceManager::GetSystemColorPalette()->GetColor(15));
}
