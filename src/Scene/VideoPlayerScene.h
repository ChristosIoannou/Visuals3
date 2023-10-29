#pragma once

#include "ofMain.h"
#include "ofxCv.h"
#include "AbstractScene.h"
#include "ofxDropDown.h"

class VideoPlayerScene : public AbstractScene {

	enum class Style
	{
		NORMAL
	};

public:
	VideoPlayerScene(const std::string& name_)
	: AbstractScene(name_) {};

	~VideoPlayerScene() {};

	virtual void setup(ContextPtr& context) override;
	virtual void update(ContextPtr& context) override;
	virtual void draw(ContextPtr& context) override;

	virtual void declareSettings() override;
	virtual void addDropdownsToPanel(std::shared_ptr<ofxPanel> panel) override;

	void drawStyleChanged(int& drawStyle);

	std::shared_ptr<ofPixels> currentFrame;
	ofTexture sobelTexture;

	ofParameter<int> drawStyle;
	unique_ptr<ofxIntDropdown> drawStyleDD;

	Style currentStyle = Style::NORMAL;

private:



	void edgeDetectVideo(std::shared_ptr<ofPixels>& frame);
	std::pair<const float, const float> getDrawWidthAndHeight(std::shared_ptr<ofPixels>& videoPlayer) const;

};