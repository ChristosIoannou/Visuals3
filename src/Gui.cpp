#pragma once

#include "ofMain.h"
#include "Gui.h"

void Gui::generatePanels(Pipeline& pipeline)
{
	createPanelForPipeline("Input Sources", pipeline.inputSourcesParams);
	createPanelForPipeline("Analysis Pipeline", pipeline.analysersParams);
	createPanelForPipeline("Image Processing Pipeline", pipeline.getImageProcessingPipeline()->imageProcessingParams);

	for (const auto& inputSource : pipeline.getInputSources())
	{
		if  (!inputSource->createGuiPanel)
			continue;

		createPanelForModule(inputSource);
	}

	for (const auto& analyser : pipeline.getAnalysers())
	{
		if  (!analyser->createGuiPanel)
			continue;

		createPanelForModule(analyser);
	}

	for (const auto& [index, imageProcessor] : pipeline.getImageProcessingPipeline()->getImageProcessors())
	{
		if (!imageProcessor->createGuiPanel)
			continue;

		createPanelForModule(imageProcessor);
	}

	for (const auto& scene : pipeline.getScenes())
	{
		if (!scene->createGuiPanel)
			continue;

		createPanelForModule(scene);
	}
}

void Gui::setup()
{
	ofSetBackgroundColor(0);
}

void Gui::draw()
{
	for (auto& panel : panels)
	{
		panel->draw();
	}

	drawFPS();
}

void Gui::createPanelForModule(const AbstractModulePtr& mod)
{
	auto panel = std::make_shared<ofxPanel>();
	std::string modName = mod->params.getName();
	auto panelPos = getPanelPositionForProcessor(modName);
	panel->setup(mod->params, "settings.xml", panelPos.first, panelPos.second);
	panel->setName(modName);
	mod->addDropdownsToPanel(panel);
	panels.emplace_back(panel);
}

void Gui::createPanelForPipeline(std::string name, ofParameterGroup& params)
{
	auto panel = std::make_shared<ofxPanel>();
	auto pipePanelPos = getPanelPositionForProcessor(name);
	panel->setup(params, "settings.xml", pipePanelPos.first, pipePanelPos.second);
	panel->setName(name);
	panels.emplace_back(panel);
}

void Gui::drawFPS()
{
	ofPushMatrix();
	ofTranslate(290, 60);
	float fps = ofGetFrameRate();
	float fpsHue = ofMap(fps, 10, 30, 0, 85, true);
	ofColor fpsColor;
	fpsColor.setHsb(fpsHue, 255, 255);
	ofSetColor(fpsColor);
	ofRect(0, 0, 180, 40);
	ofDrawBitmapString("FPS: " + ofToString(fps), 45, 25);
	ofPopMatrix();
}

void Gui::loadPositionsJson()
{
	positionsJson = ofLoadJson("positions.json");
}

void Gui::savePositionsJson()
{
	for (auto& panel : panels)
	{
		positionsJson[panel->getName()] = { panel->getPosition().x, panel->getPosition().y };
	}

	ofSaveJson("positions.json", positionsJson);
}

std::pair<int, int> Gui::getPanelPositionForProcessor(const std::string& procName)
{
	if (positionsJson.contains(procName))
	{
		const auto procPos = positionsJson[procName];
		return std::pair<int, int>(procPos[0], procPos[1]);
	}
	else
	{
		return std::pair<int, int>(0, 0);
	}
}