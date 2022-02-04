//James Rogers Jan 2021 (c) Plymouth University
#include<iostream>
#include<opencv2/opencv.hpp>

using namespace std;
using namespace cv;

int main()
{

    //Path to image file
    string Path = "../Task3/PCB Images/";

    //loop through component images
    for(int n=0; n<10; ++n){

        //read PCB and component images
        Mat PCB = imread(Path+"PCB.png");
        Mat Component = imread(Path+"Component"+to_string(n)+".png");

        //================Your code goes here=====================

        // get the image output from match
        Mat imageMatch;
        matchTemplate(PCB, Component, imageMatch, TM_SQDIFF_NORMED);

        // values and locations of matching sections
        double maxValue, minValue;
        Point maxLocation, minLocation;
        minMaxLoc(imageMatch, &minValue, &maxValue, &minLocation, &maxLocation);

        // check if the matched pixels for components are below the minimum acceptable amount
        if (minValue > 0.009) { cout << "\n Error... Component not found!" << endl; }
        else {
            Point componentRectange(minLocation.x + Component.cols, minLocation.y + Component.rows);
            rectangle(PCB, minLocation, componentRectange, Scalar(0, 0, 255), 2);
        }

        //display the results untill x is pressed
        while(waitKey(10)!='x'){
            imshow("Target", Component);
            imshow("PCB", PCB);
        }

    }

}
