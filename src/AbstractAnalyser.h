#pragma once

#include "AbstractModule.h"

class AbstractAnalyser : public AbstractModule
{

public:

	AbstractAnalyser(const std::string name_)
		: AbstractModule(name_) {};

	virtual void setup(ContextPtr& context) = 0;
	virtual void update(ContextPtr& context) = 0;
	virtual void draw(ContextPtr& context) {};

	virtual void drawToGui(ContextPtr& context) = 0;

};

using AbstractAnalyserPtr = std::shared_ptr<AbstractAnalyser>;