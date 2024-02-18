#pragma once

#include "ofMain.h"
#include "AbstractInputSource.h"

class WebcamInputSource : public AbstractInputSource
{
public:

	WebcamInputSource(const std::string& name_)
		: AbstractInputSource(name_) {};

	~WebcamInputSource() {};

	virtual void setup(ContextPtr& context) override;
	virtual void update(ContextPtr& context) override;

private:

	ofVideoGrabber webcam;
	ofPixels frame;
	int camWidth;
	int camHeight;

};