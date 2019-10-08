////////////////////////////////////////////////////////////////////////////////
/* ProjectDMD Written by: Patrick Hirsh, 2019 */
////////////////////////////////////////////////////////////////////////////////

#include "../../include/PanelMode.h"


////////////////////////////////////////////////////////////////////////////////
// MODE: PanelTest

MPanelTest::MPanelTest()
{
	_updateCount = 0;
	_red = rgb_matrix::Color(255, 0, 0);
	_green = rgb_matrix::Color(0, 255, 0);
	_blue = rgb_matrix::Color(0, 0, 255);
	_currentColor = _red;

	_fillSource = new SFill(_red);
	_sources.push_back(_fillSource);
}

MPanelTest::~MPanelTest()
{
	for (PanelSource* source : _sources)
	{
		delete source;
	}
}

void MPanelTest::internalUpdate()
{
	_updateCount++;
	if (_updateCount > UPDATES_PER_COLOR)
	{
		_updateCount = 0;
		if (_currentColor.r) { _currentColor = _green; }
		else if (_currentColor.g) { _currentColor = _blue; }
		else { _currentColor = _red; }
		_fillSource->SetColor(_currentColor);
	}
}

////////////////////////////////////////////////////////////////////////////////