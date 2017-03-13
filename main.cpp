#include <iostream>
#include <unistd.h>
#include <vector>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <string>
#include "pixelSort.h"

void trackBarCallback(int, void* dataPtr)
{
    CannyParams* cparams = (CannyParams*)dataPtr;
    cparams->edges = cannyEdgeDetect(cparams->source, cparams->lowThreshold);
    cv::imshow("edges", cparams->edges);
}

class StateMachine
{
public:
    StateMachine();
    void loadSourceImages(std::string fileName);
    void saveResults();
    void edgeTuningPhase();
    void displayResultsPhase();
    void run();
    cv::Mat processedImage;
    CannyParams cparams;
    char key;
};

StateMachine::StateMachine():
    key(0)
    {}

void StateMachine::loadSourceImages(std::string fileName)
{
    cparams.source= cv::imread(fileName);
}

void StateMachine::edgeTuningPhase()
{
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
    key = cv::waitKey(0);
    cv::destroyWindow("edges");
}

void StateMachine::displayResultsPhase()
{
    processedImage = cannySortColumn(cparams.source, cparams.lowThreshold);
    cv::namedWindow("sorted", 1);
    cv::imshow("sorted", processedImage);
    key = cv::waitKey(0);
    cv::destroyWindow("sorted");
}

void StateMachine::saveResults()
{
    std::string saveName;
    std::cout << "Enter file name to save: " <<std::endl;
    std::cin >> saveName;
    cv::imwrite(saveName, processedImage);
}

void StateMachine::run()
{
    while(key != 'q')
    {
        edgeTuningPhase();
        if(key =='q')
            break;
        displayResultsPhase();
        if(key =='s')
            saveResults();
    }
}

int main(int argc, char* arg[])
{
    StateMachine sm;
    sm.loadSourceImages("japan.jpg");
    sm.run();
    return 0;
}
