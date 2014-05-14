#pragma once
#include "ofFileUtils.h"
#include "ofTexture.h"
#include "ofBaseTypes.h"
#include "ofConstants.h"

#include "ofxTexture3D.h"
#include "ofxVoxels.h"
#include "ofxImageSequence.h"

/*      ofxVolume - process volumetric data on the CPU
 
 Most of the code has its origins in ofImage and ofPixels.
 Adapted by Jordi Puig (http://wasawi.com).
 Use at your own risk and
 Please let me know if anything is not working.
 Use ofxVolumetrix for rendering.
 */


//----------------------------------------------------
// Image based stuff:
bool ofLoadVolume(ofxVoxels & voxls, string folder);
bool ofLoadVolume(ofxFloatVoxels & voxls, string folder);
bool ofLoadVolume(ofxShortVoxels & voxls, string folder);
bool ofLoadVolume(ofxTexture3d & tex, string folder);

void ofSaveVolume(ofxVoxels & voxls,
				  string fileName = "",
				  string folderName = "",
				  ofImageQualityType qualityLevel = OF_IMAGE_QUALITY_BEST);

void ofSaveVolume(ofxFloatVoxels & voxls,
				  string fileName = "",
				  string folderName = "",
				  ofImageQualityType qualityLevel = OF_IMAGE_QUALITY_BEST);

void ofSaveVolume(ofxShortVoxels & voxls,
				  string fileName = "",
				  string folderName = "",
				  ofImageQualityType qualityLevel = OF_IMAGE_QUALITY_BEST);

// ofBuffer is not implemented. probably it doesn't make much sense.
//bool ofLoadVolume(ofxVoxels & voxls, const ofBuffer & buffer);
//bool ofLoadVolume(ofxFloatVoxels & voxls, const ofBuffer & buffer);
//bool ofLoadVolume(ofxShortVoxels & voxls, const ofBuffer & buffer);
//bool ofLoadVolume(ofxTexture3d & tex, const ofBuffer & buffer);
//void ofSaveVolume(ofxVoxels & voxls, ofBuffer & buffer, ofImageFormat format = OF_IMAGE_FORMAT_PNG, ofImageQualityType qualityLevel = OF_IMAGE_QUALITY_BEST);
//void ofSaveVolume(ofxFloatVoxels & voxls, ofBuffer & buffer, ofImageFormat format = OF_IMAGE_FORMAT_PNG, ofImageQualityType qualityLevel = OF_IMAGE_QUALITY_BEST);
//void ofSaveVolume(ofxShortVoxels & voxls, ofBuffer & buffer, ofImageFormat format = OF_IMAGE_FORMAT_PNG, ofImageQualityType qualityLevel = OF_IMAGE_QUALITY_BEST);

//----------------------------------------------------
template<typename PixelType>
class ofxVolumeT_{// : public ofBaseImage_<PixelType>{

	public :

		ofxVolumeT_();
		virtual ~ofxVolumeT_();

		ofxVolumeT_(const ofxVoxels_<PixelType> & voxls);
		ofxVolumeT_(const ofDirectory & folder);
		ofxVolumeT_(const string & filename);

		// allocation / deallocation routines
		void 				allocate(int w, int h, int d, ofImageType type);
		void 				clear();

		// default copy overwriting (for = or std::vector)
		ofxVolumeT_(const ofxVolumeT_<PixelType>& mom);
		ofxVolumeT_<PixelType>& operator= (const ofxVolumeT_<PixelType>& mom);

		template<typename SrcType>
		ofxVolumeT_(const ofxVolumeT_<SrcType>& mom);

		template<typename SrcType>
		ofxVolumeT_<PixelType>& operator= (const ofxVolumeT_<SrcType>& mom);

		// copying:
		template<typename SrcType>
		void 				clone(const ofxVolumeT_<SrcType> &mom);

		// enable or disable using the texture of this image
		void 				setUseTexture(bool bUse);
		bool				isUsingTexture();

		// for getting a reference to the texture
		ofxTexture3d & getTextureReference();

		// quick texture binding shortcut
		void bind();
		void unbind();

		// file loading / saving
		bool 	loadVolume(string folder);
		bool 	loadVolume(const ofDirectory & folder);
		void 	saveVolume(string fileName = "",
						   string folderName = "",
						   ofImageQualityType qualityLevel = OF_IMAGE_QUALITY_BEST);
		void 	saveVolume(const ofDirectory & folder,
						   ofImageQualityType compressionLevel = OF_IMAGE_QUALITY_BEST);

	//		bool				loadVolume(const ofBuffer & buffer);
	//		void 				saveVolume(ofBuffer & buffer, ofImageQualityType compressionLevel = OF_IMAGE_QUALITY_BEST);

		// Sosolimited: texture compression and mipmaps
		void				setCompression(ofTexCompression compression);

		// getting the data
		PixelType * 				getPixels();			// up to you to get this right
		ofxVoxels_<PixelType> &	 	getPixelsRef();

		operator ofxVoxels_<PixelType>&();

		ofColor_<PixelType> getColor(int x, int y, int z) const;

		// alter the image
		void				setColor(int x, int y, int z, const ofColor_<PixelType>& color);
		void				setColor(int index, const ofColor_<PixelType>& color);
		void				setColor(const ofColor_<PixelType>& color);
		void 				setFromVoxels(const PixelType * voxels, int w, int h, int d, ofImageType type, bool bOrderIsRGB = true);
		void				setFromVoxels(const ofxVoxels_<PixelType> & voxels);
		void 				setImageType(ofImageType type);
		void 				resize(int newWidth, int newHeight);
		void 				grabScreen(int x, int y, int w, int h);		// grab voxels from opengl, using glreadvoxels
		// this does an inplace crop. 
		// NOTE: this reallocates memory.
		void				crop(int x, int y, int z, int w, int h, int d);
		// this does a crop from another image.
		// NOTE: this will reallocate memory if the image types are different, or if the w & h do not
		// equal this images w & h
		void				cropFrom(ofxVolumeT_<PixelType> & otherImage, int x, int y, int z, int w, int h, int d);
		// perform rotation of 90 degress clockwise rotation amont times. 
		void				rotate90(int rotation);
		void				mirror(bool _width, bool _height, bool _depth);
	
	
	
		// if you've altered the voxels (e.g., from getPixels())
		// call update() to see a change (move the voxels to the texture)
		void update();

		// the anchor is the point the image is drawn around.
		// this can be useful if you want to rotate an image around a particular point.
        void				setAnchorPercent(float xPct, float yPct);	// set the anchor as a percentage of the image width/height ( 0.0-1.0 range )
        void				setAnchorPoint(float x, float y);			// set the anchor point in voxels
        void				resetAnchor();								// resets the anchor to (0, 0)

/*		// draw:
		using ofBaseDraws::draw;
		void draw(float x, float y);
		void draw(float x, float y, float z);
		void draw(float x, float y, float w, float h);
		void draw(float x, float y, float z, float w, float h);
		
		void drawSubsection(float x, float y, float w, float h, float sx, float sy);
		void drawSubsection(float x, float y, float z, float w, float h, float sx, float sy);
		void drawSubsection(float x, float y, float w, float h, float sx, float sy, float sw, float sh);
		void drawSubsection(float x, float y, float z, float w, float h, float sx, float sy, float sw, float sh);
*/
		float 				getHeight();
		float 				getWidth();
		float 				getDepth();
	
		bool 				bAllocated() {return voxels.isAllocated();};	// legacy function
		bool 				isAllocated() {return voxels.isAllocated();};

		void				reloadTexture();

		int 				width, height, depth, bpp;		// w,h, bits per voxlsel
		int					type;					// OF_IMAGE_GRAYSCALE, OF_IMAGE_COLOR, OF_IMAGE_COLOR_ALPHA

		ofxVolumeT_<PixelType> & operator=(ofxVoxels_<PixelType> & voxels);
	protected:
	
		void				changeTypeOfPixels(ofxVoxels_<PixelType> &voxls, ofImageType type);
		void				resizePixels(ofxVoxels_<PixelType> &voxls, int newWidth, int newHeight);

		ofxVoxels_<PixelType>		voxels;
		bool						bUseTexture;
		ofxTexture3d				tex;
};

typedef ofxVolumeT_<unsigned char> ofxVolumeT;
typedef ofxVolumeT_<float> ofxFloatVolumeT;
typedef ofxVolumeT_<unsigned short> ofxShortVolumeT;


//----------------------------------------------------------
template<typename PixelType>
template<typename SrcType>
ofxVolumeT_<PixelType>& ofxVolumeT_<PixelType>::operator=(const ofxVolumeT_<SrcType>& mom) {
	clone(mom);
	update();
	return *this;
}

//----------------------------------------------------------
template<typename PixelType>
template<typename SrcType>
ofxVolumeT_<PixelType>::ofxVolumeT_(const ofxVolumeT_<SrcType>& mom) {
	clear();
	clone(mom);
	update();
}

//------------------------------------
template<typename PixelType>
template<typename SrcType>
void ofxVolumeT_<PixelType>::clone(const ofxVolumeT_<SrcType> &mom){
	ofxVolumeT_<SrcType> & nonConst = const_cast<ofxVolumeT_<SrcType> & >(mom);
	voxels = nonConst.getPixelsRef();

	tex.clear();
	bUseTexture = nonConst.isUsingTexture();
	if (bUseTexture == true && nonConst.getTextureReference().isAllocated()){
		tex.allocate(voxels.getWidth(), voxels.getHeight(), voxels.getDepth(), voxels.getGlFormat());
	}

	update();
}
