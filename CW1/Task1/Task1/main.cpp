//James Rogers Jan 2022 (c) Plymouth University
#include <iostream>

#include<opencv2/opencv.hpp>
#include<opencv2/opencv_modules.hpp>

using namespace std;
using namespace cv;

int main(){

    //Path of image folder
    string PathToFolder = "../Task1/Car Images/";

    //Loop through the 30 car images (40 have been added as an expanded dataset)
    for(int n=0; n<70; ++n){

        //Each image is named 0.png, 1.png, 2.png, etc. So generate the image file path based on n and the folder path
        string PathToImage = PathToFolder+to_string(n)+".png";

        cout<<PathToImage<<endl;

        //Load car image at the file paths location
        Mat Car=imread(PathToImage);

        //Your code goes here. The example code below shows you how to read the red, green, and blue colour values of the
        //pixel at position (0,0). Modify this section to check not just one pixel, but all of them in the 640x480 image
        //(using for-loops), and using the RGB values classifiy if a given pixel looks red, green, blue, or other.

        int blueCount = 0;
        int redCount = 0;
        int greenCount = 0;

        for (int i = 0;i<Car.rows; i++) { // runs through all the rows in the image
            for (int j = 0;j < Car.cols; j++) { // runs through all the columns in the image
                Vec3b PixelValue = Car.at<Vec3b>(i,j); // stores the RBG values in the PixelValue vector
                if((PixelValue[0] > 1.5 * PixelValue[1])&&(PixelValue[0] > 1.5 * PixelValue[2])){ // checks if the blue in the pixel is at least 1.5x higher than the other colours
                    // pixel is blue
                    blueCount++; // add the pixel to the blue count
                }
                else if((PixelValue[1] > 1.5 * PixelValue[0])&&(PixelValue[1] > 1.5 * PixelValue[2])){ // checks if the green in the pixel is at least 1.5x higher than the other colours
                    // pixel is green
                    greenCount++; // add the pixel to the green count
                }
                else if((PixelValue[2] > 1.5 * PixelValue[1])&&(PixelValue[2] > 1.5 * PixelValue[0])){ // checks if the red in the pixel is at least 1.5x higher than the other colours
                    // pixel is red
                    redCount++; // add the pixel to the red count
                }
                else {
                    // inconclusive - do not add any values to the count
                }
            }
        }

        cout<<"The blue value is " << blueCount <<endl;
        cout<<"The green value is " << greenCount <<endl;
        cout<<"The red value is " << redCount <<endl;

        if((blueCount > redCount)&&(blueCount > greenCount)){ // if blue is the higest count the car is blue
             cout<< "The car is blue" <<endl;
        }
        else if((greenCount > redCount)&&(greenCount > blueCount)){ // if green is the higest count the car is green
            cout<< "The car is green" <<endl;
        }
        else if((redCount > blueCount)&&(redCount > greenCount)){ // if red is the higest count the car is red
            cout<< "The car is red" <<endl;
        }

        //display the car image untill x is pressed
        while(waitKey(10)!='x'){
            imshow("Car", Car);
        }

    }

}




















