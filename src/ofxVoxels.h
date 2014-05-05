#pragma once

#include "ofConstants.h"
#include "ofUtils.h"
#include "ofColor.h"
#include "ofMath.h"
#include <limits>
#include "ofxBox.h"

/*
 Sometimes when looping throug a volume i use some terminology:
 rows	= across width, x axis
 columns = across height, y axis
 pages	= across depth, z axis
 ---
 where in ofPixels was used some terms (mirror, rotate..) here we use following:
 ofPixels:		ofVolume:
 horizontal	=	_width
 vertical	=	_height
 (none)		=	_depth
 */


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

	void allocate(ofxBox box, int channels);
	void allocate(ofVec3f size, int channels);
	void allocate(ofVec3f size, ofPixelFormat type);
	void allocate(int w, int h, int d, int channels);
	void allocate(int w, int h, int d, ofPixelFormat type);
	void allocate(int w, int h, int d, ofImageType type);

	void setAll(PixelType val);
	void setAll(int channel,PixelType val);
	void setFromVoxels(const PixelType * newVoxels, int w, int h, int d, int channels);
	void setFromVoxels(const PixelType * newVoxels, int w, int h, int d, ofImageType type);
	void setFromExternalVoxels(PixelType * newVoxels, int w, int h, int d, int channels);
	void setFromAlignedVoxels(const PixelType * newVoxels, int w, int h, int d, int channels, int stride);
	void swap(ofxVoxels_<PixelType> & pix);

	//From ofxVoxelsUtils
	// crop to a new width and height, this reallocates memory.
	void crop(int x, int y, int z, int w, int h, int d);
	// not in place
	void cropTo(ofxVoxels_<PixelType> &toPix, int x, int y, int z, int w, int h, int d);
	void rotate90(int nClockwiseRotations);
	void rotate90To(ofxVoxels_<PixelType> & dst, int nClockwiseRotations);
	void mirrorTo(ofxVoxels_<PixelType> & dst, bool _width, bool _height, bool _depth);
	void mirror(bool _height, bool _width, bool _depth);
//	bool resize(int dstWidth, int dstHeight, int dstDepth, ofInterpolationMethod interpMethod=OF_INTERPOLATE_NEAREST_NEIGHBOR);
//	bool resizeTo(ofxVoxels_<PixelType> & dst, ofInterpolationMethod interpMethod=OF_INTERPOLATE_NEAREST_NEIGHBOR);
	bool pasteInto(ofxVoxels_<PixelType> &dst, int x, int y, int z);

	void swapRgb();

	void clear();
	
	PixelType * getVoxels();
	const PixelType * getVoxels() const;

	int getVoxelIndex(int x, int y, int z) const;
	ofColor_<PixelType> getColor(int x, int y, int z) const;
	void setColor(int x, int y, int z, const ofColor_<PixelType>& color);
	void setColor(int index, const ofColor_<PixelType>& color);
	void setColor(const ofColor_<PixelType>& color);

	const PixelType& operator[](int pos) const;
	PixelType& operator[](int pos);

	bool isAllocated() const;
	bool isPow2() const;
	
	int getWidth() const;
	int getHeight() const;
	int getDepth() const;
	ofPoint  getSize() const;

	int getVoxelCount() const;
	int getTotalSize() const;
	int getBytesPerVoxel() const;
	int getBitsPerVoxel() const;
	int getBytesPerChannel() const;
	int getBitsPerChannel() const;
	int getNumChannels() const;

	ofxVoxels_<PixelType> getChannel(int channel) const;
	void setChannel(int channel, const ofxVoxels_<PixelType> channelVoxels);

	ofImageType getImageType() const;
	void setImageType(ofImageType imageType);
	void setNumChannels(int numChannels);

private:
//	float bicubicInterpolate(const float *patch, float x,float y, float x2,float y2, float x3,float y3);

	void copyFrom( const ofxVoxels_<PixelType>& mom );

	template<typename SrcType>
	void copyFrom( const ofxVoxels_<SrcType>& mom );
	bool isPowerOfTwo(int x) const;
	
	PixelType * voxels;
	int 	width;
	int 	height;
	int 	depth;
	
	int 	channels; // 1, 3, 4 channels per pixel (grayscale, rgb, rgba)
	bool	bAllocated;
	bool	voxelsOwner;			// if set from external data don't delete it
};


typedef ofxVoxels_<unsigned char> ofxVoxels;
typedef ofxVoxels_<float> ofFloatVoxels;
typedef ofxVoxels_<unsigned short> ofShortVoxels;


typedef ofxVoxels& ofxVoxelsRef;
typedef ofFloatVoxels& ofFloatVoxelsRef;
typedef ofShortVoxels& ofShortVoxelsRef;

// sorry for these ones, being templated functions inside a template i needed to do it in the .h
// they allow to do things like:
//
// ofxVoxels pix;
// ofFloatVoxels pixf;
// pix = pixf;

template<typename PixelType>
template<typename SrcType>
ofxVoxels_<PixelType>::ofxVoxels_(const ofxVoxels_<SrcType> & mom){
	bAllocated = false;
	voxelsOwner = false;
	channels = 0;
	voxels = NULL;
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
				voxels[i] = CLAMP(mom[i], 0, 1) * factor;
			}
		} else{
			// everything else is a straight scaling
			for(int i = 0; i < mom.size(); i++){
				voxels[i] = mom[i] * factor;
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
