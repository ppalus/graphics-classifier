#include "histogramcomparer.h"
#include <qdebug.h>
HistogramComparer::HistogramComparer()
{
}

void HistogramComparer::compare()
{
    position = 0;
    result.images.clear();
    int histSize[] = { 50, 60 };
    float h_ranges[] = { 0, 180 };
    float s_ranges[] = { 0, 256 };
    const float* ranges[] = { h_ranges, s_ranges };
    int channels[] = { 0, 1 };

    result.images.push_back(getCurrentImage());

    QTime timer;
    timer.start();
    while (hasNextTwo())
    {
        notifyProgress("Images compared: " + QString::number(position)
                                   + " (time: " + QString::number(timer.elapsed()/1000) + "s)");

        cv::Mat currHsv;
        cv::Mat nextHsv;
        cv::MatND currHist;
        cv::MatND nextHist;
        cv::cvtColor(cv::imread(getNext().toStdString().c_str()), currHsv, CV_BGR2HSV);
        cv::cvtColor(cv::imread(getNext().toStdString().c_str()), nextHsv, CV_BGR2HSV);

        cv::calcHist(&currHsv, 1, channels, cv::Mat(), currHist, 2, histSize, ranges, true, false);
        cv::normalize(currHist, currHist, 0, 1, cv::NORM_MINMAX, -1, cv::Mat());

        cv::calcHist(&nextHsv, 1, channels, cv::Mat(), nextHist, 2, histSize, ranges, true, false);
        cv::normalize(nextHist, nextHist, 0, 1, cv::NORM_MINMAX, -1, cv::Mat());

        double correlation = cv::compareHist(currHist, nextHist, CV_COMP_CORREL);

        if (correlation < threshold)
        {
            result.images.push_back(getNextImage());
        }
    }

    result.images.push_back(getCurrentImage());

    notifyResult(result);
}
