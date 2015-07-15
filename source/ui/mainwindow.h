#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QAbstractButton>

#include "manager/datamanager.h"
#include "manager/algorithmmanager.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_outputList1Button_clicked();

    void on_outputList2Button_clicked();

    void on_outputList3Button_clicked();

    void on_outputList4Button_clicked();

    void on_outputList5Button_clicked();

    void on_compareButton_clicked();

    void on_actionLoadFolder_triggered();

    void on_algorithmBox_currentIndexChanged(const QString &arg1);

private:
    void expandCollapseWidget(QWidget *widget, QAbstractButton *button);

    Ui::MainWindow *ui;
    DataManager *dataManager;
    AlgorithmManager *algorithmManager;
};

#endif // MAINWINDOW_H
