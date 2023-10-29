#pragma once

#include "ofMain.h"
#include "AbstractInputSource.h"
#include "AbstractAnalyser.h"
#include "AbstractScene.h"
#include "ModuleFarm.h"
#include "ImageProcessingPipeline.h"

class Pipeline
{
public:

	Pipeline() 
	{
		imageProcessingPipeline = std::make_unique<ImageProcessingPipeline>();
		scenes = std::make_unique<ModuleFarm>();
	};

	~Pipeline() {};

	void update(ContextPtr& context);
	void draw(ContextPtr& context);
	void drawGui(ContextPtr& context);

	void addInputSource(AbstractInputSourcePtr inputSource, ContextPtr& context);
	void addAnalyser(AbstractAnalyserPtr analyser, ContextPtr& context);
	void addImageProcessor(AbstractImageProcessorPtr imageProcessor, ContextPtr& cocntext);
	void addScene(AbstractScenePtr scene, ContextPtr& context);

	const std::vector<AbstractInputSourcePtr>& getInputSources() const;
	const std::vector<AbstractAnalyserPtr>& getAnalysers() const;
	const ImageProcessingPipelinePtr& getImageProcessingPipeline() const;
	const std::vector<AbstractModulePtr>& getScenes() const;

	ofParameterGroup inputSourcesParams;
	ofParameterGroup analysersParams;
	// imageProcessorParams are owned by imageProcessingPipeline
	ofParameterGroup scenesParams;

private:

	std::vector<AbstractInputSourcePtr> inputSources;
	std::vector<AbstractAnalyserPtr> analysers;
	ImageProcessingPipelinePtr imageProcessingPipeline;
	ModuleFarmPtr scenes;

	std::unordered_map<std::string, std::shared_ptr<ofParameter<bool>>> inputSourceInjector;
	std::unordered_map<std::string, std::shared_ptr<ofParameter<bool>>> analyserInjector;
};

using PipelinePtr = std::unique_ptr<Pipeline>;