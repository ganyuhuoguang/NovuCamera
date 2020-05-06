#ifndef MYTHREAD_H
#define MYTHREAD_H


#include <QThread>
extern QString g_IP_ADRESS;

extern QString g_CAMREA_IP_ADRESS;
extern QString g_CAMREA_PORT;
extern QString g_PORT;

extern unsigned int IMAGEID;
extern QString g_IMAGE_PATH_LIST[10];
extern QString g_TRACKING_ID[10];
extern QString g_CONFSCORE[10];

extern unsigned int g_BODY_IMAGEID ;
extern QString g_BODY_IMAGE_PATH_LIST[10];
extern QString g_BODY_TRACKING_ID[10] ;
extern QString g_BODY_CONFSCORE[10];
extern QString g_BODY_GENDER[10];
extern QString g_BODY_AGE[10];

class MyThread : public QThread
{
    Q_OBJECT
public:
    explicit MyThread(QObject *parent = 0);
    ~MyThread();//QThread的虚函数

protected:

    //线程处理函数
    //不能直接调用，通过start()间接调用
    void run();

signals:
    void isDone();  //处理完成信号

signals:

public slots:

};


#endif // MYTHREAD_H
