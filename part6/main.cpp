#include<iostream>
#include<opencv2/opencv.hpp>
#include"matrix.hpp"
using namespace std;
matrix<float> preprocess(const cv::Mat& image){
    if (image.empty())
    {
        throw invalid_argument("readimage failed");
    }
    cv::Mat resized;
    cv::resize(image,resized,cv::Size(28,28));
    matrix<float> input(1,784);
    for (int r = 0; r < 28; r++)
    {
        for (int c = 0; c < 28; c++)
        {
            input.set(0,r*28+c,resized.at<uchar>(r,c)/255.0f);
        }
        
    }
    return input;
}
int main(){
    cv::Mat image = cv::imread("../nums/0.png",cv::IMREAD_GRAYSCALE);
    matrix<float> input = preprocess(image);
    input.print();
    return 0;
}