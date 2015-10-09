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
  signatures.insert(QByteArray::fromHex("c9ff30006893"), "APK");
  signatures.insert(QByteArray::fromHex("005a015a019e680000abd2ae000000"), "MARGO ZIP");
  signatures.insert(QByteArray::fromHex("98b6959c2a00e998e4e71524"), "ZWAY TGZ");
  signatures.insert(QByteArray::fromHex("030000000000000040282e00"), "BTH-D");
  signatures.insert(QByteArray::fromHex("fcff498bbff0000000488d3531670000"),"APACHE2");
  signatures.insert(QByteArray::fromHex("f3cf 32a1 e9c9 9238 01c3 9d26 f8e8 10fa 963c 1867 2c88 b083 16ea b787 219d 01d7"), "LG-SMART-PNG");
  signatures.insert(QByteArray::fromHex("8c5a6cc01ef89c9750660d7d05d4378aac89a44ffbd30522e2ee63d4e20d7ce9f1f3d17bf0a635b44de6"),"aero@2");
  signatures.insert(QByteArray::fromHex("00 6564 6974 6162 6c65 2d74 6162 6c65 2d6d 6173 7465 722f 6578 7465 726e 616c 2f67 6f6f 676c 652d 636f 6465 2d70 7265 74"),"ED-TBL-ZIP");
  signatures.insert(QByteArray::fromHex("b8 9929 a796 a9a9 1572 8f9d db51 2354 a0"), "DALTON HW Duc");
  signatures.insert(QByteArray::fromHex("af49 1212 082c d3b6 29ac2869 3c66 0063 1d"), "Kompex-ZIP");
  signatures.insert(QByteArray::fromHex("83 830a 0f60 3a6b d3226e27 e77d 4660 f208 6f67 91f6 6998 f6ea3225 b979 94e9 b34a 25f5 3b71 38ed 1a6a407a cccb 5786 e341"), "F4-ISO");

  path = "/home/mguzeev/Downloads";
}

void StrategySelector::readFileListAndSort(const QString &path)
{
  //emit sendToUiString("Scanning files");
  qDebug()<<"read files "<<path;
  totalFiles = 0;
  processedFiles = 0;
  oldProgress = 0;
  QDirIterator it(path, QDir::Files | QDir::Hidden | QDir::NoSymLinks, QDirIterator::Subdirectories);
  while (it.hasNext())
    {
      it.next();
      //qDebug()<<it.fileName()<< ", "<<it.fileInfo().size();
      if(it.fileInfo().size()<=100 || it.filePath()=="") // smallest signature
        {
          //qDebug()<<"IGNORING";
          continue; //dir or ".." or "."
        }
      if(it.fileInfo().size() < 0x4000) // 16K
        {
          //qDebug()<<"SMALL";
          //qDebug()<<"--fSz: "<<it.fileInfo().size()<<", "<<it.fileName() << " small";
          smallFiles << it.fileInfo().filePath();
        }
      else
        {
          //qDebug()<<"LARGE";
          //qDebug()<<"--fSz: "<<it.fileInfo().size()<<", "<<it.fileName() << " LARGE!";
          largeFiles << it.fileInfo().filePath();
        }

      //emit stringForUi(it.fileInfo().fileName());
      if(++totalFiles%100==0)
        qApp->processEvents();
    }
  //emit sendToUiString("Scanning files end");
}

void StrategySelector::startScan(QString folder)
{
  path = folder;
  if(poolThread || lazyThread)
    {
      emit sendToUiString("busy both");
      return;
    }
  largeFiles.clear();
  smallFiles.clear();
  emit sendToUiString("Scanning directory");
  qDebug()<<"read files";
  readFileListAndSort(path);
  qDebug()<<"start scan end";
  emit sendToUiString("Scanning directory end");
  qDebug()<<"Cnt: LARGE " << largeFiles.size() << ", small "<<smallFiles.count();

  if(largeFiles.size())
    {
      // run threaded strategy
        emit progressForUi(largeFiles.at(0), 0);
        threadedScan();
    }
  //return;
  if(smallFiles.size())
    {
      // run one thread strategy
        emit progressForUi(smallFiles.at(0), 0);
        lazyScan();
    }
  emit startScanInfo(totalFiles);
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
  processedFiles++;
  emit progressForUi(file, processedFiles);
  /*if(progress == 100)
    {
      processedFiles++;
      int newProgress = processedFiles/totalFiles*1000;
      QVariant f(processedFiles);
      QVariant n(newProgress);
      QVariant o(oldProgress);
        QString tosend = QString("Processing f: %1, old: %2, new: %3").arg(f.toString()).arg(o.toString()).arg(n.toString());
        qDebug()<<tosend;
      if(newProgress != oldProgress)
        {
          qDebug()<<"EMIT!";
          oldProgress = processedFiles/totalFiles*10;
          emit progressForUi("", processedFiles);
        }
    }*/
}

void StrategySelector::lazyScanEnded()
{
  //qApp->processEvents();
  qDebug()<<"scanEnded: lazy";
  /*lazyThread->quit();
  lazyThread->wait();*/
  lazyThread=NULL;
  emit sendToUiString("Scan fileList ended lazy");
}

void StrategySelector::poolScanEnded()
{
  //qApp->processEvents();
  qDebug()<<"scanEnded: pool";
  /*lazyThread->quit();
  lazyThread->wait();*/
  poolThread=NULL;
  emit sendToUiString("Scan fileList ended pool");
}

void StrategySelector::sigFound(QString fname, QByteArray sig)
{
  QString formatted = QString("%1 - %2").arg(signatures[sig]).arg(fname);
  qDebug()<<"            FOUND!!";
  qDebug()<<"                "<<formatted;
  //emit sendToUiString(formatted);
}

void StrategySelector::parseSignatures(QString signFilename)
{
  //
  qDebug()<< "Parse filename"<<signFilename;
  emit sendToUiString("Parsing...");
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
  worker->setFilelist(largeFiles);

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


