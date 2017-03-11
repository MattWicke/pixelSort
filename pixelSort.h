#include <iostream>
#include <unistd.h>
#include <vector>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/highgui/highgui.hpp>

cv::Mat splitThenSort(cv::Mat input);
cv::Mat cannyThreshold(cv::Mat input);

//** this function must accept a 1 dimensional 3 channel row matrix
cv::Mat sortRowVector(cv::Mat input);
cv::Mat indxOneChannel(cv::Mat input);
cv::Mat partitionThenSort(cv::Mat input, cv::Mat edges);
cv::Mat cannySortRow(cv::Mat input);
cv::Mat cannySortColumn(cv::Mat input);
cv::Mat sliceVert(cv::Mat input);
