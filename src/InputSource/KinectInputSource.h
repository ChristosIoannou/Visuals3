#pragma once

#include "ofMain.h"
#include "AbstractInputSource.h"
#include "ofxKinect.h"

class KinectInputSource : public AbstractInputSource
{
public:

	KinectInputSource(const std::string& name_)
		: AbstractInputSource(name_) {};

	~KinectInputSource() {};

	virtual void setup(ContextPtr& context) override;
	virtual void update(ContextPtr& context) override;

private:

	ofxKinect kinect;
	int angle;
	bool getKinectFrame = true;

};