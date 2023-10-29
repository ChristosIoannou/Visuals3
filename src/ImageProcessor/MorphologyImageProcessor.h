#pragma once

#include "ofMain.h"
#include "AbstractImageProcessor.h"

class MorphologyImageProcessor : public AbstractImageProcessor
{
public:

	MorphologyImageProcessor(const std::string& name_)
		: AbstractImageProcessor(name_)
	{};

	~MorphologyImageProcessor() {};

	virtual void declareSettings() override;
	virtual void setup(ContextPtr& context) override;
	virtual void update(ContextPtr& context) override;
	virtual void drawToGui(ContextPtr& context) override;

private:

	ofParameter<int> size;
};