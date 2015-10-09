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
  void stringForUi(QString msg);
  void stopWorker();

public slots:
  void startScan();
  void stopScan();
  void errorHandler(QString error);
  void scanProgress(QString, int);
  void lazyScanEnded();
  void poolScanEnded();
  void sigFound(QString, QByteArray);

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

};

#endif // STRATEGYSELECTOR_H
