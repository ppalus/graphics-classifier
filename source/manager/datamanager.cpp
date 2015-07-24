#include "datamanager.h"

const QString DataManager::BX_ID = "radioButtonBx";
const QString DataManager::BY_ID = "radioButtonBy";
const QString DataManager::BZ_ID = "radioButtonBz";
const QString DataManager::MX_ID = "radioButtonMx";
const QString DataManager::MY_ID = "radioButtonMy";
const QString DataManager::MZ_ID = "radioButtonMz";
const int DataManager::RESULT_MAX_COUNT = 5;
const QColor DataManager::PLOT_COLOR[] = {QColor(255, 0, 255), QColor(0, 0, 255), QColor(0, 255, 255), QColor(0, 255, 0), QColor(255, 255, 0)};
const QCPScatterStyle::ScatterShape DataManager::PLOT_SHAPE[] = {QCPScatterStyle::ssCircle, QCPScatterStyle::ssSquare, QCPScatterStyle::ssDiamond, QCPScatterStyle::ssTriangle, QCPScatterStyle::ssTriangleInverted};

DataManager::DataManager(Ui::MainWindow *ui) :
    ui(ui),
    output(RESULT_MAX_COUNT),
    index(-1)
{
    QObject::connect(&loader, SIGNAL(notifyProgress(const QString &)), ui->statusBar, SLOT(showMessage(const QString &)));
    QObject::connect(&loader, SIGNAL(notifyResult(const DataSet&)), this, SLOT(setInputData(const DataSet&)));
}

DataManager::~DataManager()
{
}

void DataManager::loadData(const QString path)
{
    loader.setPath(path);
    loader.start();
}

void DataManager::setInputData(const DataSet &dataSet)
{
    input = dataSet;
    QString info = dataSet.prefix;
    info.chop(1);
    info += "\nCount: " + QString::number(input.images.size());
    ui->inputList1Label->setText(info);
    ui->inputList2Label->setText(info);
    ui->inputList3Label->setText(info);
    ui->inputList1->clear();
    ui->inputList2->clear();
    ui->inputList3->clear();
    for(Image image : dataSet.images)
    {
        QString path = dataSet.path + "/" +
                dataSet.prefix + image.stage + ST_IT_SEPARATOR + image.iteration + "." + IMAGE_FILE_EXTENSION;
        QString name = image.stage + ST_IT_SEPARATOR + image.iteration;
        if(QFileInfo(path).isFile())
        {
            ui->inputList1->addItem(new QListWidgetItem(QIcon(path), name));
            ui->inputList2->addItem(new QListWidgetItem(QIcon(path), name));
            ui->inputList3->addItem(new QListWidgetItem(QIcon(path), name));
        }
    }
}

void DataManager::setOutputData(const DataSet &dataSet, const QString algorithm)
{
    if(++index >= RESULT_MAX_COUNT)
        index = 0;
    output[index] = dataSet;
    QListWidget *outputList;
    QAbstractButton *button;
    switch(index)
    {
    case 0:
        outputList = ui->outputList1;
        button = ui->outputList1Button;
        break;
    case 1:
        outputList = ui->outputList2;
        button = ui->outputList2Button;
        break;
    case 2:
        outputList = ui->outputList3;
        button = ui->outputList3Button;
        break;
    case 3:
        outputList = ui->outputList4;
        button = ui->outputList4Button;
        break;
    case 4:
        outputList = ui->outputList5;
        button = ui->outputList5Button;
        break;
    default:
        return;
    }
    showOutputData(outputList, button, algorithm);
}

void DataManager::showOutputData(QListWidget *outputList, QAbstractButton *button, const QString algorithm)
{
    outputList->clear();
    ui->outputList6->clear();
    QString info = output[index].prefix;
    info.chop(1);
    info += " [" + algorithm + "]\nCount: " + QString::number(output[index].images.size());
    button->setText(info);
    ui->outputList6Label->setText(info);
    for(Image image : output[index].images)
    {
        QString path = output[index].path + "/" +
                output[index].prefix + image.stage + ST_IT_SEPARATOR + image.iteration + "." + IMAGE_FILE_EXTENSION;
        QString name = image.stage + ST_IT_SEPARATOR + image.iteration;
        if(QFileInfo(path).isFile())
        {
            outputList->addItem(new QListWidgetItem(QIcon(path), name));
            ui->outputList6->addItem(new QListWidgetItem(QIcon(path), name));
        }
    }
}

void DataManager::plotInput()
{
    ui->plotView->addGraph();
    QVector<qreal> x;
    QVector<qreal> y;
    for(Image image : input.images)
    {
        if(xCol == BX_ID)
        {
            x.append(image.bx);
        }
        else if(xCol == BY_ID)
        {
            x.append(image.by);
        }
        else if(xCol == BZ_ID)
        {
            x.append(image.bz);
        }
        if(yCol == MX_ID)
        {
            y.append(image.mx);
        }
        else if(yCol == MY_ID)
        {
            y.append(image.my);
        }
        else if(yCol == MZ_ID)
        {
            y.append(image.mz);
        }
    }
    int plotIdx = ui->plotView->graphCount() - 1;
    ui->plotView->graph(plotIdx)->setData(x, y);
    ui->plotView->graph(plotIdx)->setPen(QPen(QColor(255, 0, 0)));
    ui->plotView->graph(plotIdx)->setLineStyle(QCPGraph::lsLine);
    ui->plotView->graph(plotIdx)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssStar, 5));
    ui->plotView->xAxis->setRange(ui->xMin->text().toDouble(), ui->xMax->text().toDouble());
    ui->plotView->yAxis->setRange(ui->yMin->text().toDouble(), ui->yMax->text().toDouble());
    ui->plotView->graph(plotIdx)->setSelectable(true);
    ui->plotView->setInteraction(QCP::iSelectPlottables);
    ui->plotView->replot();
}

void DataManager::plotOutput()
{
    if(dataPlotMap.contains(index))
    {
        QCPGraph* toRemove = dataPlotMap.value(index);
        ui->plotView->removeGraph(toRemove);
        ui->plotView->replot();
    }
    QCPGraph *graph = ui->plotView->addGraph();
    dataPlotMap.insert(index, graph);
    QVector<qreal> x;
    QVector<qreal> y;
    for(Image image : output[index].images)
    {
        if(xCol == BX_ID)
        {
            x.append(image.bx);
        }
        else if(xCol == BY_ID)
        {
            x.append(image.by);
        }
        else if(xCol == BZ_ID)
        {
            x.append(image.bz);
        }
        if(yCol == MX_ID)
        {
            y.append(image.mx);
        }
        else if(yCol == MY_ID)
        {
            y.append(image.my);
        }
        else if(yCol == MZ_ID)
        {
            y.append(image.mz);
        }
    }
    graph->setData(x, y);
    graph->setPen(QPen(PLOT_COLOR[index]));
    graph->setLineStyle(QCPGraph::lsLine);
    graph->setScatterStyle(QCPScatterStyle(PLOT_SHAPE[index], 5));
    ui->plotView->xAxis->setRange(ui->xMin->text().toDouble(), ui->xMax->text().toDouble());
    ui->plotView->yAxis->setRange(ui->yMin->text().toDouble(), ui->yMax->text().toDouble());
    graph->setSelectable(true);
    ui->plotView->setInteraction(QCP::iSelectPlottables);
    ui->plotView->replot();
}

void DataManager::replot()
{
    ui->plotView->xAxis->setRange(ui->xMin->text().toDouble(), ui->xMax->text().toDouble());
    ui->plotView->yAxis->setRange(ui->yMin->text().toDouble(), ui->yMax->text().toDouble());
    ui->plotView->replot();
}


QStringList DataManager::findImage(qreal b, qreal m)
{
    QVector<ImageData> data;
    for(Image image : input.images)
    {
        ImageData imgData;
        imgData.stage = image.stage;
        imgData.iteration = image.iteration;

        if(xCol == BX_ID)
        {
            imgData.x = image.bx;
        }
        else if(xCol == BY_ID)
        {
            imgData.x = image.by;
        }
        else if(xCol == BZ_ID)
        {
            imgData.x = image.bz;
        }

        if(yCol == MX_ID)
        {
            imgData.y = image.mx;
        }
        else if(yCol == MY_ID)
        {
            imgData.y = image.my;
        }
        else if(yCol == MZ_ID)
        {
            imgData.y = image.mz;
        }
        data.append(imgData);
    }

    for(DataSet dataSet : output)
    {
        for(Image image : dataSet.images)
        {
            ImageData imgData;
            imgData.stage = image.stage;
            imgData.iteration = image.iteration;

            if(xCol == BX_ID)
            {
                imgData.x = image.bx;
            }
            else if(xCol == BY_ID)
            {
                imgData.x = image.by;
            }
            else if(xCol == BZ_ID)
            {
                imgData.x = image.bz;
            }

            if(yCol == MX_ID)
            {
                imgData.y = image.mx;
            }
            else if(yCol == MY_ID)
            {
                imgData.y = image.my;
            }
            else if(yCol == MZ_ID)
            {
                imgData.y = image.mz;
            }
            data.append(imgData);
        }
    }

    for(ImageData imageData : data)
    {
        qreal bThreshold = ui->xMax->text().toDouble() / 100.;
        qreal mThreshold = ui->yMax->text().toDouble() / 100.;
        if((imageData.x <= b+bThreshold && imageData.x >= b-bThreshold)
                && (imageData.y <= m+mThreshold && imageData.y >= m-mThreshold))
        {
            QString stage;
            QString iteration;
            for(int i = input.stageLength - imageData.stage.size(); i > 0; --i)
             {
                stage += "0";
            }
            stage += imageData.stage;
            for(int i = input.iterationLength - imageData.iteration.size(); i > 0; --i)
            {
                iteration += "0";
            }
            iteration += imageData.iteration;
            return QStringList() << stage << iteration;
        }
    }
    return QStringList();
}

void DataManager::saveResult()
{
    if(!input.images.isEmpty())
    {
        if(output[index].images.isEmpty())
        {
            QMessageBox msgBox;
            msgBox.setText("No result.");
            msgBox.exec();
        }
        QFileDialog dialog;
        dialog.setFileMode(QFileDialog::Directory);
        dialog.setOption(QFileDialog::ShowDirsOnly);
        int result = dialog.exec();
        QString path = dialog.directory().path();
        dialog.close();
        QVector<QString> resultVector;
        for(Image image : output[index].images)
        {
            resultVector.append(output[index].prefix + image.stage + ST_IT_SEPARATOR + image.iteration+ "." + IMAGE_FILE_EXTENSION);
        }

        if(result)
        {
            for(QString name : resultVector)
            {
                QFile::copy(input.path + "/" + name, path + "/" + name);
            }
            ui->statusBar->showMessage("Result saved");
        }
    }
    else
    {
        QMessageBox msgBox;
        msgBox.setText("No files loaded.");
        msgBox.exec();
    }
}


void DataManager::removeData(QCPGraph *graph)
{
    int index = dataPlotMap.key(graph, -1);
    if(index != -1)
    {
        output.remove(index);
        switch(index)
        {
        case 0:
            ui->outputList1->clear();
            ui->outputList1Button->setText("");
            break;
        case 1:
            ui->outputList2->clear();
            ui->outputList2Button->setText("");
            break;
        case 2:
            ui->outputList3->clear();
            ui->outputList3Button->setText("");
            break;
        case 3:
            ui->outputList4->clear();
            ui->outputList4Button->setText("");
            break;
        case 4:
            ui->outputList5->clear();
            ui->outputList5Button->setText("");
            break;
        default:
            return;
        }
        --index;
    }
}
