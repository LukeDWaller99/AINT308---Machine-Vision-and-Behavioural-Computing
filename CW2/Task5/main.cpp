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
        Canny(detectedEdges, detectedEdges, lowThreshold, lowThreshold*ratio,kernel_size);

        // Find coordinates of road lines
        vector<Vec2f> lines;
        int rho = 1;
        double theta = CV_PI/450;
        int thershold = 230;
        HoughLines(detectedEdges, lines, rho, theta, thershold, 0, 0);

        // Add these coodinates to the canny detection frame
        int q = lines.size();
        int bottomOfLine = Frame.rows -1;
        int topOfLine = Frame.rows - 300;
        vector< Point> cornersOfLane;
        int x1 = 0, x2 = 0, x3 = 0, x4 = 0;
        int prevx1 = 0, prevx2 = 0 , prevx3 = 0, prevx4 = 0;
        int currx1 = 0, currx2 = 0, currx3 = 0, currx4 = 0;
        float upperbound = 1.01;
        float lowerbound = 0.99;

        for (int i = 0;i < (int)lines.size(); i++) {
            if((lines[i][0] < -230)||lines[i][0] > 690){
                // lineRT(Frame, lines[i], Scalar(0,0,255), 2);
                for(int g = Frame.rows; g > Frame.rows - 300; g--){
                    for(int k = 0; k < q; k++){
                        if(lines[k][1]<=1){
                            currx1 = (lines[k][0]/cos(lines[k][1]) - (topOfLine*tan(lines[k][1])));
                            currx3 = (lines[k][0]/cos(lines[k][1]) - (bottomOfLine*tan(lines[k][1])));

                            if ((currx1 >= lowerbound*prevx1)&&(currx1 <= upperbound*prevx1)){
                                x1 = prevx1 * 0.9 + currx1 * 0.1;
                            } else {
                                x1 = prevx1;
                            }

                            if ((currx3 >= lowerbound*prevx3)&&(currx3 <= upperbound*prevx3)){
                                x3 = prevx3 * 0.9 + currx3 * 0.1;

                            } else {
                                x3 = prevx3;
                            }
                            prevx1 = currx1;
                            prevx3 = currx3;
                        } else if(lines[k][1]>=2.3){
                            currx2 = (lines[k][0]/cos(lines[k][1]) - (topOfLine*tan(lines[k][1])));
                            currx4 = (lines[k][0]/cos(lines[k][1]) - (bottomOfLine*tan(lines[k][1])));

                            if ((currx2 >= lowerbound*prevx2)&&(currx2 <= upperbound*prevx2)){
                                x2 = prevx2 * 0.9 + currx2 * 0.1;
                            } else {
                                x2 = prevx2;
                            }

                            if ((currx4 >= lowerbound*prevx4)&&(currx4 <= upperbound*prevx4)){
                                x4 = prevx4 * 0.9 + currx4 * 0.1;
                            } else {
                                x4 = prevx4;
                            }
                            prevx2 = currx2;
                            prevx4 = currx4;
                        }
                    }
                }

                cornersOfLane.push_back(Point(x1, topOfLine));
                cornersOfLane.push_back(Point(x2, topOfLine));
                cornersOfLane.push_back(Point(x4, bottomOfLine));
                cornersOfLane.push_back(Point(x3, bottomOfLine));

                Point topMiddleOfLane (((x1 + x2) / 2), topOfLine);
                Point bottomMiddleOfLane (((x3 + x4) / 2), bottomOfLine);

                line(Frame, topMiddleOfLane, bottomMiddleOfLane, Scalar(255,0,0), 2);

                Mat overlayFrame;
                double alpha = 0.2;
                Frame.copyTo(overlayFrame);
                const Point *pts = (const cv::Point*) Mat(cornersOfLane).data;
                int npts = Mat(cornersOfLane).rows;
                fillPoly(overlayFrame, &pts, &npts, 1, Scalar(0, 255, 0));
                addWeighted(overlayFrame, alpha, Frame, 1 - alpha, 0, Frame);

            }
        }

        // x = (r/sin(theta)) -(y*tan(theta))

        //display frame
        imshow("Grey Scale", greyFrame);
        imshow("Video", detectedEdges);
        imshow("Video 2: Electic Boogaloo", Frame);
        waitKey(10);
    }
}


