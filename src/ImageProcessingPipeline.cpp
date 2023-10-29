#include "ImageProcessingPipeline.h"

void ImageProcessingPipeline::addImageProcessor(AbstractImageProcessorPtr imageProcessor, ContextPtr& context)
{
	if (!imageProcessor)
	{
		std::cerr << "Error: Failed to add processor to ImageProcessingPipeline - processor is null" << std::endl;
		return;
	}

	imageProcessor->setupModule(context);

	int index = imageProcessors.size();
	imageProcessors[index] = imageProcessor;

	ofParameter<bool> injectImageProcessor;
	injectImageProcessor.set(imageProcessor->name, true);
	imageProcessorInjector[imageProcessor->name] = std::make_shared<ofParameter<bool>>(injectImageProcessor);
	imageProcessingParams.add(injectImageProcessor);
}

void ImageProcessingPipeline::update(ContextPtr& context)
{
	for (auto index = 0; index < imageProcessors.size(); ++index)
	{
		std::shared_ptr<ofParameter<bool>> doAnalyser = imageProcessorInjector[imageProcessors[index]->name];

		if (*doAnalyser)
			imageProcessors[index]->update(context);
	}
}


std::unordered_map<int, AbstractImageProcessorPtr>& ImageProcessingPipeline::getImageProcessors()
{
	return imageProcessors;
}