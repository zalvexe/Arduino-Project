#include <Windows.h>
#include <iostream>
#include<opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
using namespace std;
using namespace cv;
// Need to include this for serial port communication


///////////////////////////////////////////////////////////////////////////////////////////////////
double find_focal_len(double known_len, double real_width, double width_in_rf_image) {
	double found_focal_len = (width_in_rf_image * known_len) / real_width;
	return found_focal_len;
}
double find_dist(double focal_len, double real_object_width, double width_pixel) {
	double found_dist = (real_object_width * focal_len) / width_pixel;
	return found_dist;
}

int main(int argc, char* argv[]) {
	// Setup serial port connection and needed variables.
	HANDLE hSerial = CreateFile(L"COM9", GENERIC_READ | GENERIC_WRITE, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0); // change the com for the receiver

	if (hSerial != INVALID_HANDLE_VALUE) {
		printf("Port opened! \n");

		DCB dcbSerialParams;
		GetCommState(hSerial, &dcbSerialParams);

		dcbSerialParams.BaudRate = CBR_9600;
		dcbSerialParams.ByteSize = 8;
		dcbSerialParams.Parity = NOPARITY;
		dcbSerialParams.StopBits = ONESTOPBIT;

		SetCommState(hSerial, &dcbSerialParams);
	}
	else {
		if (GetLastError() == ERROR_FILE_NOT_FOUND) {
			printf("Serial port doesn't exist! \n");
		}

		printf("Error while setting up serial port! \n");
	}

	char outputChars[] = "c";
	DWORD btsIO;
	// Setup OpenCV variables and structures

	//---Video capture ---//
	Mat frameOri, framePros(480, 640, CV_8UC3), frameHSV(480, 640, CV_8UC3);

	VideoCapture cap(0);

	int servoPos = 90, servoOrientation = 0;
	double dist = 50.0;
	double known_width = 24.0;

	while (true) {
		cap.read(frameOri);

		if (frameOri.empty()) {
			cout << "frame not found";
			cin.get();
			break;	
		}
		//flip(frameOri, frameOri, 1);
		cvtColor(frameOri, framePros, COLOR_BGR2RGB);
		cvtColor(framePros, frameHSV, COLOR_RGB2HSV);
		Mat gray;
		cvtColor(frameOri, gray, COLOR_BGR2GRAY);
		medianBlur(gray, gray, 5);
		Mat masked_BLUE, dilate_BLUE;
		inRange(frameHSV, Scalar(90, 100, 100), Scalar(130, 255, 255), masked_BLUE);


		imshow("Masked Blue", masked_BLUE);
		vector<vector<Point> >contourBLUE;
		findContours(masked_BLUE, contourBLUE, RETR_TREE, CHAIN_APPROX_SIMPLE);
		vector<vector<Point> > contourBLUE_poly(contourBLUE.size());
		vector<Rect> boundRect_BLUE(contourBLUE.size());

		float centerX, centerY;
		vector<vector<Point> >res(1);
		double temp_area = 700;
		for (int i = 0; i < contourBLUE.size(); i++) {
			double contourBLUE_area = contourArea(contourBLUE[i]);
			if (contourBLUE_area > temp_area) {
				temp_area = contourBLUE_area;
				res[0] = contourBLUE[i];
			}
		}
		//if there's blue
		if (!res[0].empty() && res[0].size() >= 3) {
			vector<Point> approx;
			approxPolyDP(res[0], approx, 3, true);
			boundRect_BLUE[0] = boundingRect(approx);
			rectangle(frameOri, boundRect_BLUE[0].tl(), boundRect_BLUE[0].br(), Scalar(255, 0, 0), 2);
			putText(frameOri, "BLUE", boundRect_BLUE[0].tl(), FONT_HERSHEY_PLAIN, 1, Scalar(255, 0, 0), 2);
			centerX = boundRect_BLUE[0].tl().x + boundRect_BLUE[0].width / 2;
			centerY = boundRect_BLUE[0].tl().y + boundRect_BLUE[0].height / 2;
			printf("object position x = %f, y = %f \n", centerX, centerY);

			//--- find distance ---//
			double width_ref = boundRect_BLUE[0].width;
			double focal_len = find_focal_len(dist, known_width, width_ref);
			string focal_len_str = to_string(focal_len);
			putText(frameOri, focal_len_str, boundRect_BLUE[0].tl(), FONT_HERSHEY_PLAIN, 1, Scalar(0, 0, 255), 1);
			//cout << focal_len_str << endl;
			double found_distance = find_dist(504.166667, known_width, width_ref);
			string found_dist_string = to_string(found_distance);
			putText(frameOri, found_dist_string, boundRect_BLUE[0].br(), FONT_HERSHEY_PLAIN, 1, Scalar(0, 0, 255), 1);
			//---end---//

			servoOrientation = 0; //reset servo position
			if (found_distance > 50) {
				outputChars[0] = 'f';
				WriteFile(hSerial, outputChars, strlen(outputChars), &btsIO, NULL);
			}
			else if (found_distance <= 50 && found_distance > 20) {
				outputChars[0] = 'b';
				WriteFile(hSerial, outputChars, strlen(outputChars), &btsIO, NULL);
			}
			else if (found_distance <= 20) {
				outputChars[0] = 's';
				WriteFile(hSerial, outputChars, strlen(outputChars), &btsIO, NULL);
			}
			//----- direction ----//
			if (centerX > 450) {
				outputChars[0] = 'r';
				WriteFile(hSerial, outputChars, strlen(outputChars), &btsIO, NULL);
			}

			if (centerX < 150) {
				outputChars[0] = 'l';
				WriteFile(hSerial, outputChars, strlen(outputChars), &btsIO, NULL);
			}
		}
		else {
			cout << "no object" << endl;
			outputChars[0] = 'r';
			WriteFile(hSerial, outputChars, strlen(outputChars), &btsIO, NULL);
		}
		imshow("frame", frameOri);
		cout << outputChars[0] << endl;
		if (waitKey(30) == 27) {
			outputChars[0] = 's';
			WriteFile(hSerial, outputChars, strlen(outputChars), &btsIO, NULL);
			break;
		}
	}
	//destroyAllWindows();
	CloseHandle(hSerial);
	return 0;
}
