#pragma once

#include "ofMain.h"
#include "Context.h"
#include "ofxGui.h"

class AbstractModule
{

public:

	AbstractModule(const std::string& name_)
		: name(name_) {}
	~AbstractModule() {};

	virtual void setupModule(ContextPtr& context)
	{
		declareSettings();
		setup(context);
	};

	virtual void addDropdownsToPanel(std::shared_ptr<ofxPanel> panel)
	{};

	virtual void declareSettings()
	{
		createGuiPanel = false;
		params.setName(name);
	};

	virtual void setup(ContextPtr& context) = 0;
	virtual void update(ContextPtr& context) = 0;
	virtual void draw(ContextPtr& context) = 0;

	std::string name;
	ofParameterGroup params;
	bool createGuiPanel = true;

};

using AbstractModulePtr = std::shared_ptr<AbstractModule>;