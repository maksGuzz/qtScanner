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
  if(command=="loadsignatures")
    {
      qDebug()<<"Loading";
    }
  if(command=="setscanoptions")
    {
      qDebug()<<"setscanoptions";
    }
  if(command=="lazy")
    {
      qDebug()<<"lazyscan";
      emit startScan();
    }
  if(command=="start")
    {
      qDebug()<<"startscan";
      emit startScan();
    }
  if(command=="stop")
    {
      qDebug()<<"stopscan";
      emit stopScan();
    }
  if(command=="getstatus")
    {
      qDebug()<<"getstaus";
    }
  if(command=="exit")
    {
      qDebug()<<"exiting";
      QCoreApplication::instance()->quit();
    }
}


