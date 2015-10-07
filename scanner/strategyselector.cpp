#include "strategyselector.h"
#include <QDebug>
#include <QApplication>
#include <QThread>
#include <QDirIterator>

#include "algorithmlazy.h"


StrategySelector::StrategySelector(QObject *parent) :
  QObject(parent)
{
  //fake signatures
  signatures.insert("gif", "GIF");
  signatures.insert("GIF", "GIF");
  signatures.insert("<?php", "PHP");
  signatures.insert("ELF", "ELF");
  signatures.insert("SIG", "SIG");
  signatures.insert("123", "Sig1");
  signatures.insert("5432325", "Sig2");
  signatures.insert("fds", "Sig3");
  signatures.insert(QByteArray().fromHex("b6b7b8b9bac2"), "Sig4");//b6b7b8b9bac2

}

void StrategySelector::startScan()
{
  //if alot of files: one thread-one file with all signatures
  //one file: check size - small: one thread, big - thread pool each thread-piece of signatures
  // ...
  if(lazyThread)
    {
      qDebug()<<"busy";
      emit stringForUi("busy");
    }
  lazyScan();
}

void StrategySelector::stopScan()
{

  emit stopWorker();
  /*if(lazyThread)
    {
      scanEnded();
    }*/
  qDebug()<<"scan stop";
}

void StrategySelector::errorHandler(QString error)
{
  qDebug()<<"Error from thread: "<<error;
}

void StrategySelector::scanProgress(int progress)
{
  //qDebug()<<"Progress: "<<progress;
  QVariant v(progress);
  QString tosend = QString("Processing %1").arg(v.toString());
  emit stringForUi(tosend);
}

void StrategySelector::scanEnded()
{
  qDebug()<<"scanEnded: 1";
  lazyThread->quit();
  lazyThread->wait();
  lazyThread=NULL;
  qDebug()<<"scanEnded: 2";
  emit stringForUi("lazyScanEnded");
}

void StrategySelector::sigFound(QByteArray sig)
{
  qDebug()<<"            FOUND!!";
  emit stringForUi(signatures[sig]);
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
  AlgorithmLazy* worker = new AlgorithmLazy();
  worker->setSignatures(sig);

  worker->moveToThread(lazyThread);

  connect(worker, SIGNAL(error(QString)), this, SLOT(errorHandler(QString)));
  connect(lazyThread, SIGNAL(started()), worker, SLOT(process()));
  connect(worker, SIGNAL(finished()), lazyThread, SLOT(quit()));
  connect(worker, SIGNAL(finished()), worker, SLOT(deleteLater()));
  connect(lazyThread, SIGNAL(finished()), lazyThread, SLOT(deleteLater()));

  connect(lazyThread, SIGNAL(finished()), this, SLOT(scanEnded()));
  connect(worker, SIGNAL(progress(int)), this, SLOT(scanProgress(int)));
  connect(this, SIGNAL(stopWorker()), worker, SLOT(stop()));
  connect(worker, SIGNAL(found(QByteArray)), this, SLOT(sigFound(QByteArray)));

  lazyThread->start();

}

/*long long StrategySelector::dirSize(const QString str)
{
    long long sizex = 0;
    QFileInfo str_info(str);
    if (str_info.isDir())
    {
    QDir dir(str);
    QStringList ext_list;
    dir.setFilter(QDir::Files | QDir::Dirs |  QDir::Hidden | QDir::NoSymLinks);
    QFileInfoList list = dir.entryInfoList();
    for (int i = 0; i < list.size(); ++i)
            {
                QFileInfo fileInfo = list.at(i);
                if ((fileInfo.fileName() != ".") && (fileInfo.fileName() != ".."))
                {
                    if(fileInfo.isDir())
                    {
                        sizex += this->dirSize(fileInfo.filePath());
                        QApplication::processEvents();
                    }
                    else
                    {
                        sizex += fileInfo.size();
                        QApplication::processEvents();
                    }

                }
                qDebug()<<"Sz: "<<sizex;
            }

    }
    return sizex;
}*/


