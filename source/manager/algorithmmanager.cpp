#include "algorithmmanager.h"

const QString AlgorithmManager::HISTOGRAM_ID = "Histogram comparison";
const QString AlgorithmManager::SSIM_ID = "SSIM";
const QString AlgorithmManager::DIFFERENCE_ID = "Difference comparison";

AlgorithmManager::AlgorithmManager(Ui::MainWindow *ui) :
    ui(ui)
{
    ui->algorithmBox->addItem(HISTOGRAM_ID);
    ui->algorithmBox->addItem(SSIM_ID);
    ui->algorithmBox->addItem(DIFFERENCE_ID);
    algorithm = ui->algorithmBox->currentText();
}

