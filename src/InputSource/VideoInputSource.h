#pragma once

#include "ofMain.h"
#include "AbstractInputSource.h"
#include "ofxDropDown.h"

class VideoInputSource : public AbstractInputSource
{
	enum class Video
	{
		ART_THROUGH_TIME,
		BIKE,
		PAPA_SPEAKS_JAPANESE,
		FINGERS,
		WILLIAMSBURG_BRIDGE,
		COMMUNITY,
		NONE
	};

	constexpr static int numVideos = std::underlying_type_t<Video>(Video::NONE) + 1;
	constexpr static std::array<char*, numVideos> videoNames =
	{
		"artthroughtime.mp4", "Victoria.mov", "PapaSpeaksJapanese.MOV", "fingers.mov", "WilliamsburgBridge.MOV", "Community.mp4", "NONE"
	};


public:

	VideoInputSource(const std::string& name_)
		: AbstractInputSource(name_) {};

	~VideoInputSource() {};

	virtual void declareSettings() override;
	virtual void addDropdownsToPanel(std::shared_ptr<ofxPanel> panel) override;

	virtual void setup(ContextPtr& context) override;
	virtual void update(ContextPtr& context) override;
	virtual void draw(ContextPtr& context) {};
	virtual void drawToGui(ContextPtr& context) {};

	// Listeners
	void vidChoiceChanged(int& vidChoice);

private:

	std::vector<ofVideoPlayer> videoPlayers;
	ofParameter<int> vidChoice;
	unique_ptr<ofxIntDropdown> vidChoiceDD;

	Video currentVideo = Video::NONE;

	const std::string getVideoName(int video) const
	{
		return std::string{ videoNames[video] };
	}
};