#ifndef COMPARER_H
#define COMPARER_H

#include <QThread>

#include "util/datastructures.h"

class Comparer : public QThread
{
    Q_OBJECT
public:
    Comparer();
    virtual ~Comparer();

    virtual void compare() = 0;
    void setDataSet(DataSet &dataSet);
    void setThreshold(double threshold) { this->threshold = threshold; }

signals:
    void notifyProgress(const QString &message);
    void notifyResult(const DataSet &dataSet);   

protected:
    DataSet dataSet;
    DataSet result;
    double threshold;
    int position;

    bool hasNextTwo();
    Image getCurrentImage();
    Image getNextImage();
    QString getNext();

private:
    virtual void run();

};

#endif // COMPARER_H
