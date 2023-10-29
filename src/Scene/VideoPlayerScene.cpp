#include "VideoPlayerScene.h"

//--------------------------------------------------------
void VideoPlayerScene::declareSettings()
{
    params.setName(name);

    drawStyleDD = std::make_unique<ofxIntDropdown>(drawStyle);
    drawStyleDD->add((int)Style::NORMAL, "Normal");
    drawStyleDD->disableMultipleSelection();
    drawStyleDD->enableCollapseOnSelection();
    drawStyleDD->setName("Style");
    drawStyleDD->setSelectedValueByIndex((int)currentStyle, false);
}

//--------------------------------------------------------
void VideoPlayerScene::addDropdownsToPanel(std::shared_ptr<ofxPanel> panel)
{
    panel->add(drawStyleDD.get());
}

//--------------------------------------------------------
void VideoPlayerScene::setup(ContextPtr& context)
{

    drawStyle.addListener(this, &VideoPlayerScene::drawStyleChanged);
}

//--------------------------------------------------------
void VideoPlayerScene::update(ContextPtr& context)
{
    if (!context->hasResult("videoFrame"))
        return;

    currentFrame = context->getResultSharedPtr<ofPixels>("videoFrame");

}

//--------------------------------------------------------
void VideoPlayerScene::draw(ContextPtr& context)
{
    if (!currentFrame)
        return;

    auto drawDims = getDrawWidthAndHeight(currentFrame);
    ofTexture frameTex;
    frameTex.loadData(*currentFrame);

    float drawWidth = drawDims.first;
    float drawHeight = drawDims.second;
    float x = (ofGetWidth() - drawWidth) / 2.0f;
    float y = (ofGetHeight() - drawHeight) / 2.0f;

    ofPushMatrix();
    ofTranslate(x + drawWidth / 2, y + drawHeight / 2); // Translate to center

    switch (currentStyle)
    {
    case Style::NORMAL:
        frameTex.draw(-drawWidth / 2, -drawHeight / 2, drawWidth, drawHeight);
        break;
    }

    ofPopMatrix();
}

//------------------------------------------------------
std::pair<const float, const float> VideoPlayerScene::getDrawWidthAndHeight(std::shared_ptr<ofPixels>& frame) const
{
    float videoWidth = frame->getWidth();
    float videoHeight = frame->getHeight();
    float screenWidth = ofGetWidth();
    float screenHeight = ofGetHeight();
    float screenAspectRatio = screenWidth / screenHeight;
    float videoAspectRatio = videoWidth / videoHeight;
    float drawWidth, drawHeight;

    if (screenAspectRatio > videoAspectRatio) {
        // The screen is wider than the video, so the video's height should fill the screen
        drawHeight = screenHeight;
        drawWidth = drawHeight * videoAspectRatio;
    }
    else {
        // The screen is narrower than the video, so the video's width should fill the screen
        drawWidth = screenWidth;
        drawHeight = drawWidth / videoAspectRatio;
    }

    return std::pair<const float, const float>(drawWidth, drawHeight);
}

//------------------------------------------------------
void VideoPlayerScene::drawStyleChanged(int& drawStyle)
{
    currentStyle = static_cast<Style>(drawStyle);
}