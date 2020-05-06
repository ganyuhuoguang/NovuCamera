#include"mythread.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
//#include <sstream>
//#include <fstream>
//#include <string>
#include <string.h>
#include <QTime>
#include <qdebug.h>
#include "Util.h"
//#include "cJSON.h"
#include <served/served.hpp>
#include <QtNetwork/QTcpServer>
#include <QtNetwork/QTcpSocket>
//#include <QtNetwork>


QString g_IP_ADRESS = "192.168.5.145";
QString g_CAMREA_IP_ADRESS = "192.168.5.122";
QString g_CAMREA_PORT = "6123";
QString g_PORT = "3000";
#define PORT "3000"                       // 服务器监听端口

void errorHandling(char *message){
    fputs(message, stderr);
    fputc('\n', stderr);
  //  exit(1);
}

void restful()
{
    served::multiplexer mux;
        mux.handle("/form_post")
            .post([&](served::response & res, const served::request & req) {
                // check headers for the correct content type
                if ( req.header("content-type") != "application/x-www-form-urlencoded" )
                {
                    // return 400 bad request if the content type is wrong for this example.
                    served::response::stock_reply(400, res);
                }
                else
                {
                    res << "Body data: " << req.body();
                }
            });

        std::cout << "Try this example with:" << std::endl;
        std::cout << " curl http://localhost:3000/form_post -d \"hello world!\"" << std::endl;
        std::cout << g_IP_ADRESS.toStdString() << std::endl;
        std::cout << g_PORT.toStdString() << std::endl;
        served::net::server server(g_IP_ADRESS.toStdString(), g_PORT.toStdString(), mux);
        server.run(10);

       // return (EXIT_SUCCESS);
}


void server(){


//    int serv_sock;             // 保存后面创建的服务器套接字
//    int clnt_sock;             // 保存接受请求的客户端套接字

//    char buf[1024*1024*2];      // 缓冲区

//    struct sockaddr_in serv_addr; // 保存服务器套接字地址信息
//    struct sockaddr_in clnt_addr; // 保存客户端套接字地址信息
//    socklen_t clnt_addr_size;   // 客户端套接字地址变量的大小
//  //  served::multiplexer mux;
//    // 发送给客户端的固定内容
//    char status[] = "HTTP/1.0 200 OK\r\n";
//    char header[] = "Server: A Simple Web Server\r\nContent-Type: text/html\r\n\r\n";
//    char body[] = "<html><head><title>A Simple Web Server</title></head><body><h2>Welcome!</h2><p>This is shiyanlou!</p></body></html>";

//    while((IMAGEID < UINT_MAX))
//    {
//        printf("socket 0begin:\n");
//        // 创建一个服务器套接字
//        serv_sock = socket(PF_INET, SOCK_STREAM, 0);
//        if(-1 == serv_sock){
//            errorHandling("socket() error");
//        }

//        // 配置套接字IP和端口信息
//        memset(&serv_addr, 0, sizeof(serv_addr));
//        serv_addr.sin_family = AF_INET;
//        serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
//        serv_addr.sin_port = htons(3000);

//        const int on=1;
//        setsockopt(serv_sock,SOL_SOCKET,SO_REUSEADDR,&on,sizeof(on));
//        // 绑定服务器套接字
//        if(-1 == bind(serv_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr))){
//            errorHandling("bind() error");
//        }
//        // 监听服务器套接字
//        if(-1 == listen(serv_sock, 5)){
//            errorHandling("listen() error");
//        }

//        // 接受客户端的请求
//        clnt_addr_size = sizeof(clnt_addr);
//        printf("accept begin:\n");

//        clnt_sock = accept(serv_sock, (struct sockaddr *) &clnt_addr, &clnt_addr_size);
//        printf("read 0begin:\n");
//        if(-1 == clnt_sock){
//            errorHandling("accept() error");
//        }

//        printf("read begin:\n");

//        // 读取客户端请求
//        read(clnt_sock, buf, sizeof(buf) -1);
//        printf("read buf:%s\n",buf);
        //using cJSON to parse

//        cJSON           *json;
//        char            *out;
//     json=cJSON_Parse(buf); //获取整个大的句柄
//     out=cJSON_Print(json);  //这个是可以输出的。为获取的整个json的值
//     printf("read 1begin:\n");

//     cJSON *arrayItem = cJSON_GetObjectItem(json,"data"); //获取这个对象成员
//     printf("read begin:\n");

//     cJSON *object = cJSON_GetArrayItem(arrayItem,0);   //因为这个对象是个数组获取，且只有一个元素所以写下标为0获取
//      printf("read 2begin:\n");
//     /*下面就是可以重复使用cJSON_GetObjectItem来获取每个成员的值了*/
//     cJSON *item = cJSON_GetObjectItem(object,"face-coordinates");  //
//     printf("face-coordinates%s\n",item->valuestring);

//     item = cJSON_GetObjectItem(object,"detect-time");
//     printf("detect-time:%s\n",item->valuestring);
//     cJSON_Delete(json);


//        printf("buf begin:\n");
//        //printf("%s\n", buf);

//        char *p = strstr(buf, "dog.");
//        printf("buf1 begin:\n");

//        char *p2 = strstr(buf, "dog.jpg");
//        bool flag = true;
//        if(p2 != NULL)
//        {
//            flag = false;
//        }

//        char *p3 = strstr(buf, "face-coordinates");//get trackingID and conf score


//        char *p1 = strstr(buf,"Content-Length:");
//        unsigned char length[10]= {0};
//        if(flag == true)
//        {
//            memcpy(length,p1 + strlen("Content-Length:") + 1,5);
//        }
//        else
//        {
//            memcpy(length,p1 + strlen("Content-Length:") + 1,6);
//        }

//        printf("*****-IMAGEID-**%d\n",IMAGEID);
//        printf("*****-length-**%d\n",atoi((const char*)length));
//        unsigned int size = atoi((const char*)length);

//        unsigned char firmware[size];
//        memcpy((char *)firmware,p + strlen("dog.png")+5,size);//   164759


//        FILE *fp;
//        char PrintResult[40];
//        sprintf(PrintResult,"./face_captures/face_%d.png",IMAGEID++);


//        g_IMAGE_PATH_LIST[(IMAGEID-1)%10] = PrintResult;
//        qDebug() <<  "g_IMAGE_PATH_LIST:";
//        qDebug() <<  g_IMAGE_PATH_LIST[0];

//        fp = fopen(PrintResult, "wb");
//       // fwrite(p + strlen("dog.jpg")+5, 163759, 1, fp);
//        fwrite(firmware, size, 1, fp);//163759
//        fflush(fp);

//        // 向客户端套接字发送信息
//        write(clnt_sock, status, sizeof(status));
//        write(clnt_sock, header, sizeof(header));
//        write(clnt_sock, body, sizeof(body));
//        write(clnt_sock, "hello", 6);

//        if(IMAGEID==0)
//        {
//            break;
//        }

//    // 关闭套接字
//    close(clnt_sock);
//    close(serv_sock);

//   }
   // return 0;

}

void MyThread::run()
{
    //很复杂的数据处理
    //需要耗时5秒
    printf("***enter mythread run\n");
    //server();
    restful();
    sleep(5);

    emit isDone();  //发送完成信号
}

MyThread::MyThread(QObject *parent)
{


}

MyThread::~MyThread()
{


}
