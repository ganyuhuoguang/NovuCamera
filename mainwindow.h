#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QString>
#include <QTimer>
#include <QImage>
#include <QMutex>
#include <QFileDialog>
#include <QInputDialog>

#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include "qimage.h"
#include <iostream>
#include "vlc/vlc.h"

#include <libavformat/avformat.h>
#include <libavfilter/avfilter.h>
#include <libswscale/swscale.h>
#include <libavutil/frame.h>
#include "mythread.h"
#include "mysql.h"

#ifndef INT64_C
#define INT64_C
#define UINT64_C
#endif

using namespace cv;
using namespace std;

namespace Ui {
class MainWindow;
class VideoStream;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    char* GetVideoBuf() const;
    void OpenVideo(const char* rtsp);
    void handleTimeout();

private slots:

    void on_openGLWidget_destroyed();

    void on_pushButton_close_clicked();

    void on_pushButton_open_clicked();

    void on_pushButton_pause_clicked();

    void on_lineEdit_returnPressed();

    void on_label_linkActivated(const QString &link);

    void on_label_22_linkActivated(const QString &link);

    void on_label_20_linkActivated(const QString &link);

    void on_label_21_linkActivated(const QString &link);

    void on_horizontalScrollBar_valueChanged(int value);



    void on_label_50_linkActivated(const QString &link);

    void on_label_24_linkActivated(const QString &link);

    void on_label_29_linkActivated(const QString &link);

    void on_label_31_linkActivated(const QString &link);

    void on_label_26_linkActivated(const QString &link);

    void on_label_25_linkActivated(const QString &link);

    void on_label_101_linkActivated(const QString &link);

    void on_label_27_linkActivated(const QString &link);

    void on_label_39_linkActivated(const QString &link);

    void on_label_41_linkActivated(const QString &link);

    void on_label_35_linkActivated(const QString &link);

    void on_label_33_linkActivated(const QString &link);

    void on_label_102_linkActivated(const QString &link);

    void on_label_37_linkActivated(const QString &link);

    void on_label_45_linkActivated(const QString &link);

    void on_label_46_linkActivated(const QString &link);

    void on_label_43_linkActivated(const QString &link);

    void on_label_42_linkActivated(const QString &link);

    void on_label_103_linkActivated(const QString &link);

    void on_label_44_linkActivated(const QString &link);

    void on_label_104_linkActivated(const QString &link);

    void on_label_105_linkActivated(const QString &link);

    void on_label_48_linkActivated(const QString &link);

    void on_label_47_linkActivated(const QString &link);

    void on_label_106_linkActivated(const QString &link);

    void on_label_49_linkActivated(const QString &link);

    void on_horizontalScrollBar_2_valueChanged(int value);

    void on_label_52_linkActivated(const QString &link);

    void on_label_55_linkActivated(const QString &link);

    void on_label_53_linkActivated(const QString &link);

    void on_label_51_linkActivated(const QString &link);

    void on_label_54_linkActivated(const QString &link);

    void on_label_58_linkActivated(const QString &link);

    void on_label_57_linkActivated(const QString &link);

    void on_label_59_linkActivated(const QString &link);

    void on_label_60_linkActivated(const QString &link);

    void on_label_56_linkActivated(const QString &link);

    void on_label_66_linkActivated(const QString &link);

    void on_label_65_linkActivated(const QString &link);

    void on_label_67_linkActivated(const QString &link);

    void on_label_70_linkActivated(const QString &link);

    void on_label_64_linkActivated(const QString &link);

    void on_label_68_linkActivated(const QString &link);

    void on_label_69_linkActivated(const QString &link);

    void on_label_61_linkActivated(const QString &link);

    void on_label_63_linkActivated(const QString &link);

    void on_label_62_linkActivated(const QString &link);

    void on_label_76_linkActivated(const QString &link);

    void on_label_75_linkActivated(const QString &link);

    void on_label_77_linkActivated(const QString &link);

    void on_label_80_linkActivated(const QString &link);

    void on_label_74_linkActivated(const QString &link);

    void on_label_78_linkActivated(const QString &link);

    void on_label_79_linkActivated(const QString &link);

    void on_label_71_linkActivated(const QString &link);

    void on_label_73_linkActivated(const QString &link);

    void on_label_72_linkActivated(const QString &link);

    void on_label_86_linkActivated(const QString &link);

    void on_label_85_linkActivated(const QString &link);

    void on_label_87_linkActivated(const QString &link);

    void on_label_90_linkActivated(const QString &link);

    void on_label_84_linkActivated(const QString &link);

    void on_label_88_linkActivated(const QString &link);

    void on_label_89_linkActivated(const QString &link);

    void on_label_81_linkActivated(const QString &link);

    void on_label_83_linkActivated(const QString &link);

    void on_label_82_linkActivated(const QString &link);

    void on_label_96_linkActivated(const QString &link);

    void on_label_95_linkActivated(const QString &link);

    void on_label_97_linkActivated(const QString &link);

    void on_label_100_linkActivated(const QString &link);

    void on_label_94_linkActivated(const QString &link);

    void on_label_98_linkActivated(const QString &link);

    void on_label_99_linkActivated(const QString &link);

    void on_label_91_linkActivated(const QString &link);

    void on_label_93_linkActivated(const QString &link);

    void on_label_92_linkActivated(const QString &link);

    void on_lineEdit_4_returnPressed();

    void on_pushButton_2_clicked();

    void on_lineEdit_7_returnPressed();

    void on_lineEdit_2_returnPressed();

    void on_lineEdit_3_returnPressed();

    void on_label_destroyed();

    void on_label_22_destroyed();

    void on_label_29_destroyed();

    void on_label_25_destroyed();

    void on_label_39_destroyed();

    void on_label_33_destroyed();

    void on_label_45_destroyed();

    void on_label_42_destroyed();

    void on_label_104_destroyed();

    void on_label_47_destroyed();

    void on_label_52_destroyed();

    void on_label_58_destroyed();

    void on_label_66_destroyed();

    void on_label_68_destroyed();

    void on_label_76_destroyed();

    void on_label_78_destroyed();

    void on_label_86_destroyed();

    void on_label_88_destroyed();

    void on_label_96_destroyed();

    void on_label_98_destroyed();

    void on_pushButton_restart_clicked();

private:
    void AllocVideoBuf(int width, int height);

    static void* PreDecode_cb(void *opaque, void **planes);
    static void HandleStream_cb(void *opaque, void *picture, void *const *planes);
    static void Render_cb(void *opaque, void *picture);

private:
    Ui::MainWindow *ui;
    QTimer *timer;
    VideoCapture capture;
    Mat frame;
    Mat result_frame;
    QString filename;
    Mat src;
    QImage Qimg;
    void DelayMS(unsigned int ms);

    QMutex mutex;
    AVPicture  pAVPicture;
    AVFormatContext *pAVFormatContext;
    AVCodecContext *pAVCodecContext;
    AVFrame *pAVFrame;
    SwsContext * pSwsContext;
    AVPacket pAVPacket;

    char * m_videobuf;
    int m_frameWidth;
    int m_frameHeight;

    libvlc_instance_t* m_vlcInstance;
    libvlc_media_player_t* m_vlcMediaPlayer;
    libvlc_media_t* m_pvlcMedia;
    MyThread* thread;
};

#endif // MAINWINDOW_H
