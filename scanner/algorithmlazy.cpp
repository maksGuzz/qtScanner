#include "algorithmlazy.h"
#include <QDirIterator>
#include <QDebug>
#include <QThread>
#include <QApplication>

AlgorithmLazy::AlgorithmLazy(QObject *parent) :
  QObject(parent)
  ,wantStop(false)
{

}

void AlgorithmLazy::setSignatures(QList<QByteArray> sig)
{
  signatures = sig;
}

void AlgorithmLazy::processSearchBuffer(QByteArray blob)
{
  //qDebug()<<"BLOB!";
  QList<QByteArray>::iterator it;
  for(it=signatures.begin(); it!= signatures.end(); ++it)
    {
      if(blob.indexOf(*it) != -1)
        {
          //found!
          emit found(*it);
        }
    }
}

void AlgorithmLazy::process()
{
  long cnt=0;
  QDirIterator it("/home/mguzeev/Downloads", QStringList() << "*.*", QDir::Files, QDirIterator::Subdirectories);
  while (it.hasNext())
    {
      if(wantStop)
        {
          qDebug()<< "--- FINISHING";
          break;
        }
      //qDebug()<<"--fSz: "<<it.fileInfo().size()<<", "<<it.filePath();

      QFile file(it.filePath());
      if(!file.open(QIODevice::ReadOnly))
        {
          qDebug()<< "Error open file " << it.filePath();
          it.next();
          continue;
        }
      int chunkSz = 1024 * 5;
      int chunksCount = it.fileInfo().size() / chunkSz;
      if(chunksCount <  10 ) //50KB
        {
          qDebug()<< "readall "<<it.fileName();;
          QByteArray blob = file.readAll();
          processSearchBuffer(blob);
        }
      else
        {
          QByteArray blob;
          while ((blob = file.read(chunkSz*10)) > 0)
            {
              processSearchBuffer(blob);
              if(++cnt%100==0)
                qApp->processEvents();
            }
        }
      file.close();



      emit progress(it.fileInfo().size());

      //QThread::msleep(10);
      it.next();
      if(++cnt%100==0)
        qApp->processEvents();
    }
  emit finished();
}

void AlgorithmLazy::stop()
{
  qDebug()<< "--- STOP requested";
  wantStop = true;
}
