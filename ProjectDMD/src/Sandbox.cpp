#include "Ember.h"

class Sandbox : public Ember::Application
{
public:
	std::vector<Ember::Ref<Ember::DMDText>>* activeObjects = new std::vector<Ember::Ref<Ember::DMDText>>();

	Sandbox()
	{
		auto object = Ember::DMDText::Create();
		activeObjects->push_back(object);
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