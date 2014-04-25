#pragma once

#include "ofMain.h"
#include "ofxGifEncoder.h"
#include "ofxVolumetrics.h"
#include "ofxVolume.h"

class testApp : public ofBaseApp
{

public:
    void setup();
    void update();
    void draw();
	void drawLabels();
	void initVolume();
	void initVolumeRendering();

    void keyPressed  (int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);

    void onGifSaved(string & fileName);
    void exit();

	void selectVoxels();
	
    ofEasyCam cam;
    bool blabels;
    ofxGifEncoder gifEncoder;
	
	// Volume Rendering
    ofxVolumetrics volumeRender;
	ofFbo myfboRender;
	ofVec3f cubeSize;
	
	// Volume Slice
	ofxVolume	volume;
	int volWidth, volHeight, volDepth;
	ofVec3f volSize;
	
	// Volume rendering UI vars
	float FBOq, Zq, thresh, density, dithering;
	float lastClipPlaneDepth;
	float clipPlaneDepth, azimuth, elevation;
    bool linearFilter;

	float	boxW, boxH;

	string date;

};
