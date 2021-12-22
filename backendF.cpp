#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>

using namespace std;
using namespace cv;

void addNoise(Mat &src)
{
    Mat noise(src.size(), src.type());
    float m = (10,12,34);
    float sigma = (1,5,50);
    cv::randn(noise, m, sigma);
    src += noise;
}

void colorize(Mat &src)
{
    Mat filter = (Mat_<double>(3, 3) <<17.8824, 43.5161, 4.11935, 
                                   3.45565, 27.1554, 3.86714, 
                                   0.0299566, 0.184309, 1.46709);
    Mat output;
    filter2D(src, output, -1, filter);
    src = output;
}


void solarize(Mat &image)
{
     Mat new_image = Mat::zeros( image.size(), image.type() );
    double alpha = 2.0; /*< Simple contrast control */
    int beta = 55;       /*< Simple brightness control */
    for( int y = 0; y < image.rows; y++ ) {
        for( int x = 0; x < image.cols; x++ ) {
            for( int c = 0; c < image.channels(); c++ ) {
                new_image.at<Vec3b>(y,x)[c] =
                  saturate_cast<uchar>( alpha*image.at<Vec3b>(y,x)[c] + beta );
            }
        }
    }
    image = new_image;
}

double compare(Mat image, Mat image2)
{
    Mat scoreImg;
    double maxScore;

    matchTemplate(image, image2, scoreImg, TM_CCOEFF_NORMED);
    minMaxLoc(scoreImg, 0, &maxScore);
    
    return maxScore;

}

void combine(Mat &img1, Mat img2)
{
    int rows = max(img1.rows, img2.rows);
    int cols = img1.cols + img2.cols;

    // Create a black image
    Mat3b res(rows, cols, Vec3b(0,0,0));

    // Copy images in correct position
    img1.copyTo(res(Rect(0, 0, img1.cols, img1.rows)));
    img2.copyTo(res(Rect(img1.cols, 0, img2.cols, img2.rows)));

    // Show result
    img1 = res;

}

void combine2(Mat &imag1, Mat imag2)
{
    cv::resize(imag1, imag1,Size(512,512));
    cv::resize(imag2, imag2,Size(512,512));
    cv::addWeighted(imag1, 0.3, imag2, 0.7, 0.0, imag1);

}

int main(int argc, char** argv )
{

    Mat image;
    Mat image2;
    image = imread("D:/8vo semestre/Programacion paralela/GPU_1/GPU_TAREA_1_GREY/img-to-grey/img-to-grey/test_image.jpg");
    image2 = imread("D:/8vo semestre/Programacion paralela/GPU_1/GPU_TAREA_1_GREY/img-to-grey/img-to-grey/result.jpg");

    //NOISE
    //addNoise(image);
    //COLORIZE
    //colorize(image);
    //SOLARIZE
    //solarize(image);
    //COMPARE
    //cout << "Este es el porcentaje de similitud: " << compare(image, image2) << '\n';
    //COMBINE
    combine2(image, image2);

    namedWindow("Display Image", WINDOW_AUTOSIZE );
    imshow("Display Image", image);
    waitKey(0);
    return 0;
}