#include "dialog.h"
#include <QApplication>
#include <QDebug>
#include "neuronizh.h"
#include <iostream>
deque<int> d;

int main(int argc, char *argv[])
{
//    QApplication a(argc, argv);
//    Dialog w;

//     w.show();

    d.resize(3);
    d.push_front(3);
    d.pop_back();
    d.push_front(2);
    d.pop_back();
    d.push_front(1);
    d.pop_back();
    //qDebug()<<d[0]<<d[1]<<d[2]<<d.size();
    //cout<<"hello";
     copy( d.begin(), d.end(), ostream_iterator<int>(cout," ") );
    

//    return a.exec();
}
