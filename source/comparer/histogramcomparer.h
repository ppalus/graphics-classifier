#ifndef HISTOGRAMCOMPARER_H
#define HISTOGRAMCOMPARER_H

#include <QTime>
#include <QString>

#include <opencv2/opencv.hpp>

#include "comparer.h"

class HistogramComparer : public Comparer
{
public:
    HistogramComparer();

    virtual void compare();
};

#endif // HISTOGRAMCOMPARER_H
