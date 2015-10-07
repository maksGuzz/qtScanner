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

public slots:
  void startScan();
  void stopScan();

private:
  long long dirSize(const QString str);

private:
  QList<QString> files;
  QMap<QString,QByteArray> signatures;//QByteArray QByteArray::fromHex(const QByteArray & hexEncoded) static

};

#endif // STRATEGYSELECTOR_H
