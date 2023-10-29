#pragma once

#include "ofMain.h"
#include "AbstractInputSource.h"
#include "Constants.h"

class AudioInputSource : public AbstractInputSource, public ofBaseSoundInput
{
public:

	AudioInputSource(const std::string& name_)
		: AbstractInputSource(name_) {};

	~AudioInputSource()
	{
		soundStream.stop();
		soundStream.close();
	};

	virtual void declareSettings() override;
	virtual void setup(ContextPtr& context) override;
	virtual void update(ContextPtr& context) override;
	virtual void drawToGui(ContextPtr& context) override;

	virtual void audioIn(ofSoundBuffer& input) override;

private:

	ofSoundStream soundStream;
	ofSoundBuffer soundBuffer;
	std::mutex soundMutex;

	float waveformScale = 40.0f;
	ofPolyline waveform;

	const int sampleRate = 5025;
	const int bufferSize = 192;

	ofParameter<float> volumeMultiplier = 1.0f;
};