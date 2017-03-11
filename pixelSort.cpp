#include "pixelSort.h"
cv::Mat splitThenSort(cv::Mat input)
{
    cv::Mat output;
    cv::Mat outputChannelArray[3];
    cv::Mat inputChannelArray[3];

    cv::split(input, inputChannelArray);

    for(int ii = 0; ii < 3; ii++)
    {
        cv::sort(inputChannelArray[ii], 
                 outputChannelArray[ii],
                 CV_SORT_EVERY_ROW + CV_SORT_ASCENDING
                 );
    }

    cv::merge(outputChannelArray, 3, output);

    return output;
}

cv::Mat cannyThreshold(cv::Mat input)
{
    cv::Mat output = cv::Mat(input.size(), input.type());
    cvtColor(input, output, CV_BGR2GRAY);
    blur(output, output, cv::Size(5,5));
    cv::Canny(output, output, 80,240, 3);
    cv::namedWindow("canny", 1);
    return output;
}

//** this function must accept a 1 dimensional 3 channel row matrix
cv::Mat sortRowVector(cv::Mat input)
{
    cv::Mat inRowChannelArray[3];
    cv::Mat outRowChannelArray[3];
    cv::Mat tempRow(1, input.cols, input.type());

    split(input, inRowChannelArray);
    for(int jj = 0; jj < 3; jj++)
    {
        outRowChannelArray[jj] = cv::Mat(1, input.cols, inRowChannelArray[jj].type());
    }

    for(int jj = 0; jj < 3; jj++)
    {
        cv::Mat ndxRow;
        cv::sortIdx(inRowChannelArray[jj], 
                    ndxRow, 
                    //CV_SORT_EVERY_COLUMN + CV_SORT_ASCENDING);
                    CV_SORT_EVERY_ROW + CV_SORT_DESCENDING);
        for(int mm = 0; mm < ndxRow.cols; mm++)
        {
            outRowChannelArray[jj].at<unsigned char>(mm) =
                inRowChannelArray[jj].at<unsigned char>(ndxRow.at<int>(mm));
        }

        merge(outRowChannelArray, 3, tempRow);
    }

    tempRow;

    return tempRow;
}

cv::Mat indxOneChannel(cv::Mat input)
{
    cv::Mat output = cv::Mat(input.rows, input.cols, input.type());

    for(int ii = 0; ii < input.rows; ii++)
    {
        cv::Mat temp = sortRowVector(input.row(ii));
        temp.copyTo(output.row(ii));
    }

    return output;
}

cv::Mat partitionThenSort(cv::Mat input, cv::Mat edges)
{
    cv::Mat output = input;
    int begin = 0;
    int end = 0;

    while(end < input.cols)
    {
        while(edges.at<unsigned char>(end) != 0 && 
             end < input.cols)
            end++;
        while(edges.at<unsigned char>(end) == 0 && 
             end < input.cols)
            end++;

        cv::Mat slice = input(cv::Rect(begin, 0, end - begin, 1));
        slice = sortRowVector(slice);
        slice.copyTo(output(cv::Rect(begin, 0, end - begin, 1)));

        while(edges.at<unsigned char>(end) != 0 && 
             end < input.cols)
            end++;

        begin = end;
        end = end++;
    }

    return output;
}

cv::Mat cannySortRow(cv::Mat input)
{
    cv::Mat output = cv::Mat(input.rows, input.cols, input.type());
    //cv::Mat output = input;
    cv::Mat edges = cannyThreshold(input);
    
    for(int ii = 0; ii < input.rows; ii++)
    {
        cv::Mat tempRow = partitionThenSort(input.row(ii), edges.row(ii));
        tempRow.copyTo(output.row(ii));
    }

    return output;
}

cv::Mat cannySortColumn(cv::Mat input)
{
    cv::Mat output = cv::Mat(input.rows, input.cols, input.type());
    //cv::Mat output = input;
    cv::Mat edges = cannyThreshold(input);
    
    for(int ii = 0; ii < input.cols; ii++)
    {
        cv::Mat tempRowImg;
        cv::Mat tempColImg;
        cv::Mat tempRowEdg;

        cv::transpose(input.col(ii), tempRowImg);
        cv::transpose(edges.col(ii), tempRowEdg);

        tempRowImg = partitionThenSort(
                            tempRowImg, 
                            tempRowEdg);

        cv::transpose(tempRowImg, tempColImg);
        tempColImg.copyTo(output.col(ii));
    }

    return output;
}

cv::Mat sliceVert(cv::Mat input)
{
    cv::Mat output = cv::Mat(input.rows, input.cols, input.type());
    cv::Mat sliceAin = cv::Mat(input, cv::Rect(0, 0, input.cols/2, input.rows));
    cv::Mat sliceBin = cv::Mat(input, cv::Rect(input.cols/2, 0, input.cols/2, input.rows));
    cv::Mat sliceAout = cv::Mat(sliceAin.rows, sliceAin.cols, sliceAin.type());
    cv::Mat sliceBout = cv::Mat(sliceBin.rows, sliceBin.cols, sliceBin.type());
    sliceAin.copyTo(output(cv::Rect(input.cols/2, 0, input.cols/2, input.rows)));

    //for(int ii = 0; ii < input.rows; ii++)
    //{
    //    cv::Mat temp;
    //    temp = sortRowVector(sliceAin.row(ii));
    //    temp.copyTo(sliceAout.row(ii));
    //    temp = sortRowVector(sliceBin.row(ii));
    //    temp.copyTo(sliceBout.row(ii));
    //}

    //cv::hconcat(sliceAout, sliceBout, output);

    return output;
}
