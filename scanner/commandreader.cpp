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
      /*QDirIterator it("/home/mguzeev/Downloads", QStringList() << "*.*", QDir::Files, QDirIterator::Subdirectories);
      while (it.hasNext()) {
          qDebug() << it.next();*/

      dirSize("/home/mguzeev/Downloads");
    }
  if(command=="setscanoptions")
    {
      qDebug()<<"setscanoptions";
    }
  if(command=="lazyscan")
    {
      qDebug()<<"setscanoptions";
    }
  if(command=="startscan")
    {
      qDebug()<<"startscan";
    }
  if(command=="stopscan")
    {
      qDebug()<<"stopscan";
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


long long CommandReader::dirSize(const QString str)
{
    long long sizex = 0;
    QFileInfo str_info(str);
    if (str_info.isDir())
    {
    QDir dir(str);
    QStringList ext_list;
    dir.setFilter(QDir::Files | QDir::Dirs |  QDir::Hidden | QDir::NoSymLinks);
    QFileInfoList list = dir.entryInfoList();
    for (int i = 0; i < list.size(); ++i)
            {
                QFileInfo fileInfo = list.at(i);
                if ((fileInfo.fileName() != ".") && (fileInfo.fileName() != ".."))
                {
                    if(fileInfo.isDir())
                    {
                        sizex += this->dirSize(fileInfo.filePath());
                        QApplication::processEvents();
                    }
                    else
                    {
                        sizex += fileInfo.size();
                        QApplication::processEvents();
                    }

                }
                qDebug()<<"Sz: "<<sizex;
            }

    }
    return sizex;
}
