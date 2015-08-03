#ifndef DATAMANAGER_H
#define DATAMANAGER_H

#include <cmath>
#include <QColor>

#include "ui_mainwindow.h"
#include "util/loader.h"
#include "util/datastructures.h"
#include "plot/qcustomplot.h"

//TODO dataSlot
class DataManager : public QObject
{
    Q_OBJECT

    static const int RESULT_MAX_COUNT;
    static const QColor PLOT_COLOR[];
    static const QCPScatterStyle::ScatterShape PLOT_SHAPE[];

    struct ImageData
    {
        QString stage;
        QString iteration;
        qreal x;
        qreal y;
    };

    struct OutputDataSlot
    {
        QString id;
        QCPGraph *graph;
        QListWidget *list;
        QAbstractButton *label;
        bool active;
        DataSet dataSet;
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
    void removeOutputDataByGraph(QCPGraph* graph);

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
    void setActiveData1();
    void setActiveData2();
    void setActiveData3();
    void setActiveData4();
    void setActiveData5();

private slots:
    void setInputData(const DataSet &dataSet);

private:
    void removeOutputData(QString id);
    void moveOutputUp(int index);
    void fixOutputLists();

    Loader loader;
    Ui::MainWindow *ui;
    DataSet input;
    OutputDataSlot output[5];
    QString xCol;
    QString yCol;

};

#endif // DATAMANAGER_H
