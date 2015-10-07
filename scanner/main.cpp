#include <QApplication>
 #include <QtCore>

 #include <stdlib.h>

 #include "server.h"
#include "commandreader.h"

 int main(int argc, char *argv[])
 {
     QApplication app(argc, argv);
     Server server;
     CommandReader reader;

     QObject::connect(&server, SIGNAL(uicommand(QDataStream&)), &reader, SLOT(readCommand(QDataStream&)));

     return app.exec();
 }
 /*
QFile file(argv[1]);
    file.open(QFile::ReadWrite);
    uchar *image = file.map(0, file.size());
    memset(image, 0, file.size());


const int file_size=something;
const int page_size=0x1000;
int off=0;
void *data;

int fd = open("filename.bin", O_RDONLY);

while (off < file_size)
{
  data = mmap(NULL, page_size, PROT_READ, 0, fd, off);
  // do stuff with data
  munmap(data, page_size);
  off += page_size;
}
  */
