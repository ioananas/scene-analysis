#include "Shape.h"

//constructor
Shape::Shape()
{
}

//constructor
Shape::Shape(vector<vector<Point>> contours, Mat& result, int index, Point& selection, string shape, int red, int green, int blue)
{
	Shape::contours = contours;
	Shape::index = index;
	Shape::result = result;
	Shape::selection = selection;
	Shape::shape = shape;
	Shape::blue = blue;
	Shape::red = red;
	Shape::green = green;
}

// Function to display text in the center of a contour
void Shape::displayText(Mat& im, const string label, vector<Point>& contour)
{
    int fontface = FONT_HERSHEY_PLAIN;
    double scale = 0.8;    int thickness = 1;   int baseline = 0;
 
	Size text = getTextSize(label, fontface, scale, thickness, &baseline);
	Rect r = boundingRect(contour);
 
	//point at the center of the contour
    Point pt(r.x + ((r.width - text.width) / 2), r.y + ((r.height + text.height) / 2));
    rectangle(im, pt + Point(0, baseline), pt + Point(text.width, -text.height), CV_RGB(255,255,255), CV_FILLED);
    //draws the text on screen
	putText(im, label, pt, fontface, scale, CV_RGB(0,0,0), thickness, 8);
}

//Function to draw the deteted shapes on screen
void Shape::drawShape()
{
	//draw the shape 
	Scalar color = Scalar(blue, green, red); 
	drawContours(result, contours, index, color, 2, 8);
					
	respondToClick();
}

//if user clicked inside a shape display its type
void Shape::respondToClick()
{
	if(selection != Point())
	{

		if(pointPolygonTest(contours[index], selection, false)==1 || pointPolygonTest(contours[index], selection, false)==0)
		{
			//cout << "You clicked on a shape ..." << endl;
			displayText(result, shape, contours[index]); 
						
			selection = Point();
		}
	}
}

Point Shape::getSelection()
{
	return selection;
}


