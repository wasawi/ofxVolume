/*  ofxVolume - draw ofxVolumetric data
 Created by Jordi on 01/04/14.
 */

#pragma once
#include "ofxImageSequencePlayer.h"
#include "ofxBox.h"
#include "ofxVoxels.h"

//Volume Cutting Viepoint
enum slice
{
	CORONAL,	//	collar
	SAGITTAL,	//	cresta
	AXIAL		//	diadema
};

class ofxVolume : public ofxVoxels{
public:
	
	ofxVolume();
    virtual ~ofxVolume();

	void load(string path="");
	void setup(float bW, float bH);
	void redraw(slice vP, int depth);
	void destroy();
	
	// Getters
//	ofVec3f getSize();
//	ofVec3f getPos();
	
	int getVoxelValue();
	int getVoxelNumber();
	ofVec3f getVoxelCoordinates(int _index);
	bool getVoxelCoordAndVal(int _index, ofVec3f& _coord, int& _val);
	ofVec3f	getNormalizedCoords(ofVec3f _coord);
	
	
	void draw(slice vP);
//	vector<unsigned char> selectVoxels(vector <ofVec3f> _coord, vector <float> radius);
	void clearSelected();
	
	void selectVoxels(vector <ofxBox>& boxes);
	void selectVoxels(ofxBox& box);
	vector <ofVec3f> getVoxelsinBox(ofxBox& box);
//	void getVoxelsinBox(ofVec3f& _coord, ofVec3f& _size);
		
protected:
private:
	void drawBox();
	void redrawSagittal();
	void redrawAxial();
	void redrawCoronal();

	ofxBox outerBox; //the box outside the volume useful if volume is non symetrical and you nedd a symetrical box to handle gui or others..
	
	vector <ofVec3f> getVoxelsinRadius(ofVec3f& _coord, float& _radius);
	bool inside(ofVec3f _coord);
	
	int w,h,d;
	int renderWidth, renderHeight;
	int halfH;
	int halfW;
	int halfD;
	float boxW;
	float boxH;
	
	// the depth of the current cutting plane
	int coronalS;
	int sagittalS;
	int axialS;

	// one image for each cutting plane
	ofImage coronal;
	ofImage sagittal;
	ofImage axial;
	
	// one pixel array for cutting plane
	ofPixels coronalPixels;
	ofPixels sagittalPixels;
	ofPixels axialPixels;
	
	bool insideCoronal;
	bool insideSagittal;
	bool insideAxial;

	// quick ways of checking if inside radius:
	// http://stackoverflow.com/questions/481144/equation-for-testing-if-a-point-is-inside-a-circle
	
	vector<ofPixels> selectedVoxels;
};















