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

        Point end(width, 350);

        // Convert Frame to Grey
        cvtColor(Frame,greyFrame,COLOR_BGR2GRAY);

        // Blur Image
        blur(greyFrame, detectedEdges, Size(3,3));

        rectangle(detectedEdges, start, end, Scalar(255,255,255), -1);

        // Canny Edge Detection
        Canny(detectedEdges, detectedEdges, lowThreshold, lowThreshold*ratio,kernel_size);

        // Find coordinates of road lines
        vector<Vec2f> lines;
         HoughLines(detectedEdges, lines, 1, CV_PI/180, 240, 0, 0);



        // Add these coodinates to the canny detection frame


         int y = Frame.rows/2;
         int q = lines.size();
         int xVals[4];
         int x;

         for (int i = 0; i < lines.size(); i++) {
             cout << lines[i][0] << endl;
             if((lines[i][0] < -230)||lines[i][0] > 680){
                 lineRT(Frame, lines[i], Scalar(0,0,255), 2);
                 for(int g = Frame.rows; g > Frame.rows - 300; g--){
                     y = g;
                     int val = 0;
                     for(int k = 0; k < q; k++){
                         if(lines[k][1]<=1){
                             x = (lines[k][0]/cos(lines[k][1]) - (y*tan(lines[k][1])));
                             circle(Frame, Point(x,y), 2, Scalar(255,0,0), -1);
                         } else if(lines[k][1]>=2.3){
                             x = (lines[k][0]/cos(lines[k][1]) - (y*tan(lines[k][1])));
                             circle(Frame, Point(x,y), 2, Scalar(255,0,0), -1);
                         } else {x = 0;}
                         if(x != 0){
                         }
                     }
                 }
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



