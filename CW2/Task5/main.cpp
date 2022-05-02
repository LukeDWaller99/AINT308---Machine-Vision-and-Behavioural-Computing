//James Rogers Mar 2022 (c) Plymouth University
#include<iostream>
#include<fstream>
#include<opencv2/opencv.hpp>

using namespace std;
using namespace cv;

// a drawing function that can draw a line based on rho and theta values.
// useful for drawing lines from the hough line detector.
void lineRT(Mat &Src, Vec2f L, Scalar color, int thickness){
    Point pt1, pt2;
    double a = cos(static_cast<double>(L[1]));
    double b = sin(static_cast<double>(L[1]));
    double x0 = a*static_cast<double>(L[0]);
    double y0 = b*static_cast<double>(L[0]);
    pt1.x = cvRound(x0 + 10000*(-b));
    pt1.y = cvRound(y0 + 10000*(a));
    pt2.x = cvRound(x0 - 10000*(-b));
    pt2.y = cvRound(y0 - 10000*(a));
    line(Src, pt1, pt2, color, thickness, LINE_AA);
}

int main()
{


    //Open video file
    VideoCapture CarVideo("../Task5/DashCam.mp4");
    if(!CarVideo.isOpened()){
        cout<<"Error opening video"<<endl;
        return -1;
    }

    //main program loop
    while(true){

        //open the next frame from the video file, or exit the loop if its the end
        Mat Frame;
        CarVideo.read(Frame);

        if(Frame.empty()){
            break;
        }

        //==========================Your code goes here==========================
        Mat greyFrame, detectedEdges;
        int lowThreshold = 20;
        int ratio = 10;
        int kernel_size = 3;

        int width = Frame.cols;

        // create a rectangle the width of the screen
        // and half the height of the screen

        Point start(0,0);

        Point end(width, Frame.rows/2);

        // Convert Frame to Grey
        cvtColor(Frame,greyFrame,COLOR_BGR2GRAY);

        // Blur Image
        blur(greyFrame, detectedEdges, Size(3,3));

        rectangle(detectedEdges, start, end, Scalar(255,255,255), -1);

        // Canny Edge Detection
        Canny(detectedEdges, detectedEdges,
              lowThreshold, lowThreshold*ratio,kernel_size);

        // Find coordinates of road lines
        vector<Vec2f> lines;
        int rho = 1;                // resolution parameter (rho) in pixels
        double theta = CV_PI/450;   // resolution parameter (theta) in radians
        int thershold = 230;        // Minimum number of interesctions to 'detect' a line
        HoughLines(detectedEdges, lines, rho, theta, thershold, 0, 0);


        int q = lines.size(); // constant used for loop interation
        int bottomOfLine = Frame.rows -1; // bottom y coordinate of detected road lanes
        int topOfLine = Frame.rows - 300; // top y corrdinate of detected road lanes
        vector< Point> cornersOfLane; // vector of four points, corners of drawn lane region
        int xVals[4]; // values of the x-coordinates of the detected lines
        int prevX[4]; // old coordinates of x-coordinates of the detected lines
        int currX[4]; // new coordinates of x-coordinates of the detected lines
        float upperbound = 1.01; // upper limit of difference in detected lines
        float lowerbound = 0.99; // lower limit of difference in detected lines

        // iterate through number of lines found
        for (int i = 0;i < (int)lines.size(); i++) {
            // output the lines found using the Hough lines algorithm
            // lineRT(Frame, lines[i], Scalar(0,0,255), 2);

            // iterate through the desired y coordinates for lines
            for(int g = Frame.rows; g > Frame.rows - 300; g--){

                // iterate through the number of lines found for each given y value
                for(int k = 0; k < q; k++){

                    if(lines[k][1]<=1){ // if the angle of the lines (in radians) is less than 1

                        // equations used to extract x coordinate for a given y value
                        currX[0] = (lines[k][0]/cos(lines[k][1]) - (topOfLine*tan(lines[k][1])));
                        currX[2] = (lines[k][0]/cos(lines[k][1]) - (bottomOfLine*tan(lines[k][1])));

                        // if the current value is within a threshold ( to reduce jittering)
                        if ((currX[0] >= lowerbound*prevX[0])&&(currX[0] <= upperbound*prevX[0])){
                            xVals[0] = prevX[0] * 0.9 + currX[0] * 0.1;
                        } else {
                            xVals[0] = prevX[0];
                        }

                        // if the current value is within a threshold ( to reduce jittering)
                        if ((currX[2] >= lowerbound*prevX[2])&&(currX[2] <= upperbound*prevX[2])){
                            xVals[2] = prevX[2] * 0.9 + currX[2] * 0.1;

                        } else {
                            xVals[2] = prevX[2];
                        }

                        // update current value
                        prevX[0] = currX[0];
                        prevX[2] = currX[2];

                    } else if(lines[k][1]>=2.3){ // if the angle of the lines (in radians) is greater than 2.3

                        // equations used to extract x coordinate for a given y value
                        currX[1] = (lines[k][0]/cos(lines[k][1]) - (topOfLine*tan(lines[k][1])));
                        currX[3] = (lines[k][0]/cos(lines[k][1]) - (bottomOfLine*tan(lines[k][1])));

                        // if the current value is within a threshold ( to reduce jittering)
                        if ((currX[1] >= lowerbound*prevX[1])&&(currX[1] <= upperbound*prevX[1])){
                            xVals[1] = prevX[1] * 0.9 + currX[1] * 0.1;
                        } else {
                            xVals[1] = prevX[1];
                        }

                        // if the current value is within a threshold ( to reduce jittering)
                        if ((currX[3] >= lowerbound*prevX[3])&&(currX[3] <= upperbound*prevX[3])){
                            xVals[3] = prevX[3] * 0.9 + currX[3] * 0.1;
                        } else {
                            xVals[3] = prevX[3];
                        }

                        // update current value
                        prevX[1] = currX[1];
                        prevX[3] = currX[3];

                    }
                }
            }

            // map the line in the middle of the road
            Point topMiddleOfLane (((xVals[0] + xVals[1]) / 2), topOfLine);
            Point bottomMiddleOfLane (((xVals[2] + xVals[3]) / 2), bottomOfLine);

            // plot line in the middle of the road
            line(Frame, topMiddleOfLane, bottomMiddleOfLane, Scalar(255,0,0), 2);

            // add previously calculated values
            cornersOfLane.push_back(Point(xVals[0], topOfLine));
            cornersOfLane.push_back(Point(xVals[1], topOfLine));
            cornersOfLane.push_back(Point(xVals[3], bottomOfLine));
            cornersOfLane.push_back(Point(xVals[2], bottomOfLine));

            // draw the detected lane onto the frame
            Mat overlayFrame;
            double alpha = 0.2;
            Frame.copyTo(overlayFrame);
            const Point *pts = (const cv::Point*) Mat(cornersOfLane).data;
            int npts = Mat(cornersOfLane).rows;
            fillPoly(overlayFrame, &pts, &npts, 1, Scalar(0, 255, 0));
            addWeighted(overlayFrame, alpha, Frame, 1 - alpha, 0, Frame);
        }

        //display frames
        imshow("Grey Scale - 10618407", greyFrame);
        imshow("Canny Detected Lines - 10618407", detectedEdges);
        imshow("Output Video of Detected Lane - 10618407", Frame);
        waitKey(10);
    }
}


