#include "AudioInputSource.h"

//--------------------------------------------------------------
void AudioInputSource::declareSettings()
{
    params.setName(name);
    params.add(volumeMultiplier.set("Volume Multiplier", 1.0, 0.0, 10.0));
}

//--------------------------------------------------------------
void AudioInputSource::setup(ContextPtr& context)
{
    ofSoundStreamSettings settings;
    auto devices = soundStream.getDeviceList(ofSoundDevice::Api::MS_WASAPI);

    for (int i = 0; i < devices.size(); ++i) {
        std::cout << "[MS_WASAPI : " << devices[i].deviceID << "] | Name: " << devices[i].name <<
            " [in: " << devices[i].inputChannels <<
            " , out: " << devices[i].outputChannels << "]" << std::endl;
    }

    int deviceId;
    std::cout << "Choose input device index for MS_WASAPI: " << std::endl;
    std::cin >> deviceId;
    settings.setInDevice(devices[deviceId]);
    std::cout << "Device selected: " << devices[deviceId].name << std::endl;

    settings.setInListener(this);
    settings.sampleRate = sampleRate;
    settings.numOutputChannels = 0;
    settings.numInputChannels = 2;
    settings.bufferSize = bufferSize;
    soundStream.setup(settings);

    // not sure we can have this here? if the setup context only gets called at the beginning
    context->setResult("soundBufferSize", bufferSize);
    context->setResult("sampleRate", sampleRate);
}

//--------------------------------------------------------------
void AudioInputSource::update(ContextPtr& context)
{
    std::unique_lock<std::mutex> scopedLock(soundMutex);

    context->setResult("soundBuffer", soundBuffer * volumeMultiplier);

    waveform.clear();
    for (size_t i = 0; i < soundBuffer.getNumFrames(); i++)
    {
        float x = ofMap(i, 0, soundBuffer.getNumFrames(), 0, bufferSize);
        float y = ofMap(soundBuffer[i], -1, 1, -waveformScale, waveformScale);
        waveform.addVertex(x, y);
    }
}

//--------------------------------------------------------------
void AudioInputSource::drawToGui(ContextPtr& context)
{
    std::unique_lock<std::mutex> scopedLock(soundMutex);

    // draw buffer
    ofPushMatrix();
    ofTranslate(500, 80);
    ofNoFill();
    ofSetColor(ofColor::orange);

    ofDrawBitmapString("Signal:", 0, -60);
    ofRect(0, -50, bufferSize, 100);

    // Buffer samples are between -1 and 1 so we want to somehow show when these extremes
    // are being reached and we are getting clipping.
    // The rectangle has height 100 pixels and we are mapping our soundBuffer signal values to 
    // [-40, 40]. This means that if we were clipping we would reach the maximum value = +-45.
    // We draw a dashed red line to show this.
    waveform.draw();

    ofSetColor(ofColor::indianRed);
    ofLine(0, -waveformScale, bufferSize, -waveformScale);
    ofLine(0, waveformScale, bufferSize, waveformScale);

    ofPopMatrix();
}

//--------------------------------------------------------------
void AudioInputSource::audioIn(ofSoundBuffer& input)
{
    std::unique_lock<std::mutex> scopedLock(soundMutex);

    soundBuffer = input;
    //fftProcessor.setAudioData(input * volumeMultiplier);// MAYBE WE COULD ADD A MULTIPLIER HERE ????
}