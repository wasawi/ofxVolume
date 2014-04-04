/*  ofxVolume - draw ofxVolumetric data
 Created by Jordi on 01/04/14.
 */

#pragma once
#include "ofxImageSequencePlayer.h"
#include "ofxGifEncoder.h"

//Volume Cutting Viepoint
enum viewPoint
{
	CORONAL,	//	collar
	SAGITTAL,	//	cresta
	AXIAL		//	diadema
};

class ofxVolume{
public:
	
	ofxVolume();
    virtual ~ofxVolume();

	void allocate(ofVec3f size);
	void load(string path="");
	void setup(float bW, float bH);
	void redraw(viewPoint vP, int depth);

	
	// Getters
	unsigned char* getVoxels();
	ofVec3f getVolSize();
	ofVec3f getVolPos();
	
	int getVoxelValue();
	int getVoxelNumber();
	ofVec3f getVoxelCoordinates(int _index);
	bool getVoxelCoordAndVal(int _index, ofVec3f& _coord, int& _val);
	ofVec3f	getNormalizedCoords(ofVec3f _coord);
	
	
	void draw(viewPoint vP);
//	vector<unsigned char> selectVoxels(vector <ofVec3f> _coord, vector <float> radius);
	void selectVoxels(vector <ofVec3f>& _coord, vector <float>& radius);
	vector <ofVec3f> getVoxelsinBox(ofVec3f& _coord, ofVec3f& _size);
//	void getVoxelsinBox(ofVec3f& _coord, ofVec3f& _size);
	
	unsigned char * voxels;
	
protected:
private:
	void drawBox();
	void redrawSagittal();
	void redrawAxial();
	void redrawCoronal();

	vector <ofVec3f> getVoxelsinRadius(ofVec3f& _coord, float& _radius);
	bool inside(ofVec3f _coord);
	
    ofxImageSequencePlayer imageSequence;


	ofPixels myPixels;

	int volWidth, volHeight, volDepth;
	ofVec3f	volSize;
	ofVec3f	volPos;

	int renderWidth, renderHeight;
	
	/*
	int volWidth, volHeight, volDepth;
	int voxelValue;
	int voxelNumber;
	*/
	
	int halfH;
	int halfW;
	int halfD;
	
	float boxW;
	float boxH;

	// the depth of the current slice
	int coronalS;
	int sagittalS;
	int axialS;

	// one image for eaxh point of view
	ofImage coronal;
	ofImage sagittal;
	ofImage axial;
	
	// one pixel array for each image
	ofPixels coronalPixels;
	ofPixels sagittalPixels;
	ofPixels axialPixels;
	
	ofColor lineColor;
	
	bool insideCoronal;
	bool insideSagittal;
	bool insideAxial;
	
	// quick ways of checking if inside radius:
	// http://stackoverflow.com/questions/481144/equation-for-testing-if-a-point-is-inside-a-circle
	
	
	vector<ofPixels> selectedVoxels;
	
};