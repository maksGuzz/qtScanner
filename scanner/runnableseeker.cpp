#include "runnableseeker.h"
#include <QDebug>
#include <QThread>
#include <QApplication>

RunnableSeeker::RunnableSeeker(QObject *parent) :
  QObject(parent)
{

}

void RunnableSeeker::setAll(const QString name, char* buff, unsigned long long sz, const QList<QByteArray> &s)
{
  filename =name;
  mappedPtr=buff;
  signatures=s;
  bufferSz=sz;
}


void RunnableSeeker::run()
{
  //qDebug()<<"Seek " << QThread::currentThread() << ", " << filename;
  QByteArray signature;
  foreach(signature,signatures)
    {
      qApp->processEvents();
      qDebug()<<"Progress...";
      // ALGORYTHM HERE!
      unsigned long long siglen = signature.size();
      for(unsigned long long i=0; i<bufferSz-siglen; i++)
        {
          char* begin=mappedPtr+i;
          if(!memcmp(begin,signature.constData(),siglen))
            {
              emit found(filename, signature);
              qDebug()<<"---------------------------------------- FOUND! "<<filename;
              signatures.removeAll(signature);
              break;
            }
        }
    }
}
