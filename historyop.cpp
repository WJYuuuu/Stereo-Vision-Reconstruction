#include "historyop.h"
#include "ui_historyop.h"
#include <QPushButton>
#include <iostream>
#include<QDebug>
#include <opencv2/opencv.hpp>
#include <QFileDialog>

using namespace cv;
using namespace std;

HistoryOp::HistoryOp(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::HistoryOp)
{
    ui->setupUi(this);
    //点击返回按钮，返回主页面
    connect(ui->button_back,&QPushButton::clicked,this,&HistoryOp::back_historyOp);

    connect(ui->button_renew,&QPushButton::clicked,this,&HistoryOp::renewHistory);
    connect(ui->delButton,&QPushButton::clicked,this,&HistoryOp::delHistory);
}
void HistoryOp::delHistory(){

   QString filePath = "D:/Desktop/code/QTproject/photo/history_data.txt";
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

}

void HistoryOp::renewHistory(){
    // 打开文件
        QFile file("D:/Desktop/code/QTproject/photo/history_data.txt");
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

HistoryOp::~HistoryOp()
{
    delete ui;
}
