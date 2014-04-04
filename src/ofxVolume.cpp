/*  ofxVolume - draw ofxVolumetric data
 Created by Jordi on 01/04/14.
 */

#include "ofxVolume.h"

ofxVolume::ofxVolume()
{
	//Volume
	volPos		= ofVec3f(0);
	volSize		= ofVec3f(0);
	
    volWidth	= 0;
    volHeight	= 0;
    volDepth	= 0;
}
//--------------------------------------------------------------
ofxVolume::~ofxVolume()
{

}

void ofxVolume::load(string path)
{
	imageSequence.init(path + "IM-0001-0", 3, ".tif", 0);
	
	// calculate ofxVolume size
	volWidth	= imageSequence.getWidth();
    volHeight	= imageSequence.getHeight();
    volDepth	= imageSequence.getSequenceLength();
	volSize		= ofVec3f(volWidth, volHeight, volDepth);
	
	ofLogNotice("vizManager") << "setting up ofxVolume data buffer at " << volWidth << "x" << volHeight << "x" << volDepth;
    voxels = new unsigned char[volWidth*volHeight*volDepth];
	
	//fill out the array pixel in white for easy debugging
	for (int i=0; i<volWidth*volHeight*volDepth; i++ )
	{
		voxels[i]= (unsigned char) 255;
	}
	
	// fill my array with pixels
    for(int z=0; z<volDepth; z++)
    {
        imageSequence.loadFrame(z);
		int gradient = 0;
		for(int y=0; y<volHeight; y++)
        {
			for(int x=0; x<volWidth; x++)
			{
				if (x<volWidth && y<volHeight)
				{																// get values from image
					int i = ((x + volWidth*y) + z*volWidth*volHeight);			// the pointer position at Array
					int sample = imageSequence.getPixels()[x+y*volWidth];		// the pixel on the image
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
	volWidth	= size.x;
    volHeight	= size.y;
    volDepth	= size.z;
	volSize		= size;
	
	ofLogNotice("vizManager") << "setting up ofxVolume data buffer at " << volWidth << "x" << volHeight << "x" << volDepth;
    voxels = new unsigned char[volWidth*volHeight*volDepth*4];
	
	/*
	//fill out the array pixel in white for easy debugging
	for (int i=0; i<volWidth*volHeight*volDepth*4; i++ )
	{
		voxels[i]= (unsigned char) 55;
	}
	*/
	
/*
	int max = 255;
	ofColor myColor = ofColor(0,0,0,255);
	// fill my array with pixels
    for(int z=0; z<volDepth; z++)
    {
		myColor= ofColor(myColor.r, myColor.g, z/volDepth*max, myColor.a);
		for(int y=0; y<volHeight; y++)
        {
			myColor= ofColor(myColor.r, y/volHeight*max, myColor.b, myColor.a);
			for(int x=0; x<volWidth; x++)
			{
				myColor= ofColor(y/volWidth*max, myColor.g, myColor.b, myColor.a);

				int i = ((x + volWidth*y) + z*volWidth*volHeight)*4;			// the pointer position at Array
//				int sample = imageSequence.getPixels()[x+y*volWidth];		// the pixel on the image
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
	halfH = (boxW - volHeight) /2;
	halfW = (boxW - volWidth) /2;
	halfD = (boxW - volDepth) /2;
	
	//allocate my pixls type of the image slices
	coronalPixels.allocate(volWidth, volDepth, OF_IMAGE_GRAYSCALE);
	coronalPixels.set(255);
	sagittalPixels.allocate(volDepth, volHeight, OF_IMAGE_GRAYSCALE);
	sagittalPixels.set(255);
	axialPixels.allocate(volWidth, volHeight, OF_IMAGE_GRAYSCALE);
	axialPixels.set(255);	
}

//--------------------------------------------------------------
int ofxVolume::getVoxelValue(){
	
	int value	=0;
	for(int z=0; z<volDepth; z++){
		if (z==axialS){
			for(int y=0; y<volHeight; y++){
				if (y==coronalS) {
					for(int x=0; x<volWidth; x++){
						if (x==sagittalS){
							int line = y*volWidth;
							int page = z*volWidth*volHeight;
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
	for(int z=0; z<volDepth; z++){
		value.z=z;
		if (z==axialS){
			for(int y=0; y<volHeight; y++){
				value.y=y;
				if (y==coronalS) {
					for(int x=0; x<volWidth; x++){
						value.x=x;

						int line = y*volWidth;
						int page = z*volWidth*volHeight;
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
	
	for(int z=0; z<volDepth; z++){
		value.z=z;
		
		for(int y=0; y<volHeight; y++){
			value.y=y;
			
			for(int x=0; x<volWidth; x++){
				value.x=x;
				
				row = y*volWidth;
				page = z*volWidth*volHeight;
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
	for(int z=0; z<volDepth; z++){
		if (z==axialS){
			for(int y=0; y<volHeight; y++){
				if (y==coronalS) {
					for(int x=0; x<volWidth; x++){
						if (x==sagittalS){
							int line = y*volWidth;
							int page = z*volWidth*volHeight;
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
void ofxVolume::redraw(viewPoint vP, int depth)
{

	// try clamp the value like this out = MIN(volHeight, MAX(in, 0));
	if(vP==CORONAL)
	{
		coronalS = depth;	// maybe talairch will need unclamped position??
		if(depth>=0 && depth<volHeight)
		{
			insideCoronal=true;
			redrawCoronal();
		}else{
			insideCoronal=false;
//			coronalS = MIN(volHeight, MAX(depth, 0));
		}
	}
	else if(vP==SAGITTAL)
	{
		sagittalS = depth;
		if(depth>=0 && depth<volWidth)
		{
			insideSagittal=true;
			redrawSagittal();
		}else{
			insideSagittal=false;
//			sagittalS = MIN(volWidth, MAX(depth, 0));
		}
	}
	else if (vP==AXIAL)
	{
		axialS = depth;
		if(depth>=0 && depth<volDepth)
		{
			insideAxial=true;
			redrawAxial();
		}else{
			insideAxial=false;
//			axialS = MIN(volDepth, MAX(depth, 0));
		}
	}
}

//--------------------------------------------------------------
void ofxVolume::draw(viewPoint vP)
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
	for(int z=0; z<volDepth; z++)
	{
		for(int y=0; y<volHeight; y++)
		{
			if (y==coronalS){
				for(int x=0; x<volWidth; x++)
				{
					int line = y*volWidth;
					int page = z*volWidth*volHeight;
					int i = x + line + page;					// the position at the pixel array
					coronalPixels[x+(z*volWidth)] = voxels[i];	// get the correct voxel and put it to the pixel array
				}
			}
		}
	}
	
	//draw image
	coronal.setFromPixels(coronalPixels.getPixels(), volWidth, volDepth, OF_IMAGE_GRAYSCALE);
	coronal.mirror(true, false);
}

//--------------------------------------------------------------
void ofxVolume::redrawSagittal()
{
	for(int z=0; z<volDepth; z++)
    {
		for(int y=0; y<volHeight; y++)
        {
			for(int x=0; x<volWidth; x++)
			{
				if (x==sagittalS){
					int line = y*volWidth;
					int page = z*volWidth*volHeight;
					int i = x + line + page;					// the position at the pixel array
					sagittalPixels[z+(y*volDepth)] = voxels[i];	// get the correct voxel and put it to the pixel array
				}
			}
		}
    }
	
	//draw image
	sagittal.setFromPixels(sagittalPixels.getPixels(), volDepth, volHeight, OF_IMAGE_GRAYSCALE);
	sagittal.rotate90(3);
}

//--------------------------------------------------------------
void ofxVolume::redrawAxial()
{
	for(int z=0; z<volDepth; z++)
    {
		if (z==axialS){
			for(int y=0; y<volHeight; y++)
			{
				for(int x=0; x<volWidth; x++)
				{
					int line = y*volWidth;
					int page = z*volWidth*volHeight;
					int i = x + line + page;			// the position at the pixel array
					axialPixels[x+line] = voxels[i];	// get the correct voxel and put it to the pixel array
				}
			}
		}
    }
	
	//draw image
	axial.setFromPixels(axialPixels.getPixels(), volWidth, volHeight, OF_IMAGE_GRAYSCALE);
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
//vector<unsigned char> ofxVolume::selectVoxels(vector <ofVec3f> _coord, vector <float> _radius){
void ofxVolume::selectVoxels(vector <ofVec3f>& _coord, vector <float>& _radius){
	
	selectedVoxels.clear();
//	cout << "voxelCoord= " << _coord[0];
	
	
	for(int i=0; i<_coord.size(); i++){
		
		ofVec3f size= ofVec3f(_radius[i], _radius[i], _radius[i]);
		
//		getVoxelsinRadius(_coord[i], _radius[i]);
		
		vector <ofVec3f> outputVoxels;
		
		outputVoxels = getVoxelsinBox(_coord[i], size);
		for(int i=0; i<outputVoxels.size(); i++){
			outputVoxels[i];
			cout << "outputVoxels= " << outputVoxels[i] << endl;
		}
	/*
		ofVec3f coord	(0);
		int row			=0;
		int page		=0;
		int index		=0;
		unsigned char value	=0;
		
		for(int z=0; z<volDepth; z++){
			coord.z=z;
			
			if (z==_coord[i].z){
			for(int y=0; y<volHeight; y++){
				coord.y=y;
				
				if (y==_coord[i].y) {
				for(int x=0; x<volWidth; x++){
					coord.x=x;
					
					if (x==_coord[i].x){
						row = y*volWidth;
						page = z*volWidth*volHeight;
						index = x + row + page;

						coord= ofVec3f(x,y,z);
						value= voxels[index];
						ofLogVerbose("ofxVolume") << "voxelCoord= " << coord;
						ofLogVerbose("ofxVolume") << "voxelVal= " << value;
						//return true;
					}
				}
				}
			}
			}
		}
		//not found
		//return false;
*/
	}
//	return selectedVoxels;
}

vector <ofVec3f> ofxVolume::getVoxelsinRadius(ofVec3f& _coord, float& _radius){

	vector <ofVec3f> outputVoxels;
	outputVoxels.push_back(_coord);
	
	ofVec3f coord	(0);
	int row			=0;
	int page		=0;
	int index		=0;

	
	for(int z=0; z<volDepth; z++){
		coord.z=z;
		
		if (z==_coord.z){
			for(int y=0; y<volHeight; y++){
				coord.y=y;
				
				if (y==_coord.y) {
					for(int x=0; x<volWidth; x++){
						coord.x=x;
						
						if (x==_coord.x){
							row = y*volWidth;
							page = z*volWidth*volHeight;
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
vector <ofVec3f> ofxVolume::getVoxelsinBox(ofVec3f& _coord, ofVec3f& _size){
	
	vector <ofVec3f> outputVoxels;
	outputVoxels.push_back(_coord);
	
	if (_size==ofVec3f(0)) return outputVoxels;
		
	
	
}



bool ofxVolume::inside(ofVec3f _coord){

	ofRectangle rect;
	rect.inside(<#float px#>, <#float py#>)
	
	if (_coord.x<0) return false;
	if (_coord.y<0) return false;
	if (_coord.z<0) return false;
	
	if (_coord.x>volWidth)	return false;
	if (_coord.y>volHeight) return false;
	if (_coord.z>volDepth)	return false;
	
	return true;
}


ofVec3f ofxVolume::getNormalizedCoords(ofVec3f _coord){
	
	
}


















