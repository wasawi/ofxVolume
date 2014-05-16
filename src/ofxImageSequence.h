#pragma once

#include "ofMain.h"

/*
 Remixed code from ofxImageSequenceRecorder
 and ofxImageSequence by Timothy Scaffidi (http://timothyscaffidi.com)
 and ofxImageSequence by James George (FlightPhase)
 */

class ofxImageSequence: public ofImage, public ofThread {
	
public:
	ofxImageSequence() {};
	
	// methods to load
	bool setupLoad(string folderName);
	bool loadNextFrame();

	bool loadPreviousFrame();
	bool loadFrame(int n);
	string generateFullFilename();
	string getImgName();
	
	// methods to save
	void setupSave(string fileName="", string folderName="");
    void saveImage(ofPixels imageToSave);
	void setFileName(string name);
	void threadedFunction();
	
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
	void setQuality(ofImageQualityType q){	qualityLevel = q;}
	void setCurrentFrame(int n) {			curFrame = n; }
	
	//----------------------------------------------------------
	//getters
	//----------------------------------------------------------
	int	getCurrentFrameNumber() {			return curFrame; }
	int getSequenceLength() {				return totalFrames; }
	
private:

	ofDirectory			dir;
	bool				initialized;
	int					curFrame;
	int					numDigits;
	int					totalFrames;
	
    queue<QueuedImage>	q;
    string				folderName;
    string				prefix;
    string				extension;
	ofImageFormat		format;
	ofImageQualityType	qualityLevel;
};





