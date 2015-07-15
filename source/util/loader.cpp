#include "loader.h"

Loader::Loader()
{
}

void Loader::run()
{
    if(path.isEmpty())
    {
        emit notifyProgress("Loading failed - no data path");
        return;
    }
    DataSet dataSet;
    dataSet.path = path;
    QString dataFile;
    QStringList images;
    register bool dataSetInitialized = false;
    int loadCounter = 0;

    emit notifyProgress("Initalizing metadata");

    for(QDirIterator iterator(path); iterator.hasNext(); iterator.next())
    {
        QString extension = iterator.fileInfo().suffix();
        if(!extension.compare(IMAGE_FILE_EXTENSION, Qt::CaseInsensitive))
        {
            if(!dataSetInitialized)
            {
                QStringList name = iterator.fileName().split(ST_IT_SEPARATOR);
                int size = name.size();

                dataSet.iterationLength = name[size-1].size() - (IMAGE_FILE_EXTENSION.size()+1);
                dataSet.stageLength = name[size-2].size();
                for(int i = 0; i < size-2; ++i)
                {
                    dataSet.prefix += name[i]+ST_IT_SEPARATOR;
                }
                dataSetInitialized = true;
            }

            images.append(iterator.fileName());

        }
        else if(!extension.compare(DATA_FILE_EXTENSION, Qt::CaseInsensitive))
        {
            dataFile = iterator.filePath();
        }
    }

    if(dataFile.isEmpty())
    {
        emit notifyProgress("Loading failed - no data file");
        return;
    }
    if(images.isEmpty())
    {
        emit notifyProgress("Loading failed - no images");
        return;
    }

    images.sort();

    int bxColumn = -1;
    int byColumn = -1;
    int bzColumn = -1;
    int mxColumn = -1;
    int myColumn = -1;
    int mzColumn = -1;
    int stColumn = -1;
    int itColumn = -1;
    register bool columnsInitialized = false;
    QFile file(dataFile);
    if(file.open(QIODevice::ReadOnly))
    {
        QTextStream data(&file);
        while(!data.atEnd())
        {
            QString line = data.readLine();
            if(line.startsWith("#"))
            {
                if(line.contains("columns", Qt::CaseInsensitive))
                {
                    bxColumn = getColumnIndex(line, "Bx");
                    byColumn = getColumnIndex(line, "By");
                    bzColumn = getColumnIndex(line, "Bz");
                    mxColumn = getColumnIndex(line, "mx");
                    myColumn = getColumnIndex(line, "my");
                    mzColumn = getColumnIndex(line, "mz");
                    stColumn = getColumnIndex(line, "stage", "iteration");
                    itColumn = getColumnIndex(line, "iteration", "stage");
                    if(bxColumn != -1 && byColumn != -1 && bzColumn != -1 &&
                        mxColumn != -1 && myColumn != -1 && mzColumn != -1 &&
                        stColumn != -1 && itColumn != -1)
                    {
                        columnsInitialized = true;
                    }
                }
            }
            else if(columnsInitialized)
            {
                QStringList splittedLine = line.split(QRegExp("\\s+"));
                int splitSize = splittedLine.size();
                if(splitSize < bxColumn || splitSize < byColumn || splitSize < bzColumn ||
                        splitSize < mxColumn || splitSize < myColumn || splitSize < mzColumn ||
                        splitSize < stColumn || splitSize < itColumn)
                {
                    continue;
                }

                QString stage;
                QString iteration;
                for(int i = dataSet.stageLength - splittedLine.at(stColumn).size(); i > 0; --i)
                {
                    stage += "0";
                }
                stage += splittedLine.at(stColumn);
                for(int i = dataSet.iterationLength - splittedLine.at(itColumn).size(); i > 0; --i)
                {
                    iteration += "0";
                }
                iteration += splittedLine.at(itColumn);

                QString name = dataSet.prefix + stage + ST_IT_SEPARATOR + iteration + "." + IMAGE_FILE_EXTENSION;

                if(images.indexOf(name) != -1)
                {
                    Image image;
                    image.stage = stage;
                    image.iteration = iteration;
                    image.bx = splittedLine.at(bxColumn).toDouble();
                    image.by = splittedLine.at(byColumn).toDouble();
                    image.bz = splittedLine.at(bzColumn).toDouble();
                    image.mx = splittedLine.at(mxColumn).toDouble();
                    image.my = splittedLine.at(myColumn).toDouble();
                    image.mz = splittedLine.at(mzColumn).toDouble();
                    dataSet.images.append(image);
                    emit notifyProgress("Images loaded " + QString::number(++loadCounter) + " / " + QString::number(images.size()));
                }
            }
            else
            {
                emit notifyProgress("Loading failed - can not parse data file (no data or '#columns' line)");
                return;
            }
        }
        emit result(dataSet);
        file.close();
        emit notifyProgress("Loading finished");
        return;
    }
    else
    {
        emit notifyProgress("Loading failed - can not open data file");
        return;
    }
}

int Loader::getColumnIndex(QString line, QString contain, QString notContain)
{
    while(true)
    {
        QString tmp;
        int columnIndex = -2;
        bool ignoreWhitespaces = false;
        for(QChar c : line)
        {
            tmp += c;
            if(c == '{')
            {
                ignoreWhitespaces = true;
            }
            if(c == '}')
            {
                ignoreWhitespaces = false;
                if(tmp.size() > 1)
                {
                    ++columnIndex;
                }
                if(tmp.contains(contain, Qt::CaseInsensitive)
                        && (!tmp.contains(notContain, Qt::CaseInsensitive) || notContain == ""))
                {
                    return columnIndex;
                }
                tmp.clear();
            }
            if(c == ' ' && !ignoreWhitespaces)
            {
                if(tmp.size() > 1)
                {
                    ++columnIndex;
                }
                if(tmp.contains(contain, Qt::CaseInsensitive)
                        && (!tmp.contains(notContain, Qt::CaseInsensitive) || notContain == ""))
                {
                    return columnIndex;
                }
                tmp.clear();
            }
        }
    }
    return -1;
}

