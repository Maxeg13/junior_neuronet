#include "dialog.h"
#include <QApplication>
#include <QDebug>
#include "neuronizh.h"
#include <iostream>
deque<int> d;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Dialog w;

     w.show();

    //qDebug()<<d[0]<<d[1]<<d[2]<<d.size();
    //cout<<"hello";
     copy( d.begin(), d.end(), ostream_iterator<int>(cout," ") );
    

    return a.exec();
}
