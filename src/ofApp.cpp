#include "ofApp.h"
#include "Pipeline.h"

// Input Sources
#include "InputSource/AudioInputSource.h"
#include "InputSource/VideoInputSource.h"

// Analysers
#include "Analyser/FFTAnalyser.h"
#include "Analyser/BarkTransformAnalyser.h"

// Image Processors
#include "ImageProcessor/EdgeImageProcessor.h"
#include "ImageProcessor/MorphologyImageProcessor.h"

// Scenes
#include "Scene/SurfaceMeshScene.h"
#include "Scene/VideoPlayerScene.h"

// Processors
#include "AudioSphereProcessor.h"


//--------------------------------------------------------------
void ofApp::setup()
{
    pipeline = std::make_unique<Pipeline>();
	gui = std::make_unique<Gui>();
	context = createContext();

	// Input Sources
	auto audioInputSource = std::make_shared<AudioInputSource>("Audio Input Source");
	auto videoInputSource = std::make_shared<VideoInputSource>("Video Input Source");

	pipeline->addInputSource(audioInputSource, context);
	pipeline->addInputSource(videoInputSource, context);

	// Analysers
	auto fftAnalyser = std::make_shared<FFTAnalyser>("FFTAnalyser");

	pipeline->addAnalyser(fftAnalyser, context);

	// Image Processors
	auto edgeImageProcessor = std::make_shared<EdgeImageProcessor>("EdgeImageProcessor");
	auto morphologyImageProcessor = std::make_shared<MorphologyImageProcessor>("MorphologyImageProcessor");

	pipeline->addImageProcessor(edgeImageProcessor, context);
	pipeline->addImageProcessor(morphologyImageProcessor, context);

	// Scenes
	auto surfaceMeshScene = std::make_shared<SurfaceMeshScene>("Surface Mesh", easyCam);
	auto videoPlayerScene = std::make_shared<VideoPlayerScene>("Video Player");

	pipeline->addScene(surfaceMeshScene, context);
	pipeline->addScene(videoPlayerScene, context);

	gui->generatePanels(*pipeline);
}

//--------------------------------------------------------------
void ofApp::update()
{
	context = createContext();

	pipeline->update(context);
}

//--------------------------------------------------------------
void ofApp::draw()
{
	pipeline->draw(context);
}

//--------------------------------------------------------------
void ofApp::setupGui()
{
	gui->setup();
}

//--------------------------------------------------------------
void ofApp::drawGui(ofEventArgs& args)
{
	gui->draw();
	pipeline->drawGui(context);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key)
{
	switch (key)
	{
	case 's':
	case 'S':
		gui->savePositionsJson();
		break;
	}
}