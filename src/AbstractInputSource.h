#pragma once

#include "AbstractModule.h"

class AbstractInputSource : public AbstractModule
{

public:

	AbstractInputSource(const std::string& name_)
		: AbstractModule(name_) {};

	~AbstractInputSource() {};

	virtual void setup(ContextPtr& context) = 0;
	virtual void update(ContextPtr& context) = 0;
	virtual void draw(ContextPtr& context) {};

	virtual void drawToGui(ContextPtr& context) = 0;


};

using AbstractInputSourcePtr = std::shared_ptr<AbstractInputSource>;