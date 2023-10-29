#pragma once

#include "ofMain.h"
#include "AbstractImageProcessor.h"

class ImageProcessingPipeline
{
public:
	ImageProcessingPipeline() {};
	~ImageProcessingPipeline() {};

	void addImageProcessor(AbstractImageProcessorPtr imgProc, ContextPtr& context);
	void update(ContextPtr& context);
	//void reorderImageProcessingPipeline();
	std::unordered_map<int, AbstractImageProcessorPtr>& getImageProcessors();

	ofParameterGroup imageProcessingParams;

private:
	std::unordered_map<int, AbstractImageProcessorPtr> imageProcessors;
	std::unordered_map<std::string, std::shared_ptr<ofParameter<bool>>> imageProcessorInjector;

};

using ImageProcessingPipelinePtr = std::shared_ptr<ImageProcessingPipeline>;