#include "mainwindow.h"
#include <QApplication>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    printf("888\n");
    MainWindow w;
    printf("999\n");
    w.show();
    printf("1010aaa\n");
    return a.exec();
    printf("1010bbb\n");
}
