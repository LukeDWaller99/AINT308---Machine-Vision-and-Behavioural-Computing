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

        Point point_1(592, 51), point_2(592, 650);
        int thickness = 2;
        int radiusOfCircle = 20;

        line(Frame, // target frame
             point_1, //starting point for line
             point_2, //ending point for line
             Scalar(0, 0, 255), // colour of line
             thickness); //line thickness

        cvtColor(Frame, FrameHSV, COLOR_BGR2HSV);
        Vec3b greenLowerBound(30, 50, 70);
        Vec3b greenUpperBound(95, 255, 255);

        inRange(FrameHSV, greenLowerBound, greenUpperBound, FrameFiltered);
        Moments moment = moments(FrameFiltered, true);

        Point pendulumPoint(moment.m10/moment.m00, //
                moment.m01/moment.m00); //

        circle(Frame, // target frame
               pendulumPoint, // center of circle
               radiusOfCircle, //radius of circle
               Scalar(0,255,0), // colour of circle
               thickness * 1.5); //thickness of line
        line(Frame, // target frame
             Pivot, // starting point of line
             pendulumPoint, // ending point for line
             Scalar(255,0,0), //colour of line
             thickness); //thickness of line

        double angleInRadians = (atan2(pendulumPoint.x, pendulumPoint.y )/ M_PI_4) - M_PI_4;
        char angleInRadiansOutput[30];
        sprintf(angleInRadiansOutput, "Angle in radians: %f", angleInRadians);
        putText(Frame, //target image
                angleInRadiansOutput, // text to be outputted
                Point(800, 30), // top-left position of text
                FONT_HERSHEY_DUPLEX, // font
                1.0, // size of text,
                Scalar(0, 255, 0), // text colour
                thickness);

        double angleInDegrees = angleInRadians * 180 / CV_PI;
        char angleInDegreesOutput[30];
        sprintf(angleInDegreesOutput, "Angle in degrees: %f", angleInDegrees);

        putText(Frame, //target image
                angleInDegreesOutput, //text
                Point(800, 60), // top-left position of text
                FONT_HERSHEY_DUPLEX, // font
                1.0, // size of text
              Scalar(255, 0, 0), // text colour
                thickness);


        cout << "Pendulum angle:  " << angleInDegrees << endl;

        DataFile << angleInDegrees << endl;

        //if frame is empty then the video has ended, so break the loop
        if(FrameFiltered.empty()){
            break;
        }

        //video is very high resolution, reduce it to 720p to run faster
        resize(FrameFiltered,FrameFiltered,Size(1280,720));

        //==============================================================================================================================

        //display the frame
        imshow("Video - 10618407", Frame);
        imshow("New Video - 10618407", FrameFiltered);
        waitKey(10);
    }

    DataFile.close(); //close output file
}
