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
    ui(ui)
{
    QObject::connect(&loader, SIGNAL(notifyProgress(const QString &)), ui->statusBar, SLOT(showMessage(const QString &)));
    QObject::connect(&loader, SIGNAL(notifyResult(const DataSet&)), this, SLOT(setInputData(const DataSet&)));
    QObject::connect(ui->outputList1RemoveButton, SIGNAL(clicked(bool)), this, SLOT(clearOutputData1()));
    QObject::connect(ui->outputList2RemoveButton, SIGNAL(clicked(bool)), this, SLOT(clearOutputData2()));
    QObject::connect(ui->outputList3RemoveButton, SIGNAL(clicked(bool)), this, SLOT(clearOutputData3()));
    QObject::connect(ui->outputList4RemoveButton, SIGNAL(clicked(bool)), this, SLOT(clearOutputData4()));
    QObject::connect(ui->outputList5RemoveButton, SIGNAL(clicked(bool)), this, SLOT(clearOutputData5()));
    QObject::connect(ui->outputList1CheckBox, SIGNAL(clicked(bool)), this, SLOT(setActiveData1()));
    QObject::connect(ui->outputList2CheckBox, SIGNAL(clicked(bool)), this, SLOT(setActiveData2()));
    QObject::connect(ui->outputList3CheckBox, SIGNAL(clicked(bool)), this, SLOT(setActiveData3()));
    QObject::connect(ui->outputList4CheckBox, SIGNAL(clicked(bool)), this, SLOT(setActiveData4()));
    QObject::connect(ui->outputList5CheckBox, SIGNAL(clicked(bool)), this, SLOT(setActiveData5()));
    QObject::connect(ui->outputList6ComboBox, SIGNAL(activated(QString)), this, SLOT(setOutputList6Content(QString)));

    output[0].label = ui->outputList1CheckBox;
    output[0].list = ui->outputList1;
    output[0].graph = nullptr;
    output[0].active = false;
    output[1].label = ui->outputList2CheckBox;
    output[1].list = ui->outputList2;
    output[1].graph = nullptr;
    output[1].active = false;
    output[2].label = ui->outputList3CheckBox;
    output[2].list = ui->outputList3;
    output[2].graph = nullptr;
    output[2].active = false;
    output[3].label = ui->outputList4CheckBox;
    output[3].list = ui->outputList4;
    output[3].graph = nullptr;
    output[4].active = false;
    output[4].label = ui->outputList5CheckBox;
    output[4].list = ui->outputList5;
    output[4].graph = nullptr;
    output[4].active = false;
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

    QString id = dataSet.prefix;
    id.chop(1);
    id += "-" + algorithm + "-count: " + QString::number(dataSet.images.size());

    if(!output[4].id.isEmpty() && !output[3].id.isEmpty() && !output[2].id.isEmpty() &&
            !output[1].id.isEmpty() && !output[0].id.isEmpty())
    {
        ui->outputList6ComboBox->removeItem(0);
        moveOutputUp(1);
        moveOutputUp(2);
        moveOutputUp(3);
        moveOutputUp(4);
    }
    else if(!output[4].id.isEmpty() && !output[3].id.isEmpty() && !output[2].id.isEmpty() &&
            !output[1].id.isEmpty() && output[0].id.isEmpty())
    {
        moveOutputUp(1);
        moveOutputUp(2);
        moveOutputUp(3);
        moveOutputUp(4);
    }
    else if(!output[4].id.isEmpty() && !output[3].id.isEmpty() && !output[2].id.isEmpty() &&
            output[1].id.isEmpty() && output[0].id.isEmpty())
    {
        moveOutputUp(2);
        moveOutputUp(3);
        moveOutputUp(4);
    }
    else if(!output[4].id.isEmpty() && !output[3].id.isEmpty() && output[2].id.isEmpty() &&
            output[1].id.isEmpty() && output[0].id.isEmpty())
    {
        moveOutputUp(3);
        moveOutputUp(4);
    }
    else if(!output[4].id.isEmpty() && output[3].id.isEmpty() && output[2].id.isEmpty() &&
            output[1].id.isEmpty() && output[0].id.isEmpty())
    {
        moveOutputUp(4);
    }

    ui->outputList6ComboBox->addItem(id);
    output[4].label->setText(id);
    output[4].id = id;
    output[4].dataSet = dataSet;
    for(Image image : output[4].dataSet.images)
    {
        QString path = output[4].dataSet.path + "/" +
                output[4].dataSet.prefix + image.stage + ST_IT_SEPARATOR + image.iteration + "." + IMAGE_FILE_EXTENSION;
        QString name = image.stage + ST_IT_SEPARATOR + image.iteration;
        if(QFileInfo(path).isFile())
        {
            output[4].list->addItem(new QListWidgetItem(QIcon(path), name));
        }
    }
}

void DataManager::moveOutputUp(int index)
{
    output[index-1].list->clear();
    for(Image image : output[index].dataSet.images)
    {
        QString path = output[index].dataSet.path + "/" +
                output[index].dataSet.prefix + image.stage + ST_IT_SEPARATOR + image.iteration + "." + IMAGE_FILE_EXTENSION;
        QString name = image.stage + ST_IT_SEPARATOR + image.iteration;
        if(QFileInfo(path).isFile())
        {
            output[index-1].list->addItem(new QListWidgetItem(QIcon(path), name));
        }
    }
    output[index].list->clear();
    output[index-1].id = output[index].id;
    output[index-1].graph = output[index].graph;
    output[index-1].dataSet = output[index].dataSet;
    output[index-1].label->setText(output[index-1].id);
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
    QVector<int> activeIndexes;
    for(int i = 0; i < RESULT_MAX_COUNT; ++i)
    {
        if(output[i].active)
        {
            activeIndexes.append(i);
        }
    }
    for(int index : activeIndexes)
    {
        if(output[index].graph)
        {
            ui->plotView->removeGraph(output[index].graph);
            ui->plotView->replot();
        }
        QCPGraph *graph = ui->plotView->addGraph();
        output[index].graph = graph;
        QVector<qreal> x;
        QVector<qreal> y;
        for(Image image : output[index].dataSet.images)
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
        graph->setScatterStyle(QCPScatterStyle(PLOT_SHAPE[index], 10));
        ui->plotView->xAxis->setRange(ui->xMin->text().toDouble(), ui->xMax->text().toDouble());
        ui->plotView->yAxis->setRange(ui->yMin->text().toDouble(), ui->yMax->text().toDouble());
        graph->setSelectable(true);
        ui->plotView->setInteraction(QCP::iSelectPlottables);
        ui->plotView->replot();
    }
}

void DataManager::replot()
{
    ui->plotView->xAxis->setRange(ui->xMin->text().toDouble(), ui->xMax->text().toDouble());
    ui->plotView->yAxis->setRange(ui->yMin->text().toDouble(), ui->yMax->text().toDouble());
    ui->plotView->replot();
}

// TODO optimalize -> DataSet::getDataFor(xCol, yCol)?
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

    for(OutputDataSlot outputData : output)
    {
        for(Image image : outputData.dataSet.images)
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

// TODO check errors (error codes)
void DataManager::saveResult()
{
    QFileDialog dialog;
    dialog.setFileMode(QFileDialog::Directory);
    dialog.setOption(QFileDialog::ShowDirsOnly);
    int result = dialog.exec();
    QString path = dialog.directory().path();
    for(OutputDataSlot data : output)
    {
        if(data.active)
        {
            if(data.dataSet.images.isEmpty())
            {
                QMessageBox msgBox;
                msgBox.setText("No result.");
                msgBox.exec();
                continue;
            }

            QDir(path).mkdir(data.id);
            QString pathToSave = path +  "/" + data.id;
            dialog.close();
            QVector<QString> resultVector;
            for(Image image : data.dataSet.images)
            {
                resultVector.append(data.dataSet.prefix + image.stage + ST_IT_SEPARATOR + image.iteration+ "." + IMAGE_FILE_EXTENSION);
            }

            if(result)
            {
                for(QString name : resultVector)
                {
                    QFile::copy(input.path + "/" + name, pathToSave + "/" + name);
                }
            }
        }
    }
}

void DataManager::clearOutputData1()
{
    ui->outputList6ComboBox->removeItem(0);
    ui->plotView->removeGraph(output[0].graph);
    ui->plotView->replot();
    output[0].label->setText("");
    output[0].id.clear();
    fixOutputLists();
}

void DataManager::clearOutputData2()
{
    ui->outputList6ComboBox->removeItem(1);
    ui->plotView->removeGraph(output[1].graph);
    ui->plotView->replot();
    output[1].label->setText("");
    output[1].id.clear();
    fixOutputLists();
}

void DataManager::clearOutputData3()
{
    ui->outputList6ComboBox->removeItem(2);
    ui->plotView->removeGraph(output[2].graph);
    ui->plotView->replot();
    output[2].label->setText("");
    output[2].id.clear();
    fixOutputLists();
}

void DataManager::clearOutputData4()
{
    ui->outputList6ComboBox->removeItem(3);
    ui->plotView->removeGraph(output[3].graph);
    ui->plotView->replot();
    output[3].label->setText("");
    output[3].id.clear();
    fixOutputLists();
}

void DataManager::clearOutputData5()
{
    ui->outputList6ComboBox->removeItem(4);
    ui->plotView->removeGraph(output[4].graph);
    ui->plotView->replot();
    output[4].id.clear();
    ui->outputList5CheckBox->setText("");
    ui->outputList5->clear();
}

void DataManager::setActiveData1()
{
    output[0].active = !output[0].active;
}

void DataManager::setActiveData2()
{
    output[1].active = !output[1].active;
}

void DataManager::setActiveData3()
{
    output[2].active = !output[2].active;
}

void DataManager::setActiveData4()
{
    output[3].active = !output[3].active;
}

void DataManager::setActiveData5()
{
    output[4].active = !output[4].active;
}

void DataManager::setOutputList6Content(QString id)
{
    ui->outputList6->clear();
    for(OutputDataSlot data : output)
    {
        if(id == data.id)
        {
            for(Image image : data.dataSet.images)
            {
                QString path = data.dataSet.path + "/" +
                        data.dataSet.prefix + image.stage + ST_IT_SEPARATOR + image.iteration + "." + IMAGE_FILE_EXTENSION;
                QString name = image.stage + ST_IT_SEPARATOR + image.iteration;
                if(QFileInfo(path).isFile())
                {
                    ui->outputList6->addItem(new QListWidgetItem(QIcon(path), name));
                }
            }
            break;
        }
    }

}

void DataManager::removeOutputData(QString id)
{
    for(int i = 0; i < RESULT_MAX_COUNT; ++i)
    {
        if(id == output[i].id)
        {
            output[i].id = "";
            ui->outputList6ComboBox->removeItem(i);
            ui->plotView->removeGraph(output[i].graph);
            ui->plotView->replot();
            output[i].label->setText("");
            output[i].list->clear();
            output[i].graph = nullptr;
            output[i].active = false;
            fixOutputLists();
        }
    }
}

void DataManager::removeOutputDataByGraph(QCPGraph *graph)
{
    QString id;
    for(OutputDataSlot data : output)
    {
        if(data.graph == graph)
        {
            id = data.id;
            break;
        }
    }
    if(id.isEmpty())
    {
        //TODO remove inputplot
    }
    removeOutputData(id);
}

void DataManager::fixOutputLists()
{
    for(int i = 0; i < RESULT_MAX_COUNT; ++i)
    {
        if(output[i].id.isEmpty())
        {
            for(int j = 0; j < i; ++j)
            {
                output[i-j-1].list->clear();
                for(Image image : output[i-j].dataSet.images)
                {
                    QString path = output[i-j].dataSet.path + "/" +
                            output[i-j].dataSet.prefix + image.stage + ST_IT_SEPARATOR + image.iteration + "." + IMAGE_FILE_EXTENSION;
                    QString name = image.stage + ST_IT_SEPARATOR + image.iteration;
                    if(QFileInfo(path).isFile())
                    {
                        output[i-j-1].list->addItem(new QListWidgetItem(QIcon(path), name));
                    }
                }
                /*for(int k = 0; k < output[i-j-1].list->count(); ++k)
                {
                    QListWidgetItem *item = output[i-j-1].list->item(k);
                    output[i-j-1].list->addItem(item);
                }*/

                output[i-j].list->clear();
                output[i-j].id = output[i-j-1].id;
                output[i-j].graph = output[i-j-1].graph;
                output[i-j].dataSet = output[i-j-1].dataSet;
                output[i-j].label->setText(output[i-j].id);
            }
        }
    }
}
