ofxVolume
=====================================
An Openframeworks addon to load, save, and process volumes in the CPU.
It follows ofImage and ofPixels pattern.


Introduction
------------
This addon is aims to offer the same utilities of ofImage and ofPixels but for 3D images.

	Contents:				Equivalent to:
	ofxVolume				ofImage
	ofxVoxels				ofPixels
	ofxImageSequence


The rendering is done with [ofxVolumetrics](https://github.com/timscaffidi/ofxVolumetrics) wich is a great addon by Tim Scaffidi. I started using this addon for a project and I realised I wanted to have more tools to handle volumes. So I decided to learn the image handling structure in openframeworks and translate it for the use of volumes.

I have to warn everybody that I consider myself a novice c++ programmer so there are high chances that my code the best out there. Some parts are still to be done.

In the future i would like to extend this addon with an interface to ITK so that i can load DICOM and NIfTI file formats.


Licence
-------
[MIT License](https://en.wikipedia.org/wiki/MIT_License)


Installation
------------
clone, check (and install) dependencies and run.


Dependencies
------------
+	[ofxPoint](https://github.com/wasawi/ofxPoint)
+	[ofxVolumetrics](https://github.com/timscaffidi/ofxVolumetrics) by Tim Scaffidi 
+	The example provided uses [my fork](https://github.com/wasawi/ofxVolumetrics/tree/addon_ofxVolume)


Compatibility
------------
This addon has been tested in OF 0.8.1 in Mac. 
Please let me know about issues in other platforms.
