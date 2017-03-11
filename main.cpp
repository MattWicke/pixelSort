#include <iostream>
#include <unistd.h>
#include <vector>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "pixelSort.h"


typedef struct Pixel
{
    unsigned char b;
};



int main(int argc, char* arg[])
{
    //cv::Mat input = cv::imread("lenna.jpg");
    cv::Mat input = cv::imread("japan.jpg");
    cv::Mat output = cannySortColumn(input);

    cv::namedWindow("sort", 1);
    cv::namedWindow("raw", 1);
    cv::imshow("sort", output);
    cv::imshow("raw", input);
    cv::imwrite("output.jpg", output );
    cv::waitKey(0);
    return 0;
}
