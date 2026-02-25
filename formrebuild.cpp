#include "formrebuild.h"
#include "ui_formrebuild.h"
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
#include "matchimage.h"
#include "QDesktopServices"

using namespace cv;
using namespace std;


FormRebuild::FormRebuild(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FormRebuild)
{
    ui->setupUi(this);

    //返回主页面
    connect(ui->pushButton_back,&QPushButton::clicked,this,&FormRebuild::back_rebuild);

    //刷新重建相册
    connect(ui->pushButton,&QPushButton::clicked,this,&FormRebuild::renew);

    //开始重建
    connect(ui->pushButton_rebuild,&QPushButton::clicked,this,&FormRebuild::reBuild);

    //刷新深度和视差
    connect(ui->pushButton_deep,&QPushButton::clicked,this,&FormRebuild::reDeep);

    //清空深度缓存
    connect(ui->pushButton_depDel,&QPushButton::clicked,this,&FormRebuild::depDel);


    //刷新匹配图预显
    connect(ui->seeMatchButton,&QPushButton::clicked,this,&FormRebuild::renew_match);

    //刷新匹配图预显
    connect(ui->seeDataButton,&QPushButton::clicked,this,&FormRebuild::openPointData);

}

void FormRebuild::depDel(){
    QString filePath = "D:/Desktop/code/QTproject/photo/deepth.txt";
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
        outfile_history << "type of operation:Clear parallax and depth cache";
        outfile_history << "\n\n\n\n";
        // 关闭文件流
        outfile_history.close();
}

void FormRebuild::reDeep(){
    // 打开文件
        QFile file("D:/Desktop/code/QTproject/photo/deepth.txt");
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

//重建图像预显
void FormRebuild::renew(){
    // 加载左边的图片
        QPixmap left_image("D:/Desktop/code/QTproject/photo/left_rebuild/l1.jpg");
        if (left_image.isNull()) {
            qDebug() << "Failed to load left image.";
        } else {
            // 缩放图片以适应标签大小
            QPixmap scaled_left = left_image.scaled(ui->label_l->size(), Qt::KeepAspectRatio);
            ui->label_l->setPixmap(scaled_left);
        }

        // 加载右边的图片
        QPixmap right_image("D:/Desktop/code/QTproject/photo/right_rebuild/r1.jpg");
        if (right_image.isNull()) {
            qDebug() << "Failed to load right image.";
        } else {
            // 缩放图片以适应标签大小
            QPixmap scaled_right = right_image.scaled(ui->label_r->size(), Qt::KeepAspectRatio);
            ui->label_r->setPixmap(scaled_right);
        }


}


//匹配图像预显
void FormRebuild::renew_match() {
    // 加载左边的图片
        QPixmap left_image("D:/Desktop/code/QTproject/photo/p_deep_match/img_matches.png");
        if (left_image.isNull()) {
            qDebug() << "Failed to load left image.";
        } else {
            // 缩放图片以适应标签大小
            QPixmap scaled_left = left_image.scaled(ui->match_label->size(), Qt::KeepAspectRatio);
            ui->match_label->setPixmap(scaled_left);
        }

        // 加载右边的图片
        QPixmap right_image("D:/Desktop/code/QTproject/photo/p_deep_match/depth_map.png");
        if (right_image.isNull()) {
            qDebug() << "Failed to load right image.";
        } else {
            // 缩放图片以适应标签大小
            QPixmap scaled_right = right_image.scaled(ui->deep_label->size(), Qt::KeepAspectRatio);
            ui->deep_label->setPixmap(scaled_right);
        }


}



//匹配图像预显
void FormRebuild::openPointData() {

    QString filePath = "D:/Desktop/code/QTproject/photo/point_cloud.csv";  // 替换为实际文件路径
    QDesktopServices::openUrl(QUrl::fromLocalFile(filePath));
}




//开始重建
void FormRebuild::reBuild(){
    // 读取源图像和目标图像
    Mat img1 = imread("D:/Desktop/code/QTproject/photo/left_rebuild/l1.jpg", IMREAD_GRAYSCALE); // 读取左侧图像，以灰度模式加载
    Mat img2 = imread("D:/Desktop/code/QTproject/photo/right_rebuild/r1.jpg", IMREAD_GRAYSCALE); // 读取右侧图像，以灰度模式加载

    if (img1.empty() || img2.empty()) { // 检查图像是否为空
        cout << "Error: Unable to load images!" << endl; // 输出错误信息
        return ; // 返回错误代码
    }

//    // 定义相机内参矩阵
//    Mat K1 = (Mat_<double>(3, 3) << 432.84, 0, 675.8, 0, 432.76, 453.7, 0, 0, 1); // 左相机内参矩阵
//    Mat K2 = (Mat_<double>(3, 3) << 443.75, 0, 699.3, 0, 445.1, 385.2, 0, 0, 1); // 右相机内参矩阵

//    // 定义相机外参矩阵
//    Mat R = (Mat_<double>(3, 3) << 0.8987, -0.0838, 0.4305, 0.0873, 0.9961, 0.0118, -0.4299, 0.027, 0.9025); // 旋转矩阵
//    Mat T = (Mat_<double>(3, 1) << -844.185, 21.2725, 265.2906); // 平移向量



    // 创建Mat对象用于存储读取的参数
        Mat K1, K2, R, T;

        // 读取标定结果
        FileStorage fss("calibration.yml", FileStorage::READ);
        if (!fss.isOpened()) {
            cerr << "Failed to open calibration file." << endl;
            return ;
        }

        fss["camera_matrix1"] >> K1;
        fss["camera_matrix2"] >> K2;
        fss["R"] >> R;
        fss["T"] >> T;
        fss.release();

cout<<"biaojik1::::::::::::::::"<<K1<<endl;
cout<<"biaojik2::::::::::::::::"<<K2<<endl;
cout<<"biaojiR::::::::::::::::"<<R<<endl;
cout<<"biaoji::::::::::::::::"<<T<<endl;


    //创建ORB特征检测器
    Ptr<ORB> orb = ORB::create(50000);

    //检测图像中的特征点和图像描述符
    vector<KeyPoint> keypoints1,keypoints2;//创建存储特征点的特征向量
    Mat descriptors1,descriptors2;//创建存储描述符的矩阵
    orb->detectAndCompute(img1,Mat(),keypoints1,descriptors1);//检测第一幅图的特征点和计算描述符
    orb->detectAndCompute(img2,Mat(),keypoints2,descriptors2);//检测第二幅图的特征点和计算描述符

    //创建特征匹配器
    Ptr<DescriptorMatcher> matcher = DescriptorMatcher::create("BruteForce-Hamming"); // 创建Brute-Force匹配器

    //匹配描述符
    vector<DMatch> matchs;//创建存储匹配结果的向量
    matcher->match(descriptors1,descriptors2,matchs);//对特征点描述符进行匹配

    //缓存视差和深度
    QString filePath_depth = "D:/Desktop/code/QTproject/photo/deepth.txt";


    // 打开文件以进行写入，如果文件不为空，则在末尾添加
       ofstream outfile("D:/Desktop/code/QTproject/photo/deepth.txt", ios::app);

       // 写入标定结果到文件中
       outfile << "\n\n\n\n";
//       //添加时间戳
       auto now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
       outfile << "操作时间: " << std::ctime(&now); // 将当前时间写入文件


    //输出视差和深度
    cout << "Matched Points Disparity and Depth:" << endl; // 输出标题
    outfile<<"Matched Points Disparity and Depth:" << endl;

    // 创建存储深度信息的矩阵
    Mat depth_map(img1.size(), CV_64F); // 深度图像与左图像大小相同，数据类型为双精度浮点型

    //创建三维点向量
    vector<Point3f> point_cloud;

    //绘制匹配结果的图像
    Mat img_matches;
    drawMatches(img1,keypoints1,img2,keypoints2,matchs,img_matches);

    for(int i = 0; i < (int)matchs.size();i++){
        Point2f pt1 = keypoints1[matchs[i].queryIdx].pt;//获取第一幅图像中特征点的索引坐标
        Point2f pt2 = keypoints2[matchs[i].trainIdx].pt;//获取第二幅图像中特征点的索引坐标
        double disparity = abs(pt1.x-pt2.x);//计算视差
        double baseline = 150.0;//基线长度,mm
        double depth = K1.at<double>(0.0)*baseline/disparity; //计算深度，使用左相机焦距

        cout << "Point " << i << ": Disparity = " << disparity << ", Depth = " << depth << endl; // 输出视差和深度信息
        //深度信息和视差进行缓存
        outfile<<"Point " << i << ": Disparity = " << disparity << ", Depth = " << depth << endl;

        //将匹配坐标添加到三维点云中
        double X = (pt1.x - K1.at<double>(0, 2)) * depth / K1.at<double>(0, 0); // 计算X坐标
        double Y = (pt1.y - K1.at<double>(1, 2)) * depth / K1.at<double>(1, 1); // 计算Y坐标
        double Z = depth; // Z坐标即深度值
        point_cloud.push_back(Point3f(X, Y, Z)); // 将三维坐标添加到点云中

        // 将深度信息存储到深度图像中
        depth_map.at<double>(pt1) = depth; // 在深度图像中对应位置存储深度值

    }

    // 关闭文件流
    outfile.close();



    // 绘制特征点
    for (int i = 0; i < keypoints1.size(); i++) { // 遍历第一幅图像的所有特征点
        Point pt = keypoints1[i].pt; // 获取特征点坐标
        circle(img_matches, pt, 3, Scalar(0, 255, 0), -1); // 绘制圆圈表示特征点
    }
    for (int i = 0; i < keypoints2.size(); i++) { // 遍历第二幅图像的所有特征点
        Point pt = keypoints2[i].pt + Point2f(img1.cols, 0); // 获取特征点坐标，并将x坐标偏移，使得在右侧图像上绘制
        circle(img_matches, pt, 3, Scalar(0, 255, 0), -1); // 绘制圆圈表示特征点
    }

    // 调整图像大小，以适应屏幕
   // double scale = 0.5; // 设置缩放比例
//    resize(img_matches, img_matches, Size(), scale, scale); // 调整图像大小

    // 显示匹配结果和源图像、目标图像
    //imshow("Matches and Images", img_matches); // 在窗口中显示匹配结果和原始图像

    // 将深度图像显示出来
    //namedWindow("Depth Map", WINDOW_NORMAL); // 创建窗口
    //imshow("Depth Map", depth_map); // 在窗口中显示深度图像
    //waitKey(0); // 等待键盘按键

    // 将深度图像保存为文件（可选）
    imwrite("depth_map.png", depth_map);


    // 保存深度图和匹配图到指定路径
    imwrite("D:\\Desktop\\code\\QTproject\\photo\\p_deep_match\\depth_map.png", depth_map);
    imwrite("D:\\Desktop\\code\\QTproject\\photo\\p_deep_match\\img_matches.png", img_matches);


    // 将点云保存到文件（可选）
    FileStorage fs("D:/Desktop/code/QTproject/photo/point_cloud.yml", FileStorage::WRITE); // 创建文件存储对象
    fs << "point_cloud" << point_cloud; // 将点云写入文件
    fs.release(); // 释放文件存储对象

    // 打开 CSV 文件用于写入
    ofstream csv_file("D:/Desktop/code/QTproject/photo/point_cloud.csv");
    // 写入 CSV 文件标题行
    csv_file << "X,Y,Z\n";
    // 遍历点云数据并写入到 CSV 文件中
    for (const auto& point : point_cloud) {
        csv_file << point.x << "," << point.y << "," << point.z << "\n";
    }

    // 关闭文件
    csv_file.close();

    //记录
    ofstream outfile_history("D:/Desktop/code/QTproject/photo/history_data.txt", ios::app);
    if (!outfile_history.is_open()) {
        cerr << "无法打开文件。" << endl;
        return;
    }
    //添加时间戳
    auto now_history = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    outfile_history << "time of operation: " << std::ctime(&now_history); // 将当前时间写入文件
    outfile_history << "type of operation:3D reconstruction operations";
    outfile_history << "\n\n\n\n";
    // 关闭文件流
    outfile_history.close();


    return ;
}

FormRebuild::~FormRebuild()
{
    delete ui;
}
