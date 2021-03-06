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
    enum Mode
    {
        HORZ,
        VERT
    }mode;
    StateMachine();
    void loadSourceImages(std::string fileName);
    void saveResults();
    void edgeTuningPhase();
    void displayResultsPhase();
    void run();
    void process();
    void setMode(Mode nMode);
    cv::Mat processedImage;
    CannyParams cparams;
    char key;
};

StateMachine::StateMachine():
    key(0),
    mode(HORZ)
    {}

void StateMachine::loadSourceImages(std::string fileName)
{
    cparams.source= cv::imread(fileName);
}

void StateMachine::setMode(StateMachine::Mode nMode)
{
    mode = nMode;
}

void StateMachine::edgeTuningPhase()
{
    cv::namedWindow("edges", CV_WINDOW_NORMAL);
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

void StateMachine::process()
{
    switch(mode)
    {
        case VERT:
            processedImage = cannySortColumn(cparams.source, cparams.lowThreshold);
        break;

        case HORZ:
            processedImage = cannySortRow(cparams.source, cparams.lowThreshold);
        break;
    }
}

void StateMachine::displayResultsPhase()
{
    cv::namedWindow("sorted", CV_WINDOW_NORMAL);
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
        if(key =='h')
            setMode(HORZ);
        if(key =='v')
            setMode(VERT);
        process();
        displayResultsPhase();
        if(key =='h')
            setMode(HORZ);
        if(key =='v')
            setMode(VERT);
        if(key =='s')
            saveResults();
    }
}

int main(int argc, char* argv[])
{
    StateMachine sm;
    sm.loadSourceImages(std::string(argv[1]));
    
    if(argc > 2)
    {
        switch(argv[2][0])
        {
            case 'h':
            sm.setMode(StateMachine::HORZ);
            break;

            case 'v':
            sm.setMode(StateMachine::VERT);
            break;
        }
    }
    sm.run();
    return 0;
}
