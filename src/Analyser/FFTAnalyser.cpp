#include "FFTAnalyser.h"
#include "Constants.h"

//--------------------------------------------------------------
void FFTAnalyser::declareSettings()
{
    params.setName(name);
    params.add(bassMid.set("BassMid", 3, 0, SPECTRAL_BANDS));
    params.add(midHigh.set("MidHigh", 20, 0, SPECTRAL_BANDS));
}

//--------------------------------------------------------
void FFTAnalyser::setup(ContextPtr& context)
{

}

//--------------------------------------------------------
void FFTAnalyser::update(ContextPtr& context)
{
    std::unique_lock<std::mutex> scopedLock(audioDataMutex);

    if (!context->hasResult("soundBuffer"))
        return;

    audioData = context->getResult<ofSoundBuffer>("soundBuffer");

    if (audioData.getBuffer().empty()) //potentially remove
        return;

    fft->setSignal(audioData.getBuffer().data());

    for (int i = 0; i < fftSpectrum.size(); i++)
    {
        fftSpectrum[i] = fft->getAmplitudeAtBin(i);
    }

    midHigh = midHigh <= bassMid ? bassMid : midHigh;
    std::vector<float>::iterator rg_it = std::next(fftSpectrum.begin(), bassMid);
    std::vector<float>::iterator gb_it = std::next(fftSpectrum.begin(), midHigh);

    float bass = std::accumulate(fftSpectrum.begin(), rg_it, 0.0f);
    float mids = std::accumulate(rg_it, gb_it, 0.0f);
    float highs = std::accumulate(gb_it, fftSpectrum.end(), 0.0f);
    float totals = std::accumulate(fftSpectrum.begin(), fftSpectrum.end(), 0.0f);

    context->setResult("fftSpectrum", fftSpectrum);
    context->setResult("bass", bass);
    context->setResult("mids", mids);
    context->setResult("highs", highs);
    context->setResult("totals", totals);
}

//--------------------------------------------------------
void FFTAnalyser::drawToGui(ContextPtr& context)
{
    int barWidth = 3;
    int barSpacing = 2;

    ofPushMatrix();
    ofTranslate(20, 20);
    ofSetLineWidth(1);
    ofNoFill();
    ofSetColor(ofColor::yellow);
    ofDrawBitmapString("FFT Spectrum:", 0, 0);
    ofRect(0, 10, SPECTRAL_BANDS * 5, 100);

    for (int i = 0; i < fftSpectrum.size(); i++) 
    {
        if (i < bassMid)
            ofSetColor(ofColor::red);
        else if (i < midHigh)
            ofSetColor(ofColor::green);
        else
            ofSetColor(ofColor::blue);

        float barHeight = fftSpectrum[i] * 50;
        ofDrawRectangle(i * (barWidth + barSpacing), 110, barWidth, -barHeight);
    }

    ofPopMatrix();
}
//
////--------------------------------------------------------
//void FFTAnalyser::setAudioData(const ofSoundBuffer& buffer)
//{
//    std::unique_lock<std::mutex> scopedLock(audioDataMutex);
//
//    audioData = buffer;
//   // fft->setSignal(buffer.getBuffer().data());
//}