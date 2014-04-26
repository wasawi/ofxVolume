#pragma once

#include "ofConstants.h"
#include "ofUtils.h"
#include "ofColor.h"
#include "ofMath.h"
#include <limits>

//---------------------------------------
enum ofInterpolationMethod {
	OF_INTERPOLATE_NEAREST_NEIGHBOR =1,
	OF_INTERPOLATE_BILINEAR			=2,
	OF_INTERPOLATE_BICUBIC			=3
};

template <typename PixelType>
class ofxVoxels_ {
public:

	ofxVoxels_();
	~ofxVoxels_();
	ofxVoxels_(const ofxVoxels_<PixelType> & mom);
	ofxVoxels_<PixelType>& operator=(const ofxVoxels_<PixelType> & mom);

	template<typename SrcType>
	ofxVoxels_(const ofxVoxels_<SrcType> & mom);

	template<typename SrcType>
	ofxVoxels_<PixelType>& operator=(const ofxVoxels_<SrcType> & mom);

	void allocate(int w, int h, int channels);
	void allocate(int w, int h, ofPixelFormat type);
	void allocate(int w, int h, ofImageType type);

	void set(PixelType val);
	void set(int channel,PixelType val);
	void setFromPixels(const PixelType * newPixels, int w, int h, int channels);
	void setFromPixels(const PixelType * newPixels, int w, int h, ofImageType type);
	void setFromExternalPixels(PixelType * newPixels, int w, int h, int channels);
	void setFromAlignedPixels(const PixelType * newPixels, int width, int height, int channels, int stride);
	void swap(ofxVoxels_<PixelType> & pix);

	//From ofxVoxelsUtils
	// crop to a new width and height, this reallocates memory.
	void crop(int x, int y, int width, int height);
	// not in place
	
	void cropTo(ofxVoxels_<PixelType> &toPix, int x, int y, int _width, int _height);

	// crop to a new width and height, this reallocates memory.
	void rotate90(int nClockwiseRotations);
	void rotate90To(ofxVoxels_<PixelType> & dst, int nClockwiseRotations);
	void mirrorTo(ofxVoxels_<PixelType> & dst, bool vertically, bool horizontal);
	void mirror(bool vertically, bool horizontal);
	bool resize(int dstWidth, int dstHeight, ofInterpolationMethod interpMethod=OF_INTERPOLATE_NEAREST_NEIGHBOR);	
	bool resizeTo(ofxVoxels_<PixelType> & dst, ofInterpolationMethod interpMethod=OF_INTERPOLATE_NEAREST_NEIGHBOR);
	bool pasteInto(ofxVoxels_<PixelType> &dst, int x, int y);

	void swapRgb();

	void clear();
	
	PixelType * getPixels();
	const PixelType * getPixels() const;

	int getPixelIndex(int x, int y) const;
	ofColor_<PixelType> getColor(int x, int y) const;
	void setColor(int x, int y, const ofColor_<PixelType>& color);
	void setColor(int index, const ofColor_<PixelType>& color);
	void setColor(const ofColor_<PixelType>& color);

	const PixelType& operator[](int pos) const;
	PixelType& operator[](int pos);

	bool isAllocated() const;

	int getWidth() const;
	int getHeight() const;

	int getBytesPerPixel() const;
	int getBitsPerPixel() const;
	int getBytesPerChannel() const;
	int getBitsPerChannel() const;
	int getNumChannels() const;

	ofxVoxels_<PixelType> getChannel(int channel) const;
	void setChannel(int channel, const ofxVoxels_<PixelType> channelPixels);

	ofImageType getImageType() const;
	void setImageType(ofImageType imageType);
	void setNumChannels(int numChannels);

	int size() const;

private:
	float bicubicInterpolate(const float *patch, float x,float y, float x2,float y2, float x3,float y3);

	void copyFrom( const ofxVoxels_<PixelType>& mom );

	template<typename SrcType>
	void copyFrom( const ofxVoxels_<SrcType>& mom );
	
	PixelType * pixels;
	int 	width;
	int 	height;

	int 	channels; // 1, 3, 4 channels per pixel (grayscale, rgb, rgba)
	bool	bAllocated;
	bool	pixelsOwner;			// if set from external data don't delete it

};


typedef ofxVoxels_<unsigned char> ofxVoxels;
typedef ofxVoxels_<float> ofFloatPixels;
typedef ofxVoxels_<unsigned short> ofShortPixels;


typedef ofxVoxels& ofxVoxelsRef;
typedef ofFloatPixels& ofFloatPixelsRef;
typedef ofShortPixels& ofShortPixelsRef;

// sorry for these ones, being templated functions inside a template i needed to do it in the .h
// they allow to do things like:
//
// ofxVoxels pix;
// ofFloatPixels pixf;
// pix = pixf;

template<typename PixelType>
template<typename SrcType>
ofxVoxels_<PixelType>::ofxVoxels_(const ofxVoxels_<SrcType> & mom){
	bAllocated = false;
	pixelsOwner = false;
	channels = 0;
	pixels = NULL;
	width = 0;
	height = 0;
	copyFrom( mom );
}

template<typename PixelType>
template<typename SrcType>
ofxVoxels_<PixelType>& ofxVoxels_<PixelType>::operator=(const ofxVoxels_<SrcType> & mom){
	copyFrom( mom );
	return *this;
}

template<typename PixelType>
template<typename SrcType>
void ofxVoxels_<PixelType>::copyFrom(const ofxVoxels_<SrcType> & mom){
	if(mom.isAllocated()){
		allocate(mom.getWidth(),mom.getHeight(),mom.getNumChannels());

		const float srcMax = ( (sizeof(SrcType) == sizeof(float) ) ? 1.f : numeric_limits<SrcType>::max() );
		const float dstMax = ( (sizeof(PixelType) == sizeof(float) ) ? 1.f : numeric_limits<PixelType>::max() );
		const float factor = dstMax / srcMax;

		if(sizeof(SrcType) == sizeof(float)) {
			// coming from float we need a special case to clamp the values
			for(int i = 0; i < mom.size(); i++){
				pixels[i] = CLAMP(mom[i], 0, 1) * factor;
			}
		} else{
			// everything else is a straight scaling
			for(int i = 0; i < mom.size(); i++){
				pixels[i] = mom[i] * factor;
			}
		}
	}
}

namespace std{
template<typename PixelType>
void swap(ofxVoxels_<PixelType> & src, ofxVoxels_<PixelType> & dst){
	src.swap(dst);
}
}
