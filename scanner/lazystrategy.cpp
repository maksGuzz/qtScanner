#include "lazystrategy.h"
#include <QDirIterator>
#include <QDebug>
#include <QThread>
#include <QApplication>

LazyStrategy::LazyStrategy(QObject *parent) :
  QObject(parent)
  ,wantStop(false)
{

}

void LazyStrategy::setFilelist(QList<QString> &files)
{
  filenames = files;
}

void LazyStrategy::setSignatures(QList<QByteArray> &sig)
{
  signatures = sig;
}

void LazyStrategy::processSearchBuffer(QByteArray blob)
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
      if(--cnt%10==0)
        qApp->processEvents();
    }
}

void LazyStrategy::process()
{
  long cnt=0;
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

      //emit progress(processingFile, signatures.size());
      QByteArray blob = file.readAll();
      processSearchBuffer(blob);

      file.close();

      //QThread::msleep(10);
      if(++cnt%10==0)
        qApp->processEvents();
    }
  qApp->processEvents();
  emit finished();
}

void LazyStrategy::stop()
{
  qDebug()<< "--- STOP requested";
  wantStop = true;
}
