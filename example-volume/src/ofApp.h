#pragma once

#include "ofMain.h"
#include "ofxGifEncoder.h"
#include "ofxVolumetrics.h"
#include "ofxVolume.h"
#include "ofxPoint.h"

class ofApp : public ofBaseApp
{

public:
    void setup();
    void update();
    void draw();
	void drawLabels();
	void initVolume();
	   
	void keyPressed  (int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);
	
	// others
	ofEasyCam cam;
    bool blabels;
	
    // GIF
	ofxGifEncoder gifEncoder;
	void onGifSaved(string & fileName);
    void exit();
	string		date;
	
	// Volume Rendering
    ofxVolumetrics volumeRender;
	ofxVolume	volume;
	GLint format;
	bool bPow2;
	
	// Volume rendering UI vars
	float FBOq, Zq, thresh, density, dithering;
	float lastClipPlaneDepth;
	float clipPlaneDepth, azimuth, elevation;
    bool linearFilter;

	// test functions
	void selectVoxels();
	void paintRandomBoxes();

    
};





