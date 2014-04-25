/*  ofxVolume - draw ofxVolumetric data
 Created by Jordi on 01/04/14.
 */

#include "ofxVolume.h"

ofxVolume::ofxVolume()
{
}
//--------------------------------------------------------------
ofxVolume::~ofxVolume()
{

}

void ofxVolume::load(string path)
{
	imageSequence.init(path + "IM-0001-0", 3, ".tif", 0);
	
	// calculate ofxVolume size
	volBox.w	= imageSequence.getWidth();
    volBox.h	= imageSequence.getHeight();
    volBox.d	= imageSequence.getSequenceLength();
	
	ofLogNotice("vizManager") << "setting up ofxVolume data buffer at " << volBox.w << "x" << volBox.h << "x" << volBox.d;
    voxels = new unsigned char[(int) (volBox.w*volBox.h*volBox.d)];
	
	//fill out the array pixel in white for easy debugging
	for (int i=0; i<volBox.w*volBox.h*volBox.d; i++ )
	{
		voxels[i]= (unsigned char) 255;
	}
	
	// fill my array with pixels
    for(int z=0; z<volBox.d; z++)
    {
        imageSequence.loadFrame(z);
		int gradient = 0;
		for(int y=0; y<volBox.h; y++)
        {
			for(int x=0; x<volBox.w; x++)
			{
				if (x<volBox.w && y<volBox.h)
				{																// get values from image
					int i = ((x + volBox.w*y) + z*volBox.w*volBox.h);			// the pointer position at Array
					int sample = imageSequence.getPixels()[(int)(x+y*volBox.w)];		// the pixel on the image
					voxels[i] = sample;
					//					ofLogVerbose("vizManager") << sample << " ";
				}
            }
        }
    }
}


void ofxVolume::allocate(ofVec3f size)
{
	// set ofxVolume size
	volBox.w	= size.x;
    volBox.h	= size.y;
    volBox.d	= size.z;
	volSize		= size;
	
	ofLogNotice("vizManager") << "setting up ofxVolume data buffer at " << volBox.w << "x" << volBox.h << "x" << volBox.d;
    voxels = new unsigned char[(int) (volBox.w*volBox.h*volBox.d*4)];
	
	/*
	//fill out the array pixel in white for easy debugging
	for (int i=0; i<volBox.w*volBox.h*volBox.d*4; i++ )
	{
		voxels[i]= (unsigned char) 55;
	}
	*/
	
/*
	int max = 255;
	ofColor myColor = ofColor(0,0,0,255);
	// fill my array with pixels
    for(int z=0; z<volBox.d; z++)
    {
		myColor= ofColor(myColor.r, myColor.g, z/volBox.d*max, myColor.a);
		for(int y=0; y<volBox.h; y++)
        {
			myColor= ofColor(myColor.r, y/volBox.h*max, myColor.b, myColor.a);
			for(int x=0; x<volBox.w; x++)
			{
				myColor= ofColor(y/volBox.w*max, myColor.g, myColor.b, myColor.a);

				int i = ((x + volBox.w*y) + z*volBox.w*volBox.h)*4;			// the pointer position at Array
//				int sample = imageSequence.getPixels()[x+y*volBox.w];		// the pixel on the image
				voxels[i] = myColor.r;
				voxels[i+1] = myColor.g;
				voxels[i+2] = myColor.b;
				voxels[i+3] = myColor.a;
			}
        }
    }
*/
}

//--------------------------------------------------------------
void ofxVolume::setup(float bW, float bH)
{
	boxW		= bW;
	boxH		= bH;
	
	// Needed to align the ofxVolume at the center of the box
	// Attention! this is not correct..
	// will give problems if W & H are not d same
	halfH = (boxW - volBox.h) /2;
	halfW = (boxW - volBox.w) /2;
	halfD = (boxW - volBox.d) /2;
	
	//allocate my pixls type of the image slices
	coronalPixels.allocate(volBox.w, volBox.d, OF_IMAGE_GRAYSCALE);
	coronalPixels.set(255);
	sagittalPixels.allocate(volBox.d, volBox.h, OF_IMAGE_GRAYSCALE);
	sagittalPixels.set(255);
	axialPixels.allocate(volBox.w, volBox.h, OF_IMAGE_GRAYSCALE);
	axialPixels.set(255);	
}

//--------------------------------------------------------------
int ofxVolume::getVoxelValue(){
	
	int value	=0;
	for(int z=0; z<volBox.d; z++){
		if (z==axialS){
			for(int y=0; y<volBox.h; y++){
				if (y==coronalS) {
					for(int x=0; x<volBox.w; x++){
						if (x==sagittalS){
							int line = y*volBox.w;
							int page = z*volBox.w*volBox.h;
							int i = x + line + page;					// the pointer position at Array
							value= voxels[i];							// the pixel on the image
						}
					}
				}
			}
		}
	}

	ofLogVerbose("ofxVolume") << "sagittalS= " << sagittalS;
	ofLogVerbose("ofxVolume") << "coronalS= " << coronalS;
	ofLogVerbose("ofxVolume") << "axialS= " << axialS;
	ofLogVerbose("ofxVolume") << "voxelValue= " << value;
	return value;
}

//--------------------------------------------------------------
ofVec3f ofxVolume::getVoxelCoordinates(int _index){
	
	ofVec3f value (0);
	for(int z=0; z<volBox.d; z++){
		value.z=z;
		if (z==axialS){
			for(int y=0; y<volBox.h; y++){
				value.y=y;
				if (y==coronalS) {
					for(int x=0; x<volBox.w; x++){
						value.x=x;

						int line = y*volBox.w;
						int page = z*volBox.w*volBox.h;
						int i = x + line + page;

						if (_index==i){
							value= ofVec3f(x,y,z);
							ofLogVerbose("ofxVolume") << "voxelCoordinates= " << value;
							return value;
						}
					}
				}
			}
		}
	}
	
}

//--------------------------------------------------------------
bool ofxVolume::getVoxelCoordAndVal(int _index, ofVec3f& _coord, int& _val){
	
	ofVec3f value(0);
	int row=0;
	int page=0;
	int index=0;
	
	for(int z=0; z<volBox.d; z++){
		value.z=z;
		
		for(int y=0; y<volBox.h; y++){
			value.y=y;
			
			for(int x=0; x<volBox.w; x++){
				value.x=x;
				
				row = y*volBox.w;
				page = z*volBox.w*volBox.h;
				index = x + row + page;
				
				if (_index==index){
					value= ofVec3f(x,y,z);

					_coord= value;
					_val= voxels[index];
//					ofLogVerbose("ofxVolume") << "voxelCoord= " << _coord;
//					ofLogVerbose("ofxVolume") << "voxelVal= " << _val;
					return true;
				}
			}
		}
	}
	//not found
	return false;
}


//--------------------------------------------------------------
int ofxVolume::getVoxelNumber(){
	
	int value	=0;
	for(int z=0; z<volBox.d; z++){
		if (z==axialS){
			for(int y=0; y<volBox.h; y++){
				if (y==coronalS) {
					for(int x=0; x<volBox.w; x++){
						if (x==sagittalS){
							int line = y*volBox.w;
							int page = z*volBox.w*volBox.h;
							int i = x + line + page;			// the pointer position at Array
							value= i;							// the pixel on the image
						}
					}
				}
			}
		}
	}
	
	ofLogVerbose("ofxVolume") << "voxelNumber= " << value;
	return value;
}


//--------------------------------------------------------------
void ofxVolume::redraw(slice vP, int depth)
{

	// try clamp the value like this out = MIN(volBox.h, MAX(in, 0));
	if(vP==CORONAL)
	{
		coronalS = depth;	// maybe talairch will need unclamped position??
		if(depth>=0 && depth<volBox.h)
		{
			insideCoronal=true;
			redrawCoronal();
		}else{
			insideCoronal=false;
//			coronalS = MIN(volBox.h, MAX(depth, 0));
		}
	}
	else if(vP==SAGITTAL)
	{
		sagittalS = depth;
		if(depth>=0 && depth<volBox.w)
		{
			insideSagittal=true;
			redrawSagittal();
		}else{
			insideSagittal=false;
//			sagittalS = MIN(volBox.w, MAX(depth, 0));
		}
	}
	else if (vP==AXIAL)
	{
		axialS = depth;
		if(depth>=0 && depth<volBox.d)
		{
			insideAxial=true;
			redrawAxial();
		}else{
			insideAxial=false;
//			axialS = MIN(volBox.d, MAX(depth, 0));
		}
	}
}

//--------------------------------------------------------------
void ofxVolume::draw(slice vP)
{
	drawBox();
	if(vP==CORONAL)
	{
		if(insideCoronal) coronal.draw(halfW, halfD);	// otherwise set to black (do not draw).
	}
	else if(vP==SAGITTAL)
	{
		if(insideSagittal) sagittal.draw(halfH, halfD);	// otherwise set to black (do not draw).
	}
	else if(vP==AXIAL)
	{
		if(insideAxial) axial.draw(halfW, halfH);		// otherwise set to black (do not draw).
	}
}
//--------------------------------------------------------------
void ofxVolume::drawBox()
{
	// Draw Box
	ofPushStyle();
	ofSetColor(0);
	ofRect(0, 0, boxW, boxH);
	ofPopStyle();
	ofSetColor(255);
}

//--------------------------------------------------------------
void ofxVolume::redrawCoronal()
{
	for(int z=0; z<volBox.d; z++)
	{
		for(int y=0; y<volBox.h; y++)
		{
			if (y==coronalS){
				for(int x=0; x<volBox.w; x++)
				{
					int line = y*volBox.w;
					int page = z*volBox.w*volBox.h;
					int i = x + line + page;					// the position at the pixel array
					coronalPixels[x+(z*volBox.w)] = voxels[i];	// get the correct voxel and put it to the pixel array
				}
			}
		}
	}
	
	//draw image
	coronal.setFromPixels(coronalPixels.getPixels(), volBox.w, volBox.d, OF_IMAGE_GRAYSCALE);
	coronal.mirror(true, false);
}

//--------------------------------------------------------------
void ofxVolume::redrawSagittal()
{
	for(int z=0; z<volBox.d; z++)
    {
		for(int y=0; y<volBox.h; y++)
        {
			for(int x=0; x<volBox.w; x++)
			{
				if (x==sagittalS){
					int line = y*volBox.w;
					int page = z*volBox.w*volBox.h;
					int i = x + line + page;					// the position at the pixel array
					sagittalPixels[z+(y*volBox.d)] = voxels[i];	// get the correct voxel and put it to the pixel array
				}
			}
		}
    }
	
	//draw image
	sagittal.setFromPixels(sagittalPixels.getPixels(), volBox.d, volBox.h, OF_IMAGE_GRAYSCALE);
	sagittal.rotate90(3);
}

//--------------------------------------------------------------
void ofxVolume::redrawAxial()
{
	for(int z=0; z<volBox.d; z++)
    {
		if (z==axialS){
			for(int y=0; y<volBox.h; y++)
			{
				for(int x=0; x<volBox.w; x++)
				{
					int line = y*volBox.w;
					int page = z*volBox.w*volBox.h;
					int i = x + line + page;			// the position at the pixel array
					axialPixels[x+line] = voxels[i];	// get the correct voxel and put it to the pixel array
				}
			}
		}
    }
	
	//draw image
	axial.setFromPixels(axialPixels.getPixels(), volBox.w, volBox.h, OF_IMAGE_GRAYSCALE);
}


//--------------------------------------------------------------
ofVec3f ofxVolume::getVolSize(){
	return volSize;
}
//--------------------------------------------------------------
ofVec3f ofxVolume::getVolPos(){
	return volPos;
}

//--------------------------------------------------------------
unsigned char* ofxVolume::getVoxels(){
	return voxels;
}

//--------------------------------------------------------------
void ofxVolume::clearSelected(){
	selectedVoxels.clear();
}
//--------------------------------------------------------------
void ofxVolume::selectVoxels(ofxBox& box){
	
//	selectedVoxels.pushBack(getVoxelsinBox(box));
}
//--------------------------------------------------------------
void ofxVolume::selectVoxels(vector <ofxBox>& boxes){
	
	selectedVoxels.clear();
	
	for(int i=0; i<boxes.size(); i++){
//		cout << "size = "<< boxes[i].size<<endl;
//		getVoxelsinRadius(_coord[i], _radius[i]);
		vector <ofVec3f> outputVoxels;
		outputVoxels = getVoxelsinBox(boxes[i]);
	}
}
//--------------------------------------------------------------
vector <ofVec3f> ofxVolume::getVoxelsinBox(ofxBox& box){
	
	vector <ofVec3f> outputVoxels;
	
	if (box.size==ofVec3f(0)){
		outputVoxels.push_back(box.position);
		return outputVoxels;
		cout << box.position<< " dintre \n";
	}

	ofVec3f pointer	(0);
	for(int z=0; z<box.d*2; z++){
		for(int y=0; y<box.h*2; y++){
			for(int x=0; x< box.w*2; x++){
				pointer.set(x-box.w, y-box.h, z-box.d);
				pointer+=box.position;
				outputVoxels.push_back(pointer);
				ofLogVerbose("ofxVolume") << "voxelCoord= " << pointer;
			}
		}
	}
	//not found
	return outputVoxels;
}

vector <ofVec3f> ofxVolume::getVoxelsinRadius(ofVec3f& _coord, float& _radius){

	vector <ofVec3f> outputVoxels;
	outputVoxels.push_back(_coord);
	
	ofVec3f coord	(0);
	int row			=0;
	int page		=0;
	int index		=0;

	for(int z=0; z<volBox.d; z++){
		coord.z=z;
		
		if (z==_coord.z){
			for(int y=0; y<volBox.h; y++){
				coord.y=y;
				
				if (y==_coord.y) {
					for(int x=0; x<volBox.w; x++){
						coord.x=x;
						
						if (x==_coord.x){
							row = y*volBox.w;
							page = z*volBox.w*volBox.h;
							index = x + row + page;
							
							coord= ofVec3f(x,y,z);
//							value= voxels[index];
							ofLogVerbose("ofxVolume") << "voxelCoord= " << coord;
//							ofLogVerbose("ofxVolume") << "voxelVal= " << value;
							//return true;
						}
					}
				}
			}
		}
	}
	
}

/*
 size of box is symetrical for now
 so, size (1,2,3)=
 
 x= -1 to 1;
 y= -2 to 2;
 z= -3 to 3;
 
 */



bool ofxVolume::inside(ofVec3f _coord){
	
	if (_coord.x<0) return false;
	if (_coord.y<0) return false;
	if (_coord.z<0) return false;
	
	if (_coord.x>volBox.w)	return false;
	if (_coord.y>volBox.h) return false;
	if (_coord.z>volBox.d)	return false;
	
	return true;
}


ofVec3f ofxVolume::getNormalizedCoords(ofVec3f _coord){
	
	
}


















