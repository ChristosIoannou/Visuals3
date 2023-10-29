#include "MorphologyImageProcessor.h"
#include "ofxCv.h"

//--------------------------------------------------------------
void MorphologyImageProcessor::declareSettings()
{
    params.setName(name);
    params.add(size.set("Size", 0, 0, 2));
}

//--------------------------------------------------------
void MorphologyImageProcessor::setup(ContextPtr& context)
{

}

//--------------------------------------------------------
void MorphologyImageProcessor::update(ContextPtr& context)
{
    if (!context->hasResult("videoFrame"))
        return;

    auto videoFrame = context->getResultSharedPtr<ofPixels>("videoFrame");

    if (!videoFrame) //potentially remove
        return;

    cv::Mat videoImg = ofxCv::toCv(*videoFrame);
    cv::Mat gray;

    if (videoImg.channels() == 3)
        cv::cvtColor(videoImg, gray, cv::COLOR_RGB2GRAY);
    else
        gray = videoImg;

    cv::Mat erode;
    cv::Mat element = cv::getStructuringElement(cv::MORPH_RECT,
        cv::Size(2 * size + 1, 2 * size + 1),
        cv::Point(size, size));
    cv::erode(gray, erode, element);

    cv::Mat dilate;
    cv::dilate(erode, dilate, element);

    ofPixels morphPixels;
    ofxCv::toOf(dilate, morphPixels);

    context->setResult("videoFrame", std::make_shared<ofPixels>(morphPixels));
}

//--------------------------------------------------------
void MorphologyImageProcessor::drawToGui(ContextPtr& context)
{
}