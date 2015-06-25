#TRAFFICCOUNTER

exploring OpenCV: background subtraction and object tracking

The code in this repository implements a simple object tracking algorithm using
background subtraction. It is intended for use with overhead traffic videos, to
count the number of vehicles moving in and out of the frame against a static
background. A mixture of Gaussians is used to implement background subtraction
and produce a binary threshold image. A check to see whether the centroid of a
foreground object in one frame is within the contour of a foreground object of
the previous frame serves as a naive object tracker.

**Instructions**:

1. **Compiling**: $g++ -I/usr/local/include/opencv -I/usr/local/include/opencv2
-L/usr/local/lib/ -g -o count count.cpp -lopencv\_core -lopencv\_imgproc
-lopencv\_highgui -lopencv\_video
2. **Browsing Usage**: $./count -h

**Note**:

* Requires g++.
* Requires [OpenCV](http://opencv.org).
