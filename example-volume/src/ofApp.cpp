#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup()
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
    ofAddListener(ofxGifEncoder::OFX_GIF_SAVE_FINISHED, this, &ofApp::onGifSaved);
	
	// shader
	FBOq		= 1;
	Zq			= 2;
	thresh		= 0;
	density		= 10;
	dithering	= 0;
	linearFilter= false;
	
	// slice
	clipPlaneDepth	= 1;//.50;
	azimuth			= 1;//0;
	elevation		= 1;//-.50;
	
	//	cam.setFov(1);
	blabels = false;
	
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
//	volume.loadColor("volumes/Colin27T1_tight/");
	volume.loadColor("volumes/Colin27T1_tight/");
	//	volume.loadColor("volumes/head/cthead-8bit/");
	//	volume.load("volumes/talairach_nii/");
	
	// Init Volume Rendering
    volumeRender.setup(&volume, ofVec3f(1,1,1), true, GL_RGBA);
	volumeRender.setRenderSettings(FBOq, Zq, density, thresh);
	volumeRender.setDithering(1);
	volumeRender.setClipDepth(clipPlaneDepth);
	bNew = true;
}
//--------------------------------------------------------------
void ofApp::draw()
{
		//ofSetColor(255);
		cam.begin();
		volumeRender.update();
		cam.end();
		
		//volumeRender.draw(0, ofGetHeight(), ofGetWidth(), -ofGetHeight());
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
//			volume.mirror(false, true, false);
//			volume.swapRgb();
			volume.colourRandomVoxels(200);
//			volume.setColor(ofColor::grey);
			volumeRender.setVolume(&volume, true, GL_RGBA);

//			cout << "size = "<<volume.getTotalSize() << endl;
//			volume.setImageType(OF_IMAGE_GRAYSCALE);
//			cout << "size = "<<volume.getTotalSize() << endl;
			//			volume.rotate90(1);
//			cout << volume.getWidth() <<"x"<<volume.getHeight() <<"x"<<volume.getDepth() <<"\n";
//			cout << imageSequence.getWidth() <<"x"<<imageSequence.getHeight() <<"x"<<imageSequence.getSequenceLength() <<"\n";
//			volumeRender.setup(&volume, ofVec3f(1,1,1), true, GL_LUMINANCE);
//			volumeRender.setVolume(&volume, true, GL_LUMINANCE);
//			volumeRender.updateVolumeData(volumeData, imageSequence.getWidth(), imageSequence.getHeight(), imageSequence.getSequenceLength(),0,0,0);
/*			if (bNew){
//				volumeRender.setVolume(&volume, true, GL_RGBA);
				//volumeRender.setup(&volume, ofVec3f(1,1,1), true, GL_RGBA);
				volumeRender.updateVolumeData(volume.getVoxels(), volume.getWidth(), volume.getHeight(), volume.getDepth(),0,0,0);
			}else{
//				volumeRender.setVolume(&volume, false, GL_RGBA);
				volumeRender.updateVolumeData(volume.getVoxels(), volume.getWidth(), volume.getHeight(), volume.getDepth(),0,0,0);
//				volumeRender.updateVolumeData(volumeData, volume.getWidth(), volume.getHeight(), volume.getDepth(),0,0,0);
			}
*/			//			volumeRender.setVolume(volumeData, volume.getSize(), false, GL_RGBA);
			//			volumeRender.setVolume(volume.getVoxelsData());
			//			volumeRender.setup(&volume, false, GL_LUMINANCE);
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
	vector<ofxBox> boxes;
	
	ofVec3f position	= ofVec3f(10);
	ofVec3f size		= ofVec3f(3);
	
	ofxBox box(position, size);
	boxes.push_back(box);
	
	//	ofRemove(position, false);
	
	volume.selectVoxels(boxes);
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













