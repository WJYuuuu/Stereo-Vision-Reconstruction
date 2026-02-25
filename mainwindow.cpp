#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPushButton>
#include <QPixmap>
#include <QApplication>
#include <iostream>
#include <opencv2/opencv.hpp>
#include <QCameraInfo>
#include <QCameraViewfinder>
#include <QCameraImageCapture>
#include <QFileDialog>
#include <QDebug>
#include <QVBoxLayout>

using namespace cv;
using namespace std;


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

/******************************************************************************************************************/
/********************************************* 摄像头操作 ***********************************************************/
/******************************************************************************************************************/
    // 获取可用的摄像头信息
    QList<QCameraInfo> cameras = QCameraInfo::availableCameras();

    // 检查是否至少有两个摄像头可用
    if(cameras.size() < 2)
    {
        qDebug() << "Error: At least two cameras are required.";
        return;
    }

    // 初始化两个摄像头
    camera1 = new QCamera(cameras[0]);
    camera2 = new QCamera(cameras[1]);

    //设置摄像头预览
    cameraViewfinder1 = new QCameraViewfinder(this);
    cameraViewfinder2 = new QCameraViewfinder(this);

    //左相机
    ui->label_l->setScaledContents(true);// 图像按比例缩放以适应 QLabel
    ui->label_l->setLayout(new QVBoxLayout);
    ui->label_l->layout()->addWidget(cameraViewfinder1);
    //右相机
    ui->label_r->setScaledContents(true);// 图像按比例缩放以适应 QLabel
    ui->label_r->setLayout(new QVBoxLayout);
    ui->label_r->layout()->addWidget(cameraViewfinder2);

    // 设置相机视图查看器
    camera1->setViewfinder(cameraViewfinder1);
    camera2->setViewfinder(cameraViewfinder2);

    // 启动相机
    camera1->start();
    camera2->start();

    ofstream outfile_history("D:/Desktop/code/QTproject/photo/history_data.txt", ios::app);
    if (!outfile_history.is_open()) {
        cerr << "无法打开文件。" << endl;
        return;
    }
    //添加时间戳
    auto now_history = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    outfile_history << "time of operation: " << std::ctime(&now_history); // 将当前时间写入文件
    outfile_history << "type of operation:Turn on the left and right cameras";
    outfile_history << "\n\n\n\n";
    // 关闭文件流
    outfile_history.close();

/******************************************************************************************************************/
/********************************************* 标定图像库操作 ********************************************************/
/******************************************************************************************************************/
//注：left为左相机标定库，left_rebuild为左相机重建库
//注：right为右相机标定库，right_rebuild为右相机重建库
       // 创建标定图像捕获
        QCameraImageCapture *imageCapture1 = new QCameraImageCapture(camera1, this);
        QCameraImageCapture *imageCapture2 = new QCameraImageCapture(camera2, this);
        // 左相机：连接捕获到的图像信号
        QObject::connect(imageCapture1, &QCameraImageCapture::imageCaptured, this, [&](int id, const QImage &image){
            //判断文件夹是否为空，若为空则从头开始编号，否则从最高编号出编号
            QString directory = "D:/Desktop/code/QTproject/photo/left/";
            QString fileName;
            QDir dir(directory);
            QStringList filters;
            filters << "*.jpg";
            dir.setNameFilters(filters);
            QStringList fileList = dir.entryList(QDir::Files | QDir::NoDotAndDotDot);
            if (fileList.isEmpty()) {
                fileName = directory + "l1.jpg";
            } else {
                int nextId = fileList.count() + 1;
                fileName = directory + "l" + QString::number(nextId) + ".jpg";
            }
            if(lastClickedButton == ui->capButton){
                image.save(fileName);
            }
            //将相机拍摄的照片显示在 ui->label 上
            ui->label_1->setPixmap(QPixmap::fromImage(image));
            ui->label_1->setScaledContents(true); // 图像按比例缩放以适应 QLabel
        });

        // 右相机：连接捕获到的图像信号
        QObject::connect(imageCapture2, &QCameraImageCapture::imageCaptured, this, [&](int id, const QImage &image){
            QString directory = "D:/Desktop/code/QTproject/photo/right/";
            QString fileName;
            QDir dir(directory);
            QStringList filters;
            filters << "*.jpg";
            dir.setNameFilters(filters);
            QStringList fileList = dir.entryList(QDir::Files | QDir::NoDotAndDotDot);
            if (fileList.isEmpty()) {
                fileName = directory + "r1.jpg";
            } else {
                int nextId = fileList.count() + 1;
                fileName = directory + "r" + QString::number(nextId) + ".jpg";
            }
            if(lastClickedButton == ui->capButton){
                image.save(fileName);
            }
            //将相机拍摄的照片显示在 ui->label 上
            ui->label_2->setPixmap(QPixmap::fromImage(image));
            ui->label_2->setScaledContents(true); // 图像按比例缩放以适应 QLabel
        });


/******************************************************************************************************************/
/********************************************* 重建图像库操作 ********************************************************/
/******************************************************************************************************************/

        QObject::connect(imageCapture1, &QCameraImageCapture::imageCaptured, this, [&](int id, const QImage &image){
            QString directory = "D:/Desktop/code/QTproject/photo/left_rebuild/";
            QString fileName;
                fileName = directory + "l1"  + ".jpg";

            if(lastClickedButton == ui->capButton_rebuild){
                image.save(fileName);
            }
            //将相机拍摄的照片显示在 ui->label 上
            ui->label_1->setPixmap(QPixmap::fromImage(image));
            ui->label_1->setScaledContents(true); // 图像按比例缩放以适应 QLabel
        });

        // 右相机：连接捕获到的图像信号
        QObject::connect(imageCapture2, &QCameraImageCapture::imageCaptured, this, [&](int id, const QImage &image){
            //QString fileName = "D:/Desktop/code/QTproject/photo/right_rebuild/r" + QString::number(id) + ".jpg";
            //判断文件夹是否为空，若为空则从头开始编号，否则从最高编号出编号
            QString directory = "D:/Desktop/code/QTproject/photo/right_rebuild/";
            QString fileName;
            fileName = directory + "r1" + ".jpg";
            if(lastClickedButton == ui->capButton_rebuild){
                image.save(fileName);
            }
            //将相机拍摄的照片显示在 ui->label 上
            ui->label_2->setPixmap(QPixmap::fromImage(image));
            ui->label_2->setScaledContents(true); // 图像按比例缩放以适应 QLabel
        });

        // 连接捕获按钮点击事件到捕获图像
        //标定捕获
        QObject::connect(ui->capButton, &QPushButton::clicked, this, [=](){
            lastClickedButton  = ui->capButton;
            imageCapture1->capture();
            imageCapture2->capture();

            //记录
            ofstream outfile_history("D:/Desktop/code/QTproject/photo/history_data.txt", ios::app);
            if (!outfile_history.is_open()) {
                cerr << "无法打开文件。" << endl;
                return;
            }
            //添加时间戳
            auto now_history = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
            outfile_history << "time of operation: " << std::ctime(&now_history); // 将当前时间写入文件
            outfile_history << "type of operation:The camera the photo-taking pictures";
            outfile_history << "\n\n\n\n";
            // 关闭文件流
            outfile_history.close();
        });
        //重建捕获
        QObject::connect(ui->capButton_rebuild, &QPushButton::clicked, this, [=](){
            lastClickedButton  = ui->capButton_rebuild;
            imageCapture1->capture();
            imageCapture2->capture();


            //记录
            ofstream outfile_history("D:/Desktop/code/QTproject/photo/history_data.txt", ios::app);
            if (!outfile_history.is_open()) {
                cerr << "无法打开文件。" << endl;
                return;
            }
            //添加时间戳
            auto now_history = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
            outfile_history << "time of operation: " << std::ctime(&now_history); // 将当前时间写入文件
            outfile_history << "type of operation:The camera reconstructs the photo-taking operation";
            outfile_history << "\n\n\n\n";
            // 关闭文件流
            outfile_history.close();
        });


/******************************************************************************************************************/
/********************************************* 初始化相册操作 ********************************************************/
/******************************************************************************************************************/
        imageOp = new imageOperation();
        connect(ui->pushButton_renew1,&QPushButton::clicked,imageOp,&imageOperation::renewAlbun_biaod);
        connect(ui->pushButton_renew2,&QPushButton::clicked,imageOp,&imageOperation::renewAlbum_rebuild);


/******************************************************************************************************************/
/*********************************************跳转标定页面操作********************************************************/
/******************************************************************************************************************/
        this->new_window = new Form();
        connect(ui->pushButton,&QPushButton::clicked,this,[=](){
            this->hide();
            this->new_window->show();

            //记录
            ofstream outfile_history("D:/Desktop/code/QTproject/photo/history_data.txt", ios::app);
            if (!outfile_history.is_open()) {
                cerr << "无法打开文件。" << endl;
                return;
            }
            //添加时间戳
            auto now_history = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
            outfile_history << "time of operation: " << std::ctime(&now_history); // 将当前时间写入文件
            outfile_history << "type of operation:The parameter calibration page opens";
            outfile_history << "\n\n\n\n";
            // 关闭文件流
            outfile_history.close();
        });
        connect(this->new_window,&Form::back,this,[=](){
            this->new_window->hide();
            this->show();

            //记录
            ofstream outfile_history("D:/Desktop/code/QTproject/photo/history_data.txt", ios::app);
            if (!outfile_history.is_open()) {
                cerr << "无法打开文件。" << endl;
                return;
            }
            //添加时间戳
            auto now_history = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
            outfile_history << "time of operation:" << std::ctime(&now_history); // 将当前时间写入文件
            outfile_history << "type of operation:Return to the main page from the parameter calibration page";
            outfile_history << "\n\n\n\n";
            // 关闭文件流
            outfile_history.close();
        });

/******************************************************************************************************************/
/*********************************************跳转重建页面操作********************************************************/
/******************************************************************************************************************/
        this->new_rebuildWindow = new FormRebuild();
        connect(ui->button_rebuild,&QPushButton::clicked,this,[=](){
            this->hide();
            this->new_rebuildWindow->show();

            //记录
            ofstream outfile_history("D:/Desktop/code/QTproject/photo/history_data.txt", ios::app);
            if (!outfile_history.is_open()) {
                cerr << "无法打开文件。" << endl;
                return;
            }
            //添加时间戳
            auto now_history = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
            outfile_history << "time of operation: " << std::ctime(&now_history); // 将当前时间写入文件
            outfile_history << "type of operation: Open the Rebuild page";
            outfile_history << "\n\n\n\n";
            // 关闭文件流
            outfile_history.close();
        });
        connect(this->new_rebuildWindow,&FormRebuild::back_rebuild,this,[=](){
            this->new_rebuildWindow->hide();
            this->show();

            //记录
            ofstream outfile_history("D:/Desktop/code/QTproject/photo/history_data.txt", ios::app);
            if (!outfile_history.is_open()) {
                cerr << "无法打开文件。" << endl;
                return;
            }
            //添加时间戳
            auto now_history = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
            outfile_history << "time of operation: " << std::ctime(&now_history); // 将当前时间写入文件
            outfile_history << "type of operation: Return to the main page from the rebuilt page";
            outfile_history << "\n\n\n\n";
            // 关闭文件流
            outfile_history.close();
        });




/******************************************************************************************************************/
/*********************************************跳转历史记录页面********************************************************/
/******************************************************************************************************************/
        this->historyOp = new HistoryOp();
        connect(ui->button_history,&QPushButton::clicked,this,[=](){
           this->hide();
            historyOp->show();

            //记录
            ofstream outfile_history("D:/Desktop/code/QTproject/photo/history_data.txt", ios::app);
            if (!outfile_history.is_open()) {
                cerr << "无法打开文件。" << endl;
                return;
            }
            //添加时间戳
            auto now_history = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
            outfile_history << "time of operation: " << std::ctime(&now_history); // 将当前时间写入文件
            outfile_history << "type of operation: This page goes to the Operation History page";
            outfile_history << "\n\n\n\n";
            // 关闭文件流
            outfile_history.close();
        });
        connect(this->historyOp,&HistoryOp::back_historyOp,this,[=](){
            this->historyOp->hide();
            this->show();
            //记录
            ofstream outfile_history("D:/Desktop/code/QTproject/photo/history_data.txt", ios::app);
            if (!outfile_history.is_open()) {
                cerr << "无法打开文件。" << endl;
                return;
            }
            //添加时间戳
            auto now_history = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
            outfile_history << "time of operation: " << std::ctime(&now_history); // 将当前时间写入文件
            outfile_history << "type of operation:Return to the main page from the Operation History page";
            outfile_history << "\n\n\n\n";
            // 关闭文件流
            outfile_history.close();
        });


}
void MainWindow::history(){

}

MainWindow::~MainWindow()
{
    delete ui;
}


