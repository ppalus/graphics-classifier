#ifndef DIFFERENCECOMPARER_H
#define DIFFERENCECOMPARER_H

#include <QTime>
#include <QString>
#include <cmath>

#include <opencv/cv.h>
#include <opencv/highgui.h>

#include "comparer.h"

class DifferenceComparer : public Comparer
{
public:
    DifferenceComparer();

    virtual void compare();

private:
    int calculateDifference(cv::Mat first, cv::Mat second);
};

#endif // DIFFERENCECOMPARER_H
