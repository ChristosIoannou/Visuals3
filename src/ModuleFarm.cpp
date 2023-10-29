#include "ModuleFarm.h"

//--------------------------------------------------------
void ModuleFarm::setup(ContextPtr& context) 
{
    // Set up each module
    for (auto& module : modules) 
    {
        module->setupModule(context);
    }
}

//--------------------------------------------------------
void ModuleFarm::update(ContextPtr& context)
{
    if (activeModuleIndex >= 0 && activeModuleIndex < modules.size()) 
    {
        modules[activeModuleIndex]->update(context);
    }
}

//--------------------------------------------------------
void ModuleFarm::draw(ContextPtr& context)
{
    if (activeModuleIndex >= 0 && activeModuleIndex < modules.size()) 
    {
        modules[activeModuleIndex]->draw(context);
    }
}

//--------------------------------------------------------
void ModuleFarm::addModule(AbstractModulePtr mod, ContextPtr& context)
{
    if (!mod) 
    {
        std::cerr << "Error: Failed to add module to ModuleFarm - module is null" << std::endl;
        return;
    }

    mod->setupModule(context);
    modules.emplace_back(mod);

    ofParameter<bool> enabledParam;
    enabledParam.set(mod->name, false);
    enabledParameters.push_back(enabledParam);
    mod->params.add(enabledParameters.back());

    int index = modules.size() - 1;
    enabledListeners.push(enabledParameters[index].newListener([this, index](bool& value) {
        this->onEnabledParameterChanged(value, index);
        })
    );
}

//--------------------------------------------------------
void ModuleFarm::setActiveModule(int index) 
{
    if (index >= 0 && index < modules.size()) 
    {
        if (activeModuleIndex != -1) 
        {
            // Stop the previous module
            enabledParameters[activeModuleIndex] = false;
        }
        activeModuleIndex = index;
    }
}

//--------------------------------------------------------
void ModuleFarm::onEnabledParameterChanged(bool& value, int index) 
{
    if (value) {
        setActiveModule(index);
    }
    else {
        activeModuleIndex = -1;
    }
}

//--------------------------------------------------------
const std::vector<AbstractModulePtr>& ModuleFarm::getModules()
{
    return modules;
}