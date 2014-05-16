#include "ofxImageSequence.h"


/*	bool setup(string prefix, int digits, string extension, int start) {
 loadFromFolder = false;
 curFrame = start;
 startFrame = start;
 fPrefix = prefix;
 fExt = extension;
 numDigits = digits;
 totalFrames = 0;
 setUseTexture(false);
 
 initialized = loadImage(generateFullFilename());
 
 if(!initialized){
 ofLogWarning() << "ofxImageSequence: unable to load inital image in the sequence: " << generateFullFilename();
 return false;
 }
 
 ofFile testFile;
 while(testFile.open(generateFullFilename()))
 {
 totalFrames++;
 curFrame++;
 testFile.close();
 }
 testFile.close();
 curFrame = startFrame;
 return true;
 };
 */

//----------------------------------------------------------
// loader
//----------------------------------------------------------
bool ofxImageSequence::setupLoad(string _folder){

	totalFrames = 0;
	curFrame = 0;
	numDigits = 3;
	folderName = _folder;
	
	// read the directory for the images
	// we know that they are named in seq
	totalFrames = dir.listDir(_folder);
	dir.sort();
	
	if(!totalFrames){
		ofLog(OF_LOG_ERROR, "Could not find folder or folder is empty " + _folder);
		return false;
	}
	
	ofLogVerbose("ofxImageSequence") << "loading " << dir.getPath(curFrame);
	initialized = loadImage(dir.getPath(curFrame));
	
	return true;
}
//----------------------------------------------------------
bool ofxImageSequence::loadNextFrame() {

	bool worked;
	worked = loadImage(dir.getPath(curFrame));
	if(worked)
		curFrame++;

	return worked;
};
//----------------------------------------------------------
bool ofxImageSequence::loadPreviousFrame() {
	bool worked = loadImage(getImgName());
	if(worked)
		curFrame = curFrame > 0 ? curFrame - 1 : 0;
	return worked;
};
//----------------------------------------------------------
bool ofxImageSequence::loadFrame(int n) {
	curFrame = n;
	string file = dir.getPath(n);
	bool worked = loadImage(file);
	if(worked)
		curFrame++;
	return worked;
};
//----------------------------------------------------------
string ofxImageSequence::getImgName() {
	
	char fullPath[200];
	sprintf(fullPath,  "%s/%s%.4i.%s" , folderName.c_str(), prefix.c_str(), curFrame, extension.c_str());
	
	return fullPath;
};

//----------------------------------------------------------
// recorder
//----------------------------------------------------------

//----------------------------------------------------------
void ofxImageSequence::setupSave(string _file , string _folder){
	
	totalFrames = 0;
	curFrame = 0;
	numDigits = 3;
	
	setFolder(_folder);
	
	dir = _folder;
	if (dir.getOriginalDirectory() == "")
	{
		ofLogNotice("ofxVolume::saveVolume") << "folderName is empty";
		folderName = ofGetTimestampString();
		dir.createDirectory(folderName);
		ofLogNotice("ofxVolume::saveVolume") << "created dir = "<< folderName;
	}
	else if(!dir.isDirectory())
	{
		ofLogVerbose("ofxVolume::saveVolume") << "folder does not exist";
		dir.createDirectory(folderName);
		ofLogNotice("ofxVolume::saveVolume") << "created dir = "<< dir.path();
	}
	else if(dir.isDirectory())
	{
		ofLogVerbose("ofxVolume::saveVolume") << "folderName exists";
	}
	
	if (_file == "")
	{
		setFileName("img_");
		setExtension("jpg");
	}else{
		ofLogVerbose("ofxVolume::saveVolume")<<"file name = "<<_file;
		setFileName(_file);
	}
}

void ofxImageSequence::threadedFunction() {
	while(isThreadRunning()) {
		if(!q.empty()){
			QueuedImage i = q.front();
			ofSaveImage(i.image, i.fullPath);
			cout << i.fullPath<< " saved.\n";
			q.pop();
		}else{
			waitForThread();
		}
	}
}

//----------------------------------------------------------
void ofxImageSequence::saveImage(ofPixels imageToSave) {
		
	QueuedImage qImage;
	
	qImage.fullPath = getImgName();
	qImage.image = imageToSave;
	
	q.push(qImage);

	curFrame++;
}

//----------------------------------------------------------
void ofxImageSequence::setFileName(string name){
	vector <string> split = ofSplitString(name, ".");
	prefix		= split[0];
	extension	= split[1];
}




