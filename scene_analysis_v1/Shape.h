#include "opencv2/opencv.hpp"

using namespace cv;
using namespace std;

class Shape {

public:
	//costructors
	Shape();
	Shape(vector<vector<Point>> contours, Mat& result, int index, Point& selection, string contourshape, int red, int green, int blue);

	void displayText(Mat& im, const string label, vector<Point>& contour);
	void drawShape();
	void respondToClick();
	Point getSelection();

private:
	vector<vector<Point>> contours;
	Mat result;
	int index;
	Point selection;
	string shape;
	int red;
	int green;
	int blue;
}; 
