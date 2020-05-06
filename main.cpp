#include "mainwindow.h"
#include "server.h"
#include <QApplication>
#include <sys/types.h>   // 提供类型 pid_t 的定义
#include <unistd.h>
#include <qdebug.h>
#include <signal.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <QThread>
#include <QDebug>
#include <sys/wait.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();    

    return a.exec();
}
