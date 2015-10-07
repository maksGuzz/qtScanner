#ifndef STRATEGYSELECTOR_H
#define STRATEGYSELECTOR_H

#include <QObject>
#include <QMap>


class StrategySelector : public QObject
{
  Q_OBJECT
public:
  explicit StrategySelector(QObject *parent = 0);

signals:
  void stringForUi(QString msg);
  void stopWorker();

public slots:
  void startScan();
  void stopScan();
  void errorHandler(QString error);
  void scanProgress(int);
  void scanEnded();
  void sigFound(QByteArray);

private:
  //long long dirSize(const QString str);
  void lazyScan();

private:
  QList<QString> files;
  QMap<QByteArray, QString> signatures;//QByteArray QByteArray::fromHex(const QByteArray & hexEncoded) static
  QThread* lazyThread;
};

#endif // STRATEGYSELECTOR_H
