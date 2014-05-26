#include "ofImage.h"
#include "ofAppRunner.h"
#include "ofTypes.h"
#include "ofURLFileLoader.h"
#include "ofGraphics.h"
#include "FreeImage.h"

#include "ofxVolume.h"

#if defined(TARGET_ANDROID) || defined(TARGET_OF_IOS)
#include <set>
// android destroys the opengl context on screen orientation change
// or when the application runs in the background so we need to reload
// all the textures when the context is created again.
// keeping a pointer to all the images we can tell them to reload from a static method
static set<ofImage *> & all_images(){
	static set<ofxVolume *> * images = new set<ofxVolume *>;
	return *images;
}
static set<ofxFloatVolume *> & all_float_images(){
	static set<ofxFloatVolume *> * images = new set<ofxFloatVolume *>;
	return *images;
}

static set<ofxShortVolume *> & all_short_images(){
	static set<ofxShortVolume *> * images = new set<ofxShortVolume *>;
	return *images;
}

static void registerImage(ofxVolume * img){
	all_images().insert(img);
}

static void registerImage(ofxFloatVolume * img){
	all_float_images().insert(img);
}

static void registerImage(ofxShortVolume * img){
	all_short_images().insert(img);
}

static void unregisterImage(ofxVolume * img){
	all_images().erase(img);
}

static void unregisterImage(ofxFloatVolume * img){
	all_float_images().erase(img);
}

static void unregisterImage(ofxShortVolume * img){
	all_short_images().erase(img);
}

void ofReloadAllImageTextures(){
	set<ofImage *>::iterator it;
	for(it=all_images().begin(); it!=all_images().end(); it++){
		(*it)->reloadTexture();
	}
	set<ofFloatImage *>::iterator f_it;
	for(f_it=all_float_images().begin(); f_it!=all_float_images().end(); f_it++){
		(*f_it)->reloadTexture();
	}
}
#endif


template<typename PixelType>
static bool loadVolume(ofxVoxels_<PixelType> & voxls, string folder, bool forcePow2=false){
	/*
	 TODO:
	 Loading from url not implemented.
	 */

	ofxImageSequence imageSequence;
	if (!imageSequence.setupLoad(folder)) return false;
	
	// get image attributes
	int w	= imageSequence.getWidth();
    int h	= imageSequence.getHeight();
    int d	= imageSequence.getSequenceLength();
	int channels		= imageSequence.getPixelsRef().getNumChannels();
	int bytesPerChannel = imageSequence.getPixelsRef().getBytesPerChannel();
	
	ofLogNotice("ofxVolume::loadVolume") << "Loading volume "
	<< w << "x" << h << "x" << d
	<< " channels= "<<channels
	<< " bytesPerChannel= "<<bytesPerChannel;
		
	if (forcePow2){
		// override dimensions to pow2
		// this will creat an added padding to the volume
		w = ofNextPow2(w);
		h = ofNextPow2(h);
		d = ofNextPow2(d);
		ofLogNotice("ofxVolume::loadVolume") << "forcing pow2 size "
		<< ofNextPow2(w) << "x" << ofNextPow2(h)<< "x" << ofNextPow2(d) << " channels= "<<channels;
	}

	voxls.allocate(w, h, d, channels);
	
	// copy images to volume
	for(int z=0; z<d; z++)
	{
		bool success = imageSequence.loadFrame(z);
//		ofLogVerbose("ofxVolume::loadVolume") << "loading img "<<z;

//		this would be nicer but imageSequence is not templated:
//		voxls.copyFrontSliceFrom(imageSequence.getPixels(), z);

		for(int y=0; y<h; y++)
		{
			for(int x=0; x<w; x++)
			{
				// get values from image
				int dstVox = (x + (y*w) + z*w*h);	// cursor position at Volume
				int srcPix = (x+ (y*w));			// cursor position at Image
				
				for (int l = 0; l < channels; l++){
					voxls[dstVox* channels + l] = imageSequence.getPixels()[srcPix* channels + l];
				}
			}
		}
	}
	
	ofLogNotice("ofxVolume::loadVolume") << "Volume loaded.";
	return true;
}
//----------------------------------------------------
bool ofLoadVolume(ofxVoxels & voxls, string folder) {
	return loadVolume(voxls,folder);
}
//----------------------------------------------------
bool ofLoadVolume(ofxFloatVoxels & voxls, string folder){
	return loadVolume(voxls,folder);
}
//----------------------------------------------------------------
bool ofLoadVolume(ofxTexture3d & tex, string folder){
	ofxVoxels voxels;
	bool loaded = ofLoadVolume(voxels,folder);
	if(loaded){
		tex.allocate(voxels.getWidth(), voxels.getHeight(), voxels.getDepth(), voxels.getGlFormat());
		tex.loadData(voxels);
	}
	return loaded;
}

/*
// TODO: nothing really smart here for now. buffer probably does not make much sense for volumes.
//----------------------------------------------------
template<typename PixelType>
static bool loadVolume(ofxVoxels_<PixelType> & voxls, const ofBuffer & buffer){
loadVolume(voxls, buffer.getText());
}
//----------------------------------------------------
bool ofLoadVolume(ofxShortVoxels & voxls, string path){
	return loadVolume(voxls,path);
}
//----------------------------------------------------
bool ofLoadVolume(ofxVoxels & voxls, const ofBuffer & buffer) {
	return loadVolume(voxls,buffer);
}
//----------------------------------------------------
bool ofLoadVolume(ofxFloatVoxels & voxls, const ofBuffer & buffer){
	return loadVolume(voxls,buffer);
}
//----------------------------------------------------
bool ofLoadVolume(ofxShortVoxels & voxls, const ofBuffer & buffer){
	return loadVolume(voxls,buffer);
}
//----------------------------------------------------------------
bool ofLoadVolume(ofxTexture3d & tex, const ofBuffer & buffer){
	ofxVoxels voxels;
	bool loaded = ofLoadVolume(voxels,buffer);
	if(loaded){
		tex.allocate(voxels.getWidth(), voxels.getHeight(), voxels.getDepth(), voxels.getGlFormat());
		tex.loadData(voxels);
	}
	return loaded;
}
*/

//----------------------------------------------------------------
template<typename PixelType>
static void saveVolume(ofxVoxels_<PixelType> & voxls,
					   string fileName = "",
					   string folderName = "",
					   ofImageQualityType qualityLevel = OF_IMAGE_QUALITY_BEST){

	ofxImageSequence imageSequence;
	imageSequence.setupSave(fileName, folderName);
	
	//ofLogVerbose("ofxVolume::saveVolume")<< "writing " << voxls.getDepth()<< " images";
	// copy images to volume
	for(int z=0; z<voxls.getDepth(); z++)
	{
		//ofLogVerbose("ofxVolume::saveVolume")<< "image " << z<< " in q";
		imageSequence.saveImage(voxls.getSlice(FRONT, z));
	}

	imageSequence.startThread();

	while(imageSequence.isThreadRunning()){
		//ofLogVerbose("ofxVolume::saveVolume")<< "waitihg for thread to finish.\n" ;
	}
	
	//imageSequence.waitForThread();
	ofLogNotice("ofxVolume::saveVolume")<< "Volume saved.";
}

//----------------------------------------------------------------
void ofSaveVolume(ofxVoxels & voxls, string fileName, string folderName, ofImageQualityType qualityLevel){
	saveVolume(voxls,fileName,folderName,qualityLevel);
}

//----------------------------------------------------------------
void ofSaveVolume(ofxFloatVoxels & voxls, string fileName, string folderName, ofImageQualityType qualityLevel) {
	saveVolume(voxls,fileName,folderName,qualityLevel);
}

//----------------------------------------------------------------
void ofSaveVolume(ofxShortVoxels & voxls, string fileName, string folderName, ofImageQualityType qualityLevel) {
	saveVolume(voxls,fileName,folderName,qualityLevel);
}
/*
// TODO: nothing really smart here for now. buffer probably does not make much sense for volumes.
//----------------------------------------------------------------
template<typename PixelType>
static void saveVolume(ofxVoxels_<PixelType> & voxls, ofBuffer & buffer, ofImageFormat format, ofImageQualityType qualityLevel) {
	saveVolume(voxls, buffer.getText(), qualityLevel);
}
//----------------------------------------------------------------
void ofSaveVolume(ofxVoxels & voxls, ofBuffer & buffer, ofImageFormat format, ofImageQualityType qualityLevel) {
	saveVolume(voxls,buffer,format,qualityLevel);
}
//----------------------------------------------------------------
void ofSaveVolume(ofxFloatVoxels & voxls, ofBuffer & buffer, ofImageFormat format, ofImageQualityType qualityLevel) {
	saveVolume(voxls,buffer,format,qualityLevel);
}
//----------------------------------------------------------------
void ofSaveVolume(ofxShortVoxels & voxls, ofBuffer & buffer, ofImageFormat format, ofImageQualityType qualityLevel) {
	saveVolume(voxls,buffer,format,qualityLevel);
}
*/

//-------------------------------------------------------------
//  implementation

//----------------------------------------------------------
template<typename PixelType>
ofxVolume_<PixelType>::ofxVolume_(){

	width						= 0;
	height						= 0;
	depth						= 0;
	bpp							= 0;
	type						= OF_IMAGE_UNDEFINED;
	bUseTexture					= true;		// the default is, yes, use a texture

}

//----------------------------------------------------------
template<typename PixelType>
ofxVolume_<PixelType>::ofxVolume_(const ofxVoxels_<PixelType> & voxls){
	width						= 0;
	height						= 0;
	depth						= 0;
	bpp							= 0;
	type						= OF_IMAGE_UNDEFINED;
	bUseTexture					= true;		// the default is, yes, use a texture

	setFromVoxels(voxls);
}

template<typename PixelType>
ofxVolume_<PixelType>::ofxVolume_(const ofDirectory & folder){
	width						= 0;
	height						= 0;
	depth						= 0;
	bpp							= 0;
	type						= OF_IMAGE_UNDEFINED;
	bUseTexture					= true;		// the default is, yes, use a texture

	loadVolume(folder);
}

template<typename PixelType>
ofxVolume_<PixelType>::ofxVolume_(const string & filename){
	width						= 0;
	height						= 0;
	depth						= 0;
	bpp							= 0;
	type						= OF_IMAGE_UNDEFINED;
	bUseTexture					= true;		// the default is, yes, use a texture

	loadVolume(filename);
}

//----------------------------------------------------------
template<typename PixelType>
ofxVolume_<PixelType>& ofxVolume_<PixelType>::operator=(const ofxVolume_<PixelType>& mom) {
	if(&mom==this) return *this;
	clone(mom);
	update();
	return *this;
}

//----------------------------------------------------------
template<typename PixelType>
ofxVolume_<PixelType>::ofxVolume_(const ofxVolume_<PixelType>& mom) {
#if defined(TARGET_ANDROID) || defined(TARGET_OF_IOS)
	registerImage(this);
#endif
	clear();
	clone(mom);
	update();
}

//----------------------------------------------------------
template<typename PixelType>
ofxVolume_<PixelType>::~ofxVolume_(){
	clear();
}


//----------------------------------------------------------
template<typename PixelType>
void ofxVolume_<PixelType>::reloadTexture(){
	if (voxels.isAllocated() && bUseTexture){
		tex.allocate(voxels.getWidth(), voxels.getHeight(), voxels.getDepth(), voxels.getGlFormat());
		tex.loadData(voxels);
	}
}

//----------------------------------------------------------
template<typename PixelType>
bool ofxVolume_<PixelType>::loadVolume(const ofDirectory & folder){
	return loadVolume(folder.getAbsolutePath());
}

//----------------------------------------------------------
template<typename PixelType>
bool ofxVolume_<PixelType>::loadVolume(string fileName){

#if defined(TARGET_ANDROID) || defined(TARGET_OF_IOS)
	registerImage(this);
#endif
	bool bLoadedOk = ofLoadVolume(voxels, fileName);
	if (!bLoadedOk) {
		ofLogError("ofxVolume") << "loadVolume(): couldn't load image from \"" << fileName << "\"";
		clear();
		return false;
	}
	if (bLoadedOk && voxels.isAllocated() && bUseTexture){
		tex.allocate(voxels.getWidth(), voxels.getHeight(), voxels.getDepth(), voxels.getGlFormat());
		if(ofGetGLProgrammableRenderer() && (voxels.getNumChannels()==1 || voxels.getNumChannels()==2)){
			tex.setRGToRGBASwizzles(true);
		}
	}
	update();
	return bLoadedOk;
}

/*
template<typename PixelType>
bool ofxVolume_<PixelType>::loadVolume(const ofBuffer & buffer){

#if defined(TARGET_ANDROID) || defined(TARGET_OF_IOS)
	registerImage(this);
#endif
	bool bLoadedOk = ofLoadVolume(voxels, buffer);
	if (!bLoadedOk) {
		ofLogError("ofImage") << "loadVolume(): couldn't load image from ofBuffer";
		clear();
		return false;
	}
	if (bLoadedOk && voxels.isAllocated() && bUseTexture){
		tex.allocate(voxels.getWidth(), voxels.getHeight(), voxels.getDepth(), voxels.getGlFormat());
		if(ofGetGLProgrammableRenderer() && (voxels.getNumChannels()==1 || voxels.getNumChannels()==2)){
			tex.setRGToRGBASwizzles(true);
		}
	}
	update();
	return bLoadedOk;
 
}
//----------------------------------------------------------
template<typename PixelType>
void ofxVolume_<PixelType>::saveVolume(ofBuffer & buffer, ofImageQualityType qualityLevel){
 ofSaveVolume(voxels, buffer, qualityLevel);
}
*/

//----------------------------------------------------------
template<typename PixelType>
void ofxVolume_<PixelType>::saveVolume(string fileName, string folderName, ofImageQualityType qualityLevel){
	ofSaveVolume(voxels,fileName,folderName,qualityLevel);
}
//----------------------------------------------------------
template<typename PixelType>
void ofxVolume_<PixelType>::saveVolume(const ofDirectory & folder, ofImageQualityType qualityLevel){
	ofSaveVolume(voxels,"",folder.getAbsolutePath(),qualityLevel);
}


// we could cap these values - but it might be more useful
// to be able to set anchor points outside the image
/*
//----------------------------------------------------------
template<typename PixelType>
void ofxVolume_<PixelType>::setAnchorPercent(float xPct, float yPct){
    if (bUseTexture)tex.setAnchorPercent(xPct, yPct);
}

//----------------------------------------------------------
template<typename PixelType>
void ofxVolume_<PixelType>::setAnchorPoint(float x, float y){
    if (bUseTexture)tex.setAnchorPoint(x, y, z);
}

//----------------------------------------------------------
template<typename PixelType>
void ofxVolume_<PixelType>::resetAnchor(){
   	if (bUseTexture)tex.resetAnchor();
}


//------------------------------------
template<typename PixelType>
void ofxVolume_<PixelType>::draw(float x, float y){
	draw(x,y,0,getWidth(),getHeight());
}

//------------------------------------
template<typename PixelType>
void ofxVolume_<PixelType>::draw(float x, float y, float z){
	draw(x,y,z,getWidth(),getHeight());
}

//------------------------------------
template<typename PixelType>
void ofxVolume_<PixelType>::draw(float x, float y, float w, float h){
	draw(x,y,0,w,h);
}

//------------------------------------
template<typename PixelType>
void ofxVolume_<PixelType>::draw(float x, float y, float z, float w, float h){
	drawSubsection(x,y,z,w,h,0,0,getWidth(),getHeight());
}

//------------------------------------
template<typename PixelType>
void ofxVolume_<PixelType>::drawSubsection(float x, float y, float w, float h, float sx, float sy){
	drawSubsection(x,y,0,w,h,sx,sy,w,h);
}

//------------------------------------
template<typename PixelType>
void ofxVolume_<PixelType>::drawSubsection(float x, float y, float w, float h, float sx, float sy, float _sw, float _sh){
	drawSubsection(x,y,0,w,h,sx,sy,_sw,_sh);
}

//------------------------------------
template<typename PixelType>
void ofxVolume_<PixelType>::drawSubsection(float x, float y, float z, float w, float h, float sx, float sy){
	drawSubsection(x,y,z,w,h,sx,sy,w,h);
}

//------------------------------------
template<typename PixelType>
void ofxVolume_<PixelType>::drawSubsection(float x, float y, float z, float w, float h, float sx, float sy, float sw, float sh){
	ofGetCurrentRenderer()->draw(*this,x,y,z,w,h,sx,sy,sw,sh);
}
*/
//------------------------------------
template<typename PixelType>
void ofxVolume_<PixelType>::allocate(ofxIntPoint size, ofImageType newType){
	allocate(size.x, size.y, size.z, newType);
}
//------------------------------------
template<typename PixelType>
void ofxVolume_<PixelType>::allocate(int w, int h, int d, int channels){
	ofImageType newType;
	switch(channels) {
		case 4:
			allocate(w, h, d, GL_RGBA);
			break;
		case 3:
			allocate(w, h, d, GL_RGB);
			break;
		case 2:
			allocate(w, h, d, GL_LUMINANCE_ALPHA);
			break;
		case 1:
			allocate(w, h, d, GL_LUMINANCE);
			break;
		default:
			ofLogError("ofxVoxels")
			<< "getGlFormat(): internal format not recognized, returning GL_RGBA";
			allocate(w, h, d, GL_RGBA);
			break;
	}
}
//------------------------------------
template<typename PixelType>
void ofxVolume_<PixelType>::allocate(int w, int h, int d, ofImageType newType){
	
	if (width == w && height == h && depth == d && newType == type){
		return;
	}
#if defined(TARGET_ANDROID) || defined(TARGET_OF_IOS)
	registerImage(this);
#endif
	voxels.allocate(w, h, d, newType);

	// take care of texture allocation --
	if (voxels.isAllocated() && bUseTexture){
		tex.allocate(voxels.getWidth(), voxels.getHeight(), voxels.getDepth(), voxels.getGlFormat());
		if(ofGetGLProgrammableRenderer() && (voxels.getNumChannels()==1 || voxels.getNumChannels()==2)){
			tex.setRGToRGBASwizzles(true);
		}
	}
	
	width	= voxels.getWidth();
	height	= voxels.getHeight();
	depth	= voxels.getDepth();
	bpp		= voxels.getBitsPerVoxel();
	type	= voxels.getImageType();
}


//------------------------------------
template<typename PixelType>
void ofxVolume_<PixelType>::clear(){
#if defined(TARGET_ANDROID) || defined(TARGET_OF_IOS)
	unregisterImage(this);
#endif
	voxels.clear();
	if(bUseTexture)	tex.clear();

	width					= 0;
	height					= 0;
	bpp						= 0;
	type 					= OF_IMAGE_UNDEFINED;
	bUseTexture 			= true;		// the default is, yes, use a texture
}

//------------------------------------
template<typename PixelType>
PixelType * ofxVolume_<PixelType>::getVoxels(){
	return voxels.getVoxels();
}

//----------------------------------------------------------
template<typename PixelType>
ofxVoxels_<PixelType> & ofxVolume_<PixelType>::getVoxelsRef(){
	return voxels;
}

//----------------------------------------------------------
template<typename PixelType>
ofxVolume_<PixelType>::operator ofxVoxels_<PixelType>&(){
	return voxels;
}

//------------------------------------
// for getting a reference to the texture
template<typename PixelType>
ofxTexture3d & ofxVolume_<PixelType>::getTextureReference(){
/*
	// it should be the responsibility of anything using getTextureReference()
	// to check that it's allocated
	if(!tex.bAllocated() ){
		ofLogWarning("ofImage") << "getTextureReference(): texture is not allocated";
	}
	*/
	return tex;
}

//----------------------------------------------------------
template<typename PixelType>
void ofxVolume_<PixelType>::bind(){
	if (bUseTexture && tex.bAllocated())
		tex.bind();
}

//----------------------------------------------------------
template<typename PixelType>
void ofxVolume_<PixelType>::unbind(){
	if (bUseTexture && tex.bAllocated())
		tex.unbind();
}

//------------------------------------
template<typename PixelType>
ofColor_<PixelType> ofxVolume_<PixelType>::getColor(int x, int y, int z) const {
	return voxels.getColor(x, y, z);
}

//------------------------------------
template<typename PixelType>
void ofxVolume_<PixelType>::setColor(int x, int y, int z, const ofColor_<PixelType>& color) {
	voxels.setColor(x, y, z, color);
}

//------------------------------------
template<typename PixelType>
void ofxVolume_<PixelType>::setColor(int index, const ofColor_<PixelType>& color) {
	voxels.setColor(index, color);
}

//------------------------------------
template<typename PixelType>
void ofxVolume_<PixelType>::setColor(const ofColor_<PixelType>& color) {
	voxels.setColor(color);
}

//------------------------------------
template<typename PixelType>
void  ofxVolume_<PixelType>::setFromVoxels(const PixelType * newPixels, int w, int h, int d, ofImageType newType, bool bOrderIsRGB){

	allocate(w, h, d, newType);
	voxels.setFromVoxels(newPixels,w,h,d,newType);

	if (!bOrderIsRGB && sizeof(PixelType) == 1){
		voxels.swapRgb();
	}

	update();
}

//------------------------------------
template<typename PixelType>
void ofxVolume_<PixelType>::setFromVoxels(const ofxVoxels_<PixelType> & voxels){
	setFromVoxels(voxels.getVoxels(),voxels.getWidth(),voxels.getHeight(),voxels.getDepth(),voxels.getImageType());
}

//------------------------------------
template<typename PixelType>
ofxVolume_<PixelType> & ofxVolume_<PixelType>::operator=(ofxVoxels_<PixelType> & voxels){
	setFromVoxels(voxels);
	return *this;
}

//------------------------------------
template<typename PixelType>
void ofxVolume_<PixelType>::update(){
	width = voxels.getWidth();
	height = voxels.getHeight();
	depth = voxels.getDepth();
	bpp = voxels.getBitsPerVoxel();
	type = voxels.getImageType();
	if (voxels.isAllocated() && bUseTexture){
		int glTypeInternal = voxels.getGlFormat();
		if(!tex.isAllocated() || tex.getWidth() != width || tex.getHeight() != height || tex.getTextureData().glTypeInternal != glTypeInternal){
			tex.allocate(voxels.getWidth(), voxels.getHeight(), voxels.getDepth(), glTypeInternal);
			if(ofGetGLProgrammableRenderer() && (voxels.getNumChannels()==1 || voxels.getNumChannels()==2)){
				tex.setRGToRGBASwizzles(true);
			}
		}
		tex.loadData(voxels);
	}
}

//------------------------------------
template<typename PixelType>
void ofxVolume_<PixelType>::setUseTexture(bool bUse){
	bUseTexture = bUse;
}

//------------------------------------
template<typename PixelType>
bool ofxVolume_<PixelType>::isUsingTexture(){
	return bUseTexture;
}

//------------------------------------
template<typename PixelType>
void ofxVolume_<PixelType>::grabScreen(int _x, int _y){

	// just for fun!
	ofImage_<PixelType> screen;
	screen.setImageType(voxels.getImageType());
	screen.grabScreen(_x, _y, width, height);
	voxels.copyFrontSliceFrom(screen.getPixelsRef(),(int)ofRandom(depth));
	update();
}

//------------------------------------
template<typename PixelType>
void ofxVolume_<PixelType>::setImageType(ofImageType newType){
	changeTypeOfPixels(voxels, newType);
	update();
}

//------------------------------------
template<typename PixelType>
void ofxVolume_<PixelType>::resize(int newWidth, int newHeight){
	if(newWidth == width && newHeight == height) return;

	resizePixels(voxels, newWidth, newHeight);

	if (bUseTexture){
		tex.clear();
		tex.allocate(voxels.getWidth(), voxels.getHeight(), voxels.getDepth(), voxels.getGlFormat());
	}

	update();
}


//------------------------------------
template<typename PixelType>
void ofxVolume_<PixelType>::crop(int x, int y, int z, int w, int h, int d){
	w = ofClamp(w,1,getWidth());
	h = ofClamp(h,1,getHeight());

	voxels.crop(x,y,z,w,h,d);
	update();
}

//------------------------------------
template<typename PixelType>
void ofxVolume_<PixelType>::cropFrom(ofxVolume_<PixelType> & otherImage, int x, int y, int z, int w, int h, int d){
	w = ofClamp(w,1,otherImage.getWidth());
	h = ofClamp(h,1,otherImage.getHeight());

	int myOldWidth = voxels.getWidth();
	int myOldHeight = voxels.getHeight();
	int myOldDepth = voxels.getDepth();
	
	otherImage.voxels.cropTo(voxels, x, y, z, w, h, d);

	if (myOldWidth  != voxels.getWidth() ||
		myOldHeight != voxels.getHeight() ||
		myOldDepth != voxels.getDepth()){

		if (bUseTexture){
			tex.clear();
			tex.allocate(voxels.getWidth(), voxels.getHeight(), voxels.getDepth(), voxels.getGlFormat());
			if(ofGetGLProgrammableRenderer() && (voxels.getNumChannels()==1 || voxels.getNumChannels()==2)){
				tex.setRGToRGBASwizzles(true);
			}
		}
	}

	update();
}

//------------------------------------
template<typename PixelType>
void ofxVolume_<PixelType>::rotate90(int nRotations){
	int myOldWidth = voxels.getWidth();
	int myOldHeight = voxels.getHeight();
	int myOldDepth = voxels.getDepth();
	voxels.rotate90(nRotations);
	if (myOldWidth  != voxels.getWidth() ||
		myOldHeight != voxels.getHeight() ||
		myOldDepth != voxels.getDepth()){
		if (bUseTexture){
			tex.clear();
			tex.allocate(voxels.getWidth(), voxels.getHeight(), voxels.getDepth(), voxels.getGlFormat());
			if(ofGetGLProgrammableRenderer() && (voxels.getNumChannels()==1 || voxels.getNumChannels()==2)){
				tex.setRGToRGBASwizzles(true);
			}
		}
	}
	update();
}

//------------------------------------
template<typename PixelType>
void ofxVolume_<PixelType>::mirror(bool _width, bool _height, bool _depth){
	voxels.mirror( _width, _height, _depth);
	update();
}


/*
//----------------------------------------------------
template<typename PixelType>
void ofxVolume_<PixelType>::resizePixels(ofxVoxels_<PixelType> &voxls, int newWidth, int newHeight){

	FIBITMAP * bmp					= getBmpFromPixels(voxls);
	FIBITMAP * convertedBmp			= NULL;

	convertedBmp = FreeImage_Rescale(bmp, newWidth, newHeight, FILTER_BICUBIC);
	putBmpIntoPixels(convertedBmp, voxls, false);

	if (bmp != NULL)				FreeImage_Unload(bmp);
	if (convertedBmp != NULL)		FreeImage_Unload(convertedBmp);
}

*/

//----------------------------------------------------
template<typename PixelType>
void ofxVolume_<PixelType>::changeTypeOfPixels(ofxVoxels_<PixelType> &voxls, ofImageType newType){
	int oldType = voxls.getImageType();
		
	if (oldType == newType) {
		return; // no need to reallocate
	}

	
	voxels.setImageType(newType);
	
	if(bUseTexture){
		update();
	}
}


//----------------------------------------------------------
template<typename PixelType>
int ofxVolume_<PixelType>::getWidth() const{
	return width;
}
//----------------------------------------------------------
template<typename PixelType>
int ofxVolume_<PixelType>::getHeight() const{
	return height;
}
//----------------------------------------------------------
template<typename PixelType>
int ofxVolume_<PixelType>::getDepth() const{
	return depth;
}
//----------------------------------------------------------
template<typename PixelType>
ofxIntPoint ofxVolume_<PixelType>::getSize() const{
	return ofxPoint(width, height, depth);
}

/*
//----------------------------------------------------------
// Sosolimited: texture compression
// call this function before you call loadVolume()
template<typename PixelType>
void ofxVolume_<PixelType>::setCompression(ofTexCompression compression)
{
	if(bUseTexture){
		tex.setCompression( compression );
	}
}
*/

template class ofxVolume_<unsigned char>;
template class ofxVolume_<float>;
template class ofxVolume_<unsigned short>;


