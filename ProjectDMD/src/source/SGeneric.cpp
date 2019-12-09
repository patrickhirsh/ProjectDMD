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


////////////////////////////////////////////////////////////////////////////////
// SOURCE: Rectangle

SRectangle::SRectangle(
	std::tuple<int, int>			origin1,
    std::tuple<int, int>			origin2,                                   
    const rgb_matrix::Color*        color,
    float                           opacity,
	bool							fill,
	int								borderWidth
)
{
	_origin1 = origin1;
	_origin2 = origin2;
	_color = color;
	_opacity = opacity;
	_fill = fill;
	_borderWidth = borderWidth;
}

void SRectangle::Update()
{
	Render::Rectangle(_origin1, _origin2, _color, _opacity, _fill, _borderWidth);
}

void SRectangle::Translate(std::tuple<int, int> offset)
{
	_origin1 = std::tuple<int, int>(
		std::get<0>(_origin1) + std::get<0>(offset), 
		std::get<1>(_origin1) + std::get<1>(offset));
	_origin2 = std::tuple<int, int>(
		std::get<0>(_origin2) + std::get<0>(offset), 
		std::get<1>(_origin2) + std::get<1>(offset));
}

void SRectangle::SetColor(const rgb_matrix::Color* color, float opacity)
{
	_color = color;
	_opacity = opacity;
}

////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
// SOURCE: Text

SText::SText(
	std::string                     text,
	std::tuple<int, int>            origin,
	const DMDF*                     font,
	Render::TextJustification       justification,
	const rgb_matrix::Color*        color,
	float                           opacity,
	int                             horizontalTextSpacing,
	int                             verticalTextSpacing,
	bool                            wrapOnNewline
)
{
	_text = text;
	_origin = origin;
	_font = font;
	_justification = justification;
	_color = color;
	_opacity = opacity;
	_horizontalTextSpacing = horizontalTextSpacing;
	_verticalTextSpacing = verticalTextSpacing;
	_wrapOnNewline = wrapOnNewline;
}

void SText::Update()
{
	if (_wrapOnNewline)
	{
		std::stringstream stream(_text);
		std::string token;
		std::vector<std::string> tokens;
		std::tuple<int, int> currentOrigin = std::tuple<int, int>(std::get<0>(_origin), std::get<1>(_origin));
		while (std::getline(stream, token, '\n')) { tokens.push_back(token); }
		for (std::string token : tokens)
		{
			Render::Text(token, _font, currentOrigin, _color, _opacity, _justification, _horizontalTextSpacing);
			currentOrigin = std::tuple<int, int>(
				std::get<0>(currentOrigin),
				std::get<1>(currentOrigin) + _font->GetFontHeight() + _verticalTextSpacing);
		}
	}
	else
	{
		Render::Text(_text, _font, _origin, _color, _opacity, _justification, _horizontalTextSpacing);
	}
}

void SText::SetColor(const rgb_matrix::Color* color, float opacity)
{
	delete _color;
	_color = new rgb_matrix::Color(color->r, color->g, color->b);
	_opacity = opacity;
}

////////////////////////////////////////////////////////////////////////////////
