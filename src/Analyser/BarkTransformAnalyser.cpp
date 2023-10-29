#include "BarkTransformAnalyser.h"

//--------------------------------------------------------------
void BarkTransformAnalyser::setup(ContextPtr& context)
{
    //map the frequencies to bark bands
    float freq_spc = FREQ_MAX / (float)SPECTRAL_BANDS; 

    for (int i = 0; i < barkMap.size(); i++) {
        int bidx = bark(i * freq_spc);
        barkMap[i] = bidx;
    }
}

//--------------------------------------------------------------
void BarkTransformAnalyser::update(ContextPtr& context)
{
    if (!context->hasResult("fftSpectrum"))
        return;

    auto fftSpectrum = context->getResult<std::vector<float>>("fftSpectrum");

    // Update our bark mapped frequency bins    
    std::fill(barkSpectrum.begin(), barkSpectrum.end(), 0.0);

    for (int i = 0; i < fftSpectrum.size(); i++) 
    {
        int idx = barkMap[i];
        barkSpectrum[idx] += fftSpectrum[i] * 70;
    }

    context->setResult("barkSpectrum", barkSpectrum);
}

//--------------------------------------------------------------
void BarkTransformAnalyser::draw(ContextPtr& context)
{
    ofPushMatrix();
    ofTranslate(280, 20);
    ofSetColor(ofColor::pink);
    ofDrawBitmapString("Bark Spectrum:", 0, 0);
    ofRect(0, 10, barkSpectrum.size() * 10, 100);
    ofSetColor(ofColor::pink);

    for (int i = 0; i < barkSpectrum.size(); i++) 
    {    
        ofDrawRectangle(i * 10, 110, 3, -barkSpectrum[i] * 0.6);
    }

    ofPopMatrix();
}

//--------------------------------------------------------------
int BarkTransformAnalyser::bark(float f)
{
    float b = 13 * atan(0.00076 * f) + 3.5 * atan(pow(f / 7500.0f, 2));
    return ((int)floor(b));
}