#ifndef COMPARER_H
#define COMPARER_H

#include <QThread>

class Comparer : public QThread
{
    Q_OBJECT
public:
    Comparer();
    virtual ~Comparer();

    virtual void compare() = 0;

signals:
    void notifyProgress(const QString &message);

private:
    virtual void run();

};

#endif // COMPARER_H
