//  ofxVoxel.h
//  Created by Jordi.
#pragma once
#include "ofConstants.h"
#include "ofPoint.h"
#include "ofPixels.h"

//----------------------------------------------------------
// ofxVoxel
//----------------------------------------------------------

class ofxBox {
	
public:
	
	ofxBox();
	virtual ~ofxBox();
	ofxBox(float px, float py, float pz, float sw, float sh, float sd);
	ofxBox(const ofPoint& p, float w, float h, float d);
	ofxBox(const ofPoint& p, const ofPoint& s);
	ofxBox(const ofxBox& box);
	//	ofxBox(const ofPoint& p0, const ofPoint& p1);
	
	void set(float px, float py, float pz, float sw, float sh, float sd);
	void set(const ofPoint& p, float w, float h, float d);
	void set(const ofxBox& box);
	void set(const ofPoint& p, const ofPoint& s);
	//	void set(const ofPoint& p0, const ofPoint& p1);
	
	void setX(float px);
	void setY(float py);
	void setZ(float pz);
	
	void setWidth(float w);
	void setHeight(float h);
	void setDepth(float d);
	
	void setPosition(float px, float py, float pz);
	void setPosition(const ofPoint& p);
	
	void setSize(float sx, float sy, float sz);
	void setSize(const ofPoint& s);
	
	void setFromCenter(float px, float py, float pz, float w, float h, float d);
	void setFromCenter(const ofPoint& p, float w, float h, float d);
	
	void translate(float dx, float dy, float dz);
	void translate(const ofPoint& dp);
	
	void translateX(float dx);
	void translateY(float dy);
	void translateZ(float dz);
	
	void scale(float s);
	void scale(float sX, float sY, float sZ);
	void scale(const ofPoint& s);
	
	void scaleWidth(float  sX);
	void scaleHeight(float sY);
	void scaleDepth(float sZ);
	
	void scaleFromCenter(float s);
	void scaleFromCenter(float sX, float sY, float sZ);
	void scaleFromCenter(const ofPoint& s);
	/*
	 void scaleTo(const ofxBox& targetRect,
	 ofScaleMode scaleMode = OF_SCALEMODE_FIT);
	 
	 void scaleTo(const ofxBox& targetRect,
	 ofAspectRatioMode subjectAspectRatioMode,
	 ofAlignHorz sharedHorzAnchor = OF_ALIGN_HORZ_CENTER,
	 ofAlignVert sharedVertAnchor = OF_ALIGN_VERT_CENTER);
	 
	 void scaleTo(const ofxBox& targetRect,
	 ofAspectRatioMode subjectAspectRatioMode,
	 ofAlignHorz modelHorzAnchor,
	 ofAlignVert modelVertAnchor,
	 ofAlignHorz subjectHorzAnchor,
	 ofAlignVert subjectVertAnchor);
	 
	 void alignToHorz(const float& targetX,
	 ofAlignHorz thisHorzAnchor = OF_ALIGN_HORZ_CENTER);
	 
	 void alignToHorz(const ofxBox& targetRect,
	 ofAlignHorz sharedAnchor = OF_ALIGN_HORZ_CENTER);
	 
	 void alignToHorz(const ofxBox& targetRect,
	 ofAlignHorz targetHorzAnchor,
	 ofAlignHorz thisHorzAnchor);
	 
	 void alignToVert(const float& targetY,
	 ofAlignVert sharedAnchor = OF_ALIGN_VERT_CENTER);
	 
	 void alignToVert(const ofxBox& targetRect,
	 ofAlignVert sharedAnchor = OF_ALIGN_VERT_CENTER);
	 
	 void alignToVert(const ofxBox& targetRect,
	 ofAlignVert targetVertAnchor,
	 ofAlignVert thisVertAnchor);
	 
	 void alignTo(const ofPoint& targetPoint,
	 ofAlignHorz thisHorzAnchor = OF_ALIGN_HORZ_CENTER,
	 ofAlignVert thisVertAnchor = OF_ALIGN_VERT_CENTER);
	 
	 void alignTo(const ofxBox& targetRect,
	 ofAlignHorz sharedHorzAnchor = OF_ALIGN_HORZ_CENTER,
	 ofAlignVert sharedVertAnchor = OF_ALIGN_VERT_CENTER);
	 
	 void alignTo(const ofxBox& targetRect,
	 ofAlignHorz targetHorzAnchor,
	 ofAlignVert targetVertAnchor,
	 ofAlignHorz thisHorzAnchor,
	 ofAlignVert thisVertAnchor);
	 */
	bool inside(float px, float py, float pz) const;
	bool inside(const ofPoint& p) const;
	bool inside(const ofxBox& box) const;
	bool inside(const ofPoint& p0, const ofPoint& p1) const;
	
	bool intersects(const ofxBox& box) const;
	//	bool intersects(const ofPoint& p0, const ofPoint& p1) const;
	
	void growToInclude(float px, float py, float pz);
	void growToInclude(const ofPoint& p);
	void growToInclude(const ofxBox& box);
	void growToInclude(const ofPoint& p0, const ofPoint& p1);
	
	ofxBox getIntersection(const ofxBox& box) const;
	
	ofxBox getUnion(const ofxBox& box) const;
	
	void standardize();
	ofxBox getStandardized() const;
	bool isStandardized() const;  // are width/height >= 0.0f
	
	float getArea() const;
	float getVolume() const;
	//	float getAspectRatio() const;
	
	bool isEmpty() const;	 // are width/height == 0.0f
	
	ofPoint getMin() const;
	ofPoint getMax() const;
	
	float getMinX() const;
	float getMaxX() const;
	float getMinY() const;
	float getMaxY() const;
	float getMinZ() const;
	float getMaxZ() const;
	
	float getLeft()   const;
	float getRight()  const;
	float getTop()	const;
	float getBottom() const;
	float getFront()  const;
	float getBack()   const;
	
	/*	ofPoint getTopLeft() const;
	 ofPoint getTopRight() const;
	 ofPoint getBottomLeft() const;
	 ofPoint getBottomRight() const;
	 */
	float getHorzAnchor(ofAlignHorz anchor) const;
	float getVertAnchor(ofAlignVert anchor) const;
	
	ofPoint  getPosition() const;
	ofPoint& getPositionRef();
	
	ofPoint  getSize() const;
	ofPoint& getSizeRef();
	
	ofPoint getCenter() const;
	
	float getX() const;
	float getY() const;
	float getZ() const;
	float getWidth() const;
	float getHeight() const;
	float getDepth() const;
	
	ofxBox& operator = (const ofxBox& box);
	ofxBox operator + (const ofPoint& p);
	
	bool operator == (const ofxBox& box) const;
	bool operator != (const ofxBox& box) const;
	
	ofPoint position;
	float& x;
	float& y;
	float& z;
	ofPoint size;
	float& w;
	float& h;
	float& d;
};

ostream& operator<<(ostream& os, const ofxBox& box);
istream& operator>>(istream& is, ofxBox& box);

