#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    savePlotDialog(new SavePlotDialog(this))
{
    ui->setupUi(this);
    savePlotDialog->setPlotWidget(ui->plotView);
    dataManager = new DataManager(ui);
    algorithmManager = new AlgorithmManager(ui);
    QObject::connect(algorithmManager, SIGNAL(notifyResult(const DataSet&,const QString)), dataManager, SLOT(setOutputData(const DataSet&,const QString)));
}

MainWindow::~MainWindow()
{
    delete savePlotDialog;
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

void MainWindow::setSelection(QString name)
{
    QList<QListWidgetItem*> list = ui->inputList3->findItems(name, Qt::MatchExactly);
    for(QListWidgetItem *item : list)
    {
        ui->inputList3->setCurrentItem(item);
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
    algorithmManager->compare(dataManager->getInput());
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

void MainWindow::on_inputList2_itemClicked(QListWidgetItem *item)
{
    QString path = dataManager->getInput().path;
    QDirIterator iterator(path);
    QString fileName = dataManager->getInput().prefix + item->text() + "." + IMAGE_FILE_EXTENSION;
    int w = ui->imagePreview1->width();
    int h = ui->imagePreview1->height();
    while(iterator.hasNext())
    {
        iterator.next();
        QString suffix = iterator.fileInfo().suffix();
        if(!suffix.compare("bmp", Qt::CaseInsensitive))
        {
            QString name = iterator.fileName();
            if(name == fileName)
            {
                ui->imagePreview1->setPixmap(QPixmap(path + "/" + fileName).scaled(w, h, Qt::KeepAspectRatio));
            }
        }
    }
}

void MainWindow::on_outputList6_itemClicked(QListWidgetItem *item)
{
    QString path = dataManager->getInput().path;
    QDirIterator iterator(path);
    QString fileName = dataManager->getInput().prefix + item->text() + "." + IMAGE_FILE_EXTENSION;
    int w = ui->imagePreview1->width();
    int h = ui->imagePreview1->height();
    while(iterator.hasNext())
    {
        iterator.next();
        QString suffix = iterator.fileInfo().suffix();
        if(!suffix.compare("bmp", Qt::CaseInsensitive))
        {
            QString name = iterator.fileName();
            if(name == fileName)
            {
                ui->imagePreview2->setPixmap(QPixmap(path + "/" + fileName).scaled(w, h, Qt::KeepAspectRatio));
            }
        }
    }
}

void MainWindow::on_tab2Splitter_splitterMoved(int pos, int index)
{
    QString path = dataManager->getInput().path;
    QList<QListWidgetItem*> selectedItems1 = ui->inputList2->selectedItems();
    QList<QListWidgetItem*> selectedItems2 = ui->outputList6->selectedItems();
    if(!selectedItems1.isEmpty())
    {
        QString fileName = dataManager->getInput().prefix + ui->inputList2->selectedItems().at(0)->text() + + "." + IMAGE_FILE_EXTENSION;
        int w = ui->imagePreview1->width();
        int h = ui->imagePreview1->height();
        QDirIterator iterator(path);
        while(iterator.hasNext())
        {
            iterator.next();
            QString suffix = iterator.fileInfo().suffix();
            if(!suffix.compare("bmp", Qt::CaseInsensitive))
            {
                QString name = iterator.fileName();
                if(name == fileName)
                {
                    ui->imagePreview1->setPixmap(QPixmap(path + "/" + fileName).scaled(w, h, Qt::KeepAspectRatio));
                }
            }
        }
    }
    if(!selectedItems2.isEmpty())
    {
        QString fileName = dataManager->getInput().prefix + ui->outputList6->selectedItems().at(0)->text() + + "." + IMAGE_FILE_EXTENSION;
        int w = ui->imagePreview2->width();
        int h = ui->imagePreview2->height();
        QDirIterator iterator(path);
        while(iterator.hasNext())
        {
            iterator.next();
            QString suffix = iterator.fileInfo().suffix();
            if(!suffix.compare("bmp", Qt::CaseInsensitive))
            {
                QString name = iterator.fileName();
                if(name == fileName)
                {
                    ui->imagePreview2->setPixmap(QPixmap(path + "/" + fileName).scaled(w, h, Qt::KeepAspectRatio));
                }
            }
        }
    }
}

void MainWindow::on_radioButtonBx_clicked()
{
    dataManager->setXCol(DataManager::BX_ID);
}

void MainWindow::on_radioButtonBy_clicked()
{
    dataManager->setXCol(DataManager::BY_ID);
}

void MainWindow::on_radioButtonBz_clicked()
{
    dataManager->setXCol(DataManager::BZ_ID);
}

void MainWindow::on_radioButtonMx_clicked()
{
    dataManager->setYCol(DataManager::MX_ID);
}

void MainWindow::on_radioButtonMy_clicked()
{
    dataManager->setYCol(DataManager::MY_ID);
}

void MainWindow::on_radioButtonMz_clicked()
{
    dataManager->setYCol(DataManager::MZ_ID);
}

void MainWindow::on_plotView_customContextMenuRequested(const QPoint &pos)
{
    QPoint globalPosition = ui->plotView->mapToGlobal(pos);
    QList<QCPGraph*> selectionList = ui->plotView->selectedGraphs();
    QMenu contextMenu;
    contextMenu.addAction("Plot input");
    contextMenu.addAction("Plot output");
    contextMenu.addAction("Remove plot")->setDisabled(selectionList.isEmpty());
    QAction *selectedItem = contextMenu.exec(globalPosition);
    if(selectedItem && selectedItem->text() == "Remove plot")
    {
        QList<QCPGraph*> selectionList = ui->plotView->selectedGraphs();
        for(QCPGraph* selectedGraph : selectionList)
        {
            dataManager->removeData(selectedGraph);
            ui->plotView->removeGraph(selectedGraph);
        }
        ui->plotView->replot();
    }
    else if(selectedItem && selectedItem->text() == "Plot input")
    {
        dataManager->plotInput();
    }
    else if(selectedItem && selectedItem->text() == "Plot output")
    {
        dataManager->plotOutput();
    }
}

void MainWindow::on_replotButton_clicked()
{
    dataManager->replot();
}

void MainWindow::on_plotView_mouseMove(QMouseEvent *event)
{
    qreal b = ui->plotView->xAxis->pixelToCoord(event->x());
    qreal m = ui->plotView->yAxis->pixelToCoord(event->y());
    QStringList result = dataManager->findImage(b, m);
    if(!result.isEmpty())
    {
        QString name = result[0] + ST_IT_SEPARATOR + result[1];
        setSelection(name);
        QString filePath = dataManager->getInput().path + "/" + dataManager->getInput().prefix + name + ".bmp";
        QToolTip::showText(QCursor::pos() - QPoint(20, 20),
                           "<img src=\"" + filePath + "\"> \n\n" +
                           "B: " + QString::number(b) + "\tm: "+ QString::number(m));
    }
}

void MainWindow::on_actionSaveResult_triggered()
{
    dataManager->saveResult();
}

void MainWindow::on_thresholdSlider_valueChanged(int value)
{
    ui->thresholdLabel->setText("Threshold: " + QString::number(value / 100.));
}

void MainWindow::on_actionSavePlot_triggered()
{
    savePlotDialog->show();
}

void MainWindow::on_actionExit_triggered()
{
    this->close();
}
