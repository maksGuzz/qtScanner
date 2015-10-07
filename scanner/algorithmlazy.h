#ifndef ALGORITHMLAZY_H
#define ALGORITHMLAZY_H

#include <QObject>

class AlgorithmLazy : public QObject
{
  Q_OBJECT

public:
  explicit AlgorithmLazy(QObject *parent = 0);
  void setSignatures(QList<QByteArray> sig);
  void processSearchBuffer(QByteArray blob);

signals:
  void progress(int);
  void finished();
  void error(QString);
  void found(QByteArray);

public slots:
  void process();
  void stop();

private:
  bool wantStop;
  QList<QByteArray> signatures;
};

#endif // ALGORITHMLAZY_H
