#include "Ember.h"

class Sandbox : public Ember::Application
{
public:
	Sandbox()
	{

	}
	~Sandbox()
	{

	}

	void Tick()
	{

	}

	void OnEvent()
	{

	}
};

Ember::Application* Ember::CreateApplication()
{
	return new Sandbox();
}