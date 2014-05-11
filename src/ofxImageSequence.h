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
		
	};
	//----------------------------------------------------------
	void setup(string prefix, int digits, string extension, int start) {
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
			return;
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
	};
	//----------------------------------------------------------
	void setup(string _folder){
		loadFromFolder = true;
		totalFrames = 0;
		curFrame = 0;
		
		// read the directory for the images
		// we know that they are named in seq
		totalFrames = dir.listDir(_folder);
		dir.sort();
		
		if(!totalFrames)ofLog(OF_LOG_ERROR, "Could not find folder " + _folder);
		initialized = loadImage(dir.getPath(curFrame));
		
		
		ofFile testFile;
		while(testFile.open(dir.getPath(curFrame)))
		{
			totalFrames++;
			curFrame++;
			testFile.close();
		}
		testFile.close();
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
                ofSaveImage(i.image, i.fileName);
                q.pop();
            }
        }
    }
	//----------------------------------------------------------
    void addFrame(ofPixels imageToSave) {
		
        char fileName[100];
        sprintf(fileName,  "%s%.4i.%s" , prefix.c_str(), recCounter, format.c_str());
        recCounter++;
        
        QueuedImage qImage;
        
        qImage.fileName = fileName;
        qImage.image = imageToSave;
        
        q.push(qImage);
        
    }
	//----------------------------------------------------------
	void addFrame(ofImage &img){
        addFrame(img.getPixelsRef());
    }
	
	typedef struct {
		string fileName;
		ofPixels image;
	} QueuedImage;
	
	
	//----------------------------------------------------------
	// setters
	//----------------------------------------------------------
	void setPrefix(string pre){ prefix = pre;}
	void setFormat(string fmt){ format = fmt;}
	void setCurrentFrameNumber(int i) { curFrame = startFrame + i; }
	//----------------------------------------------------------
	//getters
	//----------------------------------------------------------
	int	getCurrentFrameNumber() { return curFrame - startFrame; }
	int getSequenceLength() { return totalFrames; }
	bool isInitialized() { return initialized; }
	
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
    queue<QueuedImage> q;
    string prefix;
    string format;
	
};
