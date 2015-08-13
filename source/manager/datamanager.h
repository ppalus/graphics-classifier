#ifndef DATAMANAGER_H
#define DATAMANAGER_H

#include <cmath>
#include <QColor>
#include <QQueue>

#include "ui_mainwindow.h"
#include "util/loader.h"
#include "util/datastructures.h"
#include "plot/qcustomplot.h"

#define RESULT_MAX_COUNT 5

class DataManager : public QObject
{
    Q_OBJECT

    static const QColor PLOT_COLOR[];
    static const QCPScatterStyle::ScatterShape PLOT_SHAPE[];

    //TODO refactor -> remove ImageData
    struct ImageData
    {
        QString stage;
        QString iteration;
        qreal x;
        qreal y;
    };

    struct Output
    {
        QString id;
        QListWidget *list;
        QAbstractButton *label;
        QCPGraph *graph;
        DataSet data;
    };

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
    void removeGraph(QCPGraph* graph);

    DataSet& getInput() { return input; }

    void setXCol(const QString xCol) { this->xCol = xCol; }
    void setYCol(const QString yCol) { this->yCol = yCol; }

public slots:
    void setOutputData(const DataSet &dataSet, const QString algorithm);
    void setOutputList6Content(QString id);
    void clearOutputData1();
    void clearOutputData2();
    void clearOutputData3();
    void clearOutputData4();
    void clearOutputData5();

private slots:
    void setInputData(const DataSet &dataSet);

private:
    void removeOutput(QString id);
    void removeOutput(int index);
    void refreshUI();

    Loader loader;
    Ui::MainWindow *ui;
    DataSet input;
    QList<Output> output;
    QString xCol;
    QString yCol;
};

#endif // DATAMANAGER_H
