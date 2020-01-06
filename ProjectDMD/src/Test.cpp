#include "Ember.h"

class Test : public Ember::Application
{
public:
	Test()
	{

	}
	~Test()
	{

	}
};

Ember::Application* Ember::CreateApplication()
{
	return new Test();
}