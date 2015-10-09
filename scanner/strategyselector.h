#ifndef STRATEGYSELECTOR_H
#define STRATEGYSELECTOR_H

#include <QObject>
#include <QMap>


class StrategySelector : public QObject
{
  Q_OBJECT
public:
  explicit StrategySelector(QObject *parent = 0);

  void readFileListAndSort(const QString &path);

signals:
  void sendToUiString(QString msg);
  void sendToUiInfect(QString, QString);
  void stopWorker();
  void progressForUi(QString,int);
  void startScanInfo(int);

public slots:
  void startScan(QString);
  void stopScan();
  void errorHandler(QString error);
  void scanProgress(QString, int);
  void lazyScanEnded();
  void poolScanEnded();
  void sigFound(QString, QByteArray);
  void parseSignatures(QString);

private:
  //long long dirSize(const QString str);
  void lazyScan();
  void threadedScan();

private:
  QList<QString> largeFiles; // for ThreadPool strategy (fSz >= 16k), one file at a time, each thread with a chunk of signatures
  QList<QString> smallFiles; // for lazy strategy, single thread, read all file at once

  QMap<QByteArray, QString> signatures;
  QThread* lazyThread;
  QThread* poolThread;
  QString path;
  int totalFiles;
  int processedFiles;
  int oldProgress;
};

#endif // STRATEGYSELECTOR_H
