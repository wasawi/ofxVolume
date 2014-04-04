#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup()
{
	ofSetLogLevel(OF_LOG_VERBOSE);
    ofSetFrameRate(20);
	ofSetupScreenOrtho();
//	ofBackground(0,255,255);
	boxW	= boxH = 200;
//	ofEnableBlendMode(OF_BLENDMODE_ADD);
	
	// GIF
	gifEncoder.setup(ofGetWidth(), ofGetHeight(), 0.02, 255);
    ofAddListener(ofxGifEncoder::OFX_GIF_SAVE_FINISHED, this, &testApp::onGifSaved);
	
	// shader
	FBOq		= 1;
	Zq			= 2;
	thresh		= 0;
	density		= 1;
	dithering	= 0;
	linearFilter= true;
	
	// slice
	clipPlaneDepth	= .50;
	azimuth			= 0;
	elevation		= -.50;

//	cam.setFov(1);
	blabels=true;
	
	// Init Volume
	volSize=ofVec3f(20);
	initVolume();
	
	// Volume rendering
	initVolumeRendering();

	// camera
    cam.setDistance(30000);
    cam.setDistance(1000);
    cam.enableMouseInput();
}

//--------------------------------------------------------------
void testApp::initVolume()
{
	// Init Volume
//	volume.load("volumes/Colin27T1_tight/");
//	volume.load("volumes/talairach_nii/");
	volume.allocate(volSize);
	volume.setup(boxW, boxH);
	
	volSize		= volume.getVolSize();
	volWidth	= volSize.x;
	volHeight	= volSize.y;
	volDepth	= volSize.z;
}


//--------------------------------------------------------------
void testApp::initVolumeRendering()
{
	// Init Volume Rendering
    volumeRender.setup(volume.getVolSize(), true);
	volumeRender.setVolume(volume.getVoxels());
    volumeRender.setRenderSettings(FBOq, Zq, density, thresh);
//	volumeRender.setVolumeTextureFilterMode(GL_LINEAR);
	volumeRender.setVolumeTextureFilterMode(GL_NEAREST);
	volumeRender.setClipDepth(clipPlaneDepth);
//	volumeRender.setSlices(&uiClamp);
	
	cubeSize	= volumeRender.getCubeSize();
}


//--------------------------------------------------------------
void testApp::update()
{
    ofSetWindowTitle(ofToString(ofGetFrameRate()));
	gifEncoder.update();
}

//--------------------------------------------------------------
void testApp::draw()
{
	ofBackgroundGradient(100, 50);

	ofSetColor(255);
	cam.begin();
	volumeRender.updateVolume();
	cam.end();
	volumeRender.draw(0, ofGetHeight(), ofGetWidth(), -ofGetHeight());
	
	if (blabels) drawLabels();
}

void testApp::onGifSaved(string &fileName) {
    cout << "gif saved as " << fileName << endl;
	gifEncoder.reset();
}

//--------------------------------------------------------------
void testApp::keyPressed(int key)
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
			date=ofGetTimestampString();
//			gifEncoder.save(date+".gif");
            break;
		case 'c':
			blabels=!blabels;
			break;
		case 'f':
			ofToggleFullscreen();
			break;
		case 'a':
			delete volume.voxels;
			volume.allocate(volSize);
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
void testApp::keyReleased(int key)
{

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y )
{

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button)
{

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button)
{

}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button)
{

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h)
{

}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg)
{

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo)
{

}

//--------------------------------------------------------------
void testApp::exit(){
    gifEncoder.exit();
}

//--------------------------------------------------------------
void testApp::drawLabels()
{
    ofSetColor(0,0,0,64);
    ofRect(0,0,270,190);
    ofSetColor(255,255,255,255);
	
    ofDrawBitmapString("volume dimensions: " + ofToString(volumeRender.getVolumeWidth()) + "x" + ofToString(volumeRender.getVolumeHeight()) + "x" + ofToString(volumeRender.getVolumeDepth()) + "\n" +
                       "FBO quality (q/Q): " + ofToString(volumeRender.getRenderWidth()) + "x" + ofToString(volumeRender.getRenderHeight()) + "\n" +
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
void testApp::selectVoxels()
{
	vector<ofVec3f> positions;
	vector<float>	radiuses;
	
	ofVec3f position	= ofVec3f(10, 10, 10);
	float radius		= 0;
	
	cout << "point " << position << endl;
	cout << "rad " << radius << endl;
	
	positions.push_back(position);
	radiuses.push_back(radius);
	
	volume.selectVoxels(positions, radiuses);
}











