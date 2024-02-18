#include "WebcamInputSource.h"
#include "ofxOpenCv.h"
#include "ofxCv.h"

//--------------------------------------------------------------
void WebcamInputSource::setup(ContextPtr& context)
{
    camWidth = 640;  // try to grab at this size.
    camHeight = 480;

    //get back a list of devices.
    vector<ofVideoDevice> devices = webcam.listDevices();

    for (size_t i = 0; i < devices.size(); i++) {
        if (devices[i].bAvailable) {
            //log the device
            ofLogNotice() << devices[i].id << ": " << devices[i].deviceName;
        }
        else {
            //log the device and note it as unavailable
            ofLogNotice() << devices[i].id << ": " << devices[i].deviceName << " - unavailable ";
        }
    }

    webcam.setDeviceID(0);
    webcam.setDesiredFrameRate(50);
    webcam.setup(camWidth, camHeight);

    frame.allocate(camWidth, camHeight, OF_PIXELS_RGB);
    ofSetVerticalSync(true);
}

//--------------------------------------------------------------
void WebcamInputSource::update(ContextPtr& context)
{
    webcam.update();

    if (webcam.isFrameNew())
    {
        ofPixels& pixels = webcam.getPixels();
        context->setResult("webcamImage", pixels);
    }
}