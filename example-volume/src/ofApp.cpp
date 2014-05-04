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
	linearFilter= true;
	
	// slice
	clipPlaneDepth	= 1;//.50;
	azimuth			= 1;//0;
	elevation		= 1;//-.50;

//	cam.setFov(1);
	blabels = true;
	
	// Init Volume
	initVolume();
//	initVolume_OLD();
//	initVolumeChar();
	
	// Volume rendering
	initVolumeRendering();
}

//--------------------------------------------------------------
void ofApp::initVolume()
{
	// Init Volume
	volume.loadColor("volumes/Colin27T1_tight/");
//	volume.loadColor("volumes/head/cthead-8bit/");
//	volume.load("volumes/talairach_nii/");
//	volume.getVoxels().mirror(true, false, false);
//	volume.getVoxels().mirror(false, true, false);
//	volume.mirror(true, false, false);

	
//	ofVec3f volSize(50);
//	volume.allocate(volSize, OF_PIXELS_RGBA);
//	volume.setup(boxW, boxH);
	
	// camera
    cam.setDistance(1000);
    cam.enableMouseInput();

}
//--------------------------------------------------------------
void ofApp::initVolumeRendering()
{
	// Init Volume Rendering
    volumeRender.setup(&volume, ofVec3f(1,1,1), true, GL_RGBA);
	//	cout << volume.getWidth();
	
	volumeRender.setRenderSettings(FBOq, Zq, density, thresh);
	//	volumeRender.setVolumeTextureFilterMode(GL_LINEAR);
	//	volumeRender.setVolumeTextureFilterMode(GL_NEAREST);
	volumeRender.setClipDepth(clipPlaneDepth);
	//	volumeRender.setSlices(&uiClamp);
}

//--------------------------------------------------------------
void ofApp::initVolume_OLD()
{
	int w, h, d;
	unsigned char * volumeData;
	
	imageSequence.init("volumes/head/cthead-8bit",3,".tif", 1);
	//    imageSequence.init("volumes/Colin27T1_tight/IM-0001-0",3,".tif", 1);
    w = imageSequence.getWidth();
    h = imageSequence.getHeight();
    d = imageSequence.getSequenceLength();
	
    cout << "setting up volume data buffer at " << w << "x" << h << "x" << d <<"\n";
	
    volumeData = new unsigned char[w*h*d*4];
	
    for(int z=0; z<d; z++)
    {
        imageSequence.loadFrame(z);
        for(int x=0; x<w; x++)
        {
            for(int y=0; y<h; y++)
            {
                // convert from greyscale to RGBA, false color
                int i4 = ((x+w*y)+z*w*h)*4;
                int sample = imageSequence.getPixels()[x+y*w];
                ofColor c;
                c.setHsb(sample, 255-sample, sample);
				
                volumeData[i4] = c.r;
                volumeData[i4+1] = c.g;
                volumeData[i4+2] = c.b;
                volumeData[i4+3] = sample;
            }
        }
    }
	
//	volume.setFromVoxels(volumeData, w, h, d, 4);
//    volumeRender.setup(&volume, true, GL_RGBA);
	
    volumeRender.setup(w, h, d, ofVec3f(1,1,2),true);
    volumeRender.updateVolumeData(volumeData,w,h,d,0,0,0);
    volumeRender.setRenderSettings(1.0, 1.0, 0.75, 0.1);
	volumeRender.setClipDepth(clipPlaneDepth);
	linearFilter = false;
}

//--------------------------------------------------------------
void ofApp::initVolumeChar()
{
	int w, h, d;

	
	imageSequence.init("volumes/head/cthead-8bit",3,".tif", 1);
//	imageSequence.init("volumes/Colin27T1_tight/IM-0001-0",3,".tif", 1);
    w = imageSequence.getWidth();
    h = imageSequence.getHeight();
    d = imageSequence.getSequenceLength();
	
    cout << "setting up volume data buffer at " << w << "x" << h << "x" << d <<"\n";
	
    volumeData = new unsigned char[w*h*d*4];
	
    for(int z=0; z<d; z++)
    {
        imageSequence.loadFrame(z);
        for(int x=0; x<w; x++)
        {
            for(int y=0; y<h; y++)
            {
                // convert from greyscale to RGBA, false color
                int i4 = ((x+w*y)+z*w*h)*4;
                int sample = imageSequence.getPixels()[x+y*w];
                ofColor c;
                c.setHsb(sample, 255-sample, sample);
				
                volumeData[i4] = c.r;
                volumeData[i4+1] = c.g;
                volumeData[i4+2] = c.b;
                volumeData[i4+3] = sample;
            }
        }
    }
	
	/*
	// Init Volume Rendering
	// don't use pow2 now! the shader is not working!!
    volumeRender.setup(volumeData, ofVec3f(w,h,d), ofVec3f(1,1,1), true, GL_RGBA);
	volumeRender.setRenderSettings(FBOq, Zq, density, thresh);
	volumeRender.setClipDepth(clipPlaneDepth);
	*/
}

//--------------------------------------------------------------
void ofApp::update()
{
    ofSetWindowTitle(ofToString(ofGetFrameRate()));
//	gifEncoder.update();
}

//--------------------------------------------------------------
void ofApp::draw_OLD()
{
	cam.begin();
	volumeRender.drawVolume(0,0,0, ofGetHeight(), 0);
	cam.end();
	
//	if (blabels) drawLabels();
}

//--------------------------------------------------------------
void ofApp::draw()
{

	ofSetColor(255);
	cam.begin();
	volumeRender.update();
	cam.end();
	volumeRender.draw(0, ofGetHeight(), ofGetWidth(), -ofGetHeight());
	
	
	if (blabels) drawLabels();

//	draw_OLD();
}

void ofApp::onGifSaved(string &fileName) {
    cout << "gif saved as " << fileName << endl;
	gifEncoder.reset();
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
			volume.mirror(false, true, false);
//			volume.rotate90(1);
			volumeRender.setVolume(&volume, true, GL_RGBA);
//			volumeRender.setVolume(volumeData, volume.getSize(), false, GL_RGBA);
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











