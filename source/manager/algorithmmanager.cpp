#include "algorithmmanager.h"

const QString AlgorithmManager::HISTOGRAM_ID = "Histogram comparison";
const QString AlgorithmManager::SSIM_ID = "SSIM";
const QString AlgorithmManager::DIFFERENCE_ID = "Difference comparison";

AlgorithmManager::AlgorithmManager(Ui::MainWindow *ui) :
    ui(ui),
    histogramComparer(new HistogramComparer()),
    ssimComparer(new SSIMComparer()),
    differenceComparer(new DifferenceComparer())
{
    ui->algorithmBox->addItem(HISTOGRAM_ID);
    ui->algorithmBox->addItem(SSIM_ID);
    ui->algorithmBox->addItem(DIFFERENCE_ID);
    algorithm = ui->algorithmBox->currentText();
    QObject::connect(ui->thresholdSlider, SIGNAL(valueChanged(int)), this, SLOT(setThreshold(int)));
    QObject::connect(ui->algorithmBox, SIGNAL(currentIndexChanged(QString)), this, SLOT(setAlgorithm(QString)));
    QObject::connect(histogramComparer, SIGNAL(notifyProgress(QString)), ui->statusBar, SLOT(showMessage(QString)));
    QObject::connect(ssimComparer, SIGNAL(notifyProgress(QString)), ui->statusBar, SLOT(showMessage(QString)));
    QObject::connect(differenceComparer, SIGNAL(notifyProgress(QString)), ui->statusBar, SLOT(showMessage(QString)));
    QObject::connect(histogramComparer, SIGNAL(notifyResult(const DataSet&)), this, SLOT(setData(const DataSet&)));
    QObject::connect(ssimComparer, SIGNAL(notifyResult(const DataSet&)), this, SLOT(setData(const DataSet&)));
    QObject::connect(differenceComparer, SIGNAL(notifyResult(const DataSet&)), this, SLOT(setData(const DataSet&)));
}

AlgorithmManager::~AlgorithmManager()
{
    delete histogramComparer;
    delete ssimComparer;
    delete differenceComparer;
}

void AlgorithmManager::compare(DataSet &dataSet)
{
    if(algorithm == HISTOGRAM_ID)
    {
        histogramComparer->setDataSet(dataSet);
        histogramComparer->setThreshold(threshold);
        histogramComparer->start();
    }
    else if(algorithm == SSIM_ID)
    {
        ssimComparer->setDataSet(dataSet);
        ssimComparer->setThreshold(threshold);
        ssimComparer->start();
    }
    else if(algorithm == DIFFERENCE_ID)
    {
        differenceComparer->setDataSet(dataSet);
        differenceComparer->setThreshold(threshold);
        differenceComparer->start();
    }
    else
    {
        ui->statusBar->showMessage("No algrithm choosen");
    }
}

void AlgorithmManager::setData(const DataSet &dataSet)
{
    emit notifyResult(dataSet, algorithm + " " + QString::number(threshold));
}
