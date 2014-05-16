#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup()
{
	ofSetLogLevel(OF_LOG_VERBOSE);
	//    ofSetFrameRate(20);
	ofSetVerticalSync(true);
	ofSetupScreenOrtho();
	ofBackground(40);
	//	ofEnableBlendMode(OF_BLENDMODE_ADD);
	
	// GIF
	gifEncoder.setup(ofGetWidth(), ofGetHeight(), 0.02, 255);
    ofAddListener(ofxGifEncoder::OFX_GIF_SAVE_FINISHED, this, &ofApp::onGifSaved);
	
	// shader
	FBOq		= 1;
	Zq			= 1;
	thresh		= 0;
	density		= 1;
	dithering	= 0;
	linearFilter= true;
	
	// slice
	clipPlaneDepth	= 1;//.50;
	azimuth			= .4;//0;
	elevation		= 1;//-.50;
	
	//	cam.setFov(1);
	blabels = true;
	
	// Init Volume
	initVolume();

	// camera
    cam.setDistance(1000);
    cam.enableMouseInput();
}

//--------------------------------------------------------------
void ofApp::initVolume()
{
	// Init Volume
	volume.loadVolume("volumes/Colin27T1_tight/");
//	volume.loadVolume("2014-05-16-12-26-30-216");
//	volume.setImageType(OF_IMAGE_COLOR_ALPHA);
//	volume.loadImageSequence("volumes/talairach_nii/");

//	volume.loadAsRGBA("volumes/Colin27T1_tight/");
//	volume.loadColorPow2("volumes/Colin27T1_tight/");

//	bPow2	= volume.getVoxelsRef().isPow2();
//	format	= volume.getVoxelsRef().getGlFormat();
	
	// Init Volume Rendering
//    volumeRender.setup(&volume, ofVec3f(1,1,1), bPow2, format);
    volumeRender.setup(&volume);
	volumeRender.setRenderSettings(FBOq, Zq, density, thresh);
	volumeRender.setDithering(dithering);
	volumeRender.setClipDepth(clipPlaneDepth);
	volumeRender.setElevation(elevation);
	volumeRender.setAzimuth(azimuth);
}

//--------------------------------------------------------------
void ofApp::draw()
{
		//ofSetColor(255);
		cam.begin();
		volumeRender.update();
		cam.end();
		
		volumeRender.draw(0, ofGetHeight(), ofGetWidth(), -ofGetHeight());
		if (blabels){
			drawLabels();
		}
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key)
{
	
    switch(key)
    {
		case 'w':
			selectVoxels();
			break;
		case ' ':
			//			gifEncoder.toggleRecording();
            break;
        case 's':
            cout << "Start saving..." << endl;
//			date=ofGetTimestampString();
//			gifEncoder.save(date+".gif");
			volume.saveVolume("i_.png");
            break;
		case 'h':
			blabels=!blabels;
			break;
		case 'f':
			ofToggleFullscreen();
			break;
		case 'a':
//			volume.mirror(false, true, false);
//			volume.setImageType(OF_IMAGE_COLOR_ALPHA);
			volume.grabScreen(ofGetMouseX(), ofGetMouseY());
//			volume.setColor(ofColor::magenta);
			volumeRender.setVolume(&volume);

			break;
			
		case 't':
			volumeRender.setThreshold(volumeRender.getThreshold()-0.01);
			break;
		case 'T':
			volumeRender.setThreshold(volumeRender.getThreshold()+0.01);
			break;
		case 'd':
			volumeRender.setDensity(volumeRender.getDensity()-0.01);
			break;
		case 'D':
			volumeRender.setDensity(volumeRender.getDensity()+0.01);
			break;
		case 'q':
			volumeRender.setXyQuality(volumeRender.getXyQuality()-0.01);
			break;
		case 'Q':
			volumeRender.setXyQuality(volumeRender.getXyQuality()+0.01);
			break;
		case 'z':
			volumeRender.setZQuality(volumeRender.getZQuality()-0.01);
			break;
		case 'Z':
			volumeRender.setZQuality(volumeRender.getZQuality()+0.01);
			break;
		case 'l':
			volumeRender.setVolumeTextureFilterMode(GL_LINEAR);
			linearFilter = true;
			break;
			
		case 'I':
			clipPlaneDepth += 0.01;
			volumeRender.setClipDepth(clipPlaneDepth);
			break;
		case 'i':
			clipPlaneDepth -= 0.01;
			volumeRender.setClipDepth(clipPlaneDepth);
			break;
			
		case 'O':
			elevation += 0.01;
			volumeRender.setElevation(elevation);
			break;
		case 'o':
			elevation -= 0.01;
			volumeRender.setElevation(elevation);
			break;
			
		case 'P':
			azimuth += 0.01;
			volumeRender.setAzimuth(azimuth);
			break;
		case 'p':
			azimuth -= 0.01;
			volumeRender.setAzimuth(azimuth);
			break;
		case 'n':
			volumeRender.setVolumeTextureFilterMode(GL_NEAREST);
			linearFilter = false;
			break;
		case OF_KEY_UP:
			cam.getTarget().boom(-5);
			break;
		case OF_KEY_DOWN:
			cam.getTarget().boom(5);
			break;
		case OF_KEY_LEFT:
			cam.getTarget().truck(-5);
			break;
		case OF_KEY_RIGHT:
			cam.getTarget().truck(5);
			break;
    }
	
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

//--------------------------------------------------------------
void ofApp::exit(){
    gifEncoder.exit();
}

//--------------------------------------------------------------
void ofApp::drawLabels()
{
    ofSetColor(0,0,0,64);
    ofRect(0,0,270,190);
    ofSetColor(255,255,255,255);

    ofDrawBitmapString(
					   "fps: " + ofToString(ofGetFrameRate())+ "\n" +
					   "volume dimensions: " + ofToString(volumeRender.getVolumeWidth()) +
					   "x" + ofToString(volumeRender.getVolumeHeight()) +
					   "x" + ofToString(volumeRender.getVolumeDepth()) + "\n" +
					   "FBO quality (q/Q): " + ofToString(volumeRender.getRenderWidth()) +
					   "x" + ofToString(volumeRender.getRenderHeight()) + "\n" +
					   "Z quality (z/Z):	" + ofToString(volumeRender.getZQuality()) + "\n" +
					   "Threshold (t/T):	" + ofToString(volumeRender.getThreshold()) + "\n" +
					   "Density (d/D):		" + ofToString(volumeRender.getDensity()) + "\n" +
					   "Filter mode (l/n):  " + (linearFilter?"linear":"nearest") + "\n" +
					   "clipPlaneDepth:     " + ofToString(clipPlaneDepth) + "\n" +
					   "azimuth:            " + ofToString(azimuth) + "\n" +
					   "elevation:          " + ofToString(elevation)
					   ,20,20);

}

//--------------------------------------------------------------
void ofApp::selectVoxels()
{
/*	vector<ofxIntBox> boxes;
	
	ofxIntPoint position	= ofVec3f(ofRandom(100));
	ofxIntPoint size		= ofVec3f(ofRandom(10));
	
	ofxIntBox box(position, size);
	boxes.push_back(box);
	
	volume.selectVoxels(boxes);
*/
}
//--------------------------------------------------------------
void ofApp::paintRandomBoxes()
{
	/*
	ofxIntPoint position	= ofVec3f(ofRandom(100));
	ofxIntPoint size		= ofVec3f(ofRandom(10), ofRandom(5), ofRandom(5));
	
	ofxIntBox box(position, size);
	vector<ofxPoint> c = volume.getVoxelsinBox(box);
	cout << "c size= "<< c.size() << endl;
	for(int i=0; i<c.size(); i++){
		cout << "i= "<< i << endl;
		ofxIntPoint p(c[i].x, c[i].y, c[i].z);
		cout << "ip= "<< p << endl;
		volume.setColor(p.x,p.y,p.z, ofColor::white);
	}
	volumeRender.setVolume(&volume, bPow2, format);
	 */
}



//--------------------------------------------------------------
void ofApp::update()
{
	//    ofSetWindowTitle(ofToString(ofGetFrameRate()));
	//	gifEncoder.update();
}

//--------------------------------------------------------------
void ofApp::onGifSaved(string &fileName) {
    cout << "gif saved as " << fileName << endl;
	gifEncoder.reset();
}













