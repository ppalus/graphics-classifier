#include "datamanager.h"

DataManager::DataManager(Ui::MainWindow *ui) :
    ui(ui)
{
    qRegisterMetaType<DataSet>();
    QObject::connect(&loader, SIGNAL(notifyProgress(const QString &)), ui->statusBar, SLOT(showMessage(const QString &)));
    QObject::connect(&loader, SIGNAL(result(DataSet)), this, SLOT(setData(DataSet)));
}

DataManager::~DataManager()
{
}

void DataManager::loadData(const QString path)
{
    loader.setPath(path);
    loader.start();
}

void DataManager::setData(DataSet dataSet)
{
    input = dataSet;
    QString info = dataSet.prefix;
    info.chop(1);
    ui->inputList1Label->setText(info);
    ui->inputList1->clear();
    ui->inputList2->clear();
    ui->inputList3->clear();
    for(Image image : dataSet.images)
    {
        QString path = dataSet.path + "/" +
                dataSet.prefix + image.stage + ST_IT_SEPARATOR + image.iteration + "." + IMAGE_FILE_EXTENSION;
        QString name = image.stage + ST_IT_SEPARATOR + image.iteration;
        QImage pic = QImage(path).scaled(70, 70);
        if(!pic.isNull())
        {
            ui->inputList1->addItem(new QListWidgetItem(QIcon(QPixmap::fromImage(pic)), name));
            ui->inputList2->addItem(new QListWidgetItem(QIcon(QPixmap::fromImage(pic)), name));
            ui->inputList3->addItem(new QListWidgetItem(QIcon(QPixmap::fromImage(pic)), name));
        }
    }
}
