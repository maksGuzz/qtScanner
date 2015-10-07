#ifndef COMMANDREADER_H
#define COMMANDREADER_H

#include <QObject>

class CommandReader : public QObject
{
  Q_OBJECT
public:
  explicit CommandReader(QObject *parent = 0);

signals:
  void exit();
  void startScan();
  void stopScan();

public slots:
  void readCommand(QDataStream &in);

};

#endif // COMMANDREADER_H
