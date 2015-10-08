#ifndef RUNNABLESEEKER_H
#define RUNNABLESEEKER_H

#include <QObject>
#include <QRunnable>

class RunnableSeeker : public QObject, public QRunnable
{
    Q_OBJECT
private:
    QString filename;
    char* mappedPtr;
    QList<QByteArray> signatures;
    unsigned long long bufferSz;
public:
    RunnableSeeker(QObject *parent=0);
    void setAll(const QString name, char* buff, unsigned long long sz, const QList<QByteArray> &s);

protected:
    void run();

signals:
    void found(const QString&,const QByteArray&);
};

#endif // RUNNABLESEEKER_H
