#include "EdgeImageProcessor.h"
#include "ofxCv.h"

//--------------------------------------------------------------
void EdgeImageProcessor::declareSettings()
{
    params.setName(name);
    params.add(kSize.set("kSize", 1, 0, 9));
}

//--------------------------------------------------------
void EdgeImageProcessor::setup(ContextPtr& context)
{

}

//--------------------------------------------------------
void EdgeImageProcessor::update(ContextPtr& context)
{
    if (!context->hasResult("videoFrame"))
        return;

    auto videoFrame = context->getResultSharedPtr<ofPixels>("videoFrame");

    if (!videoFrame) //potentially remove
        return;

    cv::Mat videoImg = ofxCv::toCv(*videoFrame);
    cv::Mat gray;
   // cv::cvtColor(videoImg, gray, cv::COLOR_RGB2GRAY);
    gray = videoImg;
    cv::Mat sobel;
    cv::Sobel(gray, sobel, -1, 1, 1, (kSize*2 + 1), 2.0, 0.0, cv::BorderTypes::BORDER_DEFAULT);

    ofPixels sobelPixels;
    ofxCv::toOf(sobel, sobelPixels);

    context->setResult("videoFrame", std::make_shared<ofPixels>(sobelPixels));
}

//--------------------------------------------------------
void EdgeImageProcessor::drawToGui(ContextPtr& context)
{
}