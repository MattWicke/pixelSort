#include <iostream>
#include <unistd.h>
#include <vector>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/highgui/highgui.hpp>

struct CannyParams
{
    cv::Mat source;
    cv::Mat edges;
    int lowThreshold;
    //double thresh2;
    CannyParams();
};

cv::Mat splitThenSort(cv::Mat input);
cv::Mat cannyEdgeDetect(cv::Mat input, double lowThreshold);

//** this function must accept a 1 dimensional 3 channel row matrix
cv::Mat sortRowVector(cv::Mat input);
cv::Mat indxOneChannel(cv::Mat input);
cv::Mat partitionThenSort(cv::Mat input, cv::Mat edges);
cv::Mat cannySortRow(cv::Mat input, int cannyThresh);
cv::Mat cannySortColumn(cv::Mat input, int cannyThresh);
cv::Mat sliceVert(cv::Mat input);
