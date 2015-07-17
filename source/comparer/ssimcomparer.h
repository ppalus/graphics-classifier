#ifndef SSIMCOMPARER_H
#define SSIMCOMPARER_H

#define NOMINMAX

#include <QTime>
#include <QString>

#include <opencv/cv.h>
#include <opencv/highgui.h>

#include "comparer.h"

class SSIMComparer : public Comparer
{
public:
    SSIMComparer();

    virtual void compare();

private:
    double ssimCalculate(const char* argv1, const char* argv2);
};

#endif // SSIMCOMPARER_H
