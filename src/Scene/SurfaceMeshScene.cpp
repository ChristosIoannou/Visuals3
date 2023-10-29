#include "SurfaceMeshScene.h"

//--------------------------------------------------------------
// If the processor doesn't have any parameters, remove this function from the class and 
// use the declareSettings() inherited from AbstractProcessor. This will make sure a panel
// is generated for this processor on the GUI.
void SurfaceMeshScene::declareSettings()
{
    params.setName(name);
    
    colourDD = std::make_unique<ofxIntDropdown>(colour);
    colourDD->add((int)Colour::WHITE, "White");
    colourDD->add((int)Colour::RAINBOW, "Rainbow");
    colourDD->add((int)Colour::DEPTH, "Depth");
    colourDD->disableMultipleSelection();
    colourDD->enableCollapseOnSelection();
    colourDD->setName("Colour Scheme");
    colourDD->setSelectedValueByIndex((int)currentColour, false);

    rotationDD = std::make_unique<ofxIntDropdown>(rotation);
    rotationDD->add((int)Rotation::NONE, "None");
    rotationDD->add((int)Rotation::Z, "Z");
    rotationDD->add((int)Rotation::XYZOOM, "XYZOOM");
    rotationDD->disableMultipleSelection();
    rotationDD->enableCollapseOnSelection();
    rotationDD->setName("Rotation");
    rotationDD->setSelectedValueByIndex((int)currentRotation, false);

    shapeDD = std::make_unique<ofxIntDropdown>(shape);
    shapeDD->add((int)Shape::FUZZY, "Fuzzy");
    shapeDD->add((int)Shape::BLOBS, "Blobs");
    shapeDD->disableMultipleSelection();
    shapeDD->enableCollapseOnSelection();
    shapeDD->setName("Shape");
    shapeDD->setSelectedValueByIndex((int)currentShape, false);
}

//--------------------------------------------------------
void SurfaceMeshScene::addDropdownsToPanel(std::shared_ptr<ofxPanel> panel)
{
    panel->add(colourDD.get());
    panel->add(rotationDD.get());
    panel->add(shapeDD.get());
}

//--------------------------------------------------------------
void SurfaceMeshScene::setup(ContextPtr& context)
{
    ofDisableArbTex();
    float planeScale = 0.75;
    size = ofGetHeight() * planeScale;
    int planeGridSize = 6;
    int planeColumns = size / planeGridSize;

    shader.load("shaders/surfaceMesh/surfaceMesh.vert", "shaders/surfaceMesh/surfaceMesh.frag");
    //img.allocate(planeColumns, planeColumns, OF_IMAGE_GRAYSCALE);
    img.allocate(planeColumns, planeColumns, OF_IMAGE_COLOR_ALPHA);
    plane.set(size, size, planeColumns, planeColumns);
    plane.mapTexCoordsFromTexture(img.getTexture());
}

//--------------------------------------------------------------
void SurfaceMeshScene::update(ContextPtr& context)
{
    if (!context->hasResult("bass") ||
        !context->hasResult("mids") ||
        !context->hasResult("highs"))
        return;

    auto bass = context->getResult<float>("bass");
    auto mids = context->getResult<float>("mids");
    auto highs = context->getResult<float>("highs");
    
    ofPixels& pixels = img.getPixels();
    int w = img.getWidth();
    int h = img.getHeight();

    for (size_t y = 0; y < h; ++y)
    {
        for (size_t x = 0; x < w; ++x)
        {
            int i = (y * w + x) * 4; // 4 channels RGBA

            const auto distortion = calculateDistortion(x, y, w * 2, bass, mids, highs);

            pixels[i] = 0;
            pixels[i + 1] = 0;
            pixels[i + 2] = 0;
            pixels[i + 3] = distortion;
        }
    }

    img.update();
    
}

//--------------------------------------------------------------
void SurfaceMeshScene::draw(ContextPtr& context)
{
    camera.begin();

    // bind our texture. in our shader this will now be tex0 by default
    // so we can just go ahead and access it there.
    img.getTexture().bind();

    shader.begin();

    ofPushMatrix();

    plane.drawWireframe();

    ofPopMatrix();

    shader.end();

    camera.end();

}

//------------------------------------------------------
void SurfaceMeshScene::colourChanged(int& colour)
{
    currentColour = static_cast<Colour>(colour);
}

//------------------------------------------------------
void SurfaceMeshScene::rotationChanged(int& rotation)
{
    currentRotation = static_cast<Rotation>(rotation);
}

//------------------------------------------------------
ofFloatColor SurfaceMeshScene::calculateColor(const ofVec3f& position) {
    ofFloatColor surfaceColor;
    float frameNumMod = 1.0 - float((ofGetFrameNum() % (20))) / 20.0;
    float width;

    switch (currentColour)
    {
    case Colour::DEPTH:
        surfaceColor.setHsb(ofMap(position.z, 0, 30, 0.392, 0.588, false), 1.0f, 1.0f, 0.3);
        break;
    case Colour::RAINBOW:
        width = ofMap(position.length(), 0, size * 3 * sqrt(2), 0.0f, 1.0f, true);
        surfaceColor.setHsb(ofWrap(width + frameNumMod, circleLower, circleUpper), 1.0f, 1.0f, 0.3);
        break;
    case Colour::WHITE:
        surfaceColor.set(1.f, 1.f, 1.f, 0.3);
    }

    return surfaceColor;
}

//------------------------------------------------------
float SurfaceMeshScene::calculateDistortion(const int& x, const int& y, const int& w, float bass, float mids, float highs)
{
    //float bassContrib = ofNoise(ofMap(x, 0, w, 0, 5, false), ofMap(y, 0, w, 0, 5, false), ofGetElapsedTimef()) * bass / 0.5;
    //float midsContrib = ofNoise(ofMap(x, 0, w, 0, 12, false), ofMap(y, 0, w, 0, 12, false), ofGetElapsedTimef()) * mids / 1.5;
    //float highsContrib = ofNoise(ofMap(x, 0, w, 0, 30, false), ofMap(y, 0, w, 0, 30, false), ofGetElapsedTimef()) * highs / 3.0;

    float bassContrib = ofNoise(ofMap(x, 0, w, 0, 5), ofMap(y, 0, w, 0, 5), ofGetElapsedTimef()) * bass / 0.5;
    float midsContrib = ofNoise(ofMap(x, 0, w, 0, 12), ofMap(y, 0, w, 0, 12), ofGetElapsedTimef()) * mids / 1.5;
    float highsContrib = ofNoise(ofMap(x, 0, w, 0, 30), ofMap(y, 0, w, 0, 30), ofGetElapsedTimef()) * highs / 3.0;
    return (bassContrib + midsContrib + highsContrib) * 20;
    //return ofMap((bassContrib + midsContrib + highsContrib), 0.0, 0.1, 0.0, 255.0, true);
}