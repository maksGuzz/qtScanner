#ifndef LAZYSTRATEGY_H
#define LAZYSTRATEGY_H

#include <QObject>

class LazyStrategy : public QObject
{
  Q_OBJECT

public:
  explicit LazyStrategy(QObject *parent = 0);
  void setSignatures(QList<QByteArray> &sig);
  void setFilelist(QList<QString> &files);
  void processSearchBuffer(QByteArray blob);

signals:
  void progress(QString, int);
  void finished();
  void error(QString);
  void found(QString, QByteArray);

public slots:
  void process();
  void stop();

private:
  bool wantStop;
  QList<QByteArray> signatures;
  QList<QString> filenames;
  QString processingFile;
};

#endif // LAZYSTRATEGY_H
