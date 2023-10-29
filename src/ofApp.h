#pragma once

#include "ofMain.h"
#include "Context.h"
#include "Pipeline.h"
#include "Gui.h"

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void setupGui();
		void drawGui(ofEventArgs& args);

		void keyPressed(int key);

		ContextPtr context;
		PipelinePtr pipeline;
		GuiPtr gui;
		ofEasyCam easyCam;
};
