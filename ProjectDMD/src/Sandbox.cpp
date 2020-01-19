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
		for (auto object : *activeObjects)
		{
			object->Draw();
		}
	}

	void OnEvent()
	{

	}
};

Ember::ApplicationToken Ember::CreateApplication()
{
	auto instance = new Sandbox();
	return ApplicationToken(
		instance,
		Options(),
		std::bind(&Sandbox::Tick, instance),
		std::bind(&Sandbox::OnEvent, instance));
}