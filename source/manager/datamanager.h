#ifndef DATAMANAGER_H
#define DATAMANAGER_H

#include <cmath>
#include <QColor>

#include "ui_mainwindow.h"
#include "util/loader.h"
#include "util/datastructures.h"
#include "plot/qcustomplot.h"

class DataManager : public QObject
{
    Q_OBJECT

    static const int RESULT_MAX_COUNT;
    static const QColor PLOT_COLOR[];
    static const QCPScatterStyle::ScatterShape PLOT_SHAPE[];

public:

    static const QString BX_ID;
    static const QString BY_ID;
    static const QString BZ_ID;
    static const QString MX_ID;
    static const QString MY_ID;
    static const QString MZ_ID;

    explicit DataManager(Ui::MainWindow *ui);
    virtual ~DataManager();

    void loadData(const QString path);
    void plotInput();
    void plotOutput();
    void replot();
    QStringList findImage(qreal b, qreal m);
    void saveResult();
    void removeData(QCPGraph *graph);

    DataSet& getInput() { return input; }

    void setXCol(const QString xCol) { this->xCol = xCol; }
    void setYCol(const QString yCol) { this->yCol = yCol; }

public slots:
    void setOutputData(const DataSet &dataSet, const QString algorithm);

private slots:
    void setInputData(const DataSet &dataSet);

private:
    void showOutputData(QListWidget *outputList, QAbstractButton *button, const QString algorithm);

    Loader loader;
    Ui::MainWindow *ui;
    DataSet input;
    QVector<DataSet> output;
    QString xCol;
    QString yCol;
    int index;
    QMap<int, QCPGraph*> dataPlotMap;

    struct ImageData
    {
        QString stage;
        QString iteration;
        qreal x;
        qreal y;
    };
};

#endif // DATAMANAGER_H
