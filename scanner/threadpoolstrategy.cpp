#include "threadpoolstrategy.h"
#include <QDirIterator>
#include <QDebug>
#include <QThread>
#include <QApplication>
#include <QThreadPool>
#include <QRunnable>


#include "runnableseeker.h"

ThreadPoolStrategy::ThreadPoolStrategy(QObject *parent) :
  QObject(parent)
  ,wantStop(false)
{
  maxThreadCount = QThreadPool::globalInstance()->maxThreadCount();
  qDebug()<<"max threads "<<maxThreadCount;
}

void ThreadPoolStrategy::setFilelist(QList<QString> &files)
{
  filenames = files;
}

void ThreadPoolStrategy::setSignatures(QList<QByteArray> &sig)
{
  signatures = sig;
}

void ThreadPoolStrategy::process()
{
  QThreadPool *threadPool = QThreadPool::globalInstance();
  foreach(processingFile, filenames)
    {
      if(wantStop)
        {
          qDebug()<< "--- FINISHING";
          break;
        }
      QFile file(processingFile);
      if(!file.open(QIODevice::ReadOnly))
        {
          qDebug()<< "Error open file " << processingFile;
          continue;
        }

      QList< QList<QByteArray> > perThread;
      int sigsz = signatures.size();
      int offs = sigsz/maxThreadCount;

      for(int i=0;i<maxThreadCount;i++)
        {
          perThread.push_back(signatures.mid(i*offs, offs));
        }
      if(sigsz%maxThreadCount!=0)
        {
          perThread[maxThreadCount-1] << signatures.mid(maxThreadCount*offs, offs);
        }
      //qDebug()<<perThread.count();
      qDebug()<<QThread::currentThread()<< ", " <<file.fileName()<< ", " <<file.size();
      uchar *vbuf = file.map(0, file.size());
      for(int i=0; i<maxThreadCount;i++)
        {
          //qDebug()<<perThread.at(i).count();
          RunnableSeeker *seeker= new RunnableSeeker();
          seeker->setAll(processingFile,(char*)vbuf,file.size(),perThread.at(i));
          connect(seeker,SIGNAL(found(const QString&,const QByteArray&)),this,SLOT(signatureFound(const QString&,const QByteArray&)));
          threadPool->start(seeker);
        }
      threadPool->waitForDone();
      emit progress(processingFile, 100);
      file.unmap(vbuf);
      file.close();

      //QThread::msleep(10);
      qApp->processEvents();
    }
  threadPool->waitForDone();
  emit finished();
}

void ThreadPoolStrategy::stop()
{
  qDebug()<< "--- STOP requested";
  wantStop = true;
}

void ThreadPoolStrategy::signatureFound(const QString &name, const QByteArray &sign)
{
  qDebug()<<"FOUND from ThreadPoolStrategy "<<name;
  emit found(name, sign);
}

