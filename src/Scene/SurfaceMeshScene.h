#pragma once

#include "ofMain.h"
#include "AbstractScene.h"
#include "ofxDropdown.h"

class SurfaceMeshScene : public AbstractScene
{
public:

	enum class Shape
	{
		FUZZY,
		BLOBS
	};

	enum class Colour
	{
		WHITE,
		RAINBOW,
		DEPTH
	};

	enum class Rotation
	{
		NONE,
		Z,
		XYZOOM
	};

	SurfaceMeshScene(const std::string& name_, ofEasyCam& easyCam_)
		: AbstractScene(name_)
		, camera(easyCam_) {};

	~SurfaceMeshScene() {};

	virtual void declareSettings() override;
	virtual void addDropdownsToPanel(std::shared_ptr<ofxPanel> panel) override;
	virtual void setup(ContextPtr& context) override;
	virtual void update(ContextPtr& context) override;
	virtual void draw(ContextPtr& context) override;

	// Listeners
	void colourChanged(int& colour);
	void rotationChanged(int& rotation);
	void shapeChanged(int& shape);

private:

	float calculateDistortion(const int& x, const int& y, const int& w, const float bass, const float mids, const float highs);
	ofFloatColor calculateColor(const ofVec3f& position);
	//ofVboMesh mesh;
	ofPlanePrimitive plane;

	//ofEasyCamPtr& camera;
	ofEasyCam& camera;

	//ofTexture displacementTex;
	ofImage img;
	//ofImage colImg;
	ofPixels displacementPix;
	//ofPixels colPix;
	ofShader shader;
	float circleLower;
	float circleUpper;
	int size;

	ofParameter<int> colour;
	ofParameter<int> rotation;
	ofParameter<int> shape;
	unique_ptr<ofxIntDropdown> colourDD;
	unique_ptr<ofxIntDropdown> rotationDD;
	unique_ptr<ofxIntDropdown> shapeDD;

	Colour currentColour = Colour::WHITE;
	Rotation currentRotation = Rotation::NONE;
	Shape currentShape = Shape::FUZZY;
};