#include "saveplotdialog.h"
#include "ui_saveplotdialog.h"

SavePlotDialog::SavePlotDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SavePlotDialog)
{
    ui->setupUi(this);
    ui->fileTypeBox->addItem(PDF);
    ui->fileTypeBox->addItem(PNG);
    ui->fileTypeBox->addItem(JPG);
    ui->fileTypeBox->addItem(BMP);
    ui->fileTypeBox->addItem(RASTERED);
}

SavePlotDialog::~SavePlotDialog()
{
    delete ui;
}

void SavePlotDialog::on_fileTypeBox_currentIndexChanged(const QString &arg1)
{
    ui->saveButton->setEnabled(true);
    if(arg1 == PDF)
    {
        ui->scaleEdit->setEnabled(false);
        ui->qualityEdit->setEnabled(false);
        ui->formatEdit->setEnabled(false);
        ui->costmeticCheckBox->setEnabled(true);
        ui->pdfCreatorEdit->setEnabled(true);
        ui->pdfTitleEdit->setEnabled(true);
    }
    else if(arg1 == PNG || arg1 == JPG)
    {
        ui->costmeticCheckBox->setEnabled(false);
        ui->formatEdit->setEnabled(false);
        ui->pdfCreatorEdit->setEnabled(false);
        ui->pdfTitleEdit->setEnabled(false);
        ui->scaleEdit->setEnabled(true);
        ui->qualityEdit->setEnabled(true);
    }
    else if(arg1 == BMP)
    {
        ui->costmeticCheckBox->setEnabled(false);
        ui->formatEdit->setEnabled(false);
        ui->pdfCreatorEdit->setEnabled(false);
        ui->pdfTitleEdit->setEnabled(false);
        ui->scaleEdit->setEnabled(true);
        ui->qualityEdit->setEnabled(false);
    }
    else if(arg1 == RASTERED)
    {
        ui->costmeticCheckBox->setEnabled(false);
        ui->formatEdit->setEnabled(true);
        ui->pdfCreatorEdit->setEnabled(false);
        ui->pdfTitleEdit->setEnabled(false);
        ui->scaleEdit->setEnabled(true);
        ui->qualityEdit->setEnabled(true);
    }
    else
    {
        ui->saveButton->setEnabled(false);
    }
}

void SavePlotDialog::on_cancelButton_clicked()
{
    close();
}

void SavePlotDialog::on_saveButton_clicked()
{
    QString widthText = ui->widthEdit->text();
    QString heightText = ui->heightEdit->text();
    QString scaleText = ui->scaleEdit->text();
    QString qualityText = ui->qualityEdit->text();
    bool cosmetic = ui->costmeticCheckBox->isChecked();
    QString fileType = ui->fileTypeBox->currentText();
    QString fileName = QFileDialog::getSaveFileName();
    if(fileType == PDF)
    {

        plot->savePdf(fileName + ".pdf",
                      cosmetic,
                      widthText == "" ? 0 : widthText.toInt(),
                      heightText == "" ? 0 : heightText.toInt(),
                      ui->pdfCreatorEdit->text(),
                      ui->pdfTitleEdit->text());
    }
    else if(fileType == PNG)
    {
        plot->savePng(fileName + ".png",
                      widthText == "" ? 0 : widthText.toInt(),
                      heightText == "" ? 0 : heightText.toInt(),
                      scaleText == "" ? 1.0 : scaleText.toDouble(),
                      qualityText == "" ? -1 : qualityText.toInt());
    }
    else if(fileType == JPG)
    {
        plot->saveJpg(fileName + ".jpg",
                      widthText == "" ? 0 : widthText.toInt(),
                      heightText == "" ? 0 : heightText.toInt(),
                      scaleText == "" ? 1.0 : scaleText.toDouble(),
                      qualityText == "" ? -1 : qualityText.toInt());
    }
    else if(fileType == BMP)
    {
        plot->saveBmp(fileName + ".bmp",
                      widthText == "" ? 0 : widthText.toInt(),
                      heightText == "" ? 0 : heightText.toInt(),
                      scaleText == "" ? 1.0 : scaleText.toDouble());
    }
    else if(fileType == RASTERED)
    {
        QString format = ui->formatEdit->text();
        if(widthText == "" || heightText == "" || scaleText == "" || format == "")
        {
            return;
        }
        plot->saveRastered(fileName + "." + format,
                           widthText.toInt(),
                           heightText.toInt(),
                           scaleText.toDouble(),
                           format.toStdString().c_str(),
                           qualityText == "" ? -1 : qualityText.toInt());
    }
    close();
}
