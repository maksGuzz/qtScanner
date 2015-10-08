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

void ThreadPoolStrategy::processSearchBuffer(QByteArray blob)
{
  //TODO Use separated class Algorythm. Depending on number of signatures and moon's phase
  QByteArray signature;
  int cnt = signatures.size();
  foreach(signature, signatures)
    {
      //emit progress(processingFile, cnt--);
      if(blob.indexOf(signature) != -1)
        {
          //found!
          emit found(processingFile, signature);
        }
      if(cnt%100==0)
        qApp->processEvents();
    }
}

void ThreadPoolStrategy::process()
{
  long cnt=0;
  QThreadPool *threadPool = QThreadPool::globalInstance();
  //maxThreadCount didvider for signatures per thread
  foreach(processingFile, filenames)
    {
      if(wantStop)
        {
          qDebug()<< "--- FINISHING";
          break;
        }
      //qDebug()<<"--fSz: "<<it.fileInfo().size()<<", "<<it.filePath();

      //emit progress(processingFile, 0);
      QFile file(processingFile);
      if(!file.open(QIODevice::ReadOnly))
        {
          qDebug()<< "Error open file " << processingFile;
          continue;
        }

      ////////////// mapping
      uchar *vbuf = file.map(0, file.size());
      for(int i=0; i<maxThreadCount;i++)
        {
          RunnableSeeker *seeker= new RunnableSeeker();
          seeker->setAll(processingFile,(char*)vbuf,file.size(),signatures);
          connect(seeker,SIGNAL(found(const QString&,const QByteArray&)),this,SLOT(signatureFound(const QString&,const QByteArray&)));
          threadPool->start(seeker);
        }
      threadPool->waitForDone();
      file.unmap(vbuf);
      file.close();

      //QThread::msleep(10);
      if(++cnt%100==0)
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
}

