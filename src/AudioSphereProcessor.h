#pragma once
#include "ofMain.h"
#include "Constants.h"

struct PingPongBuffer {
public:
    void allocate(int _width, int _height, int _internalformat = GL_RGBA, float _dissipation = 1.0f) {
        // Allocate
        for (int i = 0; i < 2; i++) {
            FBOs[i].allocate(_width, _height, _internalformat);
        }

        // Clean
        clear();

        // Set everything to 0
        flag = 0;
        swap();
        flag = 0;
    }

    void swap() {
        src = &(FBOs[(flag) % 2]);
        dst = &(FBOs[++(flag) % 2]);
    }

    void clear() {
        for (int i = 0; i < 2; i++) {
            FBOs[i].begin();
            ofClear(0, 255);
            FBOs[i].end();
        }
    }

    ofFbo& operator[](int n) { return FBOs[n]; }

    ofFbo* src;       // Source       ->  Ping
    ofFbo* dst;       // Destination  ->  Pong
private:
    ofFbo   FBOs[2];    // Real addresses of ping/pong FBO«s
    int     flag;       // Integer for making a quick swap
};

//class AudioSphereProcessor : public AbstractProcessor 
//{
//
//public:
//    AudioSphereProcessor(const std::string& name_)
//        : AbstractProcessor(name_) {};
//
//    virtual ~AudioSphereProcessor() {};
//
//    void declareSettings() override;
//    void setup() override;
//    void update(ContextPtr& context) override;
//    void draw() override;
//
//private:
//    void buildSphereMesh();
//
//    ofVboMesh sphereMesh;
//    ofTexture eqTexture;
//    ofPath eqPath;
//
//    PingPongBuffer posBuffer;
//    ofFbo posFbo;
//    ofShader shader;
//    ofShader posShader;
//
//    int sphereRadius = 300;
//    int sphereResolution = 250;
//    int fboResolution;
//    float axisLocation;
//    float startOffsetAngle = 90.0f;
//    float angleIncrement;
//
//    //std::vector<float> barkSpectrum;
//    float bins[BARK_MAX]; //where we sum up our spectral info
//    ofParameter<float> posDecayRate;
//};
