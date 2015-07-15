#ifndef SAVEPLOTDIALOG_H
#define SAVEPLOTDIALOG_H

#include <QDialog>
#include <QFileDialog>

#include "qcustomplot.h"

namespace Ui {
class SavePlotDialog;
}

class SavePlotDialog : public QDialog
{
    Q_OBJECT

    const QString PDF = "PDF";
    const QString PNG = "PNG";
    const QString JPG = "JPG";
    const QString BMP = "BMP";
    const QString RASTERED = "RASTERED";

public:
    SavePlotDialog(QWidget *parent = 0);
    ~SavePlotDialog();

    void setPlotWidget(QCustomPlot *plot) { this->plot = plot; }

private slots:
    void on_fileTypeBox_currentIndexChanged(const QString &arg1);

    void on_cancelButton_clicked();

    void on_saveButton_clicked();

private:
    Ui::SavePlotDialog *ui;
    QCustomPlot *plot;
};

#endif // SAVEPLOTDIALOG_H
