#include<iostream>
#include<opencv2/opencv.hpp>

using namespace cv;
using namespace std;

// **** BLUE & YELLOW COLOR DETECTION *****//      

int main() {
	VideoCapture cap(0);
	Mat frameVid, frame;
	Mat frameHSV;
	while (true) {
		cap.read(frameVid);
		if (frameVid.empty()) {
			cout << "ERRORRR" << endl;
			cin.get();
		}
		flip(frameVid, frame, 1);
		cvtColor(frame, frameHSV, COLOR_BGR2HSV);
	
		// --- BLUE ---//

		Mat masked_BLUE,dilate_BLUE;
		inRange(frameHSV, Scalar(90, 100, 100), Scalar(130, 255, 255), masked_BLUE);

		imshow("Masked Blue", masked_BLUE);
		vector<vector<Point> >contourBLUE;
		findContours(masked_BLUE, contourBLUE, RETR_TREE, CHAIN_APPROX_SIMPLE);
		vector<vector<Point> > contourBLUE_poly(contourBLUE.size());
		vector<Rect> boundRect_BLUE(contourBLUE.size());

		double temp_area = 700; 
		for (int i = 0; i < contourBLUE.size(); i++){
			double contourBLUE_area = contourArea(contourBLUE[i]); 
			if (contourBLUE_area > temp_area) { 
				approxPolyDP(contourBLUE[i], contourBLUE_poly[i], 3, true);
				boundRect_BLUE[i] = boundingRect(contourBLUE_poly[i]);
				rectangle(frame, boundRect_BLUE[i].tl(), boundRect_BLUE[i].br(), Scalar(255,0,0), 2);
				putText(frame, "BLUE", boundRect_BLUE[i].tl(), FONT_HERSHEY_PLAIN, 1, Scalar(255, 0, 0), 2);
			}
		}
    
		// ---- YELLOW ---//

		Mat masked_YELLOW;
		inRange(frameHSV, Scalar(20, 100, 100), Scalar(30, 255, 255), masked_YELLOW);
		imshow("Masked yellow", masked_YELLOW);
		vector<vector<Point> > contourYELLOW;
		findContours(masked_YELLOW, contourYELLOW, RETR_TREE, CHAIN_APPROX_SIMPLE);

		vector<vector<Point> >contourYELLOW_poly(contourYELLOW.size());
		vector<Rect>boundRect_YELLOW(contourYELLOW.size());

		for (int i = 0; i < contourYELLOW.size(); i++) {
			double contourYELLOW_area = contourArea(contourYELLOW[i]);
			if (contourYELLOW_area > temp_area) {
				approxPolyDP(contourYELLOW[i], contourYELLOW_poly[i], 3, true);
				boundRect_YELLOW[i] = boundingRect(contourYELLOW_poly[i]);
				rectangle(frame, boundRect_YELLOW[i].tl(), boundRect_YELLOW[i].br(), Scalar(0, 255, 255), 2);
				putText(frame, "YELLOW", boundRect_YELLOW[i].tl(), FONT_HERSHEY_PLAIN, 1, Scalar(0, 255, 255), 2);
			}
		}

		imshow("Color Detection", frame);
		if (waitKey(30) == 27) break; //esc
	}
}
