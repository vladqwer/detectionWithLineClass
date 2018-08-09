#include "RoadClass.h"


namespace RoadClass_space
{
	RoadClass::RoadClass(string srcVideo_)
				: srcVideo(srcVideo_)
	{
		namedWindow("binarMask");
		namedWindow("Frame");
		//namedWindow("frameColCor");
		//namedWindow("BS");
		//namedWindow("BS2");


		namedWindow("Frame");

		VideoCapture capture(srcVideo);
		if (!capture.read(primeFrame)) {
			cerr << "Unable to read next frame." << endl;
			cerr << "Exiting..." << endl;
			exit(EXIT_FAILURE);
		}
		imshow("Frame", primeFrame);
		paramForCallBack param;

		param.frameCopy = primeFrame.clone();
		param.frameCopy2 = primeFrame.clone();
		param.frameCopyBack = primeFrame.clone();
		setMouseCallback("Frame", onMouseCrop, (void*)&rect_for_frame);
		waitKey();
		param.frameCopy = primeFrame(rect_for_frame);
		imshow("copyFrame", param.frameCopy);
		setMouseCallback("copyFrame", onMouseCross, (void*)&param);
		
		//while (param.lines.size() < 3){	}
		
		marking(param.lines, param.points);
		linesCross = param.lines;
		pointsCross = param.points;

		for (int i = 1; i < pointsCross.size(); i++)
		{
			RoadLanes RL;
			RL.number = i - 1;
			RL.points.push_back(pointsCross[i - 1]);
			RL.points.push_back(pointsCross[i]);
			RL.numbersFramesWithCar.push_back(0);
			lanes.push_back(RL);
		}

		rect_for_top.x = 0;
		rect_for_top.y = 0;
		rect_for_top.width = rect_for_frame.width;
		rect_for_top.height = rect_for_frame.height / 4;

		rect_for_middle1.x = 0;
		rect_for_middle1.y = rect_for_frame.height / 4;
		rect_for_middle1.width = rect_for_frame.width;
		rect_for_middle1.height = rect_for_frame.height / 4;

		rect_for_middle2.x = 0;
		rect_for_middle2.y = rect_for_frame.height / 2;
		rect_for_middle2.width = rect_for_frame.width;
		rect_for_middle2.height = rect_for_frame.height / 4;

		rect_for_bottom.x = 0;
		rect_for_bottom.y = rect_for_frame.height / 4 * 3;
		rect_for_bottom.width = rect_for_frame.width;
		rect_for_bottom.height = rect_for_frame.height / 4;

	}

	void onMouseCrop(int event, int x, int y, int flags, void* rect_for_f)
	{
		Rect *rect_for_frame = (Rect*)rect_for_f;
		static int countP = 0;
		static vector<Point> VP;

		if (event == EVENT_LBUTTONDOWN)
		{

			Point seed = Point(x, y);
			cout << seed.x << "  " << seed.y;
			VP.push_back(seed);
		}

		if (event == EVENT_LBUTTONUP)
		{
			Point seed = Point(x, y);
			cout << seed.x << "  " << seed.y;
			VP.push_back(seed);
			int width = 0,
				height = 0;
			Point P;
			height = abs(VP[0].y - VP[1].y);
			width = abs(VP[0].x - VP[1].x);
			VP[0].x < VP[1].x ? P.x = VP[0].x : P.x = VP[1].x;
			VP[0].y < VP[1].y ? P.y = VP[0].y : P.x = VP[1].y;

			rect_for_frame->x = P.x;
			rect_for_frame->y = P.y;
			rect_for_frame->width = width;
			rect_for_frame->height = height;


			setMouseCallback("Frame", NULL, NULL);

		}

	}

	void onMouseCross(int event, int x, int y, int flags, void *params)
	{
		paramForCallBack *param = (paramForCallBack*)params;
		Mat & frame = param->frame,
			&frameCopy = param->frameCopy,
			&frameCopy2 = param->frameCopy2,
			&frameCopyBack = param->frameCopyBack;
		vector<vector<Point>> & lines = param->lines;
		vector<Point> & points = param->points;
		Rect rect_for_frame;
		static int countP = 0;
		static vector<Point> VP;
		Point seed = Point(x, y);

		if (event == EVENT_LBUTTONDOWN)
		{
			//return;
			countP++;

			cout << seed.x << "  " << seed.y << endl;
			VP.push_back(seed);

		}
		if (flags == EVENT_FLAG_LBUTTON)
		{
			frameCopy2 = frameCopy.clone();
			VP.push_back(seed);
			Point P;
			int width = 0,
				height = 0;
			height = abs(VP[0].y - y);
			width = abs(VP[0].x - x);
			VP[0].x < VP[1].x ? P.x = VP[0].x : P.x = VP[1].x;
			VP[0].y < VP[1].y ? P.y = VP[0].y : P.x = VP[1].y;
			rect_for_frame.x = P.x;
			rect_for_frame.y = P.y;
			rect_for_frame.width = width;
			rect_for_frame.height = height;
			line(frameCopy2, VP[0], VP[1], Scalar(0, 0, 255));
			imshow("copyFrame", frameCopy2);
			VP.pop_back();
		}
		if (event == EVENT_LBUTTONUP)
		{

			countP++;

			cout << seed.x << "  " << seed.y << endl;
			VP.push_back(seed);
			int width = 0,
				height = 0;
			Point P;
			height = abs(VP[0].y - VP[1].y);
			width = abs(VP[0].x - VP[1].x);
			VP[0].x < VP[1].x ? P.x = VP[0].x : P.x = VP[1].x;
			VP[0].y < VP[1].y ? P.y = VP[0].y : P.x = VP[1].y;

			rect_for_frame.x = P.x;
			rect_for_frame.y = P.y;
			rect_for_frame.width = width;
			rect_for_frame.height = height;
			frameCopyBack = frameCopy.clone();
			line(frameCopy, VP[0], VP[1], Scalar(0, 0, 255));
			imshow("copyFrame", frameCopy);

			countP = 0;
			vector<Point> VPS = { VP[0], VP[1] };
			lines.push_back(VPS);
			VP.clear();

		}
		if (event == EVENT_RBUTTONDOWN)
		{
			frameCopy = frameCopyBack.clone();
			if (lines.size() > 0)
			{
				lines.pop_back();
			}
			imshow("copyFrame", frameCopy);
		}


	}
	
	void marking(vector<vector<Point>> &lines, vector<Point> &points)
	{

		waitKey();
		for (int i = 0; i < lines.size() - 1; i++)
		{
			Point p = cross(lines[i][0], lines[i][1], lines[lines.size() - 1][0], lines[lines.size() - 1][1]);
			cout << "p" << p.x << " " << p.y << endl;
			points.push_back(p);
		}
		setMouseCallback("copyFrame", NULL, NULL);
	}

	Point cross(Point a, Point b, Point c, Point d) //точки a и b концы первого отрезка  c и d второго
	{
		Point T;
		/*cout << "a.x " << a.x << " a.y " << a.y << endl;
		cout << "b.x " << b.x << " b.y " << b.y << endl;
		cout << "c.x " << c.x << " c.y " << c.y << endl;
		cout << "d.x " << d.x << " d.y " << d.y << endl;*/
		T.x = -((a.x*b.y - b.x*a.y)*(d.x - c.x) - (c.x*d.y - d.x*c.y)*(b.x - a.x)) / ((a.y - b.y)*(d.x - c.x) - (c.y - d.y)*(b.x - a.x));
		T.y = ((c.y - d.y)*(-T.x) - (c.x*d.y - d.x*c.y)) / (d.x - c.x);
		cout << "T.x " << T.x << " T.y " << T.y << endl;
		return T;
	}

	void RoadClass::processVideo() {
		cout << "processVideo" << endl;
		int keyboard = 0; //input from keyboard
		int numberOfprint;// номер печати картинки
		int numberOfFrame = 0;// номер последнего фрейма
		int numberOfCar = 0;
		Ptr<BackgroundSubtractor> pMOG; //MOG2 Background subtractor
		Mat frameColCor; //frame rect
		Mat frame_top; //frame
		Mat frame_bottom; //frame
		Mat frame_middle1; //frame
		Mat frame_middle2; //frame
		Mat BS; //frame bs
		Mat BS2; //frame bs
		Mat binarMask; //fg mask fg mask generated by MOG2 method
		Mat copyFrame; //frame rect

		pMOG = createBackgroundSubtractorMOG2(); //MOG2 approach
		pMOG.dynamicCast<BackgroundSubtractorMOG2>()->setShadowValue(0);
		pMOG.dynamicCast<BackgroundSubtractorMOG2>()->setShadowThreshold(0.1);

		int min = 0, max = 0;
		slider(primeFrame, min, max, 10);

		VideoCapture capture(srcVideo);
		vector<vector<Point>> contours;
		vector<Vec4i> hierarchy;

		while ((char)keyboard != 'q' && (char)keyboard != 27){
			if (!capture.read(primeFrame)) {
				cerr << "Unable to read next frame." << endl;
				cerr << "Exiting..." << endl;
				exit(EXIT_FAILURE);
			}

			numberOfFrame++;
			frameColCor = primeFrame(rect_for_frame).clone();
			copyFrame = primeFrame(rect_for_frame).clone();
			// цветокоррекция
			for (int i = 0; i < frameColCor.rows; i++)
			{
				for (int j = 0; j < frameColCor.cols; j++)
				{
					frameColCor.at<Vec3b>(i, j)[0] = saturate_cast<uchar>((double)
						(((double)frameColCor.at<Vec3b>(i, j)[0] - (double)min) / (double)(max - min)) * 255);
					frameColCor.at<Vec3b>(i, j)[1] = saturate_cast<uchar>((double)
						(((double)frameColCor.at<Vec3b>(i, j)[1] - (double)min) / (double)(max - min)) * 255);
					frameColCor.at<Vec3b>(i, j)[2] = saturate_cast<uchar>((double)
						(((double)frameColCor.at<Vec3b>(i, j)[2] - (double)min) / (double)(max - min)) * 255);
				}
			}
			pMOG->apply(frameColCor, binarMask);
			if (numberOfFrame % 5 == 0)
			{
				////

				//номер кадра на фрейм
				stringstream ss;
				rectangle(frameColCor, Point(10, 2), Point(10, 20),
					Scalar(255, 255, 255), -1);
				ss << capture.get(CAP_PROP_POS_FRAMES);
				string frameNumberString = ss.str();
				putText(frameColCor, frameNumberString.c_str(), cv::Point(15, 15),
					FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(0, 0, 255));
				////

				BS = binarMask.clone();
				BS2 = binarMask.clone();


				Mat kernel = getStructuringElement(MORPH_RECT, Size(3, 3));
				erode(BS2, BS2, kernel, Point(-1, -1), 1);
				morphologyEx(BS2, BS2, MORPH_CLOSE, kernel, Point(-1, -1), 2);
				holeFixed(BS2);
				kernel = getStructuringElement(MORPH_RECT, Size(7, 7));
				morphologyEx(BS2, BS2, MORPH_CLOSE, kernel, Point(-1, -1), 2);
				holeFixed(BS2);
				kernel = getStructuringElement(MORPH_RECT, Size(9, 9));
				morphologyEx(BS2, BS2, MORPH_CLOSE, kernel, Point(-1, -1), 2);
				holeFixed(BS2);
				morphologyEx(BS2, BS2, MORPH_CLOSE, kernel, Point(-1, -1), 2);
				holeFixed(BS2);
				kernel = getStructuringElement(MORPH_RECT, Size(5, 5));
				erode(BS2, BS2, kernel, Point(-1, -1), 2);
				dilate(BS2, BS2, kernel, Point(-1, -1), 1);
				Mat bs_ = BS.clone();

				binarMask = BS2.clone();
				dilate(binarMask, binarMask, kernel, Point(-1, -1), 1);
				frame_top = binarMask(rect_for_top);
				frame_middle1 = binarMask(rect_for_middle1);
				frame_middle2 = binarMask(rect_for_middle2);
				frame_bottom = binarMask(rect_for_bottom);

				medianBlur(frame_top, frame_top, 3);
				medianBlur(frame_middle1, frame_middle1, 5);
				medianBlur(frame_middle2, frame_middle2, 9);
				medianBlur(frame_bottom, frame_bottom, 15);
				medianBlur(binarMask, binarMask, 5);
				bs_ = binarMask.clone();

				kernel = getStructuringElement(MORPH_RECT, Size(10, 10));
				dilate(BS2, BS2, kernel, Point(-1, -1), 1);

				findContours(bs_, contours, hierarchy,
					CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE);

				for (int i = 0; i < linesCross.size(); i++)
				{
					line(copyFrame, linesCross[i][0], linesCross[i][1], Scalar(0, 0, 255), 2);
				}
				for (int i = 0; i < pointsCross.size(); i++)
				{
					circle(copyFrame, pointsCross[i], 3, Scalar(255, 0, 0), -1);
				}

				for (int i = 0, minx = 1000, maxx = -1, miny = 1000, maxy = -1; i < contours.size(); i++)
				{
					minx = 1000; maxx = -1; miny = 1000; maxy = -1;
					for (int j = 0; j < contours[i].size(); j++)
					{
						if (minx > contours[i][j].x) minx = contours[i][j].x;
						if (miny > contours[i][j].y) miny = contours[i][j].y;
						if (maxx < contours[i][j].x) maxx = contours[i][j].x;
						if (maxy < contours[i][j].y) maxy = contours[i][j].y;
					}
					if ((maxx - minx) > 30 && (maxy - miny) > 30)
					{
						/*int miny_c = miny,
						maxy_c = maxy;
						(miny - 1.15*(maxy - miny) < 0) ? miny = 0, maxy = maxy_c - miny_c : miny -= 1.15 * (maxy_c - miny_c), maxy -= 1.15 * (maxy_c - miny_c);*/
						rectangle(copyFrame, Rect(Point(minx, miny), Point(maxx, maxy)), Scalar(50, 200, 5), 3, 8, 0);

						drawContours(copyFrame, contours, i, Scalar(255, 0, 255), 1);
						//cout << contours[i].size() << endl;


						Point centerCar((minx + maxx) / 2, (miny + maxy) / 2);
						circle(copyFrame, centerCar, 3, Scalar(255, 0, 0), -1);


						if (linePoint(linesCross[linesCross.size() - 1], centerCar))
						{
							for (int j = 0; j < lanes.size(); j++)
							{
								if (centerCar.x > lanes[j].points[0].x && centerCar.x < lanes[j].points[1].x)
								{
									line(copyFrame, lanes[j].points[0], lanes[j].points[1], Scalar(0, 255, 0), 3);
									if (numberOfFrame - lanes[j].numbersFramesWithCar[lanes[j].numbersFramesWithCar.size()-1] > 10)
									{
										//numberOfCar++;
										//cout << numberOfCar << endl;
										circle(copyFrame, centerCar, 5, Scalar(0, 255, 0), -1);
										lanes[j].numbersFramesWithCar.push_back(numberOfFrame);
									}
								}
							}
							

						}
						/*int maxXY = maxx - minx > maxy - miny ? maxx - minx : maxy - miny;
						Mat print = primeFrame(Rect(Point(centerCar.x + rect_for_frame.x - maxXY / 2 > 0 ? centerCar.x + rect_for_frame.x - maxXY / 2 : 0,
						centerCar.y + rect_for_frame.y - maxXY / 2 > 0 ? centerCar.y + rect_for_frame.y - maxXY / 2 : 0),
						Point(centerCar.x + rect_for_frame.x + maxXY / 2 < primeFrame.cols ? centerCar.x + rect_for_frame.x + maxXY / 2 : primeFrame.cols,
						centerCar.y + rect_for_frame.y + maxXY / 2 < primeFrame.rows ? centerCar.y + rect_for_frame.y + maxXY / 2 : primeFrame.rows))).clone();
						string str1 = "C:/Vlad_E/Image/Image/Image";
						string str2 = to_string(numberOfprint);
						numberOfprint++;
						string str3 = ".jpg";
						str1 += str2;
						str1 += str3;
						if (print.cols == print.rows)
						{
						resize(print, print, Size(224, 224), 0, 0);
						imshow("print", print);
						imwrite(str1, print);
						str_out = str1 + " 2";
						out << str_out << endl;
						}*/



					}
				}
				
				imshow("copyFrame", copyFrame);
				//imshow("frameCanny", frameCanny);
				imshow("binarMask", binarMask);
				//imshow("BS", BS);
				//imshow("BS_canny", BS_canny);
				//imshow("BS2", BS2);
				//imshow("frameColCor", frameColCor);
				//imshow("Frame", primeFrame);


				//get the input from the keyboard
				keyboard = (char)waitKey(30);
			}
		}
		//delete capture object
		capture.release();
	}

	bool linePoint(vector<Point> line, Point point)
	{
		cout << abs((line[1].y - line[0].y)*point.x - (line[1].y - line[0].y)*line[0].x - point.y*(line[1].x - line[0].x) + line[0].y*(line[1].x - line[0].x)) << endl;
		if (abs((line[1].y - line[0].y)*point.x - (line[1].y - line[0].y)*line[0].x - point.y*(line[1].x - line[0].x) + line[0].y*(line[1].x - line[0].x)) < 10000)
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	void slider(Mat image, int& min, int& max, int proc)
	{
		vector<Mat> temp;
		vector<int> GistBlue(256);
		vector<int> GistGreen(256);
		vector<int> GistRed(256);
		long int CoefBlue = 0;
		long int CoefGreen = 0;
		long int CoefRed = 0;
		split(image, temp);
		for (int i = 0; i < image.rows; i++)
		{
			for (int j = 0; j < image.cols; j++)
			{
				GistBlue[(temp[0].at<uchar>(i, j))]++;
				GistGreen[(temp[1].at<uchar>(i, j))]++;
				GistRed[(temp[2].at<uchar>(i, j))]++;
			}
		}
		long int percent = image.rows*image.cols*proc / 100;
		for (int i = 0; i < 255; i++)
		{
			CoefBlue += GistBlue[i];
			CoefGreen += GistGreen[i];
			CoefRed += GistRed[i];

			if (CoefBlue >= percent || CoefGreen >= percent || CoefRed >= percent)
			{
				min = i;
				break;
			}
		}
		CoefBlue = 0; CoefGreen = 0; CoefRed = 0;
		for (int i = 255; i > 0; i--)
		{
			CoefBlue += GistBlue[i];
			CoefGreen += GistGreen[i];
			CoefRed += GistRed[i];
			if (CoefBlue >= percent || CoefGreen >= percent || CoefRed >= percent)
			{
				max = i;
				break;
			}
		}
	}

	void holeFixed(Mat& img)
	{
		Mat g1_ = img.clone();
		vector<vector<Point>> contours;
		vector<Vec4i> hierarchy;
		findContours(g1_, contours, hierarchy,
			CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE);
		//imshow("NO", img);
		//waitKey();
		for (int i = 0; i < contours.size(); i++)
		{
			drawContours(img, contours, i, 255, -1, 8, hierarchy, 0);
		}
	}


}