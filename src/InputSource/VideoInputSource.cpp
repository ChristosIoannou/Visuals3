#include "VideoInputSource.h"

//--------------------------------------------------------------
// If the processor doesn't have any parameters, remove this function from the class and 
// use the declareSettings() inherited from AbstractProcessor. This will make sure a panel
// isn't generated for this processor on the GUI.
void VideoInputSource::declareSettings()
{
    params.setName(name);

    vidChoiceDD = std::make_unique<ofxIntDropdown>(vidChoice);
    for (int i = 0; i < numVideos; ++i)
    {
        vidChoiceDD->add(i, getVideoName(i));
    }
    vidChoiceDD->disableMultipleSelection();
    vidChoiceDD->enableCollapseOnSelection();
    vidChoiceDD->setName("Video Name");
    vidChoiceDD->setSelectedValueByIndex((int)currentVideo, false);
}

//--------------------------------------------------------
void VideoInputSource::addDropdownsToPanel(std::shared_ptr<ofxPanel> panel)
{
    panel->add(vidChoiceDD.get());
}

//--------------------------------------------------------------
void VideoInputSource::setup(ContextPtr& context)
{
    ofSetVerticalSync(true);

    for (int i = 0; i < numVideos - 1; ++i)
    {
        ofVideoPlayer player;
        player.load("videos/" + getVideoName(i));
        player.setLoopState(OF_LOOP_NORMAL);
        player.setVolume(0.0);
        player.setSpeed(1.0);
        player.play();
        videoPlayers.emplace_back(player);
    }

    vidChoice.addListener(this, &VideoInputSource::vidChoiceChanged);
}

//--------------------------------------------------------------
void VideoInputSource::update(ContextPtr& context)
{
    if (currentVideo == Video::NONE)
        return;

    videoPlayers[(int)currentVideo].update();

    auto videoFrame = std::make_shared<ofPixels>(videoPlayers[(int)currentVideo].getPixels());

    context->setResult("videoFrame", videoFrame);
    context->setResult("originalFrame", videoFrame);
}

//------------------------------------------------------
void VideoInputSource::vidChoiceChanged(int& vidChoice)
{
    currentVideo = static_cast<Video>(vidChoice);

    for (int i = 0; i < videoPlayers.size(); ++i)
    {
        if (i == vidChoice)
            videoPlayers[i].setPaused(false);
        else
            videoPlayers[i].setPaused(true);
    }
}