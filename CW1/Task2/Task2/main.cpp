//James Rogers Jan 2022 (c) Plymouth University
#include<iostream>
#include <fstream>
#include<opencv2/opencv.hpp>

using namespace std;
using namespace cv;

int main()
{

    VideoCapture InputStream("../Task2/Video.mp4"); //Load in the video as an input stream
    const Point Pivot(592,52);                                      //Pivot position in the video

    //Open output file for angle data
    ofstream DataFile;
    DataFile.open ("../Task2/Data.csv");

    //loop through video frames
    while(true){

        Mat Frame, FrameHSV, FrameFiltered;


        InputStream.read(Frame);

        //if frame is empty then the video has ended, so break the loop
        if(Frame.empty()){
            break;
        }

        //video is very high resolution, reduce it to 720p to run faster
        resize(Frame,Frame,Size(1280,720));

        //======================================================Your code goes here====================================================
        //this code will run for each frame of the video. your task is to find the location of the swinging green target, and to find
        //its angle to the pivot. These angles will be saved to a .csv file where they can be plotted in Excel.

        //load next frame

        cvtColor(Frame, FrameHSV, COLOR_BGR2HSV);
        Vec3b green_lower(30, 50, 70);
        Vec3b green_upper(95, 255, 255);
        inRange(FrameHSV, green_lower, green_upper, FrameFiltered);
        Moments m = moments(FrameFiltered, true);
        Point p(m.m10/m.m00, m.m01/m.m00);
        circle(Frame, p, 20, Scalar(0,255,0), 3);
        line(Frame, Pivot, p, Scalar(255,0,0), 2);
        //double angle = atan2(p.y - Pivot.y, p.x - Pivot.x);
        double angles = atan2(p.x, p.y )/ M_PI_4;
        double degangles = angles * 180 / 3.14;
        cout << "angles are:  " << degangles << endl;
        DataFile << degangles << endl;

        //if frame is empty then the video has ended, so break the loop
        if(FrameFiltered.empty()){
            break;
        }

        //video is very high resolution, reduce it to 720p to run faster
        resize(FrameFiltered,FrameFiltered,Size(1280,720));

        //==============================================================================================================================

        //display the frame
        imshow("Video", Frame);
        imshow("new video", FrameFiltered);
        waitKey(10);
    }

    DataFile.close(); //close output file
}
