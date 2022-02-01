//James Rogers Jan 2022 (c) Plymouth University
#include <iostream>

#include<opencv2/opencv.hpp>
#include<opencv2/opencv_modules.hpp>

using namespace std;
using namespace cv;

int main(){

    //Path of image folder
    string PathToFolder = "../Task1/Car Images/";

    //Loop through the 30 car images
    for(int n=0; n<30; ++n){

        //Each image is named 0.png, 1.png, 2.png, etc. So generate the image file path based on n and the folder path
        string PathToImage = PathToFolder+to_string(n)+".png";

        cout<<PathToImage<<endl;

        //Load car image at the file paths location
        Mat Car=imread(PathToImage);

        //Your code goes here. The example code below shows you how to read the red, green, and blue colour values of the
        //pixel at position (0,0). Modify this section to check not just one pixel, but all of them in the 640x480 image
        //(using for-loops), and using the RGB values classifiy if a given pixel looks red, green, blue, or other.

//        //==============example code, feel free to delete=============
//        int x=0;
//        int y=0;
//        Vec3b PixelValue = Car.at<Vec3b>(y,x);

//        cout<<"The blue value at (0,0) is " <<(int)PixelValue[0]<<endl;
//        cout<<"The green value at (0,0) is "<<(int)PixelValue[1]<<endl;
//        cout<<"The red value at (0,0) is "  <<(int)PixelValue[2]<<endl;
//        //============================================================

        int blueCount = 0;
        int redCount = 0;
        int greenCount = 0;

        for (int i = 0;i<Car.rows; i++) {
            for (int j = 0;j < Car.cols; j++) {
//                std::cout << Car.at<Vec3b>(i,j)<<endl;
                Vec3b PixelValue = Car.at<Vec3b>(i,j);
                if((PixelValue[0] > 1.5 * PixelValue[1])&&(PixelValue[0] > 1.5 * PixelValue[2])){
                    // pixel is blue
                    blueCount++;
                }
                else if((PixelValue[1] > 1.5 * PixelValue[0])&&(PixelValue[1] > 1.5 * PixelValue[2])){
                    // pixel is green
                    greenCount++;
                }
                else if((PixelValue[2] > 1.5 * PixelValue[1])&&(PixelValue[2] > 1.5 * PixelValue[0])){
                    // pixel is red
                    redCount++;
                }
                else {
                    // inconclusive
                }
            }
        }

        cout<<"The blue value is " << blueCount <<endl;
        cout<<"The green value is " << greenCount <<endl;
        cout<<"The red value is " << redCount <<endl;

        if((blueCount > redCount)&&(blueCount > greenCount)){
             cout<< "The car is blue" <<endl;
        }
        else if((greenCount > redCount)&&(greenCount > blueCount)){
            cout<< "The car is green" <<endl;
        }
        else if((redCount > blueCount)&&(redCount > greenCount)){
            cout<< "The car is red" <<endl;
        }

        //display the car image untill x is pressed
        while(waitKey(10)!='x'){
            imshow("Car", Car);
        }

    }

}




















