#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "Context.h"
#include "AbstractModule.h"

class AbstractScene : public AbstractModule 
{

public:

	AbstractScene(const std::string& name_)
		: AbstractModule(name_) {}

	~AbstractScene() {};

	virtual void setup(ContextPtr& context) = 0;
	virtual void update(ContextPtr& context) = 0;
	virtual void draw(ContextPtr& context) = 0;

};

using AbstractScenePtr = std::shared_ptr<AbstractScene>;