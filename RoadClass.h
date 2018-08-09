// RoadClass.h

#ifndef _ROADCLASS_H_
#define _ROADCLASS_H_

//opencv
#include "opencv2/opencv.hpp"
#include <opencv2/video/background_segm.hpp>
#include "opencv2/imgcodecs.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/videoio.hpp"
#include <opencv2/highgui.hpp>
#include <opencv2/video.hpp>
//C
#include <stdio.h>
//C++
#include <iostream>
#include <sstream>
#include <string>
#include <fstream>

using namespace cv;
using namespace std;

namespace RoadClass_space
{
	struct paramForCallBack
	{
		Mat frame,
			frameCopy,
			frameCopy2,
			frameCopyBack;
		vector<vector<Point>> lines;
		vector<Point> points;

	};

	struct RoadLanes
	{
		int number;
		vector<Point> points;
		vector<int> numbersFramesWithCar;
		int amountCar;
	};

	class RoadClass
	{
	public:
		RoadClass(string src);
		~RoadClass(){};
		void processVideo();
		vector<RoadLanes> lanes;// полосы дороги на горизонтали
	private:
		Mat primeFrame; //current frame
		
		Rect rect_for_frame;
		Rect rect_for_top;
		Rect rect_for_middle1;
		Rect rect_for_middle2;
		Rect rect_for_bottom;
		
		vector<vector<Point>> linesCross;// линии разметки + последняя линия горизонтали
		vector<Point> pointsCross;// точки пересечения линий разметки с горизонталью
		
		string srcVideo;

	};

	void marking(vector<vector<Point>> &lines, vector<Point> &points);
	void onMouseCross(int event, int x, int y, int flags, void *params);
	Point cross(Point a, Point b, Point c, Point d);
	void onMouseCrop(int event, int x, int y, int flags, void* rect_for_f);
	bool linePoint(vector<Point> line, Point point);
	void slider(Mat image, int& min, int& max, int proc);
	void holeFixed(Mat& img);
}






#endif // !RoadClass.h