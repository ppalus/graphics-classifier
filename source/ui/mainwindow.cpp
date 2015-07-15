#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    dataManager(nullptr),
    algorithmManager(nullptr)
{
    ui->setupUi(this);
    dataManager = new DataManager(ui);
    algorithmManager = new AlgorithmManager(ui);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete dataManager;
    delete algorithmManager;
}

void MainWindow::expandCollapseWidget(QWidget *widget, QAbstractButton *button)
{
    if(!widget || !button)
        return;

    if(widget->isHidden())
    {
        widget->show();
        button->setIcon(QIcon(":/icons/resources/arrow-expanded.png"));
    }
    else
    {
        widget->hide();
        button->setIcon(QIcon(":/icons/resources/arrow.png"));
    }
}

void MainWindow::on_outputList1Button_clicked()
{
    expandCollapseWidget(ui->outputList1, ui->outputList1Button);
}

void MainWindow::on_outputList2Button_clicked()
{
    expandCollapseWidget(ui->outputList2, ui->outputList2Button);
}

void MainWindow::on_outputList3Button_clicked()
{
    expandCollapseWidget(ui->outputList3, ui->outputList3Button);
}

void MainWindow::on_outputList4Button_clicked()
{
    expandCollapseWidget(ui->outputList4, ui->outputList4Button);
}

void MainWindow::on_outputList5Button_clicked()
{
    expandCollapseWidget(ui->outputList5, ui->outputList5Button);
}

void MainWindow::on_compareButton_clicked()
{

}

void MainWindow::on_actionLoadFolder_triggered()
{
    QFileDialog dialog;
    dialog.setFileMode(QFileDialog::Directory);
    dialog.setOption(QFileDialog::ShowDirsOnly);
    int result = dialog.exec();
    dialog.close();
    if(result && dataManager)
    {
        dataManager->loadData(dialog.directory().path());
    }
}

void MainWindow::on_algorithmBox_currentIndexChanged(const QString &arg1)
{
    if(algorithmManager)
    {
        algorithmManager->setAlgorithm(arg1);
    }
}
