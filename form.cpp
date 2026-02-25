#include "form.h"
#include "ui_form.h"
#include<mycammer.h>
#include <iostream>
#include <dirent.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <QPushButton>
#include <QApplication>
#include <opencv2/opencv.hpp>
#include <QFileDialog>
#include <QDebug>
using namespace std;
using namespace cv;


Form::Form(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Form)
{
    ui->setupUi(this);



    mycammer *cam = new mycammer();
    //开始标定
    connect(ui->pushButton_start, &QPushButton::clicked, cam, &mycammer::biaod);
    //刷新标定数据
    connect(ui->pushButton_reRead,&QPushButton::clicked,this,&Form::on_pushButton_clicked);
    //删除标定缓存数据
    connect(ui->pushButton_del,&QPushButton::clicked,this,&Form::delete_biaodData);
    //点击返回按钮，返回主页面
    connect(ui->pushButton_back,&QPushButton::clicked,this,&Form::back);

}

void Form::on_pushButton_clicked(){
    // 打开文件
        QFile file("D:/Desktop/code/QTproject/photo/data.txt");
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            qDebug() << "无法打开文件";
            return;
        }

        // 读取文件内容
        QTextStream in(&file);
        QString fileContent = in.readAll();

        // 关闭文件
        file.close();

        // 将文件内容显示在label控件中
        ui->textEdit->setText(fileContent);
}

void Form::delete_biaodData(){
    QString filePath = "D:/Desktop/code/QTproject/photo/data.txt";

        // 创建 QFile 对象并打开文件，以读写模式打开
        QFile file(filePath);
        if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text)) {
            qDebug() << "Error: Could not open file for writing.";
            return;
        }

        // 使用 QTextStream 写入空内容到文件中
        QTextStream out(&file);
        out << "";
        // 关闭文件
        file.close();


        //记录
        ofstream outfile_history("D:/Desktop/code/QTproject/photo/history_data.txt", ios::app);
        if (!outfile_history.is_open()) {
            cerr << "无法打开文件。" << endl;
            return;
        }
        //添加时间戳
        auto now_history = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
        outfile_history << "time of operation: " << std::ctime(&now_history); // 将当前时间写入文件
        outfile_history << "type of operation:Clear the calibration parameters";
        outfile_history << "\n\n\n\n";
        // 关闭文件流
        outfile_history.close();
}

Form::~Form()
{
    delete ui;
}
