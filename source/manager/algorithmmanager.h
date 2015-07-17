#ifndef ALGORITHMMANAGER_H
#define ALGORITHMMANAGER_H

#include "ui_mainwindow.h"
#include "comparer/histogramcomparer.h"
#include "comparer/ssimcomparer.h"
#include "comparer/differencecomparer.h"

class AlgorithmManager : public QObject
{
    Q_OBJECT

    static const QString HISTOGRAM_ID;
    static const QString SSIM_ID;
    static const QString DIFFERENCE_ID;

public:
    AlgorithmManager(Ui::MainWindow *ui);
    ~AlgorithmManager();

    void compare(DataSet &dataSet);

signals:
    void notifyResult(const DataSet &dataSet, QString &algorithm);

private slots:
    void setThreshold(int threshold) { this->threshold = threshold / 100.; }
    void setAlgorithm(QString algorithm) { this->algorithm = algorithm; }
    void setData(const DataSet &dataSet);

private:
    Ui::MainWindow *ui;
    Comparer *histogramComparer;
    Comparer *ssimComparer;
    Comparer *differenceComparer;
    QString algorithm;
    double threshold;
};

#endif // ALGORITHMMANAGER_H
