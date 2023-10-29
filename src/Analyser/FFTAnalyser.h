#pragma once

#include "ofMain.h"
#include "AbstractAnalyser.h"
#include "ofxFft.h"

class FFTAnalyser : public AbstractAnalyser
{
public:

	FFTAnalyser(const std::string& name_)
		: AbstractAnalyser(name_)
		, fftSpectrum(fftBands)
	{
		// check to see if we can put this in setup() and get use bufferSize from context as the argument)
		fft = ofxFft::create(fftBands * 4, OF_FFT_WINDOW_HAMMING);
	};
	
	~FFTAnalyser() {};

	virtual void declareSettings() override;
	virtual void setup(ContextPtr& context) override;
	virtual void update(ContextPtr& context) override;
	virtual void drawToGui(ContextPtr& context) override;

	//virtual void setAudioData(const ofSoundBuffer& buffer);

private:

	ofxFft* fft;
	int fftBands = 48;
	std::vector<float> fftSpectrum;

	std::mutex audioDataMutex;
	ofSoundBuffer audioData;

	ofParameter<int> bassMid, midHigh;
};