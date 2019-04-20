#include<opencv2/imgproc.hpp>
#include<opencv2/highgui.hpp>
#include<iostream>

using namespace std;
using namespace cv;

int main(){

    //Read the image
    Mat image = imread("../assets/putin.jpg");

    //Convert to HSV color space
    Mat hsvImage;
    cvtColor(image, hsvImage, COLOR_BGR2HSV);

    //Split the channels
    vector<Mat> channels(3);
    split(hsvImage, channels);

    //Initialize the histogram params
    int histogramSize = 180;
    float range[] = {0, 179};
    const float *ranges[] = {range};

    //parameters for the histogram image
    int histogramImageWidth = histogramSize*4;
    int histogramImageHeight = histogramSize*4;
    int bin_w = 4;

    //calculate histogram. Since the values in Hue channel range from 0 to 179, so we keep number of bin as 180.
    //First argument of calcHist function in channel[0] which is Hue channel, second argument is number of images, 3rd is the
    // channel number of the image which is 0 here as we are just passing single channel, 4th argument is mask which we will send as
    //empty as we want the whole mattrix, 5th argument is the output histogram, 6th argument is dimensions of the histogram which is 1 
    //here. 7th argument is number of bins and 8th argument is range of histogram
    MatND histogram;
    calcHist(&channels[0], 1 , 0, Mat(), histogram, 1, &histogramSize, ranges, true, false);
    cout<<histogram<<endl;

    //normalize the histogram to make sure the histogram fits in the plot
    normalize(histogram, histogram, 0, histogramSize*3, NORM_MINMAX, -1, Mat());

    //construct the histogram as an image
    Mat histogramImage(histogramImageHeight, histogramImageWidth, CV_8UC3, Scalar(255, 255, 255));
    cout<<histogram<<endl;

    //draw x-axis
    line(histogramImage, Point(0, histogramImageHeight - 30), Point(histogramImageWidth, histogramImageHeight - 30), Scalar(0, 0, 0), 2, 8, 0);

    //draw origin
    line(histogramImage, Point(0, histogramImageHeight - 20), Point(0, histogramImageHeight - 40), Scalar(0, 0, 0), 2, 8, 0);
    putText(histogramImage, "0", Point(0, histogramImageHeight-5), FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0,0,0), 1, cv::LINE_AA);

    //draw histogram. We go over each value of the histogram and draw a line between current and previous points. We also put the x-axis 
    // value after every 20 values.
    for(int i = 1; i < histogramSize; i++){
        line(histogramImage, Point((i-1)*bin_w, histogramImageHeight -30 - histogram.at<float>(i-1)), 
            Point(i*bin_w, histogramImageHeight-30- histogram.at<float>(i)), Scalar(0, 0, 255), 2, 8, 0);

        // draw the x axis values
        if (i % 20 == 0){
            string text = to_string(i);
            line(histogramImage, Point(i*bin_w, histogramImageHeight - 20), Point(i*bin_w, histogramImageHeight - 40), Scalar(0, 0, 0), 2, 8, 0);
            putText(histogramImage, text, Point(i*bin_w, histogramImageHeight-5), FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0,0,0), 1, cv::LINE_AA);
        }
    }

    //Create windows to display images
    namedWindow("image", WINDOW_NORMAL);
    namedWindow("hue", WINDOW_NORMAL);
    namedWindow("saturation", WINDOW_NORMAL);
    namedWindow("value", WINDOW_NORMAL);
    namedWindow("histimage", WINDOW_NORMAL);

    //Display images
    imshow("image", image);
    imshow("hue", channels[0]);
    imshow("saturation", channels[1]);
    imshow("value", channels[2]);
    imshow("histimage", histogramImage);

    //Press esc to exit the program
    waitKey(0);

    //Close all the opened windows
    destroyAllWindows();
    return 0;
}