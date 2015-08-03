#include "differencecomparer.h"

DifferenceComparer::DifferenceComparer()
{
}

void DifferenceComparer::compare()
{
    position = 0;
    result.images.clear();
    result.images.push_back(getCurrentImage());

    QTime timer;
    timer.start();
    while (hasNextTwo())
    {
        notifyProgress("Images compared: " + QString::number(position)
                                   + " (time: " + QString::number(timer.elapsed()/1000) + "s)");
        cv::Mat first = cv::imread(getNext().toStdString().c_str());
        cv::Mat second = cv::imread(getNext().toStdString().c_str());
        double difference = (double) calculateDifference(first, second)/(first.cols * first.rows);
        if (difference > threshold)
        {
            result.images.push_back(getNextImage());
        }
        first.release();
        second.release();
    }

    result.images.push_back(getCurrentImage());

    notifyResult(result);
}

int DifferenceComparer::calculateDifference(cv::Mat first, cv::Mat second)
{
    if(first.size != second.size)
    {
        return -1;
    }
    int difference = 0;
    for(int i = 0; i < first.rows; ++i)
    {
        for(int j = 0; j < first.cols; ++j)
        {
            unsigned char firstB = first.at<cv::Vec3b>(i,j)[0];
            unsigned char firstG = first.at<cv::Vec3b>(i,j)[1];
            unsigned char firstR = first.at<cv::Vec3b>(i,j)[2];
            unsigned char secondB = second.at<cv::Vec3b>(i,j)[0];
            unsigned char secondG = second.at<cv::Vec3b>(i,j)[1];
            unsigned char secondR = second.at<cv::Vec3b>(i,j)[2];
            if(firstB - secondB != 0 ||
                    firstG - secondG != 0 ||
                    firstR - secondR != 0)
            {
                ++difference;
                continue;
            }

        }
    }
    return difference;
}
