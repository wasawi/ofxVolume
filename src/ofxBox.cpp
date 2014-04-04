//
//  ofxBox.cpp
//
//  Created by Jordi on 04/04/14.
//
//

#include <cfloat>
#include "ofxBox.h"

//----------------------------------------------------------
ofxBox::ofxBox() : x(position.x), y(position.y) {
    set(0,0,0,0);
}

//----------------------------------------------------------
ofxBox::~ ofxBox(){}

//----------------------------------------------------------
ofxBox::ofxBox(float px, float py, float w, float h) : x(position.x), y(position.y) {
	set(px,py,w,h);
}

//----------------------------------------------------------
ofxBox::ofxBox(const ofPoint& p, float w, float h) : x(position.x), y(position.y) {
    set(p,w,h);
}

//----------------------------------------------------------
ofxBox::ofxBox(const ofxBox& rect) : x(position.x), y(position.y) {
    set(rect);
}

//----------------------------------------------------------
ofxBox::ofxBox(const ofPoint& p0, const ofPoint& p1) : x(position.x), y(position.y) {
    set(p0,p1);
}

//----------------------------------------------------------
void ofxBox::set(float px, float py, float w, float h) {
	x		= px;
	y		= py;
	width	= w;
	height	= h;
}

//----------------------------------------------------------
void ofxBox::set(const ofPoint& p, float w, float h) {
    set(p.x, p.y, w, h);
}

//----------------------------------------------------------
void ofxBox::set(const ofxBox& rect){
    set(rect.x, rect.y, rect.width, rect.height);
}

//----------------------------------------------------------
void ofxBox::set(const ofPoint& p0, const ofPoint& p1) {
    float x0,y0,x1,y1;
    
    x0 = MIN(p0.x, p1.x);
    x1 = MAX(p0.x, p1.x);
    y0 = MIN(p0.y, p1.y);
    y1 = MAX(p0.y, p1.y);
    
    float w = x1 - x0;
    float h = y1 - y0;
	
    set(x0,y0,w,h);
}

//----------------------------------------------------------
void ofxBox::setX(float px) {
    x = px;
}

//----------------------------------------------------------
void ofxBox::setY(float py) {
    y = py;
}

//----------------------------------------------------------
void ofxBox::setWidth(float w) {
    width = w;
}

//----------------------------------------------------------
void ofxBox::setHeight(float h) {
    height = h;
}

//----------------------------------------------------------
void ofxBox::setPosition(float px, float py) {
    position.x = px;
    position.y = py;
}

//----------------------------------------------------------
void ofxBox::setPosition(const ofPoint& p) {
    position = p;
}

//----------------------------------------------------------
void ofxBox::setFromCenter(float px, float py, float w, float h) {
    set(px - w*0.5f, py - h*0.5f, w, h);
}

//----------------------------------------------------------
void ofxBox::setFromCenter(const ofPoint& p, float w, float h) {
    setFromCenter(p.x, p.y, w, h);
}

//----------------------------------------------------------
void ofxBox::translate(float dx, float dy) {
    translateX(dx);
    translateY(dy);
}

//----------------------------------------------------------
void ofxBox::translate(const ofPoint& dp) {
    translateX(dp.x);
    translateY(dp.y);
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
void ofxBox::scale(float s) {
    scaleWidth(s);
    scaleHeight(s);
}

//----------------------------------------------------------
void ofxBox::scale(float sX, float sY) {
    scaleWidth(sX);
    scaleHeight(sY);
}

//----------------------------------------------------------
void ofxBox::scale(const ofPoint& s) {
    scaleWidth(s.x);
    scaleHeight(s.y);
}

//----------------------------------------------------------
void ofxBox::scaleWidth(float  sX) {
    width  *= sX;
}
//----------------------------------------------------------
void ofxBox::scaleHeight(float sY) {
    height *= sY;
}

//----------------------------------------------------------
void ofxBox::scaleFromCenter(float s) {
    scaleFromCenter(s,s);
}

//----------------------------------------------------------
void ofxBox::scaleFromCenter(float sX, float sY) {
    scaleFromCenter(ofPoint(sX,sY));
}

//----------------------------------------------------------
void ofxBox::scaleFromCenter(const ofPoint& s) {
    if(s.x == 1.0f && s.y == 1.0f) return; // nothing to do
    
    float newWidth  = width  * s.x;
    float newHeight = height * s.y;
	
    ofPoint center = getCenter();
    
    x = center.x - newWidth  / 2.0f;
    y = center.y - newHeight / 2.0f;
    
    width  = newWidth;
    height = newHeight;
}

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
	
    float tw = targetRect.getWidth();    // target width
    float th = targetRect.getHeight();   // target height
    float sw = getWidth();   // subject width
    float sh = getHeight();  // subject height
	
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
            ofLogWarning("ofxBox") << "scaleTo(): no scaling applied to avoid divide by zero, rectangle has 0 width and/or height: " << sw << "x" << sh;
        }
    } else if(aspectRatioMode == OF_ASPECT_RATIO_IGNORE) {
        width  = tw;
        height = th;
    } else {
        ofLogWarning("ofxBox") << "scaleTo(): unknown ofAspectRatioMode = " << aspectRatioMode << ", using OF_ASPECT_RATIO_IGNORE";
        width  = tw;
        height = th;
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

//----------------------------------------------------------
bool ofxBox::inside(float px, float py) const {
	return inside(ofPoint(px,py));
}

//----------------------------------------------------------
bool ofxBox::inside(const ofPoint& p) const {
    return p.x > getMinX() && p.y > getMinY() &&
	p.x < getMaxX() && p.y < getMaxY();
}

//----------------------------------------------------------
bool ofxBox::inside(const ofxBox& rect) const {
    return inside(rect.getMinX(),rect.getMinY()) &&
	inside(rect.getMaxX(),rect.getMaxY());
}

//----------------------------------------------------------
bool ofxBox::inside(const ofPoint& p0, const ofPoint& p1) const {
    // check to see if a line segment is inside the rectangle
    return inside(p0) && inside(p1);
}

//----------------------------------------------------------
bool ofxBox::intersects(const ofxBox& rect) const {
    return (getMinX() < rect.getMaxX() && getMaxX() > rect.getMinX() &&
            getMinY() < rect.getMaxY() && getMaxY() > rect.getMinY());
}

//----------------------------------------------------------
bool ofxBox::intersects(const ofPoint& p0, const ofPoint& p1) const {
    // check for a line intersection
    ofPoint p;
    
    ofPoint topLeft     = getTopLeft();
    ofPoint topRight    = getTopRight();
    ofPoint bottomRight = getBottomRight();
    ofPoint bottomLeft  = getBottomLeft();
    
    return inside(p0) || // check end inside
	inside(p1) || // check end inside
	ofLineSegmentIntersection(p0, p1, topLeft,     topRight,    p) || // cross top
	ofLineSegmentIntersection(p0, p1, topRight,    bottomRight, p) || // cross right
	ofLineSegmentIntersection(p0, p1, bottomRight, bottomLeft,  p) || // cross bottom
	ofLineSegmentIntersection(p0, p1, bottomLeft,  topLeft,     p);   // cross left
}

//----------------------------------------------------------
void ofxBox::growToInclude(float px, float py) {
    growToInclude(ofPoint(px,py));
}

//----------------------------------------------------------
void ofxBox::growToInclude(const ofPoint& p) {
    float x0 = MIN(getMinX(),p.x);
    float x1 = MAX(getMaxX(),p.x);
    float y0 = MIN(getMinY(),p.y);
    float y1 = MAX(getMaxY(),p.y);
    float w = x1 - x0;
    float h = y1 - y0;
    set(x0,y0,w,h);
}

//----------------------------------------------------------
void ofxBox::growToInclude(const ofxBox& rect) {
    float x0 = MIN(getMinX(),rect.getMinX());
    float x1 = MAX(getMaxX(),rect.getMaxX());
    float y0 = MIN(getMinY(),rect.getMinY());
    float y1 = MAX(getMaxY(),rect.getMaxY());
    float w = x1 - x0;
    float h = y1 - y0;
    set(x0,y0,w,h);
}

//----------------------------------------------------------
void ofxBox::growToInclude(const ofPoint& p0, const ofPoint& p1) {
    growToInclude(p0);
    growToInclude(p1);
}

//----------------------------------------------------------
ofxBox ofxBox::getIntersection(const ofxBox& rect) const {
	
    float x0 = MAX(getMinX(),rect.getMinX());
    float x1 = MIN(getMaxX(),rect.getMaxX());
    
    float w = x1 - x0;
    if(w < 0.0f) return ofxBox(0,0,0,0); // short circuit if needed
    
    float y0 = MAX(getMinY(),rect.getMinY());
    float y1 = MIN(getMaxY(),rect.getMaxY());
    
    float h = y1 - y0;
    if(h < 0.0f) return ofxBox(0,0,0,0);  // short circuit if needed
    
    return ofxBox(x0,y0,w,h);
}

//----------------------------------------------------------
ofxBox ofxBox::getUnion(const ofxBox& rect) const {
    ofxBox united = *this;
    united.growToInclude(rect);
    return united;
}

//----------------------------------------------------------
void ofxBox::standardize() {
    if(width < 0.0f) {
        x += width;
        width = -1.0f * width;
    }
    
    if(height < 0.0f) {
        y += height;
        height = -1.0f * height;
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
    return width >= 0.0f && height >= 0.0f;
}

//----------------------------------------------------------
float ofxBox::getArea() const {
    return fabs(width) * fabs(height);
}

//----------------------------------------------------------
float ofxBox::getPerimeter() const {
    return 2.0f * fabs(width) + 2.0f * fabs(height);
}

//----------------------------------------------------------
float ofxBox::getAspectRatio() const {
    return fabs(width) / fabs(height);
}

//----------------------------------------------------------
bool ofxBox::isEmpty() const {
    return width == 0.0f && height == 0.0f;
}

//----------------------------------------------------------
ofPoint ofxBox::getMin() const {
    return ofPoint(getMinX(),getMinY());
}

//----------------------------------------------------------
ofPoint ofxBox::getMax() const {
    return ofPoint(getMaxX(),getMaxY());
}

//----------------------------------------------------------
float ofxBox::getMinX() const {
    return MIN(x, x + width);  // - width
}

//----------------------------------------------------------
float ofxBox::getMaxX() const {
    return MAX(x, x + width);  // - width
}

//----------------------------------------------------------
float ofxBox::getMinY() const{
    return MIN(y, y + height);  // - height
}

//----------------------------------------------------------
float ofxBox::getMaxY() const {
    return MAX(y, y + height);  // - height
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

//----------------------------------------------------------
bool ofxBox::operator != (const ofxBox& rect) const {
	return (x != rect.x) || (y != rect.y) || (width != rect.width) || (height != rect.height);
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
ofPoint ofxBox::getCenter() const {
	return ofPoint(x + width * 0.5f, y + height * 0.5f, 0);
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
float ofxBox::getWidth() const {
    return width;
}

//----------------------------------------------------------
float ofxBox::getHeight() const {
    return height;
}

//----------------------------------------------------------
ofxBox& ofxBox::operator = (const ofxBox& rect) {
    set(rect);
	return *this;
}

//----------------------------------------------------------
ofxBox ofxBox::operator + (const ofPoint & point){
	ofxBox rect=*this;
	rect.x += point.x;
	rect.y += point.y;
	return rect;
}

//----------------------------------------------------------
bool ofxBox::operator == (const ofxBox& rect) const {
	return (x == rect.x) && (y == rect.y) && (width == rect.width) && (height == rect.height);
}

//----------------------------------------------------------
ostream& operator<<(ostream& os, const ofxBox& rect){
	os << rect.position << ", " << rect.width << ", " << rect.height;
	return os;
}

//----------------------------------------------------------
istream& operator>>(istream& is, ofxBox& rect){
	is >> rect.position;
	is.ignore(2);
	is >> rect.width;
	is.ignore(2);
	is >> rect.height;
	return is;
}
