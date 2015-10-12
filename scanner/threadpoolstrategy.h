#ifndef THREADPOOLSTRATEGY_H
#define THREADPOOLSTRATEGY_H

#include <QObject>

class ThreadPoolStrategy : public QObject
{
  Q_OBJECT

public:
  explicit ThreadPoolStrategy(QObject *parent = 0);
  void setSignatures(QList<QByteArray> &sig);
  void setFilelist(QList<QString> &files);

signals:
  void progress(QString, int);
  void finished();
  void error(QString);
  void found(QString, QByteArray);

public slots:
  void process();
  void stop();
  void signatureFound(const QString&,const QByteArray&);

private:
  bool wantStop;
  QList<QByteArray> signatures;
  QList<QString> filenames;
  QString processingFile;
  int maxThreadCount;
};

#endif // THREADPOOLSTRATEGY_H
