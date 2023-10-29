#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "Pipeline.h"
#include "ofJson.h"

class Gui
{
public:

	Gui() 
	{
		loadPositionsJson();
	};

	~Gui() 
	{
		savePositionsJson();
	};

	void generatePanels(Pipeline& pipeline);
	void setup();
	void draw();

	void savePositionsJson();

private:

	void Gui::createPanelForPipeline(std::string name, ofParameterGroup& params);
	void createPanelForModule(const AbstractModulePtr& module);
	void drawFPS();

	void loadPositionsJson();
	std::pair<int, int> getPanelPositionForProcessor(const std::string& procName);
	std::vector<std::shared_ptr<ofxPanel>> panels;
	nlohmann::json positionsJson;
};

typedef std::unique_ptr<Gui> GuiPtr;