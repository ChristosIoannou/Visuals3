#pragma once

#include "ofMain.h"
#include "AbstractModule.h"

class ModuleFarm 
{
public:
    ModuleFarm() {};
    ~ModuleFarm() {};

    void setup(ContextPtr& context);
    void update(ContextPtr& context);
    void draw(ContextPtr& context);
    void addModule(AbstractModulePtr module, ContextPtr& context);
    void setActiveModule(int index);
    const std::vector<AbstractModulePtr>& getModules();

private:
    std::vector<AbstractModulePtr> modules;
    ofParameterGroup params;
    std::vector<ofParameter<bool>> enabledParameters;
    int activeModuleIndex = -1;
    void onEnabledParameterChanged(bool& value, int index);

    ofEventListeners enabledListeners;
};

using ModuleFarmPtr = std::shared_ptr<ModuleFarm>;