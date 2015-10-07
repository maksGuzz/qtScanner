#include <QApplication>
 #include <QtCore>

 #include <stdlib.h>

 #include "server.h"

 int main(int argc, char *argv[])
 {
     QApplication app(argc, argv);
     Server server;
     qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));
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
