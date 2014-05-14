#pragma once

#include "ofMain.h"

/*
 Remixed code from ofxImageSequenceRecorder
 and ofxImageSequence by Timothy Scaffidi (http://timothyscaffidi.com)
 and ofxImageSequence by James George (FlightPhase)
 */

class ofxImageSequence: public ofImage, public ofThread {
	
public:
	ofxImageSequence() {
		recCounter	= 0;
		totalFrames = 0;
		prefix = "";
	};
	
	//----------------------------------------------------------
	// loader
	//----------------------------------------------------------
	bool setup(string prefix, int digits, string extension, int start) {
		loadFromFolder = false;
		curFrame = start;
		startFrame = start;
		fPrefix = prefix;
		fExt = extension;
		numDigits = digits;
		totalFrames = 0;
		
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
	//----------------------------------------------------------
	bool setup(string _folder){
		loadFromFolder = true;
		totalFrames = 0;
		curFrame = 0;
		
		// read the directory for the images
		// we know that they are named in seq
		totalFrames = dir.listDir(_folder);
		dir.sort();
		
		if(!totalFrames){
			ofLog(OF_LOG_ERROR, "Could not find folder " + _folder);
			return false;
		}
		initialized = loadImage(dir.getPath(curFrame));
		
		ofFile testFile;
		while(testFile.open(dir.getPath(curFrame)))
		{
			totalFrames++;
			curFrame++;
			testFile.close();
		}
		testFile.close();
		return true;
	}
	//----------------------------------------------------------
	bool loadNextFrame() {
		bool worked;
		
		if(loadFromFolder){
			worked = loadImage(dir.getPath(curFrame));
		}else{
			worked = loadImage(generateFullFilename());
		}
		if(worked)
			curFrame++;
		return worked;
	};
	//----------------------------------------------------------
	bool loadPreviousFrame() {
		bool worked = loadImage(generateFullFilename());
		if(worked)
			curFrame = curFrame > 0 ? curFrame - 1 : 0;
		return worked;
	};
	//----------------------------------------------------------
	bool loadFrame(int n) {
		curFrame = startFrame + n;
		bool worked = loadImage(generateFullFilename());
		if(worked)
			curFrame++;
		return worked;
	};
	//----------------------------------------------------------
	string generateFullFilename() {
		std::ostringstream ss;
		ss << std::setw(numDigits) << std::setfill('0') << curFrame;
		std::string result = ss.str();
		if (result.length() > numDigits)
		{
			result.erase(0, result.length() - numDigits);
		}
		return fPrefix + ss.str() + fExt;
	};
	
	//----------------------------------------------------------
	// recorder
	//----------------------------------------------------------
	void threadedFunction() {
        while(isThreadRunning()) {
            if(!q.empty()){
                QueuedImage i = q.front();
                ofSaveImage(i.image, i.fullPath);
				q.pop();
            }
        }
    }
	//----------------------------------------------------------
    void saveImage(ofPixels imageToSave) {
		
        char fullPath[100];
        sprintf(fullPath,  "%s/%s%.4i.%s" , folderName.c_str(), prefix.c_str(), recCounter, extension.c_str());
        recCounter++;
        
        QueuedImage qImage;
        
        qImage.fullPath = fullPath;
        qImage.image = imageToSave;
        
        q.push(qImage);
        
    }

	//----------------------------------------------------------
	void setFileName(string name){
		vector <string> split = ofSplitString(name, ".");
		prefix		= split[0];
		extension	= split[1];
	}
	//----------------------------------------------------------
	typedef struct {
		string fullPath;
		ofPixels image;
	} QueuedImage;

	
	//----------------------------------------------------------
	bool isInitialized() {					return initialized; }
	void saveImage(ofImage &img){			saveImage(img.getPixelsRef());}
	
	//----------------------------------------------------------
	// setters
	//----------------------------------------------------------
	void setFolder(string fold){			folderName = fold;}
	void setPrefix(string pre){				prefix = pre;}
	void setExtension(string ext){			extension = ext;}
	void setFormat(ofImageFormat fmt){		format = fmt;}
	void setFormat(ofImageQualityType q){	qualityLevel = q;}
	void setCurrentFrameNumber(int i) {		curFrame = startFrame + i; }
	//----------------------------------------------------------
	//getters
	//----------------------------------------------------------
	int	getCurrentFrameNumber() { return curFrame - startFrame; }
	int getSequenceLength() { return totalFrames; }
	
	
private:
	bool initialized;
	int curFrame, startFrame;
	string fPrefix, fExt;
	int numDigits;
	int totalFrames;
	ofDirectory dir;
	bool loadFromFolder;
	
	// recorder
	int recCounter;
    queue<QueuedImage>	q;
    string				folderName;
    string				prefix;
    string				extension;
	ofImageFormat		format;
	ofImageQualityType	qualityLevel;
};
