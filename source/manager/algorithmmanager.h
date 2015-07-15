#ifndef ALGORITHMMANAGER_H
#define ALGORITHMMANAGER_H

#include "ui_mainwindow.h"

class AlgorithmManager
{
    static const QString HISTOGRAM_ID;
    static const QString SSIM_ID;
    static const QString DIFFERENCE_ID;

public:
    AlgorithmManager(Ui::MainWindow *ui);

    void setAlgorithm(QString algorithm) { this->algorithm = algorithm; }

private:
    Ui::MainWindow *ui;
    QString algorithm;
};

#endif // ALGORITHMMANAGER_H
