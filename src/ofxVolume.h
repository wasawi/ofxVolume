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
	
	void loadColor(string path="");
	void loadMono(string path="");
	void setup(float bW, float bH);
	void redraw(slice vP, int depth);
	void destroy();
	
	// Getters
//	ofxPoint getSize();
//	ofxPoint getPos();
	
	int getVoxelValue();
	int getVoxelNumber();
	ofxPoint getVoxelCoordinates(int _index);
	bool getVoxelCoordAndVal(int _index, ofxPoint& _coord, int& _val);
	ofxPoint	getNormalizedCoords(ofxPoint _coord);
	
	
	void draw(slice vP);
//	vector<unsigned char> selectVoxels(vector <ofxPoint> _coord, vector <float> radius);
	void clearSelected();
	
	void selectVoxels(vector <ofxBox>& boxes);
	void selectVoxels(ofxBox& box);
	vector <ofxPoint> getVoxelsinBox(ofxBox& box);
//	void getVoxelsinBox(ofxPoint& _coord, ofxPoint& _size);
		
protected:
private:
	void drawBox();
	void redrawSagittal();
	void redrawAxial();
	void redrawCoronal();

	ofxBox outerBox; //the box outside the volume useful if volume is non symetrical and you nedd a symetrical box to handle gui or others..
	
	vector <ofxPoint> getVoxelsinRadius(ofxPoint& _coord, float& _radius);
	bool inside(ofxPoint _coord);
	
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















