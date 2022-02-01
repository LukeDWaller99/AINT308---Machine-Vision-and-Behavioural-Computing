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



















        //display the results untill x is pressed
        while(waitKey(10)!='x'){
            imshow("Target", Component);
            imshow("PCB", PCB);
        }

    }

}
