#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup()
{
	ofxPoint		int3d;
	ofxFloatPoint	float3d;
	ofxUCharPoint	uchar3d;
	ofxDoublePoint	double3d;
	
	process(int3d);
	process(float3d);
	process(uchar3d);
	process(double3d);
}

//--------------------------------------------------------------
void ofApp::process(ofxPoint p3d)
{
	p3d += 3.33;
	cout << p3d << endl;
	p3d.x*=2;
	cout << p3d << endl;
}

void ofApp::process(ofxFloatPoint p3d)
{
	p3d += 3.33;
	cout << p3d << endl;
	p3d.x*=2;
	cout << p3d << endl;
}

void ofApp::process(ofxUCharPoint p3d)
{
	p3d += 250;
	cout << p3d << endl;
	p3d.x*=2;
	cout << p3d << endl;
}

void ofApp::process(ofxDoublePoint p3d)
{
	p3d.set(3.33);
	cout << p3d << endl;
	p3d.x/=2;
	cout << p3d << endl;
}

//--------------------------------------------------------------
void ofApp::update()
{
}

//--------------------------------------------------------------
void ofApp::draw()
{
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key)
{

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key)
{

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y )
{

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button)
{

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button)
{

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button)
{

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h)
{

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg)
{

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo)
{

}
