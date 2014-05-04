#include "ofxVoxels.h"
#include "ofMath.h"


static ofImageType getImageTypeFromChannels(int channels){
	switch(channels){
	case 1:
		return OF_IMAGE_GRAYSCALE;
	case 3:
		return OF_IMAGE_COLOR;
	case 4:
		return OF_IMAGE_COLOR_ALPHA;
	default:
		return OF_IMAGE_UNDEFINED;
	}
}

template<typename PixelType>
ofxVoxels_<PixelType>::ofxVoxels_(){
	bAllocated = false;
	voxelsOwner = false;
	channels = 0;
	voxels = NULL;
	clear();
}


template<typename PixelType>
ofxVoxels_<PixelType>::~ofxVoxels_(){
	clear();
}

template<typename PixelType>
ofxVoxels_<PixelType>::ofxVoxels_(const ofxVoxels_<PixelType> & mom){
	
	bAllocated = false;
	voxelsOwner = false;
	channels = 0;
	voxels = NULL;
	width = 0;
	height = 0;
	depth = 0;
	copyFrom( mom );
}

template<typename PixelType>
ofxVoxels_<PixelType>& ofxVoxels_<PixelType>::operator=(const ofxVoxels_<PixelType> & mom){
	if(this==&mom) {
		return * this;
	}
	copyFrom( mom );
	return *this;
}

template<typename PixelType>
void ofxVoxels_<PixelType>::copyFrom(const ofxVoxels_<PixelType> & mom){
	if(mom.isAllocated()) {
		allocate(mom.getWidth(), mom.getHeight(), mom.getDepth(), mom.getNumChannels());
		memcpy(voxels, mom.getVoxels(), mom.getWidth() * mom.getHeight() *mom.getDepth() * mom.getBytesPerVoxel());
	}
}

template<typename PixelType>
void ofxVoxels_<PixelType>::setAll(PixelType val){
	for(int i = 0; i < getByteSize(); i++){
		voxels[i] = val;
	}
}

template<typename PixelType>
void ofxVoxels_<PixelType>::setAll(int channel,PixelType val){
	for(int i=channel;i<getByteSize();i+=channels){
		voxels[i] = val;
	}
}

template<typename PixelType>
void ofxVoxels_<PixelType>::setFromVoxels(const PixelType * newVoxels, int w, int h, int d, int channels){
	allocate(w, h, d, channels);
	memcpy(voxels, newVoxels, w* h* d* getBytesPerVoxel());
}

template<typename PixelType>
void ofxVoxels_<PixelType>::setFromVoxels(const PixelType * newVoxels, int w, int h, int d, ofImageType type){
	allocate(w,h,d,type);
	switch(type){
	case OF_IMAGE_GRAYSCALE:
		setFromVoxels(newVoxels,w,h,d,1);
		break;
	case OF_IMAGE_COLOR:
		setFromVoxels(newVoxels,w,h,d,3);
		break;
	case OF_IMAGE_COLOR_ALPHA:
		setFromVoxels(newVoxels,w,h,d,4);
		break;
	default:
		ofLogError("ofxVoxels") << "setFromVoxels(): image type " << type << " not supported, not copying";
		break;
	}
}

template<typename PixelType>
void ofxVoxels_<PixelType>::setFromExternalVoxels(PixelType * newVoxels, int w, int h, int d, int _channels){
	clear();
	channels = _channels;
	width	= w;
	height	= h;
	depth	= d;
	
	voxels = newVoxels;
	voxelsOwner = false;
	bAllocated = true;
}

template<typename PixelType>
void ofxVoxels_<PixelType>::setFromAlignedVoxels(const PixelType * newVoxels, int w, int h, int d, int channels, int stride) {
	if(w*channels==stride){
		setFromVoxels(newVoxels, w, h, d, channels);
		return;
	}
	allocate(w, h, d, channels);
	int dstStride = width * getBytesPerVoxel();

	const unsigned char* src	= (unsigned char*) newVoxels;
	unsigned char* dst			= (unsigned char*) voxels;

	for(int i = 0; i < depth; i++) {
		for(int j = 0; j < height; j++) {
			memcpy(dst, src, dstStride);
			src += stride;
			dst += dstStride;
		}
	}
}

template<typename PixelType>
void ofxVoxels_<PixelType>::swap(ofxVoxels_<PixelType> & pix){
	std::swap(voxels,pix.voxels);
	std::swap(width, pix.width);
	std::swap(height,pix.height);
	std::swap(depth,pix.depth);
	std::swap(channels,pix.channels);
	std::swap(voxelsOwner, pix.voxelsOwner);
	std::swap(bAllocated, pix.bAllocated);
}

template<typename PixelType>
PixelType * ofxVoxels_<PixelType>::getVoxels(){
	return &voxels[0];
}

template<typename PixelType>
const PixelType * ofxVoxels_<PixelType>::getVoxels() const{
	return &voxels[0];
}

template<typename PixelType>
void ofxVoxels_<PixelType>::allocate(ofxBox box, int _channels){
	allocate(box.w, box.h, box.d, _channels);
}
template<typename PixelType>
void ofxVoxels_<PixelType>::allocate(ofVec3f size, int _channels){
	allocate(size.x, size.y, size.z, _channels);
}
template<typename PixelType>
void ofxVoxels_<PixelType>::allocate(ofVec3f size, ofPixelFormat format){
	allocate(size.x, size.y, size.z, format);
}

template<typename PixelType>
void ofxVoxels_<PixelType>::allocate(int w, int h, int d, int _channels){
	if (w < 0 || h < 0 || d < 0) {
		return;
	}

	//we check if we are already allocated at the right size
	if(bAllocated && w == width && h == height && d == depth && channels ==_channels){
		return; //we don't need to allocate
	}

	//we do need to allocate, clear the data
	clear();

	channels = _channels;
	width= w;
	height = h;
	depth = d;
	
	voxels = new PixelType[w * h * d * channels];
	bAllocated = true;
	voxelsOwner = true;
}

template<typename PixelType>
void ofxVoxels_<PixelType>::allocate(int w, int h, int d, ofPixelFormat format){

	if (w < 0 || h < 0|| d < 0) return;

	ofImageType imgType;
	switch(format){
		case OF_PIXELS_RGB:
			imgType = OF_IMAGE_COLOR;
			break;
		case OF_PIXELS_RGBA:
		case OF_PIXELS_BGRA:
			imgType = OF_IMAGE_COLOR_ALPHA;
			break;
		case OF_PIXELS_MONO:
			imgType = OF_IMAGE_GRAYSCALE;
			break;
		default:
			ofLogError("ofxVoxels") << "allocate(): unknown pixel format, not allocating";
			return;
			break;

	}
	allocate(w,h,d,imgType);
}

template<typename PixelType>
void ofxVoxels_<PixelType>::allocate(int w, int h, int d, ofImageType type){
	switch(type){
	case OF_IMAGE_GRAYSCALE:
		allocate(w,h,d,1);
		break;
	case OF_IMAGE_COLOR:
		allocate(w,h,d,3);
		break;
	case OF_IMAGE_COLOR_ALPHA:
		allocate(w,h,d,4);
		break;
	default:
		ofLogError("ofxVoxels") << "allocate(): unknown image type, not allocating";
		break;

	}
}

template<typename PixelType>
void ofxVoxels_<PixelType>::swapRgb(){
	if (channels >= 3){
		int sizeVoxels = width*height*depth*channels;
		for (int i=0; i< sizeVoxels; i+=channels){
			std::swap(voxels[i],voxels[i+2]);
		}
	}
}

template<typename PixelType>
void ofxVoxels_<PixelType>::clear(){
	if(voxels){
		if(voxelsOwner) delete[] voxels;
		voxels = NULL;
	}

	width			= 0;
	height			= 0;
	depth			= 0;
	channels		= 0;
	bAllocated		= false;
}

template<typename PixelType>
int ofxVoxels_<PixelType>::getVoxelIndex(int x, int y, int z) const {
	if( !bAllocated ){
		return 0;
	}else{
		return ( x + (y * width ) + (z * width * height)) * channels;
	}
}

template<typename PixelType>
ofColor_<PixelType> ofxVoxels_<PixelType>::getColor(int x, int y, int z) const {
	ofColor_<PixelType> c;
	int index = getVoxelIndex(x, y, z);

	if( channels == 1 ){
		c.set( voxels[index] );
	}else if( channels == 3 ){
		c.set( voxels[index], voxels[index+1], voxels[index+2] );
	}else if( channels == 4 ){
		c.set( voxels[index], voxels[index+1], voxels[index+2], voxels[index+3] );
	}

	return c;
}

template<typename PixelType>
void ofxVoxels_<PixelType>::setColor(int index, const ofColor_<PixelType>& color) {
	if( channels == 1 ){
		voxels[index] = color.getBrightness();
	}else if( channels == 3 ){
		voxels[index] = color.r;
		voxels[index+1] = color.g;
		voxels[index+2] = color.b;
	}else if( channels == 4 ){
		voxels[index] = color.r;
		voxels[index+1] = color.g;
		voxels[index+2] = color.b;
		voxels[index+3] = color.a;
	}
}

template<typename PixelType>
void ofxVoxels_<PixelType>::setColor(int x, int y, int z, const ofColor_<PixelType>& color) {
	setColor(getVoxelIndex(x, y, z), color);
}

template<typename PixelType>
void ofxVoxels_<PixelType>::setColor(const ofColor_<PixelType>& color) {
	int i = 0;
	while(i < getByteSize()) {
		for(int j = 0; j < channels; j++) {
			voxels[i++] = color[j];
		}
	}
}

template<typename PixelType>
PixelType & ofxVoxels_<PixelType>::operator[](int pos){
	return voxels[pos];
}

template<typename PixelType>
const PixelType & ofxVoxels_<PixelType>::operator[](int pos) const{
	return voxels[pos];
}

template<typename PixelType>
bool ofxVoxels_<PixelType>::isAllocated() const{
	return bAllocated;
}
template<typename PixelType>
bool ofxVoxels_<PixelType>::isPow2() const{
	bool bPow2;
	bPow2 = (isPowerOfTwo(width) && isPowerOfTwo(height) && isPowerOfTwo(depth)) ? true : false;
	return bPow2;
}
template<typename PixelType>
bool ofxVoxels_<PixelType>::isPowerOfTwo(int x) const{
    return (x != 0) && ((x & (x - 1)) == 0);
}

template<typename PixelType>
int ofxVoxels_<PixelType>::getWidth() const{
	return width;
}
template<typename PixelType>
int ofxVoxels_<PixelType>::getHeight() const{
	return height;
}
template<typename PixelType>
int ofxVoxels_<PixelType>::getDepth() const{
	return depth;
}
template<typename PixelType>
ofPoint ofxVoxels_<PixelType>::getSize() const {
	return ofPoint(width, height, depth);
}

template<typename PixelType>
int ofxVoxels_<PixelType>::getVoxelCount() const{
	return width*height*depth;
}

template<typename PixelType>
int ofxVoxels_<PixelType>::getByteSize() const{
	return getVoxelCount()*channels;
}

template<typename PixelType>
int ofxVoxels_<PixelType>::getBytesPerVoxel() const{
	return getBytesPerChannel() * channels;
}

template<typename PixelType>
int ofxVoxels_<PixelType>::getBitsPerVoxel() const{
	return getBitsPerChannel() * channels;
}

template<typename PixelType>
int ofxVoxels_<PixelType>::getBytesPerChannel() const{
	return sizeof(PixelType);
}

template<typename PixelType>
int ofxVoxels_<PixelType>::getBitsPerChannel() const{
	return getBytesPerChannel() * 8;
}

template<typename PixelType>
int ofxVoxels_<PixelType>::getNumChannels() const{
	return channels;
}

template<typename PixelType>
ofImageType ofxVoxels_<PixelType>::getImageType() const{
	return getImageTypeFromChannels(getNumChannels());
}

template<typename PixelType>
void ofxVoxels_<PixelType>::setImageType(ofImageType imageType){
	if(!isAllocated() || imageType==getImageType()) return;
	ofxVoxels_<PixelType> dst;
	dst.allocate(width,height,depth,imageType);
	PixelType * dstPtr = &dst[0];
	PixelType * srcPtr = &voxels[0];
	int diffNumChannels = 0;
	if(dst.getNumChannels()<getNumChannels()){
		diffNumChannels = getNumChannels()-dst.getNumChannels();
	}
	for(int i=0;i<width*height*depth;i++){
		const PixelType & gray = *srcPtr;
		for(int j=0;j<dst.getNumChannels();j++){
			if(j<getNumChannels()){
				*dstPtr++ =  *srcPtr++;
			}else if(j<3){
				*dstPtr++ = gray;
			}else{
				*dstPtr++ = ofColor_<PixelType>::limit();
			}
		}
		srcPtr+=diffNumChannels;
	}
	swap(dst);
}

template<typename PixelType>
void ofxVoxels_<PixelType>::setNumChannels(int numChannels){
	if(!isAllocated() || numChannels==getNumChannels()) return;
	setImageType(getImageTypeFromChannels(numChannels));
}

template<typename PixelType>
ofxVoxels_<PixelType> ofxVoxels_<PixelType>::getChannel(int channel) const{
	ofxVoxels_<PixelType> channelVoxels;
	channelVoxels.allocate(width,height,depth,1);
	channel = ofClamp(channel,0,channels-1);
	int j=0;
	for(int i=channel; i<getByteSize(); i+=channels, ++j){
		channelVoxels[j]=voxels[i];
	}
	return channelVoxels;
}

template<typename PixelType>
void ofxVoxels_<PixelType>::setChannel(int channel, const ofxVoxels_<PixelType> channelVoxels){
	channel = ofClamp(channel,0,channels-1);
	int j=0;
	for(int i=channel;i<getByteSize();i+=channels, ++j){
		voxels[i] = channelVoxels[j];
	}

}

//From ofxVoxelsUtils
//---------------------------------------------------------------------- OK? to test
template<typename PixelType>
void ofxVoxels_<PixelType>::crop(int x, int y, int z, int w, int h, int d){

	if (bAllocated == true){

		w = ofClamp(w,1,getWidth());
		h = ofClamp(h,1,getHeight());
		d = ofClamp(h,1,getDepth());
		
		int bytesPerVoxel = channels;

		int newWidth = w;
		int newHeight = h;
		int newDepth = d;
		
		// malloc
		PixelType * newVoxels = new PixelType[newWidth* newHeight* newDepth* bytesPerVoxel];
		memset(newVoxels, 0, newWidth* newHeight* newDepth* channels*sizeof(PixelType));

		// this prevents having to do a check for bounds in the for loop;
		int minX = MAX(x, 0);
		int maxX = MIN(x+w, width);
		int minY = MAX(y, 0);
		int maxY = MIN(y+h, height);
		int minZ = MAX(z, 0);
		int maxZ = MIN(z+d, depth);

		// TODO: point math can help speed this up:
		for (int i = minX; i < maxX; i++){
			for (int j = minY; j < maxY; j++){
				for (int k = minZ; k < maxZ; k++){
					//		return ( x + (y * width ) + (z * width * height)) * channels;
					int newVoxel =	(i-x) +									//x
									((j-y) * newWidth) +					//y
									((k-z) * newWidth * newHeight) ;		//z
					int oldVoxel = i + (j * width) + (z * width * height);
					
					for (int l = 0; l < bytesPerVoxel; l++){
						newVoxels[newVoxel* bytesPerVoxel + l] = voxels[oldVoxel* bytesPerVoxel + l];
					}
				}
			}
		}//end for
		delete [] voxels;
		voxels	= newVoxels;
		width	= newWidth;
		height	= newHeight;
		depth	= newDepth;
	}
}

//---------------------------------------------------------------------- OK? to test
template<typename PixelType>
void ofxVoxels_<PixelType>::cropTo(ofxVoxels_<PixelType> &toPix, int x, int y, int z, int w, int h, int d){

	if (bAllocated == true){
		
		w = ofClamp(w,1,getWidth());
		h = ofClamp(h,1,getHeight());
		d = ofClamp(d,1,getDepth());
		
		int bytesPerVoxel = channels;

		int newWidth	= w;
		int newHeight	= h;
		int newDepth	= d;
		
		if ((toPix.width != w) || (toPix.height != h) || (toPix.depth != d) || (toPix.channels != channels)){
			toPix.allocate(w, h, d, channels);
		}

		PixelType * newVoxels = toPix.voxels;

		// this prevents having to do a check for bounds in the for loop;
		int minX = MAX(x, 0);
		int maxX = MIN(x+w, width);
		int minY = MAX(y, 0);
		int maxY = MIN(y+h, height);
		int minZ = MAX(z, 0);
		int maxZ = MIN(z+d, depth);

		// TODO: point math can help speed this up:
		for (int i = minX; i < maxX; i++){
			for (int j = minY; j < maxY; j++){
				for (int k = minZ; k < maxZ; k++){
					//		return ( x + (y * width ) + (z * width * height)) * channels;
					int newVoxel =	(i-x) +									//x
									((j-y) * newWidth) +					//y
									((k-z) * newWidth * newHeight) ;		//z
					int oldVoxel = i + (j * width) + (z * width * height);
					
					for (int l = 0; l < bytesPerVoxel; l++){
						newVoxels[newVoxel* bytesPerVoxel + l] = voxels[oldVoxel* bytesPerVoxel + l];
					}
				}
			}
		}//end for
	}
}

//---------------------------------------------------------------------- TODO
/* 3rd axis not written. this means that I've just adapted the code to run through the volume,
 but you can rotate ONLY in one axis.
 */
template<typename PixelType>
void ofxVoxels_<PixelType>::rotate90(int nClockwiseRotations){

	if (bAllocated == false){
		return;
	}

	// first, figure out which type of rotation we have
	int rotation = nClockwiseRotations;
	while (rotation < 0){
		rotation+=4;
	}
	rotation %= 4;

	// if it's 0, do nothing.  if it's 2, do it by a mirror operation.
	if (rotation == 0) {
		return;	// do nothing!
	} else if (rotation == 2) {
		mirror(true, true, false);
		return;
	}
	
	// otherwise, we will need to do some new allocaiton.
	ofxVoxels_<PixelType> newVoxels;
	rotate90To(newVoxels,nClockwiseRotations);
	delete [] voxels;
	voxels = newVoxels.voxels;
	width = newVoxels.width;
	height = newVoxels.height;
	depth = newVoxels.depth;
	newVoxels.voxelsOwner = false;

}

//---------------------------------------------------------------------- TODO
/* 3rd axis not written. this means that I've just adapted the code to run through the volume,
 but you can rotate ONLY in one axis.
 */
template<typename PixelType>
void ofxVoxels_<PixelType>::rotate90To(ofxVoxels_<PixelType> & dst, int nClockwiseRotations){
	if (bAllocated == false){
		return;
	}
	
	if(&dst == this){
		rotate90(nClockwiseRotations);
		return;
	}
	
	// first, figure out which type of rotation we have
	int rotation = nClockwiseRotations;
	while (rotation < 0){
		rotation+=4;
	}
	rotation %= 4;
	
	// if it's 0, just make a copy.  if it's 2, do it by a mirror operation.
	if (rotation == 0) {
		dst = *this;
		return;
		// do nothing!
	} else if (rotation == 2) {
		mirrorTo(dst, true, true, false);
		return;
	}
	
	// otherwise, we will need to do some new allocaiton.
	dst.allocate(height,width,depth,getImageType());
    
	int strideSrc = width * channels;
	int strideDst = dst.width * channels;
	
	if(rotation == 1){
		PixelType * srcVoxels = voxels;
		for (int i = 0; i < height; i++){
			PixelType * dstVoxels = dst.getVoxels() + (strideDst - channels*(i+1));
			for (int j = 0; j < width; j++){
				for (int k = 0; k < channels; k++){
					dstVoxels[k] = srcVoxels[k];
				}
				srcVoxels += channels;
				dstVoxels += strideDst;
			}
		}
	} else if(rotation == 3){
		PixelType * dstVoxels = dst.voxels;
		for (int i = 0; i < dst.height; i++){
			PixelType * srcVoxels = voxels + (strideSrc - channels*(i+1));
			for (int j = 0; j < dst.width; j++){
				for (int k = 0; k < channels; k++){
					dstVoxels[k] = srcVoxels[k];
				}
				srcVoxels += strideSrc;
				dstVoxels += channels;
			}
		}
	}
}

//---------------------------------------------------------------------- OK? to test
template<typename PixelType>
void ofxVoxels_<PixelType>::mirror(bool _width, bool _height, bool _depth){

	if (!_width && !_height && !_depth){
		return;
	}

	int bytesPerVoxel = channels;
	PixelType * oldVoxels = voxels;
	PixelType tempVal;

	if (! (_width && _height && _depth)){
		int wToDo = _width ? width/2 : width;
		int hToDo = _height ? height/2 : height;
		int dToDo = _depth ? depth/2 : depth;
		
		for (int i = 0; i < wToDo; i++){
			for (int j = 0; j < hToDo; j++){
				for (int k = 0; k < dToDo; k++){
//					int newVoxel =	(i-x) +									//x
//									((j-y) * newWidth) +					//y
//									((k-z) * newWidth * newHeight) ;		//z
//					int oldVoxel = i + (j * width) + (z * width * height);
					
//					int pixelb = (_height ? (height - j - 1) : j) * width + (_width ? (width - i - 1) : i);
					int pixelb =(_width ? (width - i - 1) : i) +
								((_height ? (height - j - 1) : j) * width) +
								((_depth ? (depth - k - 1) : k) * width * height);
					int pixela = i + (j*width) + (k*width*height) ;
					
					for (int l = 0; l < bytesPerVoxel; l++){
						tempVal = oldVoxels[pixela*bytesPerVoxel + l];
						oldVoxels[pixela*bytesPerVoxel + l] = oldVoxels[pixelb*bytesPerVoxel + l];
						oldVoxels[pixelb*bytesPerVoxel + l] = tempVal;
					}
			}
			}
		}
	} else {
		// I couldn't think of a good way to do this in place.  I'm sure there is.
		mirror(true, false, false);
		mirror(false, true, false);
		mirror(false, false, true);
	}
}

//---------------------------------------------------------------------- OK? to test
template<typename PixelType>
void ofxVoxels_<PixelType>::mirrorTo(ofxVoxels_<PixelType> & dst, bool _width, bool _height, bool _depth){
	if(&dst == this){
		mirror(_width, _height, _depth);
		return;
	}

	if (!_width && !_height && _depth){
		dst = *this;
		return;
	}

	int bytesPerVoxel = channels;
	if (! (_width && _height && _depth)){
		int wToDo = _width ? width/2 : width;
		int hToDo = _height ? height/2 : height;
		int dToDo = _depth ? depth/2 : depth;
		
		for (int i = 0; i < wToDo; i++){
			for (int j = 0; j < hToDo; j++){
				for (int k = 0; k < dToDo; k++){
					//int newVoxel =(i-x) +									//x
					//				((j-y) * newWidth) +					//y
					//				((k-z) * newWidth * newHeight) ;		//z
					//int oldVoxel = i + (j * width) + (z * width * height);
					
					//int pixelb = (_height ? (height - j - 1) : j) * width + (_width ? (width - i - 1) : i);
					int pixelb =(_width ? (width - i - 1) : i) +
					((_height ? (height - j - 1) : j) * width) +
					((_depth ? (depth - k - 1) : k) * width * height);
					int pixela = i + (j*width) + (k*width*height) ;
					
					for (int l = 0; l < bytesPerVoxel; l++){
						dst[pixela*bytesPerVoxel + k] = voxels[pixelb*bytesPerVoxel + k];
						dst[pixelb*bytesPerVoxel + k] = voxels[pixela*bytesPerVoxel + k];
					}
				}
			}
		}
	} else {
		// I couldn't think of a good way to do this in place.  I'm sure there is.
		mirrorTo(dst, true, false, false); // why mirrorTo? We have to insert the data at least once, otherwise it would be empty.
		dst.mirror(false, true, false);
		dst.mirror(false, false, true);
	}

}
/*
//---------------------------------------------------------------------- OK? to test
template<typename PixelType>
bool ofxVoxels_<PixelType>::resize(int dstWidth, int dstHeight, int dstDepth, ofInterpolationMethod interpMethod){

	if ((dstWidth<=0) || (dstHeight<=0) || (dstDepth<=0) || !(isAllocated())) return false;

	ofxVoxels_<PixelType> dstVoxels;
	dstVoxels.allocate(dstWidth, dstHeight, dstDepth, getImageType());

	if(!resizeTo(dstVoxels,interpMethod)) return false;

	delete [] voxels;
	voxels = dstVoxels.getVoxels();
	width  = dstWidth;
	height = dstHeight;
	depth  = dstDepth;
	dstVoxels.voxelsOwner = false;
	return true;
}

//---------------------------------------------------------------------- TODO
template<typename PixelType>
bool ofxVoxels_<PixelType>::resizeTo(ofxVoxels_<PixelType>& dst, ofInterpolationMethod interpMethod){
	if(&dst == this){
		return true;
	}
	
	if (!(isAllocated()) || !(dst.isAllocated()) || getBytesPerVoxel() != dst.getBytesPerVoxel() || getNumChannels()!=dst.getNumChannels()) return false;
	
	int srcWidth      = getWidth();
	int srcHeight     = getHeight();
	int dstWidth	  = dst.getWidth();
	int dstHeight	  = dst.getHeight();
	int channels 	  = getNumChannels();
	
	
	PixelType * dstVoxels = dst.getVoxels();
	
	switch (interpMethod){
			
			//----------------------------------------
		case OF_INTERPOLATE_NEAREST_NEIGHBOR:{
			int dstIndex = 0;
			float srcxFactor = (float)srcWidth/dstWidth;
			float srcyFactor = (float)srcHeight/dstHeight;
			float srcy = 0.5;
			for (int dsty=0; dsty<dstHeight; dsty++){
				float srcx = 0.5;
				int srcIndex = int(srcy)*srcWidth;
				for (int dstx=0; dstx<dstWidth; dstx++){
					int pixelIndex = int(srcIndex + srcx) * channels;
					for (int k=0; k<channels; k++){
						dstVoxels[dstIndex] = voxels[pixelIndex];
						dstIndex++;
						pixelIndex++;
					}
					srcx+=srcxFactor;
				}
				srcy+=srcyFactor;
			}
		}break;
			
			//----------------------------------------
		case OF_INTERPOLATE_BILINEAR:
			// not implemented yet
			ofLogError("ofxVoxels") << "resizeTo(): bilinear resize not implemented, not resizing";
			break;
			
			//----------------------------------------
		case OF_INTERPOLATE_BICUBIC:
			float px1, py1;
			float px2, py2;
			float px3, py3;
			
			float srcColor = 0;
			float interpCol;
			int patchRow;
			int patchIndex;
			float patch[16];
			
			int srcRowBytes = srcWidth*channels;
			int loIndex = (srcRowBytes)+1;
			int hiIndex = (srcWidth*srcHeight*channels)-(srcRowBytes)-1;
			
			for (int dsty=0; dsty<dstHeight; dsty++){
				for (int dstx=0; dstx<dstWidth; dstx++){
					
					int   dstIndex0 = (dsty*dstWidth + dstx) * channels;
					float srcxf = srcWidth  * (float)dstx/(float)dstWidth;
					float srcyf = srcHeight * (float)dsty/(float)dstHeight;
					int   srcx = (int) MIN(srcWidth-1,   srcxf);
					int   srcy = (int) MIN(srcHeight-1,  srcyf);
					int   srcIndex0 = (srcy*srcWidth + srcx) * channels;
					
					px1 = srcxf - srcx;
					py1 = srcyf - srcy;
					px2 = px1 * px1;
					px3 = px2 * px1;
					py2 = py1 * py1;
					py3 = py2 * py1;
					
					for (int k=0; k<channels; k++){
						int   dstIndex = dstIndex0+k;
						int   srcIndex = srcIndex0+k;
						
						for (int dy=0; dy<4; dy++) {
							patchRow = srcIndex + ((dy-1)*srcRowBytes);
							for (int dx=0; dx<4; dx++) {
								patchIndex = patchRow + (dx-1)*channels;
								if ((patchIndex >= loIndex) && (patchIndex < hiIndex)) {
									srcColor = voxels[patchIndex];
								}
								patch[dx*4 + dy] = srcColor;
							}
						}
						
						interpCol = (PixelType)bicubicInterpolate(patch, px1,py1, px2,py2, px3,py3);
						dstVoxels[dstIndex] = interpCol;
					}
					
				}
			}
			break;
	}
	
	return true;
}

//---------------------------------------------------------------------- TODO
template<typename PixelType>
float ofxVoxels_<PixelType>::bicubicInterpolate (const float *patch, float x,float y, float x2,float y2, float x3,float y3) {
	// adapted from http://www.paulinternet.nl/?page=bicubic
	// Note that this code can produce values outside of 0...255, due to cubic overshoot.
	// The ofClamp() prevents this from happening.

	float p00 = patch[ 0];
	float p10 = patch[ 4];
	float p20 = patch[ 8];
	float p30 = patch[12];

	float p01 = patch[ 1];
	float p11 = patch[ 5];
	float p21 = patch[ 9];
	float p31 = patch[13];

	float p02 = patch[ 2];
	float p12 = patch[ 6];
	float p22 = patch[10];
	float p32 = patch[14];

	float p03 = patch[ 3];
	float p13 = patch[ 7];
	float p23 = patch[11];
	float p33 = patch[15];

	float a00 =    p11;
	float a01 =   -p10 +   p12;
	float a02 =  2.0f*p10 - 2.0f*p11 +   p12 -   p13;
	float a03 =   -p10 +   p11 -   p12 +   p13;
	float a10 =   -p01 +   p21;
	float a11 =    p00 -   p02 -   p20 +   p22;
	float a12 = -2.0f*p00 + 2.0f*p01 -   p02 +   p03 + 2.0f*p20 - 2.0f*p21 +   p22 -   p23;
	float a13 =    p00 -   p01 +   p02 -   p03 -   p20 +   p21 -   p22 +   p23;
	float a20 =  2.0f*p01 - 2.0f*p11 +   p21 -   p31;
	float a21 = -2.0f*p00 + 2.0f*p02 + 2.0f*p10 - 2.0f*p12 -   p20 +   p22 +   p30 -   p32;
	float a22 =  4*p00 - 4*p01 + 2.0f*p02 - 2.0f*p03 - 4*p10 + 4*p11 - 2.0f*p12 + 2.0f*p13 + 2.0f*p20 - 2.0f*p21 + p22 - p23 - 2.0f*p30 + 2.0f*p31 - p32 + p33;
	float a23 = -2.0f*p00 + 2.0f*p01 - 2.0f*p02 + 2.0f*p03 + 2.0f*p10 - 2.0f*p11 + 2.0f*p12 - 2.0f*p13 -   p20 +   p21 - p22 + p23 +   p30 -   p31 + p32 - p33;
	float a30 =   -p01 +   p11 -   p21 +   p31;
	float a31 =    p00 -   p02 -   p10 +   p12 +   p20 -   p22 -   p30 +   p32;
	float a32 = -2.0f*p00 + 2.0f*p01 -   p02 +   p03 + 2.0f*p10 - 2.0f*p11 +   p12 -   p13 - 2.0f*p20 + 2.0f*p21 - p22 + p23 + 2.0f*p30 - 2.0f*p31 + p32 - p33;
	float a33 =    p00 -   p01 +   p02 -   p03 -   p10 +   p11 -   p12 +   p13 +   p20 -   p21 + p22 - p23 -   p30 +   p31 - p32 + p33;

	float out =
    a00      + a01 * y      + a02 * y2      + a03 * y3 +
    a10 * x  + a11 * x  * y + a12 * x  * y2 + a13 * x  * y3 +
    a20 * x2 + a21 * x2 * y + a22 * x2 * y2 + a23 * x2 * y3 +
    a30 * x3 + a31 * x3 * y + a32 * x3 * y2 + a33 * x3 * y3;

	return MIN(255, MAX(out, 0));
}
*/

//---------------------------------------------------------------------- TODO
template<typename PixelType>
bool ofxVoxels_<PixelType>::pasteInto(ofxVoxels_<PixelType> &dst, int xTo, int yTo, int zTo){
	if (!(isAllocated()) || !(dst.isAllocated()) ||										// check if voxels are allocated,
		getBytesPerVoxel() != dst.getBytesPerVoxel() ||									// if bytes are same
		xTo>=dst.getWidth() || yTo>=dst.getHeight() || zTo>=dst.getDepth()) return false;// and if position is within bounds

	int bytesToCopyPerRow	= (xTo + getWidth() <=dst.getWidth() ? getWidth() : dst.getWidth() -xTo) * getBytesPerVoxel();
	int columnsToCopy		= yTo + getHeight() <= dst.getHeight() ? getHeight() : dst.getHeight() -yTo;
	int pagesToCopy			= zTo + getDepth() <= dst.getDepth() ? getDepth() :	dst.getDepth() -zTo;
	
	PixelType * dstVox = dst.getVoxels() + ((xTo + yTo*dst.getWidth())*dst.getBytesPerVoxel());
	PixelType * srcVox = getVoxels();
	
	int srcStride = getWidth() * getBytesPerVoxel();
	int dstStride = dst.getWidth() * dst.getBytesPerVoxel();

	for(int i=0;i<pagesToCopy; i++){
	for(int y=0;y<columnsToCopy; y++){
		memcpy(dstVox,srcVox,bytesToCopyPerRow);
		dstVox += dstStride;
		srcVox += srcStride;
	}
	}
	return true;
}


template class ofxVoxels_<char>;
template class ofxVoxels_<unsigned char>;
template class ofxVoxels_<short>;
template class ofxVoxels_<unsigned short>;
template class ofxVoxels_<int>;
template class ofxVoxels_<unsigned int>;
template class ofxVoxels_<long>;
template class ofxVoxels_<unsigned long>;
template class ofxVoxels_<float>;
template class ofxVoxels_<double>;
