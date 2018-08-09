#include "RoadClass.h"

using namespace cv;
using namespace std;
using namespace RoadClass_space;



int main()
{
	RoadClass RC("MVI_6114.avi");
	for (int i = 0; i < RC.lanes.size(); i++)
	{
		cout << RC.lanes[i].number << " " << RC.lanes[i].points[0] << " " << RC.lanes[i].points[1] << endl;
	}
	RC.processVideo();
	
	for (int i = 0; i < RC.lanes.size(); i++)
	{
		cout << RC.lanes[i].number << " " << RC.lanes[i].numbersFramesWithCar.size() << endl;
	}
	waitKey();
	//destroy GUI windows
	destroyAllWindows();
	return EXIT_SUCCESS;
}
