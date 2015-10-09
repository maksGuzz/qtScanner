#include "commandreader.h"
#include <QDebug>
#include <QApplication>
#include <QDirIterator>

CommandReader::CommandReader(QObject *parent) :
  QObject(parent)
{
}

void CommandReader::readCommand(QDataStream &in)
{
  QString command;
  in >> command;

  qDebug()<< "Command: " << command;
  if(command=="parsesignatures")
    {
      QString filename;
      in >> filename;
      qDebug()<<"Loading"<<filename;
      emit parseSignatures(filename);
    }
  if(command=="setscanoptions")
    {
      qDebug()<<"setscanoptions";
    }
  if(command=="start")
    {
      qDebug()<<"startscan";
      QString folder;
      in >> folder;
      emit startScan(folder);
    }
  if(command=="stop")
    {
      qDebug()<<"stopscan";
      emit stopScan();
    }
  if(command=="status")
    {
      qDebug()<<"getstaus";
    }
  if(command=="exit")
    {
      qDebug()<<"exiting";
      QCoreApplication::instance()->quit();
    }
}


