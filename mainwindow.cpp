#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QTimer>
#include <QDateTime>
#include <QImage>
#include <QTime>
#include <qdebug.h>
#include <QMessageBox>
#include <QJsonDocument>
#include <QJsonParseError>
#include <QFile>
#include <QJsonObject>
#include <QJsonArray>
#include "Util.h"
#include <QtNetwork>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
      m_vlcInstance(NULL)
    , m_vlcMediaPlayer(NULL)
    , m_pvlcMedia(NULL)
    , m_videobuf(NULL)
    , m_frameWidth(0)
    , m_frameHeight(0)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

 //   ui->setUrl("rtsp://184.72.239.149/vod/mp4:BigBuckBunny_115k.mov");
    timer = new QTimer(this);
    connect(timer,SIGNAL(timeout()),this,SLOT(handleTimeout()));
    connect(ui->horizontalScrollBar,SIGNAL(valueChanged(int)),this,SLOT(slot_ScroolWidget(int)));
    connect(ui->horizontalScrollBar_2,SIGNAL(valueChanged(int)),this,SLOT(slot_ScroolWidget(int)));
    connect(timer,SIGNAL(timeout()),this,SLOT(on_label_destroyed()));  //在.h里面定义一个槽private slots: void openUrl(QString url);
    connect(timer,SIGNAL(timeout()),this,SLOT(on_label_22_destroyed()));
    connect(timer,SIGNAL(timeout()),this,SLOT(on_label_29_destroyed()));
    connect(timer,SIGNAL(timeout()),this,SLOT(on_label_25_destroyed()));
    connect(timer,SIGNAL(timeout()),this,SLOT(on_label_39_destroyed()));
    connect(timer,SIGNAL(timeout()),this,SLOT(on_label_33_destroyed()));
    connect(timer,SIGNAL(timeout()),this,SLOT(on_label_45_destroyed()));
    connect(timer,SIGNAL(timeout()),this,SLOT(on_label_42_destroyed()));
    connect(timer,SIGNAL(timeout()),this,SLOT(on_label_104_destroyed()));
    connect(timer,SIGNAL(timeout()),this,SLOT(on_label_47_destroyed()));

    connect(timer,SIGNAL(timeout()),this,SLOT(on_label_52_destroyed()));
    connect(timer,SIGNAL(timeout()),this,SLOT(on_label_58_destroyed()));
    connect(timer,SIGNAL(timeout()),this,SLOT(on_label_66_destroyed()));
    connect(timer,SIGNAL(timeout()),this,SLOT(on_label_68_destroyed()));
    connect(timer,SIGNAL(timeout()),this,SLOT(on_label_76_destroyed()));
    connect(timer,SIGNAL(timeout()),this,SLOT(on_label_78_destroyed()));
    connect(timer,SIGNAL(timeout()),this,SLOT(on_label_86_destroyed()));
    connect(timer,SIGNAL(timeout()),this,SLOT(on_label_88_destroyed()));
    connect(timer,SIGNAL(timeout()),this,SLOT(on_label_96_destroyed()));
    connect(timer,SIGNAL(timeout()),this,SLOT(on_label_98_destroyed()));

    thread = new MyThread(this);
    char const* vlc_args[] =
    {
        "-I",
        "dummy",
        "--ignore-config",
    };

    m_vlcInstance = libvlc_new(sizeof(vlc_args) / sizeof(vlc_args[0]), vlc_args);
    m_vlcMediaPlayer = libvlc_media_player_new (m_vlcInstance);
  //  on_label_linkActivated(NULL);
}

MainWindow::~MainWindow()
{
    libvlc_media_release(m_pvlcMedia);
    libvlc_media_player_stop(m_vlcMediaPlayer);
    libvlc_media_player_release(m_vlcMediaPlayer);
    libvlc_release(m_vlcInstance);
    SAFE_DELETE_ARRAY(m_videobuf);
   // delete ui;
}

void MainWindow::handleTimeout() {
    qDebug()<<"Enter timeout processing function\n";
        if(timer->isActive()){
            timer->stop();
        }
}


char* MainWindow::GetVideoBuf() const
{
    return m_videobuf;
}

void MainWindow::AllocVideoBuf(int width, int height)
{
    m_videobuf = new char[(width * height) << 2];
}

void* MainWindow::PreDecode_cb(void *opaque, void **planes)
{
    MainWindow* obj = (MainWindow*)opaque;
    *planes = obj->GetVideoBuf();

    return obj->GetVideoBuf();
}

void MainWindow::HandleStream_cb(void *opaque, void *picture, void *const *planes)
{
    Q_UNUSED(opaque);
    Q_UNUSED(picture);
    Q_UNUSED(planes);
    // TODO - image processing
//     if (picture != NULL)
//     {
//         CPlayerView* obj = (CPlayerView*)opaque;
//         Mat img(Size(obj->m_frameWidth, obj->m_frameHeight), CV_8UC4, picture);
//
//         imshow("123", img);
//         waitKey(1);
//     }
}

void MainWindow::Render_cb(void *opaque, void *picture)
{
     MainWindow* obj = (MainWindow*)opaque;
//    QImage qimg((uchar*)picture, obj->m_frameWidth, obj->m_frameHeight, QImage::Format_ARGB32);;
//   // obj->ui->showImageLabel->setPixmap(QPixmap::fromImage(qimg));
//     //obj->ui->RenderWidget->setPixmap(QPixmap::fromImage(qimg)); //temp
//     obj->ui->showImageLabel->setPixmap( QPixmap::fromImage(qimg));


    QImage qimg((uchar*)picture, obj->m_frameWidth, obj->m_frameHeight, QImage::Format_ARGB32);;
        obj->ui->RenderWidget->setPixmap(qimg);

}




void MainWindow::OpenVideo(const char* rtsp)
{
    qDebug() << "on_processButton_clicked";
    on_pushButton_pause_clicked();
    qDebug() << "on_closeButton_clicked";
    m_pvlcMedia = libvlc_media_new_location(m_vlcInstance, rtsp);
    qDebug() << "libvlc_media_player_set_media";
    libvlc_media_player_set_media(m_vlcMediaPlayer, m_pvlcMedia);
    qDebug() << "libvlc_video_set_callbacks";
    libvlc_video_set_callbacks(m_vlcMediaPlayer, PreDecode_cb, HandleStream_cb, Render_cb, this);

    qDebug() << "on_closeButton_clicked";
    on_pushButton_restart_clicked();
}

void MainWindow::on_openGLWidget_destroyed()
{

}

void MainWindow::on_pushButton_close_clicked()
{
    close();
}

void MainWindow::on_pushButton_open_clicked()
{

    bool bOk = false;
    string sRtsp = QInputDialog::getText(this, tr("Open RTSP"),
        tr("RTSP url:"), QLineEdit::Normal,
        "rtsp://192.168.5.121/live.sdp", &bOk).toStdString();  //   rtsp://192.168.0.102:8080/test.ts    rtsp://192.168.5.145:8554/1.mkv

    if (bOk)
    {
        OpenVideo(sRtsp.c_str());
    }
    timer->start(2000);

    QString  str;
    char*  ch;
    QByteArray ba = g_CAMREA_IP_ADRESS.toLatin1(); // must
    ch = ba.data();
    qDebug() << ch;
    char result[255];

    QString strIpAddress;
    QList<QHostAddress> ipAddressesList = QNetworkInterface::allAddresses();
    // 获取第一个本主机的IPv4地址
    int nListSize = ipAddressesList.size();
    for (int i = 0; i < nListSize; ++i)
    {
           if (ipAddressesList.at(i) != QHostAddress::LocalHost &&
               ipAddressesList.at(i).toIPv4Address()) {
               strIpAddress = ipAddressesList.at(i).toString();
               break;
           }
     }
     // 如果没有找到，则以本地IP地址为IP
     if (strIpAddress.isEmpty())
        strIpAddress = QHostAddress(QHostAddress::LocalHost).toString();

    g_IP_ADRESS = strIpAddress;

    char*  c;
    QByteArray b = g_IP_ADRESS.toLatin1(); // must
    c = b.data();

    char*  c_port;
    QByteArray bb = g_CAMREA_PORT.toLatin1(); // must
    c_port = bb.data();

    sprintf(result, "curl http://%s:%s/tracking?URL=http://%s:3000",ch,c_port,c);


    system(result);//curl http://192.168.0.121:6123/tracking?URL=http://192.168.0.100:3000
    thread->start();


//    QString filename =QFileDialog::getOpenFileName(this,tr("Open Video File"),".",tr("Video Files(*.avi *.mp4 *.flv *.mkv)"));
//    if(filename.length()<=0)
//        return;


}

void MainWindow::on_pushButton_restart_clicked()
{

    libvlc_state_t state = libvlc_media_player_get_state(m_vlcMediaPlayer);

       if (state == libvlc_Playing)
       {
           return;
       }

       libvlc_media_player_play(m_vlcMediaPlayer);

       while (state == libvlc_NothingSpecial || state == libvlc_Opening)
       {
           state = libvlc_media_player_get_state(m_vlcMediaPlayer);
           MSleep(100);
       }

       if (state == libvlc_Error)
       {
           return;
       }

       libvlc_media_track_t **tracks = NULL;
       bool bFindResolution = false;

       while (!bFindResolution)
       {
           unsigned tracksCount = libvlc_media_tracks_get(m_pvlcMedia, &tracks);

           if (tracks != NULL)
           {
               for (unsigned i = 0; i < tracksCount; i++)
               {
                   if (tracks[i]->i_type == libvlc_track_video
                       && tracks[i]->video->i_height != 0
                       && tracks[i]->video->i_width != 0)
                   {
                       m_frameWidth = tracks[i]->video->i_width;
                       m_frameHeight = tracks[i]->video->i_height;
                       bFindResolution = true;
                       break;
                   }
               }
           }

           libvlc_media_tracks_release(tracks, tracksCount);
           MSleep(100);
       }


       // [workaround]: I don't know how to change format (libvlc_video_set_format) after playing...
       // reconnect the stream
       libvlc_media_player_stop(m_vlcMediaPlayer);

       libvlc_video_set_format(m_vlcMediaPlayer, "RV32", m_frameWidth, m_frameHeight, m_frameWidth << 2);
       SAFE_DELETE_ARRAY(m_videobuf);
       AllocVideoBuf(m_frameWidth, m_frameHeight);

       libvlc_media_player_play(m_vlcMediaPlayer);
       // close();

}



void MainWindow::on_pushButton_pause_clicked()
{
    libvlc_media_player_pause(m_vlcMediaPlayer);
}

void MainWindow::on_lineEdit_returnPressed()
{
     g_CAMREA_IP_ADRESS = ui->lineEdit->text();

    float yolo_det = ui->lineEdit_4->text().toFloat();
    float yolo_iou = ui->lineEdit_7->text().toFloat();


    QString  str;
    char*  ch;
    QByteArray ba = g_IP_ADRESS.toLatin1(); // must
    ch = ba.data();
 qDebug() << ch;
    char result[255];
    sprintf(result, "curl http://%s:6123/threshold?yolo_det=%.2f&yolo_iou=%.2f", ch,yolo_det,yolo_iou);
    qDebug() << result;

    system(result);
}

void MainWindow::on_label_linkActivated(const QString &link)
{
   // qDebug() << "hehe";
    QString filename("./u18.png");
    QImage* img=new QImage;
//    if(! ( img->load(link) ) ) //加载图像
//    {
//       QMessageBox::information(this,
//                                tr("打开图像失败"),
//                                tr("打开图像失败!"));
//       delete img;
//       return;
//     }
//    if(g_IMAGE_PATH_LIST[0] != NULL)
//    {
////        ui->label_20->setText("tracking ID: 001");
//        ui->label->setPixmap(QPixmap(g_IMAGE_PATH_LIST[0]));
//        ui->label->setScaledContents(true);
//        on_label_20_linkActivated(g_TRACKING_ID[0]);
//        on_label_21_linkActivated(g_CONFSCORE[0]);
//        update();

//    }

}

void MainWindow::on_label_22_linkActivated(const QString &link)
{
    QString filename("./u18.png");
    QImage* img=new QImage;

//    if(! ( img->load(link) ) ) //加载图像
//    {

//     }

//    if(g_IMAGE_PATH_LIST[1] != NULL)
//    {
////        ui->label_20->setText("tracking ID: 001");
//        ui->label->setPixmap(QPixmap(g_IMAGE_PATH_LIST[1]));
//        ui->label->setScaledContents(true);
//        on_label_50_linkActivated(g_TRACKING_ID[1]);
//        on_label_24_linkActivated(g_CONFSCORE[1]);
//        update();

//    }
//    else
//    {
//        QMessageBox::information(this,
//                                 tr("打开图像失败"),
//                                 tr("打开图像失败!"));
//        delete img;
//        return;
//    }
}

void MainWindow::on_label_20_linkActivated(const QString &link)
{

    ui->label_20->setText(link);
}

void MainWindow::on_label_21_linkActivated(const QString &link)
{
    ui->label_21->setText(link);
}

void MainWindow::on_horizontalScrollBar_valueChanged(int value)
{
    double p=static_cast<double>(value)/static_cast<double>(ui->horizontalScrollBar_2->maximum());
    qDebug() << p;
    ui->label->move(-(ui->label_19->height()+value)*2*p,510);
    ui->label_20->move(-(ui->label_19->height()+value)*2*p+50,510);
    ui->label_21->move(-(ui->label_19->height()+value)*2*p+50,530);

    ui->label_22->move(170-(ui->label_19->height()+value)*2*p,510);
    ui->label_50->move(170-(ui->label_19->height()+value)*2*p+50,510);
    ui->label_24->move(170-(ui->label_19->height()+value)*2*p+50,530);

    ui->label_29->move(340-(ui->label_19->height()+value)*2*p,510);
    ui->label_31->move(340-(ui->label_19->height()+value)*2*p+50,510);
    ui->label_26->move(340-(ui->label_19->height()+value)*2*p+50,530);

    ui->label_25->move(510-(ui->label_19->height()+value)*2*p,510);
    ui->label_101->move(510-(ui->label_19->height()+value)*2*p+50,510);
    ui->label_27->move(510-(ui->label_19->height()+value)*2*p+50,530);

    ui->label_39->move(680-(ui->label_19->height()+value)*2*p,510);
    ui->label_41->move(680-(ui->label_19->height()+value)*2*p+50,510);
    ui->label_35->move(680-(ui->label_19->height()+value)*2*p+50,530);

    ui->label_33->move(850-(ui->label_19->height()+value)*2*p,510);
    ui->label_102->move(850-(ui->label_19->height()+value)*2*p+50,510);
    ui->label_37->move(850-(ui->label_19->height()+value)*2*p+50,530);

    ui->label_45->move(1020-(ui->label_19->height()+value)*2*p,510);
    ui->label_46->move(1020-(ui->label_19->height()+value)*2*p+50,510);
    ui->label_43->move(1020-(ui->label_19->height()+value)*2*p+50,530);

    ui->label_42->move(1190-(ui->label_19->height()+value)*2*p,510);
    ui->label_103->move(1190-(ui->label_19->height()+value)*2*p+50,510);
    ui->label_44->move(1190-(ui->label_19->height()+value)*2*p+50,530);

    ui->label_104->move(1360-(ui->label_19->height()+value)*2*p,510);
    ui->label_105->move(1360-(ui->label_19->height()+value)*2*p+50,510);
    ui->label_48->move(1360-(ui->label_19->height()+value)*2*p+50,530);

    ui->label_47->move(1530-(ui->label_19->height()+value)*2*p,510);
    ui->label_106->move(1530-(ui->label_19->height()+value)*2*p+50,510);
    ui->label_49->move(1530-(ui->label_19->height()+value)*2*p+50,530);

}



void MainWindow::on_label_50_linkActivated(const QString &link)
{
     ui->label_50->setText(link);
}

void MainWindow::on_label_24_linkActivated(const QString &link)
{
     ui->label_24->setText(link);
}

void MainWindow::on_label_29_linkActivated(const QString &link)
{

    on_label_31_linkActivated(g_TRACKING_ID[1]);
    on_label_26_linkActivated(g_CONFSCORE[1]);
}

void MainWindow::on_label_31_linkActivated(const QString &link)
{
   ui->label_31->setText(link);
}

void MainWindow::on_label_26_linkActivated(const QString &link)
{
    ui->label_26->setText(link);
}

void MainWindow::on_label_25_linkActivated(const QString &link)
{

}

void MainWindow::on_label_101_linkActivated(const QString &link)
{
    ui->label_101->setText(link);
}

void MainWindow::on_label_27_linkActivated(const QString &link)
{
    ui->label_27->setText(link);
}

void MainWindow::on_label_39_linkActivated(const QString &link)
{

}

void MainWindow::on_label_41_linkActivated(const QString &link)
{
    ui->label_41->setText(link);
}

void MainWindow::on_label_35_linkActivated(const QString &link)
{
    ui->label_35->setText(link);
}

void MainWindow::on_label_33_linkActivated(const QString &link)
{

}

void MainWindow::on_label_102_linkActivated(const QString &link)
{
    ui->label_102->setText(link);
}

void MainWindow::on_label_37_linkActivated(const QString &link)
{
    ui->label_37->setText(link);
}

void MainWindow::on_label_45_linkActivated(const QString &link)
{

}

void MainWindow::on_label_46_linkActivated(const QString &link)
{
    ui->label_46->setText(link);
}

void MainWindow::on_label_43_linkActivated(const QString &link)
{
    ui->label_43->setText(link);
}

void MainWindow::on_label_42_linkActivated(const QString &link)
{

}

void MainWindow::on_label_103_linkActivated(const QString &link)
{
    ui->label_103->setText(link);
}

void MainWindow::on_label_44_linkActivated(const QString &link)
{
    ui->label_44->setText(link);
}

void MainWindow::on_label_104_linkActivated(const QString &link)
{

}

void MainWindow::on_label_105_linkActivated(const QString &link)
{
    ui->label_105->setText(link);
}

void MainWindow::on_label_48_linkActivated(const QString &link)
{
    ui->label_48->setText(link);
}

void MainWindow::on_label_47_linkActivated(const QString &link)
{

}

void MainWindow::on_label_106_linkActivated(const QString &link)
{
     ui->label_106->setText(link);
}

void MainWindow::on_label_49_linkActivated(const QString &link)
{
     ui->label_49->setText(link);
}

void MainWindow::on_horizontalScrollBar_2_valueChanged(int value)
{
    double p=static_cast<double>(value)/static_cast<double>(ui->horizontalScrollBar_2->maximum());
    qDebug() << p;
    ui->label_52->move(-(ui->label_19->height()+value)*2*p,620);
    ui->label_53->move(-(ui->label_19->height()+value)*2*p+50,620);
    ui->label_51->move(-(ui->label_19->height()+value)*2*p+50,640);
    ui->label_54->move(-(ui->label_19->height()+value)*2*p+50,660);
    ui->label_55->move(-(ui->label_19->height()+value)*2*p+50,680);

    ui->label_58->move(170-(ui->label_19->height()+value)*2*p,620);
    ui->label_57->move(170-(ui->label_19->height()+value)*2*p+50,620);
    ui->label_59->move(170-(ui->label_19->height()+value)*2*p+50,640);
    ui->label_60->move(170-(ui->label_19->height()+value)*2*p+50,660);
    ui->label_56->move(170-(ui->label_19->height()+value)*2*p+50,680);

    ui->label_66->move(340-(ui->label_19->height()+value)*2*p,620);
    ui->label_65->move(340-(ui->label_19->height()+value)*2*p+50,620);
    ui->label_67->move(340-(ui->label_19->height()+value)*2*p+50,640);
    ui->label_70->move(340-(ui->label_19->height()+value)*2*p+50,660);
    ui->label_64->move(340-(ui->label_19->height()+value)*2*p+50,680);

    ui->label_68->move(510-(ui->label_19->height()+value)*2*p,620);
    ui->label_69->move(510-(ui->label_19->height()+value)*2*p+50,620);
    ui->label_61->move(510-(ui->label_19->height()+value)*2*p+50,640);
    ui->label_63->move(510-(ui->label_19->height()+value)*2*p+50,660);
    ui->label_62->move(510-(ui->label_19->height()+value)*2*p+50,680);

    ui->label_76->move(680-(ui->label_19->height()+value)*2*p,620);
    ui->label_75->move(680-(ui->label_19->height()+value)*2*p+50,620);
    ui->label_77->move(680-(ui->label_19->height()+value)*2*p+50,640);
    ui->label_80->move(680-(ui->label_19->height()+value)*2*p+50,660);
    ui->label_74->move(680-(ui->label_19->height()+value)*2*p+50,680);

    ui->label_78->move(850-(ui->label_19->height()+value)*2*p,620);
    ui->label_79->move(850-(ui->label_19->height()+value)*2*p+50,620);
    ui->label_71->move(850-(ui->label_19->height()+value)*2*p+50,640);
    ui->label_73->move(850-(ui->label_19->height()+value)*2*p+50,660);
    ui->label_72->move(850-(ui->label_19->height()+value)*2*p+50,680);

    ui->label_86->move(1020-(ui->label_19->height()+value)*2*p,620);
    ui->label_85->move(1020-(ui->label_19->height()+value)*2*p+50,620);
    ui->label_87->move(1020-(ui->label_19->height()+value)*2*p+50,640);
    ui->label_90->move(1020-(ui->label_19->height()+value)*2*p+50,660);
    ui->label_84->move(1020-(ui->label_19->height()+value)*2*p+50,680);

    ui->label_88->move(1190-(ui->label_19->height()+value)*2*p,620);
    ui->label_89->move(1190-(ui->label_19->height()+value)*2*p+50,620);
    ui->label_81->move(1190-(ui->label_19->height()+value)*2*p+50,640);
    ui->label_83->move(1190-(ui->label_19->height()+value)*2*p+50,660);
    ui->label_82->move(1190-(ui->label_19->height()+value)*2*p+50,680);

    ui->label_96->move(1360-(ui->label_19->height()+value)*2*p,620);
    ui->label_95->move(1360-(ui->label_19->height()+value)*2*p+50,620);
    ui->label_97->move(1360-(ui->label_19->height()+value)*2*p+50,640);
    ui->label_100->move(1360-(ui->label_19->height()+value)*2*p+50,660);
    ui->label_94->move(1360-(ui->label_19->height()+value)*2*p+50,680);

    ui->label_98->move(1530-(ui->label_19->height()+value)*2*p,620);
    ui->label_99->move(1530-(ui->label_19->height()+value)*2*p+50,620);
    ui->label_91->move(1530-(ui->label_19->height()+value)*2*p+50,640);
    ui->label_93->move(1530-(ui->label_19->height()+value)*2*p+50,660);
    ui->label_92->move(1530-(ui->label_19->height()+value)*2*p+50,680);

}

void MainWindow::on_label_52_linkActivated(const QString &link)
{

}

void MainWindow::on_label_53_linkActivated(const QString &link)
{
    ui->label_53->setText(link);
}

void MainWindow::on_label_51_linkActivated(const QString &link)
{
    ui->label_51->setText(link);
}

void MainWindow::on_label_54_linkActivated(const QString &link)
{
    ui->label_54->setText(link);
}

void MainWindow::on_label_55_linkActivated(const QString &link)
{
    ui->label_55->setText(link);
}



void MainWindow::on_label_58_linkActivated(const QString &link)
{

}

void MainWindow::on_label_57_linkActivated(const QString &link)
{
    ui->label_57->setText(link);
}

void MainWindow::on_label_59_linkActivated(const QString &link)
{
    ui->label_59->setText(link);
}

void MainWindow::on_label_60_linkActivated(const QString &link)
{
    ui->label_60->setText(link);
}

void MainWindow::on_label_56_linkActivated(const QString &link)
{
    ui->label_56->setText(link);
}

void MainWindow::on_label_66_linkActivated(const QString &link)
{

}

void MainWindow::on_label_65_linkActivated(const QString &link)
{
    ui->label_65->setText(link);
}

void MainWindow::on_label_67_linkActivated(const QString &link)
{
    ui->label_67->setText(link);
}

void MainWindow::on_label_70_linkActivated(const QString &link)
{
    ui->label_70->setText(link);
}

void MainWindow::on_label_64_linkActivated(const QString &link)
{
    ui->label_64->setText(link);
}

void MainWindow::on_label_68_linkActivated(const QString &link)
{

}

void MainWindow::on_label_69_linkActivated(const QString &link)
{
    ui->label_69->setText(link);
}

void MainWindow::on_label_61_linkActivated(const QString &link)
{
    ui->label_61->setText(link);
}

void MainWindow::on_label_63_linkActivated(const QString &link)
{
    ui->label_63->setText(link);
}

void MainWindow::on_label_62_linkActivated(const QString &link)
{
    ui->label_62->setText(link);
}

void MainWindow::on_label_76_linkActivated(const QString &link)
{

}

void MainWindow::on_label_75_linkActivated(const QString &link)
{
    ui->label_75->setText(link);
}

void MainWindow::on_label_77_linkActivated(const QString &link)
{
    ui->label_77->setText(link);
}

void MainWindow::on_label_80_linkActivated(const QString &link)
{
    ui->label_80->setText(link);
}

void MainWindow::on_label_74_linkActivated(const QString &link)
{
    ui->label_74->setText(link);
}

void MainWindow::on_label_78_linkActivated(const QString &link)
{

}

void MainWindow::on_label_79_linkActivated(const QString &link)
{
    ui->label_79->setText(link);
}

void MainWindow::on_label_71_linkActivated(const QString &link)
{
    ui->label_71->setText(link);
}

void MainWindow::on_label_73_linkActivated(const QString &link)
{
    ui->label_73->setText(link);
}

void MainWindow::on_label_72_linkActivated(const QString &link)
{
    ui->label_72->setText(link);
}

void MainWindow::on_label_86_linkActivated(const QString &link)
{

}

void MainWindow::on_label_85_linkActivated(const QString &link)
{
    ui->label_85->setText(link);
}

void MainWindow::on_label_87_linkActivated(const QString &link)
{
    ui->label_87->setText(link);
}

void MainWindow::on_label_90_linkActivated(const QString &link)
{
    ui->label_90->setText(link);
}

void MainWindow::on_label_84_linkActivated(const QString &link)
{
    ui->label_84->setText(link);
}

void MainWindow::on_label_88_linkActivated(const QString &link)
{

}

void MainWindow::on_label_89_linkActivated(const QString &link)
{
    ui->label_89->setText(link);
}

void MainWindow::on_label_81_linkActivated(const QString &link)
{
    ui->label_81->setText(link);
}

void MainWindow::on_label_83_linkActivated(const QString &link)
{
    ui->label_83->setText(link);
}

void MainWindow::on_label_82_linkActivated(const QString &link)
{
    ui->label_82->setText(link);
}

void MainWindow::on_label_96_linkActivated(const QString &link)
{

}

void MainWindow::on_label_95_linkActivated(const QString &link)
{
    ui->label_95->setText(link);
}

void MainWindow::on_label_97_linkActivated(const QString &link)
{
    ui->label_97->setText(link);
}

void MainWindow::on_label_100_linkActivated(const QString &link)
{
    ui->label_100->setText(link);
}

void MainWindow::on_label_94_linkActivated(const QString &link)
{
    ui->label_94->setText(link);
}

void MainWindow::on_label_98_linkActivated(const QString &link)
{

}

void MainWindow::on_label_99_linkActivated(const QString &link)
{
    ui->label_99->setText(link);
}

void MainWindow::on_label_91_linkActivated(const QString &link)
{
    ui->label_91->setText(link);
}

void MainWindow::on_label_93_linkActivated(const QString &link)
{
    ui->label_93->setText(link);
}

void MainWindow::on_label_92_linkActivated(const QString &link)
{
    ui->label_92->setText(link);
}

void MainWindow::on_lineEdit_4_returnPressed()//人脸检测阈值
{
  qDebug() << "result";

  float yolo_det = ui->lineEdit_4->text().toFloat();
  float yolo_iou = ui->lineEdit_7->text().toFloat();

  char result[255];
  sprintf(result, "curl http://192.168.0.121:6123/threshold?yolo_det=%.2f&yolo_iou=%.2f", yolo_det,yolo_iou);
  qDebug() << result;

  system(result);//curl http://192.168.0.121:6123/tracking?URL=http://192.168.0.100:3000    curl http://192.168.0.121:6123/threshold?yolo_det=0.68&yolo_iou=0.4
}

void MainWindow::on_pushButton_2_clicked()//添加新人脸
{

    QString filename = QFileDialog::getOpenFileName(this,
            tr("Open File"),
            "",
            "",
            0);
        if (!filename.isNull())
        {
            //fileName是文件名

        }
        else{
            //点的是取消

        }
     MysqlDB  mysqlDb;
     mysqlDb.connect("localhost", "root", "root", "opencv_base" );
}

void MainWindow::on_lineEdit_7_returnPressed()//人体检测阈值
{
    float yolo_det = ui->lineEdit_4->text().toFloat();
    float yolo_iou = ui->lineEdit_7->text().toFloat();

    char result[255];
    sprintf(result, "curl http://192.168.0.121:6123/threshold?yolo_det=%.2f&yolo_iou=%.2f", yolo_det,yolo_iou);
    qDebug() << result;

    system(result);
}

void MainWindow::on_lineEdit_2_returnPressed()//端口
{
        g_CAMREA_PORT = ui->lineEdit_2->text();
}

void MainWindow::on_lineEdit_3_returnPressed()//输入帧率
{

}

void MainWindow::on_label_destroyed()
{
  //  qDebug() << "hehe";
    QString filename("./u18.png");
    QImage* img=new QImage;
//    if(! ( img->load(link) ) ) //加载图像
//    {
//       QMessageBox::information(this,
//                                tr("打开图像失败"),
//                                tr("打开图像失败!"));
//       delete img;
//       return;
//     }
    if(g_IMAGE_PATH_LIST[0] != NULL)
    {
       // ui->label_20->setText("tracking ID: 001");
        ui->label->setPixmap(QPixmap(g_IMAGE_PATH_LIST[0]));
        ui->label->setScaledContents(true);
        on_label_20_linkActivated(g_TRACKING_ID[0]);
        on_label_21_linkActivated(g_CONFSCORE[0]);
        update();
    }
//    else
//    {
//        QMessageBox::information(this,
//                                 tr("打开图像失败"),
//                                 tr("打开图像失败!"));
//        delete img;
//        return;
//    }

}

void MainWindow::on_label_22_destroyed()
{

    if(g_IMAGE_PATH_LIST[1] != NULL)
    {
        ui->label_22->setPixmap(QPixmap(g_IMAGE_PATH_LIST[1]));
        ui->label_22->setScaledContents(true);

        on_label_50_linkActivated(g_TRACKING_ID[1]);
        on_label_24_linkActivated(g_CONFSCORE[1]);
        update();


    }
//    else
//    {
//        QMessageBox::information(this,
//                                 tr("打开图像失败"),
//                                 tr("打开图像失败!"));
//      //  delete img;
//        return;
//    }

}

void MainWindow::on_label_29_destroyed()
{

    if(g_IMAGE_PATH_LIST[2] != NULL)
    {
       // ui->label_20->setText("tracking ID: 001");
        ui->label_29->setPixmap(QPixmap(g_IMAGE_PATH_LIST[2]));
        ui->label_29->setScaledContents(true);
        on_label_31_linkActivated(g_TRACKING_ID[2]);
        on_label_26_linkActivated(g_CONFSCORE[2]);
        update();
    }


}

void MainWindow::on_label_25_destroyed()
{
    if(g_IMAGE_PATH_LIST[3] != NULL)
    {
       // ui->label_20->setText("tracking ID: 001");
        ui->label_25->setPixmap(QPixmap(g_IMAGE_PATH_LIST[3]));
        ui->label_25->setScaledContents(true);
        on_label_101_linkActivated(g_TRACKING_ID[3]);
        on_label_27_linkActivated(g_CONFSCORE[3]);
        update();
    }

}

void MainWindow::on_label_39_destroyed()
{
    if(g_IMAGE_PATH_LIST[4] != NULL)
    {
       // ui->label_20->setText("tracking ID: 001");
        ui->label_39->setPixmap(QPixmap(g_IMAGE_PATH_LIST[4]));
        ui->label_39->setScaledContents(true);
        on_label_41_linkActivated(g_TRACKING_ID[4]);
        on_label_35_linkActivated(g_CONFSCORE[4]);
        update();
    }

}

void MainWindow::on_label_33_destroyed()
{
    if(g_IMAGE_PATH_LIST[5] != NULL)
    {
       // ui->label_20->setText("tracking ID: 001");
        ui->label_33->setPixmap(QPixmap(g_IMAGE_PATH_LIST[5]));
        ui->label_33->setScaledContents(true);
        on_label_102_linkActivated(g_TRACKING_ID[5]);
        on_label_37_linkActivated(g_CONFSCORE[5]);
        update();
    }

}

void MainWindow::on_label_45_destroyed()
{
    if(g_IMAGE_PATH_LIST[6] != NULL)
    {
       // ui->label_20->setText("tracking ID: 001");
        ui->label_45->setPixmap(QPixmap(g_IMAGE_PATH_LIST[6]));
        ui->label_45->setScaledContents(true);
        on_label_46_linkActivated(g_TRACKING_ID[6]);
        on_label_43_linkActivated(g_CONFSCORE[6]);
        update();
    }

}

void MainWindow::on_label_42_destroyed()
{
    if(g_IMAGE_PATH_LIST[7] != NULL)
    {
       // ui->label_20->setText("tracking ID: 001");
        ui->label_42->setPixmap(QPixmap(g_IMAGE_PATH_LIST[7]));
        ui->label_42->setScaledContents(true);
        on_label_103_linkActivated(g_TRACKING_ID[7]);
        on_label_44_linkActivated(g_CONFSCORE[7]);
        update();
    }

}

void MainWindow::on_label_104_destroyed()
{
    if(g_IMAGE_PATH_LIST[8] != NULL)
    {
       // ui->label_20->setText("tracking ID: 001");
        ui->label_104->setPixmap(QPixmap(g_IMAGE_PATH_LIST[8]));
        ui->label_104->setScaledContents(true);
        on_label_105_linkActivated(g_TRACKING_ID[8]);
        on_label_48_linkActivated(g_CONFSCORE[8]);
        update();
    }

}

void MainWindow::on_label_47_destroyed()
{
    if(g_IMAGE_PATH_LIST[9] != NULL)
    {
       // ui->label_20->setText("tracking ID: 001");
        ui->label_47->setPixmap(QPixmap(g_IMAGE_PATH_LIST[9]));
        ui->label_47->setScaledContents(true);
        on_label_106_linkActivated(g_TRACKING_ID[9]);
        on_label_49_linkActivated(g_CONFSCORE[9]);
        update();
    }

}

void MainWindow::on_label_52_destroyed()
{
    if(g_BODY_IMAGE_PATH_LIST[0] != NULL)
    {
        ui->label_52->setPixmap(QPixmap(g_BODY_IMAGE_PATH_LIST[0]));
        ui->label_52->setScaledContents(true);
        on_label_53_linkActivated(g_BODY_TRACKING_ID[0]);
        on_label_51_linkActivated(g_BODY_CONFSCORE[0]);
        on_label_54_linkActivated(g_BODY_GENDER[0]);
        on_label_55_linkActivated(g_BODY_AGE[0]);
        update();
    }

}

void MainWindow::on_label_58_destroyed()
{
    if(g_BODY_IMAGE_PATH_LIST[1] != NULL)
    {
        ui->label_58->setPixmap(QPixmap(g_BODY_IMAGE_PATH_LIST[1]));
        ui->label_58->setScaledContents(true);
        on_label_57_linkActivated(g_BODY_TRACKING_ID[1]);
        on_label_59_linkActivated(g_BODY_CONFSCORE[1]);
        on_label_60_linkActivated(g_BODY_GENDER[1]);
        on_label_56_linkActivated(g_BODY_AGE[1]);
        update();
    }

}

void MainWindow::on_label_66_destroyed()
{
    if(g_BODY_IMAGE_PATH_LIST[2] != NULL)
    {
        ui->label_66->setPixmap(QPixmap(g_BODY_IMAGE_PATH_LIST[2]));
        ui->label_66->setScaledContents(true);
        on_label_65_linkActivated(g_BODY_TRACKING_ID[2]);
        on_label_67_linkActivated(g_BODY_CONFSCORE[2]);
        on_label_70_linkActivated(g_BODY_GENDER[2]);
        on_label_64_linkActivated(g_BODY_AGE[2]);
        update();
    }

}

void MainWindow::on_label_68_destroyed()
{
    if(g_BODY_IMAGE_PATH_LIST[3] != NULL)
    {
        ui->label_68->setPixmap(QPixmap(g_BODY_IMAGE_PATH_LIST[3]));
        ui->label_68->setScaledContents(true);
        on_label_69_linkActivated(g_BODY_TRACKING_ID[3]);
        on_label_61_linkActivated(g_BODY_CONFSCORE[3]);
        on_label_63_linkActivated(g_BODY_GENDER[3]);
        on_label_62_linkActivated(g_BODY_AGE[3]);
        update();
    }

}

void MainWindow::on_label_76_destroyed()
{
    if(g_BODY_IMAGE_PATH_LIST[4] != NULL)
    {
        ui->label_76->setPixmap(QPixmap(g_BODY_IMAGE_PATH_LIST[4]));
        ui->label_76->setScaledContents(true);
        on_label_75_linkActivated(g_BODY_TRACKING_ID[4]);
        on_label_77_linkActivated(g_BODY_CONFSCORE[4]);
        on_label_80_linkActivated(g_BODY_GENDER[4]);
        on_label_74_linkActivated(g_BODY_AGE[4]);
        update();
    }

}

void MainWindow::on_label_78_destroyed()
{
    if(g_BODY_IMAGE_PATH_LIST[5] != NULL)
    {
        ui->label_78->setPixmap(QPixmap(g_BODY_IMAGE_PATH_LIST[5]));
        ui->label_78->setScaledContents(true);
        on_label_79_linkActivated(g_BODY_TRACKING_ID[5]);
        on_label_71_linkActivated(g_BODY_CONFSCORE[5]);
        on_label_73_linkActivated(g_BODY_GENDER[5]);
        on_label_72_linkActivated(g_BODY_AGE[5]);
        update();
    }

}

void MainWindow::on_label_86_destroyed()
{
    if(g_BODY_IMAGE_PATH_LIST[6] != NULL)
    {
        ui->label_86->setPixmap(QPixmap(g_BODY_IMAGE_PATH_LIST[6]));
        ui->label_86->setScaledContents(true);
        on_label_85_linkActivated(g_BODY_TRACKING_ID[6]);
        on_label_87_linkActivated(g_BODY_CONFSCORE[6]);
        on_label_90_linkActivated(g_BODY_GENDER[6]);
        on_label_84_linkActivated(g_BODY_AGE[6]);
        update();
    }

}

void MainWindow::on_label_88_destroyed()
{
    if(g_BODY_IMAGE_PATH_LIST[7] != NULL)
    {
        ui->label_88->setPixmap(QPixmap(g_BODY_IMAGE_PATH_LIST[7]));
        ui->label_88->setScaledContents(true);
        on_label_89_linkActivated(g_BODY_TRACKING_ID[7]);
        on_label_81_linkActivated(g_BODY_CONFSCORE[7]);
        on_label_83_linkActivated(g_BODY_GENDER[7]);
        on_label_82_linkActivated(g_BODY_AGE[7]);
        update();
    }

}

void MainWindow::on_label_96_destroyed()
{
    if(g_BODY_IMAGE_PATH_LIST[8] != NULL)
    {
        ui->label_96->setPixmap(QPixmap(g_BODY_IMAGE_PATH_LIST[8]));
        ui->label_96->setScaledContents(true);
        on_label_95_linkActivated(g_BODY_TRACKING_ID[8]);
        on_label_97_linkActivated(g_BODY_CONFSCORE[8]);
        on_label_100_linkActivated(g_BODY_GENDER[8]);
        on_label_94_linkActivated(g_BODY_AGE[8]);
        update();
    }

}

void MainWindow::on_label_98_destroyed()
{
    if(g_BODY_IMAGE_PATH_LIST[9] != NULL)
    {
        ui->label_98->setPixmap(QPixmap(g_BODY_IMAGE_PATH_LIST[9]));
        ui->label_98->setScaledContents(true);
        on_label_99_linkActivated(g_BODY_TRACKING_ID[9]);
        on_label_91_linkActivated(g_BODY_CONFSCORE[9]);
        on_label_93_linkActivated(g_BODY_GENDER[9]);
        on_label_92_linkActivated(g_BODY_AGE[9]);
        update();
    }

}
