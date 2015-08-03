#ifndef DATASTRUCTURES
#define DATASTRUCTURES

#include <QString>
#include <QVector>

#include "constants.h"
#include "plot/qcustomplot.h"

extern const QString ST_IT_SEPARATOR;
extern const QString IMAGE_FILE_EXTENSION;

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
Q_DECLARE_METATYPE(Image)

struct DataSet
{
    QString path;
    QString prefix;
    int stageLength;
    int iterationLength;
    QVector<Image> images;

    QString getName(int position)
    {
        Image image = images.at(position);
        return prefix + image.stage + ST_IT_SEPARATOR + image.iteration + "." + IMAGE_FILE_EXTENSION;
    }
};
Q_DECLARE_METATYPE(DataSet)

#endif // DATASTRUCTURES

