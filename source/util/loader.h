#ifndef LOADER_H
#define LOADER_H

#include <QThread>
#include <QDirIterator>
#include <QVector>
#include <QTextStream>

#include "util/datastructures.h"

extern const QString IMAGE_FILE_EXTENSION;
extern const QString DATA_FILE_EXTENSION;
extern const QString ST_IT_SEPARATOR;

class Loader : public QThread
{
    Q_OBJECT
public:
    Loader();

    void setPath(const QString path) { this->path = path; }

signals:
    void notifyProgress(const QString &message);
    void notifyResult(const DataSet &dataSet);

private:
    virtual void run();
    int getColumnIndex(QString line, QString contain, QString notContain = "");

    QString path;
};

#endif // LOADER_H
