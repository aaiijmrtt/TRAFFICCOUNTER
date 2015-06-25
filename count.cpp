#include<opencv2/opencv.hpp>
#include<iostream>
#include<vector>
#include<cstdlib>
#include<unistd.h>

using namespace cv;
using namespace std;

void usage() {
	cerr << "\n\nNAME\n\n\n\tcount - count vehicles in overhead traffic video\n\n\nSYNOPSIS\n\n\n\t./count [OPTIONS]... <FILE:VIDEO>...\n\n\nDESCRIPTION\n\n\n\tCount vehicles in overhead traffic video.\n\n\nOPTIONS\n\n\t-h\n\t\tdisplay this help and exit\n\n\t-v\n\t\tdisplay video and trackbars for tuning\n\n\t-e <INT>\n\t\taccept erosion radius: default is 9\n\n\t-d <INT>\n\t\taccept dilation radius: default is 8\n\n\nARGUMENTS\n\n\tFILE:VIDEO\n\t\taccept video file for processing\n\n\nAUTHOR\n\n\n\tWritten by Amitrajit Sarkar.\n\n\nREPORTING BUGS\n\n\n\tReport bugs to <aaiijmrtt@gmail.com>.\n\n\nCOPYRIGHT\n\n\nThe MIT License (MIT)\n\nCopyright (c) 2015 Amitrajit Sarkar\n\nPermission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the \"Software\"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:\n\nThe above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.\n\nTHE SOFTWARE IS PROVIDED \"AS IS\", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.\n\n";
	exit(EXIT_FAILURE);
}

int main(int argc, char **argv) {

	bool video = false;
	int ero = 9, dil = 8, count = 0;
	char c, *file = NULL;

	while((c = getopt(argc, argv, "hve:d:")) != -1)
		switch(c) {
			case 'v':
				video = true;
			break;
			case 'e':
				ero = atoi(optarg);
			break;
			case 'd':
				dil = atoi(optarg);
			break;
			default:
				usage();
		}

	if(optind == argc - 1)
		file = argv[optind];
	else
		usage();

	Mat frame, fore;
	VideoCapture cap(file);
	BackgroundSubtractorMOG bg(10, 5, 0.5);


	if(video) {
		namedWindow("Frame", CV_WINDOW_AUTOSIZE);
		namedWindow("Control", CV_WINDOW_AUTOSIZE);

		cvCreateTrackbar("Erosion", "Control", &ero, 20);
		cvCreateTrackbar("Dilation", "Control", &dil, 20);
	}

	vector<vector<Point> > oldc;
	vector<vector<Point> > newc;

	while(true) {

		if(!cap.read(frame))
			break;

		oldc.clear();
		oldc = newc;
		newc.clear();

		bg.operator()(frame, fore, 0.005);
		erode(fore, fore, getStructuringElement(MORPH_ELLIPSE, Size(ero, ero)));
		dilate(fore, fore, getStructuringElement(MORPH_ELLIPSE, Size(dil, dil)));
		findContours(fore, newc, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);
		drawContours(frame, newc, -1, cv::Scalar(0, 0, 255), 2);

		vector<Moments> mu(newc.size());
		for(int i = 0; i < newc.size(); i++)
			mu[i] = moments(newc[i], false);

		vector<Point2f> mc(newc.size());
		for(int i = 0; i < mu.size(); i++)
			mc[i]  = Point2f(mu[i].m10/mu[i].m00, mu[i].m01/mu[i].m00);

		if(oldc.size() != mc.size())
			for(int i = 0, j; i < mc.size(); ++i) {
				for(j = 0; j < oldc.size(); ++j)
					if(pointPolygonTest(oldc[j], mc[i], false))
						break;
				if(j == oldc.size())
					count++;
			}

		if(video)
			imshow("Frame", frame);
		if(waitKey(30) >= 0)
			break;
	}
	cout << count << endl;
	destroyAllWindows();
	return 0;
}
