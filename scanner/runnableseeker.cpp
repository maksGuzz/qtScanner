#include "runnableseeker.h"
#include <QDebug>
#include <QThread>

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
  int ccc=0;
  foreach(signature,signatures)
    {
      // ALGORYTHM HERE!
      uchar siglen = signature.size();
      for(unsigned long long i=0; i<bufferSz-siglen; i++)
        {
          //const char* bufbegin=const_cast<char*>(mappedPtr+i);
          //ccc+=mappedPtr[i];
          char* begin=mappedPtr+i;
          ccc+=begin[0];
          QByteArray where;
          where.fromRawData(begin, siglen);
          if(where.indexOf(signature) != -1)
            {
              //found!
              emit found(filename, signature);
              qDebug()<<"---------------------------------------- FOUND! "<<filename;
            }
        }
    }
}
