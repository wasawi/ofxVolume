#include "ofImage.h"
#include "ofAppRunner.h"
#include "ofTypes.h"
#include "ofURLFileLoader.h"
#include "ofGraphics.h"
#include "FreeImage.h"

#include "ofxVolumeT.h"
#include "ofxVoxels.h"

template<typename PixelType>
static bool loadImage(ofxVoxels_<PixelType> & voxls, string folder){
/*
	ofInitFreeImage();
	if(fileName.substr(0, 7) == "http://") {
		return ofLoadImage(voxls, ofLoadURL(fileName).data);
	}
	
	fileName = ofToDataPath(fileName);
	bool bLoaded = false;
	FIBITMAP * bmp = NULL;
	
	FREE_IMAGE_FORMAT fif = FIF_UNKNOWN;
	fif = FreeImage_GetFileType(fileName.c_str(), 0);
	if(fif == FIF_UNKNOWN) {
		// or guess via filename
		fif = FreeImage_GetFIFFromFilename(fileName.c_str());
	}
	if((fif != FIF_UNKNOWN) && FreeImage_FIFSupportsReading(fif)) {
		bmp = FreeImage_Load(fif, fileName.c_str(), 0);
		
		if (bmp != NULL){
			bLoaded = true;
		}
	}
	
	//-----------------------------
	
	if ( bLoaded ){
		putBmpIntoPixels(bmp,voxls);
	}
	
	if (bmp != NULL){
		FreeImage_Unload(bmp);
	}
	return bLoaded;
 */
	
	
	
	
	
}

template<typename PixelType>
static bool loadImage(ofxVoxels_<PixelType> & voxls, const ofBuffer & buffer){
/*
	ofInitFreeImage();
	bool bLoaded = false;
	FIBITMAP* bmp = NULL;
	FIMEMORY* hmem = NULL;
	
	hmem = FreeImage_OpenMemory((unsigned char*) buffer.getBinaryBuffer(), buffer.size());
	if (hmem == NULL){
		ofLogError("ofImage") << "loadImage(): couldn't load image from ofBuffer, opening FreeImage memory failed";
		return false;
	}

	//get the file type!
	FREE_IMAGE_FORMAT fif = FreeImage_GetFileTypeFromMemory(hmem);
	if( fif == -1 ){
		ofLogError("ofImage") << "loadImage(): couldn't load image from ofBuffer, unable to guess image format from memory";
		return false;
		FreeImage_CloseMemory(hmem);
	}


	//make the image!!
	bmp = FreeImage_LoadFromMemory(fif, hmem, 0);
	
	if( bmp != NULL ){
		bLoaded = true;
	}
	
	//-----------------------------
	
	if (bLoaded){
		putBmpIntoPixels(bmp,voxls);
	}

	if (bmp != NULL){
		FreeImage_Unload(bmp);
	}
	
	if( hmem != NULL ){
		FreeImage_CloseMemory(hmem);
	}

	return bLoaded;
 */
}


//----------------------------------------------------
bool ofLoadVolume(ofxVoxels & voxls, string fileName) {
	return loadImage(voxls,fileName);
}

//----------------------------------------------------
bool ofLoadVolume(ofxVoxels & voxls, const ofBuffer & buffer) {
	return loadImage(voxls,buffer);
}

//----------------------------------------------------
bool ofLoadVolume(ofxFloatVoxels & voxls, string path){
	return loadImage(voxls,path);
}

//----------------------------------------------------
bool ofLoadVolume(ofxFloatVoxels & voxls, const ofBuffer & buffer){
	return loadImage(voxls,buffer);
}

//----------------------------------------------------
bool ofLoadVolume(ofxShortVoxels & voxls, string path){
	return loadImage(voxls,path);
}

//----------------------------------------------------
bool ofLoadVolume(ofxShortVoxels & voxls, const ofBuffer & buffer){
	return loadImage(voxls,buffer);
}


//----------------------------------------------------------------
bool ofLoadVolume(ofxTexture3d & tex, string path){
	ofxVoxels voxels;
	bool loaded = ofLoadVolume(voxels,path);
	if(loaded){
		tex.allocate(voxels.getWidth(), voxels.getHeight(), voxels.getDepth(), voxels.getGlFormat());
		tex.loadData(voxels);
	}
	return loaded;
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

//----------------------------------------------------------------
template<typename PixelType>
static void saveImage(ofxVoxels_<PixelType> & voxls, string fileName, ofImageQualityType qualityLevel) {
	/*
	ofInitFreeImage();
	if (voxls.isAllocated() == false){
		ofLogError("ofImage") << "saveImage(): couldn't save \"" << fileName << "\", voxels are not allocated";
		return;
	}

	#ifdef TARGET_LITTLE_ENDIAN
	if(sizeof(PixelType) == 1) {
		voxls.swapRgb();
	}
	#endif

	FIBITMAP * bmp	= getBmpFromPixels(voxls);

	#ifdef TARGET_LITTLE_ENDIAN
	if(sizeof(PixelType) == 1) {
		voxls.swapRgb();
	}
	#endif
	
	ofFilePath::createEnclosingDirectory(fileName);
	fileName = ofToDataPath(fileName);
	FREE_IMAGE_FORMAT fif = FIF_UNKNOWN;
	fif = FreeImage_GetFileType(fileName.c_str(), 0);
	if(fif == FIF_UNKNOWN) {
		// or guess via filename
		fif = FreeImage_GetFIFFromFilename(fileName.c_str());
	}
	if((fif != FIF_UNKNOWN) && FreeImage_FIFSupportsReading(fif)) {
		if(fif == FIF_JPEG) {
			int quality = JPEG_QUALITYSUPERB;
			switch(qualityLevel) {
				case OF_IMAGE_QUALITY_WORST: quality = JPEG_QUALITYBAD; break;
				case OF_IMAGE_QUALITY_LOW: quality = JPEG_QUALITYAVERAGE; break;
				case OF_IMAGE_QUALITY_MEDIUM: quality = JPEG_QUALITYNORMAL; break;
				case OF_IMAGE_QUALITY_HIGH: quality = JPEG_QUALITYGOOD; break;
				case OF_IMAGE_QUALITY_BEST: quality = JPEG_QUALITYSUPERB; break;
			}
			FreeImage_Save(fif, bmp, fileName.c_str(), quality);
		} else {
			if(qualityLevel != OF_IMAGE_QUALITY_BEST) {
				ofLogWarning("ofImage") << "saveImage(): ofImageCompressionType only applies to JPEGs,"
					<< " ignoring value for \" "<< fileName << "\"";
			}
			
			if (fif == FIF_GIF) {
				FIBITMAP* convertedBmp;
				if(voxls.getImageType() == OF_IMAGE_COLOR_ALPHA) {
					// this just converts the image to grayscale so it can save something
					convertedBmp = FreeImage_ConvertTo8Bits(bmp);
				} else {
					// this will create a 256-color palette from the image
					convertedBmp = FreeImage_ColorQuantize(bmp, FIQ_NNQUANT);
				}
				FreeImage_Save(fif, convertedBmp, fileName.c_str());
				if (convertedBmp != NULL){
					FreeImage_Unload(convertedBmp);
				}
			} else {
				FreeImage_Save(fif, bmp, fileName.c_str());
			}
		}
	}

	if (bmp != NULL){
		FreeImage_Unload(bmp);
	}
	 
	 */
}

//----------------------------------------------------------------
void ofSaveVolume(ofxVoxels & voxls, string fileName, ofImageQualityType qualityLevel){
	saveImage(voxls,fileName,qualityLevel);
}

//----------------------------------------------------------------
void ofSaveVolume(ofxFloatVoxels & voxls, string fileName, ofImageQualityType qualityLevel) {
	saveImage(voxls,fileName,qualityLevel);
}

//----------------------------------------------------------------
void ofSaveVolume(ofxShortVoxels & voxls, string fileName, ofImageQualityType qualityLevel) {
	saveImage(voxls,fileName,qualityLevel);
}

//----------------------------------------------------------------
template<typename PixelType>
static void saveImage(ofxVoxels_<PixelType> & voxls, ofBuffer & buffer, ofImageFormat format, ofImageQualityType qualityLevel) {
	// thanks to alvaro casinelli for the implementation
/*
	ofInitFreeImage();

	if (voxls.isAllocated() == false){
		ofLogError("ofImage","saveImage(): couldn't save to ofBuffer, voxels are not allocated");
		return;
	}

	if(format==OF_IMAGE_FORMAT_JPEG && voxls.getNumChannels()==4){
		ofxVoxels voxls3 = voxls;
		voxls3.setNumChannels(3);
		saveImage(voxls3,buffer,format,qualityLevel);
		return;
	}

	#ifdef TARGET_LITTLE_ENDIAN
	if(sizeof(PixelType) == 1) {
		voxls.swapRgb();
	}
	#endif

	FIBITMAP * bmp	= getBmpFromPixels(voxls);

	#ifdef TARGET_LITTLE_ENDIAN
	if(sizeof(PixelType) == 1) {
		voxls.swapRgb();
	}
	#endif

	if (bmp)  // bitmap successfully created
	{
		   // (b) open a memory stream to compress the image onto mem_buffer:
		   //
		   FIMEMORY *hmem = FreeImage_OpenMemory();
		   // (c) encode and save the image to the memory (on dib FIBITMAP image):
		   //
		   if(FREE_IMAGE_FORMAT(format) == FIF_JPEG) {
				int quality = JPEG_QUALITYSUPERB;
				switch(qualityLevel) {
					case OF_IMAGE_QUALITY_WORST: quality = JPEG_QUALITYBAD; break;
					case OF_IMAGE_QUALITY_LOW: quality = JPEG_QUALITYAVERAGE; break;
					case OF_IMAGE_QUALITY_MEDIUM: quality = JPEG_QUALITYNORMAL; break;
					case OF_IMAGE_QUALITY_HIGH: quality = JPEG_QUALITYGOOD; break;
					case OF_IMAGE_QUALITY_BEST: quality = JPEG_QUALITYSUPERB; break;
				}
				FreeImage_SaveToMemory(FIF_JPEG, bmp, hmem, quality);
		   }else{
				FreeImage_SaveToMemory((FREE_IMAGE_FORMAT)format, bmp, hmem);
		   }
*/
		   /*

		  NOTE: at this point, hmem contains the entire data in memory stored in fif format. the
		  amount of space used by the memory is equal to file_size:
		  long file_size = FreeImage_TellMemory(hmem);
		  but can also be retrieved by FreeImage_AcquireMemory that retrieves both the
		  length of the buffer, and the buffer memory address.
		  */
	/*
			#ifdef TARGET_WIN32
		   	   DWORD size_in_bytes = 0;
			#else
		   	   uint32_t size_in_bytes = 0;
			#endif
		   // Save compressed data on mem_buffer
		   // note: FreeImage_AquireMemory allocates space for aux_mem_buffer):
		   //
		   unsigned char *mem_buffer = NULL;
		   if (!FreeImage_AcquireMemory(hmem, &mem_buffer, &size_in_bytes))
				   ofLogError("ofImage") << "saveImage(): couldn't save to ofBuffer, aquiring compressed image from memory failed";
*/
		   /*
			  Now, before closing the memory stream, copy the content of mem_buffer
			  to an auxiliary buffer
		    */
/*
		   buffer.set((char*)mem_buffer,size_in_bytes);

		   // Finally, close the FIBITMAP object, or we will get a memory leak:
		   FreeImage_Unload(bmp);

		   // Close the memory stream (otherwise we may get a memory leak).
		   FreeImage_CloseMemory(hmem);
	}
*/
}

//----------------------------------------------------------------
void ofSaveVolume(ofxVoxels & voxls, ofBuffer & buffer, ofImageFormat format, ofImageQualityType qualityLevel) {
	saveImage(voxls,buffer,format,qualityLevel);
}

void ofSaveVolume(ofxFloatVoxels & voxls, ofBuffer & buffer, ofImageFormat format, ofImageQualityType qualityLevel) {
	saveImage(voxls,buffer,format,qualityLevel);
}

void ofSaveVolume(ofxShortVoxels & voxls, ofBuffer & buffer, ofImageFormat format, ofImageQualityType qualityLevel) {
	saveImage(voxls,buffer,format,qualityLevel);
}


//-------------------------------------------------------------
//  implementation

//----------------------------------------------------------
template<typename PixelType>
ofxVolumeT_<PixelType>::ofxVolumeT_(){

	width						= 0;
	height						= 0;
	depth						= 0;
	bpp							= 0;
	type						= OF_IMAGE_UNDEFINED;
	bUseTexture					= true;		// the default is, yes, use a texture

}

//----------------------------------------------------------
template<typename PixelType>
ofxVolumeT_<PixelType>::ofxVolumeT_(const ofxVoxels_<PixelType> & voxls){
	width						= 0;
	height						= 0;
	depth						= 0;
	bpp							= 0;
	type						= OF_IMAGE_UNDEFINED;
	bUseTexture					= true;		// the default is, yes, use a texture

	setFromVoxels(voxls);
}

template<typename PixelType>
ofxVolumeT_<PixelType>::ofxVolumeT_(const ofFile & file){
	width						= 0;
	height						= 0;
	depth						= 0;
	bpp							= 0;
	type						= OF_IMAGE_UNDEFINED;
	bUseTexture					= true;		// the default is, yes, use a texture

	loadImage(file);
}

template<typename PixelType>
ofxVolumeT_<PixelType>::ofxVolumeT_(const string & filename){
	width						= 0;
	height						= 0;
	depth						= 0;
	bpp							= 0;
	type						= OF_IMAGE_UNDEFINED;
	bUseTexture					= true;		// the default is, yes, use a texture

	loadImage(filename);
}

//----------------------------------------------------------
template<typename PixelType>
ofxVolumeT_<PixelType>& ofxVolumeT_<PixelType>::operator=(const ofxVolumeT_<PixelType>& mom) {
	if(&mom==this) return *this;
	clone(mom);
	update();
	return *this;
}

//----------------------------------------------------------
template<typename PixelType>
ofxVolumeT_<PixelType>::ofxVolumeT_(const ofxVolumeT_<PixelType>& mom) {
#if defined(TARGET_ANDROID) || defined(TARGET_OF_IOS)
	registerImage(this);
#endif
	clear();
	clone(mom);
	update();
}

//----------------------------------------------------------
template<typename PixelType>
ofxVolumeT_<PixelType>::~ofxVolumeT_(){
	clear();
}


//----------------------------------------------------------
template<typename PixelType>
void ofxVolumeT_<PixelType>::reloadTexture(){
	if (voxels.isAllocated() && bUseTexture){
		tex.allocate(voxels.getWidth(), voxels.getHeight(), voxels.getDepth(), voxels.getGlFormat());
		tex.loadData(voxels);
	}
}

//----------------------------------------------------------
template<typename PixelType>
bool ofxVolumeT_<PixelType>::loadImage(const ofFile & file){
	return loadImage(file.getAbsolutePath());
}

//----------------------------------------------------------
template<typename PixelType>
bool ofxVolumeT_<PixelType>::loadImage(string fileName){
/*
#if defined(TARGET_ANDROID) || defined(TARGET_OF_IOS)
	registerImage(this);
#endif
	bool bLoadedOk = ofLoadVolume(voxels, fileName);
	if (!bLoadedOk) {
		ofLogError("ofImage") << "loadImage(): couldn't load image from \"" << fileName << "\"";
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
 */
}

template<typename PixelType>
bool ofxVolumeT_<PixelType>::loadImage(const ofBuffer & buffer){
/*
#if defined(TARGET_ANDROID) || defined(TARGET_OF_IOS)
	registerImage(this);
#endif
	bool bLoadedOk = ofLoadVolume(voxels, buffer);
	if (!bLoadedOk) {
		ofLogError("ofImage") << "loadImage(): couldn't load image from ofBuffer";
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
 */
}

//----------------------------------------------------------
template<typename PixelType>
void ofxVolumeT_<PixelType>::saveImage(string fileName, ofImageQualityType qualityLevel){
	ofSaveVolume(voxels, fileName, qualityLevel);
}

//----------------------------------------------------------
template<typename PixelType>
void ofxVolumeT_<PixelType>::saveImage(ofBuffer & buffer, ofImageQualityType qualityLevel){
	ofSaveVolume(voxels, buffer, qualityLevel);
}

//----------------------------------------------------------
template<typename PixelType>
void ofxVolumeT_<PixelType>::saveImage(const ofFile & file, ofImageQualityType compressionLevel){
	ofSaveVolume(voxels,file.getAbsolutePath(),compressionLevel);
}

// we could cap these values - but it might be more useful
// to be able to set anchor points outside the image
/*
//----------------------------------------------------------
template<typename PixelType>
void ofxVolumeT_<PixelType>::setAnchorPercent(float xPct, float yPct){
    if (bUseTexture)tex.setAnchorPercent(xPct, yPct);
}

//----------------------------------------------------------
template<typename PixelType>
void ofxVolumeT_<PixelType>::setAnchorPoint(float x, float y){
    if (bUseTexture)tex.setAnchorPoint(x, y, z);
}

//----------------------------------------------------------
template<typename PixelType>
void ofxVolumeT_<PixelType>::resetAnchor(){
   	if (bUseTexture)tex.resetAnchor();
}


//------------------------------------
template<typename PixelType>
void ofxVolumeT_<PixelType>::draw(float x, float y){
	draw(x,y,0,getWidth(),getHeight());
}

//------------------------------------
template<typename PixelType>
void ofxVolumeT_<PixelType>::draw(float x, float y, float z){
	draw(x,y,z,getWidth(),getHeight());
}

//------------------------------------
template<typename PixelType>
void ofxVolumeT_<PixelType>::draw(float x, float y, float w, float h){
	draw(x,y,0,w,h);
}

//------------------------------------
template<typename PixelType>
void ofxVolumeT_<PixelType>::draw(float x, float y, float z, float w, float h){
	drawSubsection(x,y,z,w,h,0,0,getWidth(),getHeight());
}

//------------------------------------
template<typename PixelType>
void ofxVolumeT_<PixelType>::drawSubsection(float x, float y, float w, float h, float sx, float sy){
	drawSubsection(x,y,0,w,h,sx,sy,w,h);
}

//------------------------------------
template<typename PixelType>
void ofxVolumeT_<PixelType>::drawSubsection(float x, float y, float w, float h, float sx, float sy, float _sw, float _sh){
	drawSubsection(x,y,0,w,h,sx,sy,_sw,_sh);
}

//------------------------------------
template<typename PixelType>
void ofxVolumeT_<PixelType>::drawSubsection(float x, float y, float z, float w, float h, float sx, float sy){
	drawSubsection(x,y,z,w,h,sx,sy,w,h);
}

//------------------------------------
template<typename PixelType>
void ofxVolumeT_<PixelType>::drawSubsection(float x, float y, float z, float w, float h, float sx, float sy, float sw, float sh){
	ofGetCurrentRenderer()->draw(*this,x,y,z,w,h,sx,sy,sw,sh);
}
*/
//------------------------------------
template<typename PixelType>
void ofxVolumeT_<PixelType>::allocate(int w, int h, int d, ofImageType newType){
	
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
void ofxVolumeT_<PixelType>::clear(){
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
PixelType * ofxVolumeT_<PixelType>::getPixels(){
	return voxels.getVoxels();
}

//----------------------------------------------------------
template<typename PixelType>
ofxVoxels_<PixelType> & ofxVolumeT_<PixelType>::getPixelsRef(){
	return voxels;
}

//----------------------------------------------------------
template<typename PixelType>
ofxVolumeT_<PixelType>::operator ofxVoxels_<PixelType>&(){
	return voxels;
}

//------------------------------------
// for getting a reference to the texture
template<typename PixelType>
ofxTexture3d & ofxVolumeT_<PixelType>::getTextureReference(){
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
void ofxVolumeT_<PixelType>::bind(){
	if (bUseTexture && tex.bAllocated())
		tex.bind();
}

//----------------------------------------------------------
template<typename PixelType>
void ofxVolumeT_<PixelType>::unbind(){
	if (bUseTexture && tex.bAllocated())
		tex.unbind();
}

//------------------------------------
template<typename PixelType>
ofColor_<PixelType> ofxVolumeT_<PixelType>::getColor(int x, int y, int z) const {
	return voxels.getColor(x, y, z);
}

//------------------------------------
template<typename PixelType>
void ofxVolumeT_<PixelType>::setColor(int x, int y, int z, const ofColor_<PixelType>& color) {
	voxels.setColor(x, y, z, color);
}

//------------------------------------
template<typename PixelType>
void ofxVolumeT_<PixelType>::setColor(int index, const ofColor_<PixelType>& color) {
	voxels.setColor(index, color);
}

//------------------------------------
template<typename PixelType>
void ofxVolumeT_<PixelType>::setColor(const ofColor_<PixelType>& color) {
	voxels.setColor(color);
}

//------------------------------------
template<typename PixelType>
void  ofxVolumeT_<PixelType>::setFromVoxels(const PixelType * newPixels, int w, int h, int d, ofImageType newType, bool bOrderIsRGB){

	allocate(w, h, d, newType);
	voxels.setFromVoxels(newPixels,w,h,d,newType);

	if (!bOrderIsRGB && sizeof(PixelType) == 1){
		voxels.swapRgb();
	}

	update();
}

//------------------------------------
template<typename PixelType>
void ofxVolumeT_<PixelType>::setFromVoxels(const ofxVoxels_<PixelType> & voxels){
	setFromVoxels(voxels.getVoxels(),voxels.getWidth(),voxels.getHeight(),voxels.getDepth(),voxels.getImageType());
}

//------------------------------------
template<typename PixelType>
ofxVolumeT_<PixelType> & ofxVolumeT_<PixelType>::operator=(ofxVoxels_<PixelType> & voxels){
	setFromVoxels(voxels);
	return *this;
}

//------------------------------------
template<typename PixelType>
void ofxVolumeT_<PixelType>::update(){
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
void ofxVolumeT_<PixelType>::setUseTexture(bool bUse){
	bUseTexture = bUse;
}

//------------------------------------
template<typename PixelType>
bool ofxVolumeT_<PixelType>::isUsingTexture(){
	return bUseTexture;
}

//------------------------------------
template<typename PixelType>
void ofxVolumeT_<PixelType>::grabScreen(int _x, int _y, int _w, int _h){
/*
	allocate(_w, _h, OF_IMAGE_COLOR);

    int sh = ofGetViewportHeight();     // if we are in a FBO or other viewport, this fails: ofGetHeight();


	#ifndef TARGET_OPENGLES
    
    if(ofIsVFlipped()){
		_y = sh - _y;
		_y -= _h; // top, bottom issues
    }
    
    glPushClientAttrib( GL_CLIENT_PIXEL_STORE_BIT );											// be nice to anyone else who might use voxlselStore
    glPixelStorei(GL_PACK_ALIGNMENT, 1);
    glReadPixels(_x, _y, _w, _h, ofGetGlFormat(voxels), GL_UNSIGNED_BYTE, voxels.getPixels()); // read the memory....
    glPopClientAttrib();
    
	int sizeOfOneLineOfPixels = voxels.getWidth() * voxels.getBytesPerPixel();
	PixelType * tempLineOfPix = new PixelType[sizeOfOneLineOfPixels];
	PixelType * linea;
	PixelType * lineb;
	for (int i = 0; i < voxels.getHeight()/2; i++){
		linea = voxels.getPixels() + i * sizeOfOneLineOfPixels;
		lineb = voxels.getPixels() + (voxels.getHeight()-i-1) * sizeOfOneLineOfPixels;
		memcpy(tempLineOfPix, linea, sizeOfOneLineOfPixels);
		memcpy(linea, lineb, sizeOfOneLineOfPixels);
		memcpy(lineb, tempLineOfPix, sizeOfOneLineOfPixels);
	}
	delete [] tempLineOfPix;
	
    #else
    
    int sw = ofGetViewportWidth();
    int numPixels   = width*height;
    if( numPixels == 0 ){
        ofLogError("ofImage") << "grabScreen(): unable to grab screen, image width and/or height are 0: " << width << "x" << height;
        return;
    }
    
    int numRGBA         = numPixels*4;
    GLubyte *bufferRGBA = (GLubyte *) malloc(numRGBA);

    if(ofGetOrientation() == OF_ORIENTATION_DEFAULT || ofDoesHWOrientation()) {

        if(ofIsVFlipped()){
			_y = sh - _y;   // screen is flipped vertically.
			_y -= _h;
        }
        
        glPixelStorei(GL_PACK_ALIGNMENT, 1);
        glReadPixels(_x, _y, _w, _h, GL_RGBA, GL_UNSIGNED_BYTE, bufferRGBA);
        
        for(int y = 0; y < _h; y++){  
            for(int x = 0; x < _w; x++){
                
                int i = y * _w * 3 + x * 3;
                int j = (_h-1-y) * _w * 4 + x * 4;  // rotate 90.
                
                voxels.getPixels()[i]   = bufferRGBA[j];
                voxels.getPixels()[i+1] = bufferRGBA[j+1];
                voxels.getPixels()[i+2] = bufferRGBA[j+2];
            }
        }
    }
    else if(ofGetOrientation() == OF_ORIENTATION_180) {

        if(ofIsVFlipped()){
			_x = sw - _x;   // screen is flipped horizontally.
			_x -= _w;
        }
        
        glPixelStorei(GL_PACK_ALIGNMENT, 1);
        glReadPixels(_x, _y, _w, _h, GL_RGBA, GL_UNSIGNED_BYTE, bufferRGBA);
        
        for(int y = 0; y < _h; y++){  
            for(int x = 0; x < _w; x++){
                
                int i = y * _w * 3 + x * 3;
                int j = y * _w * 4 + (_w-1-x) * 4;  // rotate 90.
                
                voxels.getPixels()[i]   = bufferRGBA[j];
                voxels.getPixels()[i+1] = bufferRGBA[j+1];
                voxels.getPixels()[i+2] = bufferRGBA[j+2];
            }
        }
    }
    else if(ofGetOrientation() == OF_ORIENTATION_90_RIGHT) {
        
        swap(_w,_h);
        swap(_x,_y);


        if(!ofIsVFlipped()){
			_x = sw - _x;   // screen is flipped horizontally.
			_x -= _w;

			_y = sh - _y;   // screen is flipped vertically.
			_y -= _h;
        }
        
        
        glPixelStorei(GL_PACK_ALIGNMENT, 1);
        glReadPixels(_x, _y, _w, _h, GL_RGBA, GL_UNSIGNED_BYTE, bufferRGBA);
        
        for(int y = 0; y < _h; y++){  
            for(int x = 0; x < _w; x++){
                
                int i = x * _h * 3 + y * 3;
                int j = y * _w * 4 + x * 4;
                
                voxels.getPixels()[i]   = bufferRGBA[j];
                voxels.getPixels()[i+1] = bufferRGBA[j+1];
                voxels.getPixels()[i+2] = bufferRGBA[j+2];
            }
        }
    }
    else if(ofGetOrientation() == OF_ORIENTATION_90_LEFT) {
        
        swap(_w, _h);
        
        swap(_x, _y);

        if(ofIsVFlipped()){
			_x = sw - _x;   // screen is flipped horizontally.
			_x -= _w;

			_y = sh - _y;   // screen is flipped vertically.
			_y -= _h;
        }
        
        glPixelStorei(GL_PACK_ALIGNMENT, 1);
        glReadPixels(_x, _y, _w, _h, GL_RGBA, GL_UNSIGNED_BYTE, bufferRGBA);
        
        for(int y = 0; y < _h; y++){  
            for(int x = 0; x < _w; x++){
                
                int i = x * _h * 3 + y * 3;
                int j = (_h-1-y) * _w * 4 + (_w-1-x) * 4;
                
                voxels.getPixels()[i]   = bufferRGBA[j];
                voxels.getPixels()[i+1] = bufferRGBA[j+1];
                voxels.getPixels()[i+2] = bufferRGBA[j+2];
            }
        }
    }
    
    free(bufferRGBA);

    #endif

	update();
 */
}

//------------------------------------
template<typename PixelType>
void ofxVolumeT_<PixelType>::setImageType(ofImageType newType){
	changeTypeOfPixels(voxels, newType);
	update();
}

//------------------------------------
template<typename PixelType>
void ofxVolumeT_<PixelType>::resize(int newWidth, int newHeight){
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
void ofxVolumeT_<PixelType>::crop(int x, int y, int z, int w, int h, int d){
	w = ofClamp(w,1,getWidth());
	h = ofClamp(h,1,getHeight());

	voxels.crop(x,y,z,w,h,d);
	update();
}

//------------------------------------
template<typename PixelType>
void ofxVolumeT_<PixelType>::cropFrom(ofxVolumeT_<PixelType> & otherImage, int x, int y, int z, int w, int h, int d){
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
void ofxVolumeT_<PixelType>::rotate90(int nRotations){
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
void ofxVolumeT_<PixelType>::mirror(bool _width, bool _height, bool _depth){
	voxels.mirror( _width, _height, _depth);
	update();
}






//----------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------
// freeImage based code & utilities:

/*
//----------------------------------------------------
template<typename PixelType>
void ofxVolumeT_<PixelType>::resizePixels(ofxVoxels_<PixelType> &voxls, int newWidth, int newHeight){

	FIBITMAP * bmp					= getBmpFromPixels(voxls);
	FIBITMAP * convertedBmp			= NULL;

	convertedBmp = FreeImage_Rescale(bmp, newWidth, newHeight, FILTER_BICUBIC);
	putBmpIntoPixels(convertedBmp, voxls, false);

	if (bmp != NULL)				FreeImage_Unload(bmp);
	if (convertedBmp != NULL)		FreeImage_Unload(convertedBmp);
}

//----------------------------------------------------
template<typename PixelType>
void ofxVolumeT_<PixelType>::changeTypeOfPixels(ofxVoxels_<PixelType> &voxls, ofImageType newType){
	int oldType = voxls.getImageType();
		
	if (oldType == newType) {
		return; // no need to reallocate
	}

	FIBITMAP * bmp = getBmpFromPixels(voxls);
	FIBITMAP * convertedBmp = NULL;

	switch (newType){
		case OF_IMAGE_GRAYSCALE:
			convertedBmp = FreeImage_ConvertToGreyscale(bmp);
			break;
		case OF_IMAGE_COLOR:
			convertedBmp = FreeImage_ConvertTo24Bits(bmp);
			break;
		case OF_IMAGE_COLOR_ALPHA:
			convertedBmp = FreeImage_ConvertTo32Bits(bmp);
			break;
		default:
			ofLogError("ofImage") << "changeTypeOfPixels(): unknown image type: " << newType;
			break;
	}
	
	putBmpIntoPixels(convertedBmp, voxls, false);

	if (bmp != NULL) {
		FreeImage_Unload(bmp);
	}
	if (convertedBmp != NULL) {
		FreeImage_Unload(convertedBmp);
	}

	if(bUseTexture){
		// always reallocate the texture. if ofxTexture3d doesn't need reallocation,
		// it doesn't have to. but it needs to change the internal format.
		tex.allocate(voxels.getWidth(), voxels.getHeight(), voxels.getDepth(), voxels.getGlFormat());
		if(ofGetGLProgrammableRenderer() && (voxels.getNumChannels()==1 || voxels.getNumChannels()==2)){
			tex.setRGToRGBASwizzles(true);
		}
	}
}
*/

//----------------------------------------------------------
template<typename PixelType>
float ofxVolumeT_<PixelType>::getWidth() {
	return width;
}
//----------------------------------------------------------
template<typename PixelType>
float ofxVolumeT_<PixelType>::getHeight() {
	return height;
}
//----------------------------------------------------------
template<typename PixelType>
float ofxVolumeT_<PixelType>::getDepth() {
	return depth;
}

/*
//----------------------------------------------------------
// Sosolimited: texture compression
// call this function before you call loadImage()
template<typename PixelType>
void ofxVolumeT_<PixelType>::setCompression(ofTexCompression compression)
{
	if(bUseTexture){
		tex.setCompression( compression );
	}
}
*/

template class ofxVolumeT_<unsigned char>;
template class ofxVolumeT_<float>;
template class ofxVolumeT_<unsigned short>;


