#include "Shape.h"

#include "opencv2/opencv.hpp"

using namespace cv;
using namespace std;

//the point that the user clicked on
Point selection = Point();

// Function which defines what happens when the left mouse button is clicked
void CallBackFunc(int event, int x, int y, int flags, void* userdata) 
{
	//events:
	//EVENT_RBUTTONDOWN
	//EVENT_LBUTTONDOWN
	//EVENT_MBUTTONDOWN
	//EVENT_MOUSEMOVE
	if  ( event == EVENT_LBUTTONDOWN )
    {
        cout << "Left button of the mouse is clicked - position (" << x << ", " << y << ")" << endl;
		selection =  Point(x,y);
    }
}

int main(int, char**)
{
	// Open the default camera
    VideoCapture cap(0); 
	// Check if camera is open
    if(!cap.isOpened()) 
	{
		cout << "There seems to be a problem with the camera..." << endl;
        cin.get();

		return -1;
	}

    Mat frame, gray, bw, result;
	vector<vector<Point>> contours;
    vector<Point> approx;
	string shape = "";
	int red, green, blue;

	for(;;)
    {
		// get a new frame from camera
        cap >> frame;

        // Convert to grayscale
        cvtColor(frame, gray, CV_BGR2GRAY);
 
       //redue noise with a kernel 3x3
       blur(gray, bw, Size(3,3));
	   Canny(gray, bw, 80, 240, 3); 
       //imshow("bw", bw);
       //bitwise_not(bw, bw);
 
       // Find contours
       findContours(bw.clone(), contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);

       frame.copyTo(result);

	   Shape myShape = Shape();
	   for (int i = 0; i < contours.size(); i++)
       {
		   // Approximate contour with accuracy proportional to the contour perimeter
           approxPolyDP(Mat(contours[i]), approx, arcLength(Mat(contours[i]), true)*0.02, true);
 
           // Skip small or non-convex objects
           if (fabs(contourArea(contours[i])) < 100 || !isContourConvex(approx))
              continue;
				
		   if (approx.size() == 3)
           {
				shape = "triangle";
				red = 0; green = 255; blue = 0; //green
		   } else if (approx.size() >= 4 && approx.size() <= 6)
           {
				// Number of vertices of polygonal curve
                int vertices = approx.size();
                   
                // Use the number of vertices to identify the shape of the contour
                if (vertices == 4 )
                {
					shape = "rectangle";
					red = 255; green = 0; blue = 0; //red
				}
				else if (vertices == 5)
                { 
					shape = "pentagon";
					red = 127; green = 0; blue = 255; //purple
				}
				else if (vertices == 6)
                { 
					shape = "hexagon";
					red = 255; green = 0; blue = 255; //pink
				}
			}else
		    {
				// Circle
				double area = contourArea(contours[i]);
				Rect r = boundingRect(contours[i]);
				int radius = r.width / 2;
 
				if (abs(1 - ((double)r.width / r.height)) <= 0.2 && abs(1 - (area / (CV_PI * (radius*radius)))) <= 0.2)
				{
					shape = "circle";
					red = 255; green = 255; blue = 0; //yellow
				}
			}

			myShape = Shape(contours, result, i, selection, shape, red, green, blue);	
			myShape.drawShape();
			selection = myShape.getSelection();		
		}//contours loop

		imshow("Camera Feed", frame);
        imshow("Result", result);	

		//set the callback function for any mouse event
		setMouseCallback("Result", CallBackFunc, NULL);
  
		//press q to terminate the application
		if(waitKey(30) == 'q') break;

	}//end for(;;)

	cap.release();

	return 0;
}