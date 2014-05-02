#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup()
{
	ofSetLogLevel(OF_LOG_VERBOSE);
//    ofSetFrameRate(20);
	ofSetVerticalSync(true);
	ofSetupScreenOrtho();
	ofBackground(40);
	boxW	= boxH = 200;
//	ofEnableBlendMode(OF_BLENDMODE_ADD);
	
	// GIF
	gifEncoder.setup(ofGetWidth(), ofGetHeight(), 0.02, 255);
    ofAddListener(ofxGifEncoder::OFX_GIF_SAVE_FINISHED, this, &testApp::onGifSaved);
	
	// shader
	FBOq		= 1;
	Zq			= 2;
	thresh		= 0;
	density		= 10;
	dithering	= 0;
	linearFilter= true;
	
	// slice
	clipPlaneDepth	= 1;//.50;
	azimuth			= 1;//0;
	elevation		= 1;//-.50;

//	cam.setFov(1);
	blabels = true;
	
	// Init Volume
	initVolume();
	
	// Volume rendering
	initVolumeRendering();

	// camera
    cam.setDistance(30000);
    cam.setDistance(1000);
    cam.enableMouseInput();


/*	v.setPosition(ofPoint(1));
	v.setSize(ofPoint(1));
	cout << "v.pix.size = "<< v.pix.size()<< endl;
	cout << "v.pix.isAllocated = "<< v.pix.isAllocated()<< endl;
	cout << "v = "<< v << endl;
//	cout << "v.pix.isAllocated = "<< v.getBitsPerPixel()<< endl;
*/
}

//--------------------------------------------------------------
void testApp::initVolume()
{
	// Init Volume
	volume.load("volumes/Colin27T1_tight/");
//	volume.load("volumes/talairach_nii/");
//	volume.getVoxels().mirror(true, false, false);
//	volume.getVoxels().mirror(false, true, false);
//	volume.mirror(true, false, false);

	
	ofVec3f volSize(100);
//	volume.allocate(volSize, OF_PIXELS_MONO);
//	volume.setup(boxW, boxH);
}


//--------------------------------------------------------------
void testApp::initVolumeRendering()
{
	// Init Volume Rendering
	// don't use pow2 now! the shader is not working!!
    volumeRender.setup(&volume, false, GL_LUMINANCE);
//	cout << volume.getWidth();
	
	volumeRender.setRenderSettings(FBOq, Zq, density, thresh);
//	volumeRender.setVolumeTextureFilterMode(GL_LINEAR);
//	volumeRender.setVolumeTextureFilterMode(GL_NEAREST);
	volumeRender.setClipDepth(clipPlaneDepth);
//	volumeRender.setSlices(&uiClamp);
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
//	ofBackgroundGradient(100, 50);

	ofSetColor(255);
	cam.begin();
	volumeRender.update();
	cam.end();
	volumeRender.draw(0, ofGetHeight(), ofGetWidth(), -ofGetHeight());
	
	if (blabels) drawLabels();
//	ofRectangle();
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
		case 'h':
			blabels=!blabels;
			break;
		case 'f':
			ofToggleFullscreen();
			break;
		case 'a':
//			volume.getVoxels().mirror(false, true, false);
//			volume.getVoxels().rotate90(1);
			volumeRender.setVolume(&volume, false, GL_LUMINANCE);
//			volumeRender.setVolume(volume.getVoxelsData());
			volumeRender.setup(&volume, false, GL_LUMINANCE);
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
	
    ofDrawBitmapString("fps: " + ofToString(ofGetFrameRate())+ "\n" +
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
void testApp::selectVoxels()
{
	vector<ofxBox> boxes;
	
	ofVec3f position	= ofVec3f(10);
	ofVec3f size		= ofVec3f(3);
	
	ofxBox box(position, size);
	boxes.push_back(box);
	
//	ofRemove(position, false);
	
	volume.selectVoxels(boxes);
}











