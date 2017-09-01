#include "dialog.h"
#include <QApplication>
//#include <QDebug>
#include "neuronizh.h"
#include <iostream>


int main(int argc, char *argv[])
{
    std::cout<<"hello!";
    QApplication a(argc, argv);
    Dialog w;
    //    w.setToolTip("hello");
    w.show();

    //     copy( d.begin(), d.end(), ostream_iterator<int>(cout," ") );
    

    return a.exec();
}
