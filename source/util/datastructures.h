#ifndef DATASTRUCTURES
#define DATASTRUCTURES

#include <QString>

struct Image
{
    QString stage;
    QString iteration;
    qreal bx;
    qreal by;
    qreal bz;
    qreal mx;
    qreal my;
    qreal mz;
};
Q_DECLARE_METATYPE(Image);

struct DataSet
{
    QString path;
    QString prefix;
    int stageLength;
    int iterationLength;
    QVector<Image> images;
};
Q_DECLARE_METATYPE(DataSet);

#endif // DATASTRUCTURES

