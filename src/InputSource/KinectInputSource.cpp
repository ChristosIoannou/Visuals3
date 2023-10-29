#include "KinectInputSource.h"
#include "ofxOpenCv.h"
#include "ofxCv.h"

//--------------------------------------------------------------
void KinectInputSource::setup(ContextPtr& context)
{
    kinect.setRegistration(true);   // enable depth->video image calibration
    kinect.init();  // shows normal RGB video image
    //kinect.init(true); // shows infrared instead of RGB video image
    //kinect.init(false, false); // disable video image (faster fps)
    kinect.open();

    angle = 0;  // zero the tilt on startup
    kinect.setCameraTiltAngle(angle);
}

//--------------------------------------------------------------
void KinectInputSource::update(ContextPtr& context)
{
    kinect.update();

    if (kinect.isFrameNew())
    {
        cv::Mat rgbImage = ofxCv::toCv(kinect.getPixels());
        cv::Mat depthImage = ofxCv::toCv(kinect.getDepthPixels());
        
        ofMesh pointCloud;
        for (int y = 0; y < kinect.height; ++y)
        {
            for (int x = 0; x < kinect.width; ++x)
            {
                pointCloud.addVertex(kinect.getWorldCoordinateAt(x, y));
            }
        }
        
        context->setResult("kinectRGBImage", rgbImage);
        context->setResult("kinectDepthImage", depthImage);
        context->setResult("kinectPointCloud", pointCloud);

    }
}