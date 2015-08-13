#include "datamanager.h"

const QString DataManager::BX_ID = "radioButtonBx";
const QString DataManager::BY_ID = "radioButtonBy";
const QString DataManager::BZ_ID = "radioButtonBz";
const QString DataManager::MX_ID = "radioButtonMx";
const QString DataManager::MY_ID = "radioButtonMy";
const QString DataManager::MZ_ID = "radioButtonMz";
const QColor DataManager::PLOT_COLOR[] = {QColor(255, 0, 255), QColor(0, 0, 255), QColor(0, 255, 255), QColor(0, 255, 0), QColor(0, 0, 0)};
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
    QObject::connect(ui->outputList6ComboBox, SIGNAL(activated(QString)), this, SLOT(setOutputList6Content(QString)));
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

    if(output.size() >= RESULT_MAX_COUNT)
        removeOutput(RESULT_MAX_COUNT-1);

    ui->outputList6ComboBox->addItem(id);

    Output result;
    result.id = id;
    result.graph = nullptr;
    result.data = dataSet;
    output.prepend(result);

    refreshUI();
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
    QString info = input.prefix;
    info.chop(1);
    int plotIdx = ui->plotView->graphCount() - 1;
    ui->plotView->graph(plotIdx)->setData(x, y);
    ui->plotView->graph(plotIdx)->setPen(QPen(QColor(255, 0, 0)));
    ui->plotView->graph(plotIdx)->setLineStyle(QCPGraph::lsLine);
    ui->plotView->graph(plotIdx)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssStar, 15));
    ui->plotView->graph(plotIdx)->setName(info);
    ui->plotView->xAxis->setRange(ui->xMin->text().toDouble(), ui->xMax->text().toDouble());
    ui->plotView->yAxis->setRange(ui->yMin->text().toDouble(), ui->yMax->text().toDouble());
    ui->plotView->graph(plotIdx)->setSelectable(true);
    ui->plotView->setInteraction(QCP::iSelectPlottables);
    ui->plotView->replot();
}

void DataManager::plotOutput()
{
    for(int i=0; i<output.size(); ++i)
    {
        if(!output[i].label->isChecked())
            continue;

        if(output[i].graph)
        {
            ui->plotView->removeGraph(output[i].graph);
            ui->plotView->replot();
        }
        QCPGraph *graph = ui->plotView->addGraph();
        output[i].graph = graph;
        QVector<qreal> x;
        QVector<qreal> y;
        for(Image image : output[i].data.images)
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
        graph->setPen(QPen(PLOT_COLOR[i]));
        graph->setLineStyle(QCPGraph::lsLine);
        graph->setScatterStyle(QCPScatterStyle(PLOT_SHAPE[i], 15));
        graph->setName(output[i].id);
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

    for(Output outputData : output)
    {
        for(Image image : outputData.data.images)
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
    for(Output data : output)
    {
        if(data.label->isChecked())
        {
            if(data.data.images.isEmpty())
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
            for(Image image : data.data.images)
            {
                resultVector.append(data.data.prefix + image.stage + ST_IT_SEPARATOR + image.iteration+ "." + IMAGE_FILE_EXTENSION);
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
    removeOutput(4);
}

void DataManager::clearOutputData2()
{
    removeOutput(3);
}

void DataManager::clearOutputData3()
{
    removeOutput(2);
}

void DataManager::clearOutputData4()
{
    removeOutput(1);
}

void DataManager::clearOutputData5()
{
    removeOutput(0);
}

void DataManager::setOutputList6Content(QString id)
{
    ui->outputList6->clear();
    for(Output slot : output)
    {
        if(id == slot.id)
        {
            for(Image image : slot.data.images)
            {
                QString path = slot.data.path + "/" +
                        slot.data.prefix + image.stage + ST_IT_SEPARATOR + image.iteration + "." + IMAGE_FILE_EXTENSION;
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

void DataManager::removeOutput(int index)
{
    if(output.isEmpty() || index > output.size()-1)
        return;

    ui->plotView->removeGraph(output[index].graph);
    ui->plotView->replot();
    for(int i=0; i<ui->outputList6ComboBox->count(); ++i)
    {
        if(ui->outputList6ComboBox->itemText(i) == output[index].id)
        {
            ui->outputList6ComboBox->removeItem(i);
            break;
        }
    }
    output[index].label->setText("");
    output[index].list->clear();
    output.removeAt(index);
}

void DataManager::removeOutput(QString id)
{
    if(id.isEmpty())
        return;

    for(int i=0; i<output.size(); ++i)
    {
        if(id == output[i].id)
        {
            removeOutput(i);
        }
    }
}

void DataManager::removeGraph(QCPGraph *graph)
{
    bool removedOutput = false;
    for(int i=0; i<output.size(); ++i)
    {
        if(output[i].graph == graph)
        {
            removeOutput(i);
            removedOutput = true;
        }
    }
    if(!removedOutput)
    {
        ui->plotView->removeGraph(graph);
        ui->plotView->replot();
    }
}

void DataManager::refreshUI()
{
    if(output.size() > 0)
    {
        output[0].list = ui->outputList5;
        output[0].label = ui->outputList5CheckBox;
    }
    if(output.size() > 1)
    {
        output[1].list = ui->outputList4;
        output[1].label = ui->outputList4CheckBox;
    }
    if(output.size() > 2)
    {
        output[2].list = ui->outputList3;
        output[2].label = ui->outputList3CheckBox;
    }
    if(output.size() > 3)
    {
        output[3].list = ui->outputList2;
        output[3].label = ui->outputList2CheckBox;
    }
    if(output.size() > 4)
    {
        output[4].list = ui->outputList1;
        output[4].label = ui->outputList1CheckBox;
    }
    for(Output slot : output)
    {
        if(!slot.id.isEmpty())
        {
            slot.label->setText(slot.id);
            slot.label->setChecked(slot.label->isChecked());
            slot.list->clear();
            for(Image image : slot.data.images)
            {
                QString path = slot.data.path + "/" +
                        slot.data.prefix + image.stage + ST_IT_SEPARATOR + image.iteration + "." + IMAGE_FILE_EXTENSION;
                QString name = image.stage + ST_IT_SEPARATOR + image.iteration;
                if(QFileInfo(path).isFile())
                {
                    slot.list->addItem(new QListWidgetItem(QIcon(path), name));
                }
            }
        }
    }
}
