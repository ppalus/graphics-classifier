#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QAbstractButton>

#include "manager/datamanager.h"
#include "manager/algorithmmanager.h"
#include "saveplotdialog.h"

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

    void on_inputList2_itemClicked(QListWidgetItem *item);

    void on_outputList6_itemClicked(QListWidgetItem *item);

    void on_tab2Splitter_splitterMoved(int pos, int index);

    void on_radioButtonBx_clicked();

    void on_radioButtonBy_clicked();

    void on_radioButtonBz_clicked();

    void on_radioButtonMx_clicked();

    void on_radioButtonMy_clicked();

    void on_radioButtonMz_clicked();

    void on_plotView_customContextMenuRequested(const QPoint &pos);

    void on_replotButton_clicked();

    void on_plotView_mouseMove(QMouseEvent *event);

    void on_actionSaveResult_triggered();

    void on_thresholdSlider_valueChanged(int value);

    void on_actionExit_triggered();

private:
    void expandCollapseWidget(QWidget *widget, QAbstractButton *button);
    void setSelection(QString name);

    Ui::MainWindow *ui;
    SavePlotDialog *savePlotDialog;
    DataManager *dataManager;
    AlgorithmManager *algorithmManager;
};

#endif // MAINWINDOW_H
