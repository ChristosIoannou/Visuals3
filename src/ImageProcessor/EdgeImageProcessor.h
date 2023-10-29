#pragma once

#include "ofMain.h"
#include "AbstractImageProcessor.h"


class EdgeImageProcessor : public AbstractImageProcessor
{
public:

	EdgeImageProcessor(const std::string& name_)
		: AbstractImageProcessor(name_)
	{};

	~EdgeImageProcessor() {};

	virtual void declareSettings() override;
	virtual void setup(ContextPtr& context) override;
	virtual void update(ContextPtr& context) override;
	virtual void drawToGui(ContextPtr& context) override;

private:

	ofParameter<int> kSize;

};