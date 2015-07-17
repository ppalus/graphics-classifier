#include "comparer.h"

Comparer::Comparer()
{
}

Comparer::~Comparer()
{
}

void Comparer::run()
{
    compare();
}


Image Comparer::getCurrentImage()
{
    return dataSet.images.at(position);
}

Image Comparer::getNextImage()
{
    return dataSet.images.at(position+1);
}

QString Comparer::getNext()
{
    return dataSet.path + "/" + dataSet.getName(position++);
}

bool Comparer::hasNextTwo()
{
    return dataSet.images.size() > position+2;
}

void Comparer::setDataSet(DataSet &dataSet)
{
    this->dataSet = dataSet;
    result.path = dataSet.path;
    result.prefix = dataSet.prefix;
    result.stageLength = dataSet.stageLength;
    result.iterationLength = dataSet.iterationLength;
}
