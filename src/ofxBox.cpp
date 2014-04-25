#include <cfloat>
#include "ofxBox.h"

//----------------------------------------------------------
ofxBox::ofxBox()
: x(position.x), y(position.y), z(position.z),
w(size.x), h(size.y), d(size.z){
	set(0,0,0,0,0,0);
}

//----------------------------------------------------------
ofxBox::~ ofxBox(){}

//----------------------------------------------------------
ofxBox::ofxBox(float px, float py, float pz, float sw, float sh, float sd)
: x(position.x), y(position.y), z(position.z),
w(size.x), h(size.y), d(size.z){
	set(px,py,pz,sw,sh,sd);
}

//----------------------------------------------------------
ofxBox::ofxBox(const ofPoint& p, float sw, float sh, float sd)
: x(position.x), y(position.y), z(position.z),
w(size.x), h(size.y), d(size.z){
	set(p,sw,sh,sd);
}

//----------------------------------------------------------
ofxBox::ofxBox(const ofxBox& box)
: x(position.x), y(position.y), z(position.z),
w(size.x), h(size.y), d(size.z){
	set(box);
}

//----------------------------------------------------------
ofxBox::ofxBox(const ofPoint& p, const ofPoint& s)
: x(position.x), y(position.y), z(position.z),
w(size.x), h(size.y), d(size.z){
	set(p,s);
}
/*
//----------------------------------------------------------
ofxBox::ofxBox(const ofPoint& p0, const ofPoint& p1)
: x(position.x), y(position.y), z(position.z),
w(size.x), h(size.y), d(size.z){
	set(p0,p1);
}
*/
//----------------------------------------------------------
void ofxBox::set(float px, float py, float pz, float sw, float sh, float sd) {
	x		= px;
	y		= py;
	z		= pz;
	w		= sw;
	h		= sh;
	d		= sd;
}

//----------------------------------------------------------
void ofxBox::set(const ofPoint& p, float sw, float sh, float sd) {
	set(p.x, p.y, p.z, sw, sh, sd);
}

//----------------------------------------------------------
void ofxBox::set(const ofxBox& box){
	set(box.x, box.y, box.z, box.w, box.h, box.d);
}

//----------------------------------------------------------
void ofxBox::set(const ofPoint& p, const ofPoint& s) {
	set(p.x,p.y,p.z,s.x,s.y,s.z);
}
/*
//----------------------------------------------------------
void ofxBox::set(const ofPoint& p0, const ofPoint& p1) {
	float x0,y0,x1,y1,z0,z1;
	
	x0 = MIN(p0.x, p1.x);
	x1 = MAX(p0.x, p1.x);
	y0 = MIN(p0.y, p1.y);
	y1 = MAX(p0.y, p1.y);
	z0 = MIN(p0.z, p1.z);
	z1 = MAX(p0.z, p1.z);
	
	float w = x1 - x0;
	float h = y1 - y0;
	float d = z1 - z0;
	
	set(x0,y0,z0,w,h,d);
}
*/
//----------------------------------------------------------
void ofxBox::setX(float px) {
	x = px;
}

//----------------------------------------------------------
void ofxBox::setY(float py) {
	y = py;
}

//----------------------------------------------------------
void ofxBox::setZ(float pz) {
	z = pz;
}

//----------------------------------------------------------
void ofxBox::setWidth(float w) {
	w = w;
}

//----------------------------------------------------------
void ofxBox::setHeight(float h) {
	h = h;
}

//----------------------------------------------------------
void ofxBox::setDepth(float d) {
	d = d;
}

//----------------------------------------------------------
void ofxBox::setPosition(float px, float py, float pz) {
	position.x = px;
	position.y = py;
	position.z = pz;
}

//----------------------------------------------------------
void ofxBox::setPosition(const ofPoint& p) {
	position = p;
}

//----------------------------------------------------------
void ofxBox::setSize(float sx, float sy, float sz) {
	size.x = sx;
	size.y = sy;
	size.z = sz;
}

//----------------------------------------------------------
void ofxBox::setSize(const ofPoint& s) {
	size = s;
}

//----------------------------------------------------------
void ofxBox::setFromCenter(float px, float py, float pz, float sw, float sh, float sd) {
	set(px - w*0.5f, py - h*0.5f, pz - d*0.5f, sw, sh,sd);
}

//----------------------------------------------------------
void ofxBox::setFromCenter(const ofPoint& p, float sw, float sh, float sd) {
	setFromCenter(p.x, p.y, p.z, sw, sh, sd);
}

//----------------------------------------------------------
void ofxBox::translate(float dx, float dy, float dz) {
	translateX(dx);
	translateY(dy);
	translateZ(dz);
}

//----------------------------------------------------------
void ofxBox::translate(const ofPoint& dp) {
	translateX(dp.x);
	translateY(dp.y);
	translateZ(dp.z);
}

//----------------------------------------------------------
void ofxBox::translateX(float dx) {
	x += dx;
}

//----------------------------------------------------------
void ofxBox::translateY(float dy) {
	y += dy;
}

//----------------------------------------------------------
void ofxBox::translateZ(float dz) {
	z += dz;
}

//----------------------------------------------------------
void ofxBox::scale(float s) {
	scaleWidth(s);
	scaleHeight(s);
	scaleDepth(s);
}

//----------------------------------------------------------
void ofxBox::scale(float sX, float sY, float sZ) {
	scaleWidth(sX);
	scaleHeight(sY);
	scaleDepth(sZ);
}

//----------------------------------------------------------
void ofxBox::scale(const ofPoint& s) {
	scaleWidth(s.x);
	scaleHeight(s.y);
}

//----------------------------------------------------------
void ofxBox::scaleWidth(float  sX) {
	w  *= sX;
}
//----------------------------------------------------------
void ofxBox::scaleHeight(float sY) {
	h *= sY;
}
//----------------------------------------------------------
void ofxBox::scaleDepth(float sZ) {
	d *= sZ;
}

//----------------------------------------------------------
void ofxBox::scaleFromCenter(float s) {
	scaleFromCenter(s,s,s);
}

//----------------------------------------------------------
void ofxBox::scaleFromCenter(float sX, float sY, float sZ) {
	scaleFromCenter(ofPoint(sX,sY,sZ));
}

//----------------------------------------------------------
void ofxBox::scaleFromCenter(const ofPoint& s) {
	if(s.x == 1.0f && s.y == 1.0f && s.z == 1.0f) return; // nothing to do
	
	float newWidth  = w  * s.x;
	float newHeight = h * s.y;
	float newDepth = d * s.z;
	
	ofPoint center = getCenter();
	
	x = center.x - newWidth  / 2.0f;
	y = center.y - newHeight / 2.0f;
	z = center.z - newDepth / 2.0f;
	
	w  = newWidth;
	h = newHeight;
	d = newDepth;
}

/* //TODO
 //----------------------------------------------------------
 void ofxBox::scaleTo(const ofxBox& targetRect,
 ofScaleMode scaleMode) {
 
 if(scaleMode == OF_SCALEMODE_FIT) {
 scaleTo(targetRect,
 OF_ASPECT_RATIO_KEEP,
 OF_ALIGN_HORZ_CENTER,
 OF_ALIGN_VERT_CENTER);
 } else if(scaleMode == OF_SCALEMODE_FILL) {
 scaleTo(targetRect,
 OF_ASPECT_RATIO_KEEP_BY_EXPANDING,
 OF_ALIGN_HORZ_CENTER,
 OF_ALIGN_VERT_CENTER);
 } else if(scaleMode == OF_SCALEMODE_CENTER) {
 alignTo(targetRect,
 OF_ALIGN_HORZ_CENTER,
 OF_ALIGN_VERT_CENTER);
 } else if(scaleMode == OF_SCALEMODE_STRETCH_TO_FILL) {
 scaleTo(targetRect,
 OF_ASPECT_RATIO_IGNORE,
 OF_ALIGN_HORZ_CENTER,
 OF_ALIGN_VERT_CENTER);
 } else {
 scaleTo(targetRect,
 OF_ASPECT_RATIO_KEEP);
 }
 
 }
 
 //----------------------------------------------------------
 void ofxBox::scaleTo(const ofxBox& targetRect,
 ofAspectRatioMode subjectAspectRatioMode,
 ofAlignHorz sharedHorzAnchor,
 ofAlignVert sharedVertAnchor) {
 scaleTo(targetRect,
 subjectAspectRatioMode,
 sharedHorzAnchor,
 sharedVertAnchor,
 sharedHorzAnchor,
 sharedVertAnchor);
 }
 
 //----------------------------------------------------------
 void ofxBox::scaleTo(const ofxBox& targetRect,
 ofAspectRatioMode aspectRatioMode,
 ofAlignHorz modelHorzAnchor,
 ofAlignVert modelVertAnchor,
 ofAlignHorz thisHorzAnchor,
 ofAlignVert thisVertAnchor) {
 
 float tw = targetRect.getWidth();	// target w
 float th = targetRect.getHeight();   // target h
 float sw = getWidth();   // subject w
 float sh = getHeight();  // subject h
 
 if(aspectRatioMode == OF_ASPECT_RATIO_KEEP_BY_EXPANDING ||
 aspectRatioMode == OF_ASPECT_RATIO_KEEP) {
 if(fabs(sw) >= FLT_EPSILON || fabs(sh) >= FLT_EPSILON) {
 float wRatio = fabs(tw) / fabs(sw);
 float hRatio = fabs(th) / fabs(sh);
 if(aspectRatioMode == OF_ASPECT_RATIO_KEEP_BY_EXPANDING) {
 scale(MAX(wRatio,hRatio));
 } else if(aspectRatioMode == OF_ASPECT_RATIO_KEEP) {
 scale(MIN(wRatio,hRatio));
 }
 } else {
 ofLogWarning("ofxBox") << "scaleTo(): no scaling applied to avoid divide by zero, box has 0 w and/or h: " << sw << "x" << sh;
 }
 } else if(aspectRatioMode == OF_ASPECT_RATIO_IGNORE) {
 w  = tw;
 h = th;
 } else {
 ofLogWarning("ofxBox") << "scaleTo(): unknown ofAspectRatioMode = " << aspectRatioMode << ", using OF_ASPECT_RATIO_IGNORE";
 w  = tw;
 h = th;
 }
 
 // now align if anchors are not ignored.
 alignTo(targetRect,
 modelHorzAnchor,
 modelVertAnchor,
 thisHorzAnchor,
 thisVertAnchor);
 }
 
 //----------------------------------------------------------
 void ofxBox::alignToHorz(const float& targetX,
 ofAlignHorz thisHorzAnchor) {
 
 if(thisHorzAnchor != OF_ALIGN_HORZ_IGNORE) {
 translateX(targetX - getHorzAnchor(thisHorzAnchor));
 } else {
 ofLogVerbose("ofxBox") << "alignToHorz(): thisHorzAnchor == OF_ALIGN_HORZ_IGNORE, no alignment applied";
 }
 }
 
 //----------------------------------------------------------
 void ofxBox::alignToHorz(const ofxBox& targetRect,
 ofAlignHorz sharedAnchor) {
 
 alignToHorz(targetRect, sharedAnchor, sharedAnchor);
 }
 
 //----------------------------------------------------------
 void ofxBox::alignToHorz(const ofxBox& targetRect,
 ofAlignHorz targetHorzAnchor,
 ofAlignHorz thisHorzAnchor) {
 
 if(targetHorzAnchor != OF_ALIGN_HORZ_IGNORE &&
 thisHorzAnchor   != OF_ALIGN_HORZ_IGNORE) {
 alignToHorz(targetRect.getHorzAnchor(targetHorzAnchor),thisHorzAnchor);
 } else {
 if(targetHorzAnchor == OF_ALIGN_HORZ_IGNORE) {
 ofLogVerbose("ofxBox") << "alignToHorz(): targetHorzAnchor == OF_ALIGN_HORZ_IGNORE, no alignment applied";
 } else {
 ofLogVerbose("ofxBox") << "alignToHorz(): thisHorzAnchor == OF_ALIGN_HORZ_IGNORE, no alignment applied";
 }
 }
 
 }
 
 //----------------------------------------------------------
 void ofxBox::alignToVert(const float& targetY,
 ofAlignVert thisVertAnchor) {
 
 if(thisVertAnchor != OF_ALIGN_VERT_IGNORE) {
 translateY(targetY - getVertAnchor(thisVertAnchor));
 } else {
 ofLogVerbose("ofxBox") << "alignToVert(): thisVertAnchor == OF_ALIGN_VERT_IGNORE, no alignment applied";
 }
 }
 
 //----------------------------------------------------------
 void ofxBox::alignToVert(const ofxBox& targetRect,
 ofAlignVert sharedAnchor) {
 
 alignToVert(targetRect,sharedAnchor,sharedAnchor);
 }
 
 //----------------------------------------------------------
 void ofxBox::alignToVert(const ofxBox& targetRect,
 ofAlignVert targetVertAnchor,
 ofAlignVert thisVertAnchor) {
 
 if(targetVertAnchor != OF_ALIGN_VERT_IGNORE &&
 thisVertAnchor   != OF_ALIGN_VERT_IGNORE) {
 alignToVert(targetRect.getVertAnchor(targetVertAnchor),thisVertAnchor);
 } else {
 if(targetVertAnchor == OF_ALIGN_VERT_IGNORE) {
 ofLogVerbose("ofxBox") << "alignToVert(): targetVertAnchor == OF_ALIGN_VERT_IGNORE, no alignment applied";
 } else {
 ofLogVerbose("ofxBox") << "alignToVert(): thisVertAnchor == OF_ALIGN_VERT_IGNORE, no alignment applied";
 }
 
 }
 }
 
 //----------------------------------------------------------
 void ofxBox::alignTo(const ofPoint& targetPoint,
 ofAlignHorz thisHorzAnchor,
 ofAlignVert thisVertAnchor) {
 
 alignToHorz(targetPoint.x, thisHorzAnchor);
 alignToVert(targetPoint.y, thisVertAnchor);
 }
 
 
 //----------------------------------------------------------
 void ofxBox::alignTo(const ofxBox& targetRect,
 ofAlignHorz sharedHorzAnchor,
 ofAlignVert sharedVertAnchor) {
 alignTo(targetRect,
 sharedHorzAnchor,
 sharedVertAnchor,
 sharedHorzAnchor,
 sharedVertAnchor);
 }
 
 //----------------------------------------------------------
 void ofxBox::alignTo(const ofxBox& targetRect,
 ofAlignHorz targetHorzAnchor,
 ofAlignVert targetVertAnchor,
 ofAlignHorz thisHorzAnchor,
 ofAlignVert thisVertAnchor) {
 
 alignToHorz(targetRect,targetHorzAnchor,thisHorzAnchor);
 alignToVert(targetRect,targetVertAnchor,thisVertAnchor);
 }
 */

//----------------------------------------------------------
bool ofxBox::inside(float px, float py, float pz) const {
	return inside(ofPoint(px,py,pz));
}

//----------------------------------------------------------
bool ofxBox::inside(const ofPoint& p) const {
	return	p.x > getMinX() && p.y > getMinY() && p.z > getMinZ() &&
	p.x < getMaxX() && p.y < getMaxY() && p.z < getMaxZ();
}

//----------------------------------------------------------
bool ofxBox::inside(const ofxBox& box) const {
	return	inside(box.getMinX(),box.getMinY(),box.getMinZ()) &&
	inside(box.getMaxX(),box.getMaxY(),box.getMaxZ());
}

//----------------------------------------------------------
bool ofxBox::inside(const ofPoint& p0, const ofPoint& p1) const {
	// check to see if a line segment is inside the box
	return inside(p0) && inside(p1);
}

//----------------------------------------------------------
bool ofxBox::intersects(const ofxBox& box) const {
	return (getMinX() < box.getMaxX() && getMaxX() > box.getMinX() &&
			getMinY() < box.getMaxY() && getMaxY() > box.getMinY() &&
			getMinZ() < box.getMaxZ() && getMaxZ() > box.getMinZ());
}

/* TODO
 //----------------------------------------------------------
 bool ofxBox::intersects(const ofPoint& p0, const ofPoint& p1) const {
 // check for a line intersection
 ofPoint p;
 
 ofPoint topLeft	 = getTopLeft();
 ofPoint topRight	= getTopRight();
 ofPoint bottomRight = getBottomRight();
 ofPoint bottomLeft  = getBottomLeft();
 
 return inside(p0) || // check end inside
 inside(p1) || // check end inside
 ofLineSegmentIntersection(p0, p1, topLeft,	 topRight,	p) || // cross top
 ofLineSegmentIntersection(p0, p1, topRight,	bottomRight, p) || // cross right
 ofLineSegmentIntersection(p0, p1, bottomRight, bottomLeft,  p) || // cross bottom
 ofLineSegmentIntersection(p0, p1, bottomLeft,  topLeft,	 p);   // cross left
 }
 */

//----------------------------------------------------------
void ofxBox::growToInclude(float px, float py, float pz) {
	growToInclude(ofPoint(px,py,pz));
}

//----------------------------------------------------------
void ofxBox::growToInclude(const ofPoint& p) {
	float x0 = MIN(getMinX(),p.x);
	float x1 = MAX(getMaxX(),p.x);
	float y0 = MIN(getMinY(),p.y);
	float y1 = MAX(getMaxY(),p.y);
	float z0 = MIN(getMinY(),p.z);
	float z1 = MAX(getMaxY(),p.z);
	float w = x1 - x0;
	float h = y1 - y0;
	float d = z1 - z0;
	set(x0,y0,z0,w,h,d);
}

//----------------------------------------------------------
void ofxBox::growToInclude(const ofxBox& box) {
	float x0 = MIN(getMinX(),box.getMinX());
	float x1 = MAX(getMaxX(),box.getMaxX());
	float y0 = MIN(getMinY(),box.getMinY());
	float y1 = MAX(getMaxY(),box.getMaxY());
	float z0 = MIN(getMinZ(),box.getMinZ());
	float z1 = MAX(getMaxZ(),box.getMaxZ());
	float w = x1 - x0;
	float h = y1 - y0;
	float d = z1 - z0;
	set(x0,y0,z0,w,h,d);
}

//----------------------------------------------------------
void ofxBox::growToInclude(const ofPoint& p0, const ofPoint& p1) {
	growToInclude(p0);
	growToInclude(p1);
}

//----------------------------------------------------------
ofxBox ofxBox::getIntersection(const ofxBox& box) const {
	
	float x0 = MAX(getMinX(),box.getMinX());
	float x1 = MIN(getMaxX(),box.getMaxX());
	
	float w = x1 - x0;
	if(w < 0.0f) return ofxBox(0,0,0,0,0,0); // short circuit if needed
	
	float y0 = MAX(getMinY(),box.getMinY());
	float y1 = MIN(getMaxY(),box.getMaxY());
	
	float h = y1 - y0;
	if(h < 0.0f) return ofxBox(0,0,0,0,0,0);  // short circuit if needed
	
	float z0 = MAX(getMinZ(),box.getMinZ());
	float z1 = MIN(getMaxZ(),box.getMaxZ());
	
	float d = z1 - z0;
	if(d < 0.0f) return ofxBox(0,0,0,0,0,0);  // short circuit if needed
	
	return ofxBox(x0,y0,z0,w,h,d);
}

//----------------------------------------------------------
ofxBox ofxBox::getUnion(const ofxBox& box) const {
	ofxBox united = *this;
	united.growToInclude(box);
	return united;
}

//----------------------------------------------------------
void ofxBox::standardize() {
	if(w < 0.0f) {
		x += w;
		w = -1.0f * w;
	}
	
	if(h < 0.0f) {
		y += h;
		h = -1.0f * h;
	}
	
	if(d < 0.0f) {
		z += d;
		d = -1.0f * d;
	}
	
}

//----------------------------------------------------------
ofxBox ofxBox::getStandardized() const {
	if(isStandardized()) {
		return *this;
	} else {
		ofxBox canRect(*this); // copy it
		canRect.standardize();
		return canRect;
	}
}

//----------------------------------------------------------
bool ofxBox::isStandardized() const {
	return w >= 0.0f && h >= 0.0f && d >= 0.0f;
}

//----------------------------------------------------------
float ofxBox::getArea() const {
	return 2.0f * (fabs(w) * fabs(h) +
				   fabs(h) * fabs(d) +
				   fabs(w) * fabs(d));
}

//----------------------------------------------------------
float ofxBox::getVolume() const {
	return fabs(w) * fabs(h) * fabs(d);
}
/*
 //----------------------------------------------------------
 float ofxBox::getAspectRatio() const {
 return fabs(w) / fabs(h);
 }
 */
//----------------------------------------------------------
bool ofxBox::isEmpty() const {
	return w == 0.0f && h == 0.0f && d == 0.0f;
}

//----------------------------------------------------------
ofPoint ofxBox::getMin() const {
	return ofPoint(getMinX(),getMinY(),getMinZ());
}

//----------------------------------------------------------
ofPoint ofxBox::getMax() const {
	return ofPoint(getMaxX(),getMaxY(),getMaxZ());
}

//----------------------------------------------------------
float ofxBox::getMinX() const {
	return MIN(x, x + w);  // - w
}
//----------------------------------------------------------
float ofxBox::getMaxX() const {
	return MAX(x, x + w);  // - w
}
//----------------------------------------------------------
float ofxBox::getMinY() const{
	return MIN(y, y + h);  // - h
}
//----------------------------------------------------------
float ofxBox::getMaxY() const {
	return MAX(y, y + h);  // - h
}
//----------------------------------------------------------
float ofxBox::getMinZ() const{
	return MIN(z, z + d);  // - d
}
//----------------------------------------------------------
float ofxBox::getMaxZ() const {
	return MAX(z, z + d);  // - d
}

//----------------------------------------------------------
float ofxBox::getLeft() const {
	return getMinX();
}
//----------------------------------------------------------
float ofxBox::getRight() const {
	return getMaxX();
}
//----------------------------------------------------------
float ofxBox::getTop() const {
	return getMinY();
}
//----------------------------------------------------------
float ofxBox::getBottom() const {
	return getMaxY();
}
//----------------------------------------------------------
float ofxBox::getFront() const {
	return getMinZ();
}
//----------------------------------------------------------
float ofxBox::getBack() const {
	return getMaxZ();
}
/*
 //----------------------------------------------------------
 ofPoint ofxBox::getTopLeft() const {
 return getMin();
 }
 //----------------------------------------------------------
 ofPoint ofxBox::getTopRight() const {
 return ofPoint(getRight(),getTop());
 }
 //----------------------------------------------------------
 ofPoint ofxBox::getBottomLeft() const {
 return ofPoint(getLeft(),getBottom());
 }
 //----------------------------------------------------------
 ofPoint ofxBox::getBottomRight() const {
 return getMax();
 }
 
 //----------------------------------------------------------
 float ofxBox::getHorzAnchor(ofAlignHorz anchor) const {
 switch (anchor) {
 case OF_ALIGN_HORZ_IGNORE:
 ofLogError("ofxBox") << "getHorzAnchor(): unable to get anchor for OF_ALIGN_HORZ_IGNORE, returning 0";
 return 0.0f;
 case OF_ALIGN_HORZ_LEFT:
 return getLeft();
 case OF_ALIGN_HORZ_RIGHT:
 return getRight();
 case OF_ALIGN_HORZ_CENTER:
 return getCenter().x;
 default:
 ofLogError("ofxBox") << "getHorzAnchor(): unknown ofAlignHorz = " << anchor << ", returning 0.0";
 return 0.0f;
 }
 }
 
 //----------------------------------------------------------
 float ofxBox::getVertAnchor(ofAlignVert anchor) const {
 switch (anchor) {
 case OF_ALIGN_VERT_IGNORE:
 ofLogError("ofxBox") << "getVertAnchor(): unable to get anchor for OF_ALIGN_VERT_IGNORE, returning 0.0";
 return 0.0f;
 case OF_ALIGN_VERT_TOP:
 return getTop();
 case OF_ALIGN_VERT_BOTTOM:
 return getBottom();
 case OF_ALIGN_VERT_CENTER:
 return getCenter().y;
 default:
 ofLogError("ofxBox") << "getVertAnchor(): unknown ofAlignVert = " << anchor << ", returning 0.0";
 return 0.0f;
 }
 }
 */

//----------------------------------------------------------
bool ofxBox::operator != (const ofxBox& box) const {
	return	(x != box.x) || (y != box.y) || (z != box.z) ||
	(w != box.w) || (h != box.h) || (d != box.d);
}

//----------------------------------------------------------
ofPoint ofxBox::getPosition() const {
	return position;
}
//----------------------------------------------------------
ofPoint& ofxBox::getPositionRef() {
	return position;
}
//----------------------------------------------------------
ofPoint ofxBox::getSize() const {
	return size;
}
//----------------------------------------------------------
ofPoint& ofxBox::getSizeRef() {
	return size;
}

//----------------------------------------------------------
ofPoint ofxBox::getCenter() const {
	return ofPoint(x + w * 0.5f, y + h * 0.5f, z + d * 0.5f);
}

//----------------------------------------------------------
float ofxBox::getX() const {
	return x;
}
//----------------------------------------------------------
float ofxBox::getY() const {
	return y;
}
//----------------------------------------------------------
float ofxBox::getZ() const {
	return z;
}
//----------------------------------------------------------
float ofxBox::getWidth() const {
	return w;
}
//----------------------------------------------------------
float ofxBox::getHeight() const {
	return h;
}
//----------------------------------------------------------
float ofxBox::getDepth() const {
	return d;
}

//----------------------------------------------------------
ofxBox& ofxBox::operator = (const ofxBox& box) {
	set(box);
	return *this;
}

//----------------------------------------------------------
ofxBox ofxBox::operator + (const ofPoint & point){
	ofxBox box=*this;
	box.x += point.x;
	box.y += point.y;
	box.z += point.z;
	return box;
}

//----------------------------------------------------------
bool ofxBox::operator == (const ofxBox& box) const {
	return	(x == box.x) && (y == box.y) && (z == box.z) &&
	(w == box.w) && (h == box.h) && (d == box.d);
}

//----------------------------------------------------------
ostream& operator<<(ostream& os, const ofxBox& box){
	os << box.position << ", " << box.w << ", " << box.h << ", " << box.d;
	return os;
}

//----------------------------------------------------------
istream& operator>>(istream& is, ofxBox& box){
	is >> box.position;
	is.ignore(2);
	is >> box.w;
	is.ignore(2);
	is >> box.h;
	is.ignore(2);
	is >> box.d;
	return is;
}
