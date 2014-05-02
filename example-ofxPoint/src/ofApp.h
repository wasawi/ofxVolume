#pragma once

#include "ofMain.h"
#include "ofxPoint.h"

class ofApp : public ofBaseApp
{

public:
    void setup();
    void update();
    void draw();
	
	void process(ofxPoint p3d);
	void process(ofxFloatPoint p3d);
	void process(ofxUCharPoint p3d);
	void process(ofxDoublePoint p3d);
	
    void keyPressed  (int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);

};
