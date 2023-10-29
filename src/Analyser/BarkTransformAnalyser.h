#pragma once

#include "ofMain.h"
#include "AbstractAnalyser.h"
#include "Constants.h"

class BarkTransformAnalyser : public AbstractAnalyser
{
public:

	BarkTransformAnalyser(const std::string& name_)
		: AbstractAnalyser(name_)
		, barkSpectrum(BARK_MAX, 0.0)
		, barkMap(SPECTRAL_BANDS) {};

	~BarkTransformAnalyser() {};

	virtual void setup(ContextPtr& context) override;
	virtual void update(ContextPtr& context) override;
	virtual void draw(ContextPtr& context) override;

private:

	int bark(float f);
	std::vector<int> barkMap; //if i is the index of spectrum band, barkmap[i] gives which bin it goes to.
	std::vector<float> barkSpectrum; //where we sum up our spectral info
};