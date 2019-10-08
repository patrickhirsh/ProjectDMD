////////////////////////////////////////////////////////////////////////////////
/* ProjectDMD Written by: Patrick Hirsh, 2019 */
////////////////////////////////////////////////////////////////////////////////

#include "../../include/PanelSource.h"


////////////////////////////////////////////////////////////////////////////////
// SOURCE: Fill

SFill::SFill(rgb_matrix::Color color, float opacity)
{
	_color = new rgb_matrix::Color(color);
	_opacity = opacity;
}

SFill::~SFill()
{
	delete _color;
}

void SFill::Update()
{
	for (int y = 0; y < (PANEL_HEIGHT * PANEL_COUNT_Y); y++)
	{
		for (int x = 0; x < (PANEL_WIDTH * PANEL_COUNT_X); x++)
		{
			Render::SetPixel(x, y, _color, _opacity);
		}
	}
}

void SFill::SetColor(rgb_matrix::Color color, float opacity)
{
	delete _color;
	_color = new rgb_matrix::Color(color);
	_opacity = opacity;
}

////////////////////////////////////////////////////////////////////////////////
