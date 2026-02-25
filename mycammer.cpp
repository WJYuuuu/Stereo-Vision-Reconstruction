#include "mycammer.h"
#include <opencv2/opencv.hpp>
#include <iostream>
#include <vector>
#include <chrono>
#include <ctime>
#include<string>
using namespace std;
using namespace cv;


mycammer::mycammer()
{

}

//相机标定槽函数
void mycammer::biaod(){

    // 文件路径
    string left_folder = "D:/Desktop/code/QTproject/photo/left/"; // 左相机图像文件夹路径
    string right_folder = "D:/Desktop/code/QTproject/photo/right/"; // 右相机图像文件夹路径

    //存储左右相机的路径
    vector<string> left_image_paths,right_image_paths;

    //获取左右相机拍摄照片的路径
    for(int i=1;i<=14;++i){
        left_image_paths.push_back(left_folder+"l"+to_string(i)+".jpg");
        cout<<left_folder+"l"+to_string(i)+".jpg"<<endl;
        right_image_paths.push_back(right_folder+"r"+to_string(i)+".jpg");
        cout<<right_folder+"r"+to_string(i)+".jpg"<<endl;

    }


    // 确保左右图像路径的数量和顺序一致
    if (left_image_paths.size() != right_image_paths.size()) {
        cerr << "错误：左右图像数量或顺序不匹配。" << endl;
        return ;
    }


    //加载左右相机图像    Mat：矩阵类型数据
    vector<Mat> left_images,right_images;
    for(size_t i=0; i<left_image_paths.size();i++){
        Mat left_image = imread(left_image_paths[i]);
        cout<<endl<<left_image_paths[i]<<endl;
        Mat right_image = imread(right_image_paths[i]);
        cout<<endl<<right_image_paths[i]<<endl;
        if(left_image.empty() || right_image.empty()){
            cerr << "错误：无法加载图像:"<< i <<endl;
            return;
        }
        left_images.push_back(left_image);
        right_images.push_back(right_image);
    }


    //设置棋盘格参数
    Size board_size(11,8);  //棋盘格内的角点数量
    float square_size = 0.01;  //棋盘格的实际尺寸（单位：米）


    //棋盘格内角点的三维点和二维图像点的向量
    vector<vector<Point3f>> object_points;
    vector<vector<Point2f>> left_image_points,right_image_points;

    //检测棋盘格角点
    for (size_t i=0;i<left_images.size();++i){
        Mat left_gray,right_gray;
        cvtColor(left_images[i],left_gray,COLOR_BGR2GRAY);
        cvtColor(right_images[i],right_gray,COLOR_BGR2GRAY);


        //left_corners用来存储找到的角点坐标，left_found用于判断是否找到
        vector<Point2f> left_corners,right_corners;
        bool left_found = findChessboardCorners(left_gray,board_size,left_corners);
        bool right_found = findChessboardCorners(right_gray,board_size,right_corners);

        //如果找到左右角点的话
        if(left_found && right_found){
            vector<Point3f> obj;
            //计算三维坐标
            for(int y=0;y<board_size.height;++y){
                for(int x=0;x<board_size.width;++x){
                    obj.push_back(Point3f(x*square_size,y*square_size,0));
                }
            }

            //将检测到的点加入向量
            object_points.push_back(obj);
            left_image_points.push_back(left_corners);
            right_image_points.push_back(right_corners);


            //
            // 保存检测到角点的图像
                        Mat left_image_with_corners = left_images[i].clone();
                        Mat right_image_with_corners = right_images[i].clone();
                        drawChessboardCorners(left_image_with_corners, board_size, left_corners, left_found);
                        drawChessboardCorners(right_image_with_corners, board_size, right_corners, right_found);

                        string left_filename =  "D:\\Desktop\\code\\QTproject\\photo\\cor\\left_image_" + to_string(i) + ".png";
                        string right_filename =  "D:\\Desktop\\code\\QTproject\\photo\\cor\\right_image_" + to_string(i) + ".png";
                        imwrite(left_filename, left_image_with_corners);
                        imwrite(right_filename, right_image_with_corners);
            //





        }

    }
    // 初始化相机矩阵和畸变系数
    Mat camera_matrix1 = initCameraMatrix2D(object_points, left_image_points, left_images[0].size(), 0);
    Mat dist_coeffs1 = Mat::zeros(8, 1, CV_64F);
    Mat camera_matrix2 = initCameraMatrix2D(object_points, right_image_points, right_images[0].size(), 0);
    Mat dist_coeffs2 = Mat::zeros(8, 1, CV_64F);

    // 双目标定
    Mat R, T, E, F;
    Size image_size = left_images[0].size();
    double rms = stereoCalibrate(object_points, left_image_points, right_image_points,
                                 camera_matrix1, dist_coeffs1, camera_matrix2, dist_coeffs2,
                                 image_size, R, T, E, F,
                                 CALIB_USE_INTRINSIC_GUESS,
                                 TermCriteria(TermCriteria::COUNT + TermCriteria::EPS, 100, 1e-5));

//***********************************************//
    // 存储标定结果
        FileStorage fs("calibration.yml", FileStorage::WRITE);
        fs << "camera_matrix1" << camera_matrix1;
        fs << "camera_matrix2" << camera_matrix2;
        fs << "R" << R;
        fs << "T" << T;
        fs.release();


    // 输出标定结果
    cout << "双目标定 RMS 误差: " << rms << endl;
    cout << "相机矩阵 1:" << endl << camera_matrix1 << endl;
//    cout << "畸变系数 1:" << endl << dist_coeffs1 << endl;
    cout << "相机矩阵 2:" << endl << camera_matrix2 << endl;
//    cout << "畸变系数 2:" << endl << dist_coeffs2 << endl;
    cout << "旋转矩阵:" << endl << R << endl;
    cout << "平移向量:" << endl << T << endl;




    // 打开文件以进行写入，如果文件不为空，则在末尾添加
       ofstream outfile("D:/Desktop/code/QTproject/photo/data.txt", ios::app);
       if (!outfile.is_open()) {
           cerr << "无法打开文件。" << endl;
           return;
       }
       //添加时间戳
       auto now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
       outfile << "操作时间: " << std::ctime(&now); // 将当前时间写入文件
       outfile << "双目标定 RMS 误差: " << rms << endl;
       outfile << "相机矩阵 1:" << endl << camera_matrix1 << endl;
//       outfile << "畸变系数 1:" << endl << dist_coeffs1 << endl;
       outfile << "相机矩阵 2:" << endl << camera_matrix2 << endl;
//       outfile << "畸变系数 2:" << endl << dist_coeffs2 << endl;
       outfile << "旋转矩阵:" << endl << R << endl;
       outfile << "平移向量:" << endl << T << endl;
       // 写入标定结果到文件中
       outfile << "\n\n\n\n";
       // 关闭文件流
       outfile.close();



       ofstream outfile_history("D:/Desktop/code/QTproject/photo/history_data.txt", ios::app);
       if (!outfile_history.is_open()) {
           cerr << "无法打开文件。" << endl;
           return;
       }
       //添加时间戳
       auto now_history = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
       outfile_history << "time of operation: " << std::ctime(&now_history); // 将当前时间写入文件
       outfile_history << "type of operation: Camera calibration";
       outfile_history << "\n\n\n\n";
       // 关闭文件流
       outfile_history.close();

}
