#include "strategyselector.h"
#include <QDebug>
#include <QApplication>
#include <QThread>
#include <QDirIterator>


#include "lazystrategy.h"
#include "threadpoolstrategy.h"


StrategySelector::StrategySelector(QObject *parent) :
  QObject(parent)
  , lazyThread(NULL)
  , poolThread(NULL)
{
  //fake signatures
  /*signatures.insert("gif", "GIF");
  signatures.insert("GIF", "GIF");
  signatures.insert("<?php", "PHP");
  signatures.insert("ELF", "ELF");
  signatures.insert("SIG", "SIG");
  signatures.insert("123", "Sig1");
  signatures.insert("5432325", "Sig2");
  signatures.insert("fds", "Sig3");*/
  signatures.insert(QByteArray::fromHex("b6b7b8b9bac2"), "JPEG");
  signatures.insert(QByteArray::fromHex("c9ff30006893"), "APK");
  signatures.insert(QByteArray::fromHex("005a 015a 019e 6800 00ab d2ae 0000 00"), "MARGO ZIP");
  signatures.insert(QByteArray::fromHex("98b6 959c 2a00 e998 e4e7 1524"), "ZWAY TGZ");
  // grep -obUaP "\xb6\xb7\xb8\xb9\xba\xc2"
  //qDebug()<<"Sz from hex: "<<QByteArray::fromHex("c9ff30006893").size();
}

void StrategySelector::readFileListAndSort(const QString &path)
{
  emit stringForUi("Scanning files");
  qDebug()<<"read files "<<path;
  int cnt=0;
  QDirIterator it(path, QStringList() << "*.*", QDir::Files, QDirIterator::Subdirectories);
  while (it.hasNext())
    {
      //qDebug()<<it.fileName();
      if(it.fileInfo().size()<=10 || it.filePath()=="") // smallest signature
        {
          it.next();
          continue; //dir or ".." or "."
        }
      if(it.fileInfo().size() < 0x4000) // 16K
        {
          //qDebug()<<"--fSz: "<<it.fileInfo().size()<<", "<<it.fileName() << " small";
          smallFiles << it.fileInfo().filePath();
        }
      else
        {
          //qDebug()<<"--fSz: "<<it.fileInfo().size()<<", "<<it.fileName() << " LARGE!";
          largeFiles << it.fileInfo().filePath();
        }

      //emit stringForUi(it.fileInfo().fileName());
      //QThread::msleep(10);
      it.next();
      if(++cnt%100==0)
        qApp->processEvents();
    }
  emit stringForUi("Scanning files end");
}

void StrategySelector::startScan()
{
  largeFiles.clear();
  smallFiles.clear();
  emit stringForUi("Scanning directory");
  qDebug()<<"read files";
  readFileListAndSort("/home/mguzeev");
  qDebug()<<"start scan end";
  emit stringForUi("Scanning directory end");
  qDebug()<<"Cnt: LARGE " << largeFiles.size() << ", small "<<smallFiles.count();

  if(largeFiles.size())
    {
      // run threaded strategy
      if(poolThread!=NULL)
          {
            qDebug()<<"busy";
            emit stringForUi("busy");
            return;
          }
        emit stringForUi("Scanning LARGE fileList");
        threadedScan();
    }
  //return;
  if(smallFiles.size())
    {
      // run one thread strategy
      if(lazyThread!=NULL)
          {
            qDebug()<<"busy";
            emit stringForUi("busy");
            return;
          }
        emit stringForUi("Scanning SMALL fileList");
        lazyScan();
    }
  qDebug()<<"start scan end";
}

void StrategySelector::stopScan()
{

  emit stopWorker();
  qDebug()<<"scan stop";
}

void StrategySelector::errorHandler(QString error)
{
  qDebug()<<"Error from thread: "<<error;
}

void StrategySelector::scanProgress(QString file, int progress)
{
  QVariant v(progress);
  QString tosend = QString("Processing %1, %2").arg(file).arg(v.toString());
  qDebug()<<tosend;
  emit stringForUi(tosend);
}

void StrategySelector::lazyScanEnded()
{
  //qApp->processEvents();
  qDebug()<<"scanEnded: 1";
  /*lazyThread->quit();
  lazyThread->wait();*/
  lazyThread=NULL;
  emit stringForUi("Scan fileList ended");
}

void StrategySelector::poolScanEnded()
{
  //qApp->processEvents();
  qDebug()<<"scanEnded: 1";
  /*lazyThread->quit();
  lazyThread->wait();*/
  lazyThread=NULL;
  emit stringForUi("Scan fileList ended");
}

void StrategySelector::sigFound(QString fname, QByteArray sig)
{
  QString formatted = QString("%1 - %2").arg(signatures[sig]).arg(fname);
  qDebug()<<"            FOUND!!";
  qDebug()<<"                "<<formatted;
  emit stringForUi(formatted);
}

void StrategySelector::threadedScan()
{
  //start one thread function
  //iterate directories and scan files one-by one with all signatures
  //get keys from signatures
  QList<QByteArray> sig;
  QMap<QByteArray, QString>::iterator i;
  for (i = signatures.begin(); i != signatures.end(); ++i)
      sig<<i.key();

  // start worker
  poolThread = new QThread;
  ThreadPoolStrategy* worker = new ThreadPoolStrategy();
  worker->setSignatures(sig);
  worker->setFilelist(smallFiles);

  worker->moveToThread(poolThread);

  connect(worker, SIGNAL(error(QString)), this, SLOT(errorHandler(QString)));
  connect(poolThread, SIGNAL(started()), worker, SLOT(process()));
  connect(worker, SIGNAL(finished()), poolThread, SLOT(quit()));
  connect(worker, SIGNAL(finished()), worker, SLOT(deleteLater()));
  connect(poolThread, SIGNAL(finished()), poolThread, SLOT(deleteLater()));

  connect(poolThread, SIGNAL(finished()), this, SLOT(poolScanEnded()));
  connect(worker, SIGNAL(progress(QString, int)), this, SLOT(scanProgress(QString, int)));
  connect(this, SIGNAL(stopWorker()), worker, SLOT(stop()));
  connect(worker, SIGNAL(found(QString, QByteArray)), this, SLOT(sigFound(QString, QByteArray)));

  poolThread->start();

}


void StrategySelector::lazyScan()
{
  //start one thread function
  //iterate directories and scan files one-by one with all signatures
  //get keys from signatures
  QList<QByteArray> sig;
  QMap<QByteArray, QString>::iterator i;
  for (i = signatures.begin(); i != signatures.end(); ++i)
      sig<<i.key();

  // start worker
  lazyThread = new QThread;
  LazyStrategy* worker = new LazyStrategy();
  worker->setSignatures(sig);
  worker->setFilelist(smallFiles);

  worker->moveToThread(lazyThread);

  connect(worker, SIGNAL(error(QString)), this, SLOT(errorHandler(QString)));
  connect(lazyThread, SIGNAL(started()), worker, SLOT(process()));
  connect(worker, SIGNAL(finished()), lazyThread, SLOT(quit()));
  connect(worker, SIGNAL(finished()), worker, SLOT(deleteLater()));
  connect(lazyThread, SIGNAL(finished()), lazyThread, SLOT(deleteLater()));

  connect(lazyThread, SIGNAL(finished()), this, SLOT(lazyScanEnded()));
  connect(worker, SIGNAL(progress(QString, int)), this, SLOT(scanProgress(QString, int)));
  connect(this, SIGNAL(stopWorker()), worker, SLOT(stop()));
  connect(worker, SIGNAL(found(QString, QByteArray)), this, SLOT(sigFound(QString, QByteArray)));

  lazyThread->start();

}


