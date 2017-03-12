#include <iostream>
#include <unistd.h>
#include <vector>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "pixelSort.h"

void trackBarCallback(int, void* dataPtr)
{
    CannyParams* cparams = (CannyParams*)dataPtr;
    cparams->edges = cannyEdgeDetect(cparams->source, cparams->lowThreshold);
    cv::imshow("edges", cparams->edges);
}

int main(int argc, char* arg[])
{
    //cv::Mat input = cv::imread("lenna.jpg");
    cv::Mat input = cv::imread("japan.jpg");
    //cv::Mat output = cannySortColumn(input, 100);
    CannyParams cparams;
    cparams.source = input;

    cv::namedWindow("edges", 1);
    cparams.edges = cannyEdgeDetect(cparams.source, cparams.lowThreshold);
    cv::createTrackbar("Min Threshold:"
                        , "edges"
                        , &cparams.lowThreshold
                        , 100
                        , trackBarCallback
                        , &cparams
                        );
    cv::imshow("edges", cparams.edges);
    cv::waitKey(0);
    //cv::namedWindow("sort", 1);
    //cv::namedWindow("raw", 1);
    //cv::imshow("sort", output);
    //cv::imshow("raw", input);
    //cv::imwrite("output.jpg", output );
    return 0;
}
