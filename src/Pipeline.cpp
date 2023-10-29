#include "Pipeline.h"

// There is no setup call in the pipeline because the modules
// are setup when they are added

//--------------------------------------------------------
void Pipeline::update(ContextPtr& context)
{
	for (auto& inputSource : inputSources)
	{
		inputSource->update(context);
	}

	for (auto& analyser : analysers)
	{
		std::shared_ptr<ofParameter<bool>> doAnalyser = analyserInjector[analyser->name];

		if (*doAnalyser)
			analyser->update(context);
	}

	imageProcessingPipeline->update(context);

	scenes->update(context);
}

//--------------------------------------------------------
void Pipeline::drawGui(ContextPtr& context)
{
	for (auto& inputSource : inputSources)
	{
		inputSource->drawToGui(context);
	}

	for (auto& analyser : analysers)
	{
		analyser->drawToGui(context);
	}
}

//--------------------------------------------------------
void Pipeline::draw(ContextPtr& context)
{
	scenes->draw(context);
}

//--------------------------------------------------------
void Pipeline::addInputSource(AbstractInputSourcePtr inputSource, ContextPtr& context)
{
	if (!inputSource)
	{
		std::cerr << "Error: Input source is null" << std::endl;
		return;
	}

	inputSource->setupModule(context);
	inputSources.emplace_back(inputSource);

	ofParameter<bool> injectInputSource;
	injectInputSource.set(inputSource->name, true);
	inputSourceInjector[inputSource->name] = std::make_shared<ofParameter<bool>>(injectInputSource);
	inputSourcesParams.add(injectInputSource);
}

//--------------------------------------------------------
void Pipeline::addAnalyser(AbstractAnalyserPtr analyser, ContextPtr& context)
{
	if (!analyser)
	{
		std::cerr << "Error: Analyser is null" << std::endl;
		return;
	}

	analyser->setupModule(context);
	analysers.emplace_back(analyser);

	ofParameter<bool> injectAnalyser;
	injectAnalyser.set(analyser->name, true);
	analyserInjector[analyser->name] = std::make_shared<ofParameter<bool>>(injectAnalyser);
	analysersParams.add(injectAnalyser);
}

//--------------------------------------------------------
void Pipeline::addImageProcessor(AbstractImageProcessorPtr imageProcessor, ContextPtr& context)
{
	if (!imageProcessor)
	{
		std::cerr << "Error:: Image Processor is null" << std::endl;
		return;
	}

	imageProcessingPipeline->addImageProcessor(imageProcessor, context);
}

//--------------------------------------------------------
void Pipeline::addScene(AbstractScenePtr scene, ContextPtr& context)
{
    if (!scene)
    {
        std::cerr << "Error: Scene is null" << std::endl;
        return;
    }

	scenes->addModule(scene, context);
}

//--------------------------------------------------------
const std::vector<AbstractInputSourcePtr>& Pipeline::getInputSources() const
{
	return inputSources;
}

//--------------------------------------------------------
const std::vector<AbstractAnalyserPtr>& Pipeline::getAnalysers() const
{
	return analysers;
}

//--------------------------------------------------------
const ImageProcessingPipelinePtr& Pipeline::getImageProcessingPipeline() const
{
	return imageProcessingPipeline;
}

//--------------------------------------------------------
const std::vector<AbstractModulePtr>& Pipeline::getScenes() const
{
	return scenes->getModules();
}