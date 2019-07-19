////////////////////////////////////////////////////////////////////////////////
/* ProjectDMD Written by: Patrick Hirsh, 2019 */
////////////////////////////////////////////////////////////////////////////////

#include "../../include/PanelSource.h"


////////////////////////////////////////////////////////////////////////////////
// SOURCE: Clock

Clock::Clock()
{
				Color testColor(255, 0, 0);
				this->time_format = "%r";
				this->color = testColor;
				this->t = (time_t*)malloc(sizeof(time_t));
}

Clock::~Clock()
{
				delete t;
}

void Clock::Reset() 
{ 
				Color testColor(255, 0, 0);
				this->time_format = "%r";
				this->color = testColor;
				this->t = (time_t*)malloc(sizeof(time_t));
}

void Clock::Update()
{
#if __linux__
    time(t);
    localtime_r(t, &tm);
    strftime(text_buffer, sizeof(text_buffer), time_format, &tm);
#endif
				Render::Text(text_buffer, ResourceManager::GetFont("StarTrek_20.dmdf"), std::tuple<int, int>(3, 3), 
								*ResourceManager::GetSystemColorPalette()->GetColor(15), TextJustification::Left, 0);
}

////////////////////////////////////////////////////////////////////////////////
