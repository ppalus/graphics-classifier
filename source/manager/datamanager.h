#ifndef DATAMANAGER_H
#define DATAMANAGER_H

#include "ui_mainwindow.h"
#include "util/loader.h"
#include "util/datastructures.h"

extern const QString ST_IT_SEPARATOR;
extern const QString IMAGE_FILE_EXTENSION;

class DataManager : public QObject
{
    Q_OBJECT
public:
    explicit DataManager(Ui::MainWindow *ui);
    virtual ~DataManager();

    void loadData(const QString path);

public slots:
    void setData(DataSet dataSet);

private:

    Loader loader;
    Ui::MainWindow *ui;
    DataSet input;
};

#endif // DATAMANAGER_H
