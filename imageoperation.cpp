#include "imageoperation.h"
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

imageOperation::imageOperation()
{

}

bool isRegularFile(const std::string& path) {
    struct stat st;
    if (stat(path.c_str(), &st) == 0) {
        return S_ISREG(st.st_mode); // check if it's a regular file
    }
    return false;
}

void removeAllFilesInDirectory(const std::string& path) {
    DIR* dir = opendir(path.c_str());
    if (dir) {
        struct dirent* entry;
        while ((entry = readdir(dir)) != NULL) {
            std::string filename = entry->d_name;
            if (filename != "." && filename != "..") { // avoid removing . and ..
                std::string filepath = path + "/" + filename;
                if (isRegularFile(filepath)) {
                    unlink(filepath.c_str()); // remove file
                }
            }
        }
        closedir(dir);
    }
}


void imageOperation::renewAlbun_biaod(){
    std::string left_dir = "D:/Desktop/code/QTproject/photo/left/";
    std::string right_dir = "D:/Desktop/code/QTproject/photo/right/";

    removeAllFilesInDirectory(left_dir);
    removeAllFilesInDirectory(right_dir);

    std::cout << "目录清空成功！" << std::endl;

    //记录
    ofstream outfile_history("D:/Desktop/code/QTproject/photo/history_data.txt", ios::app);
    if (!outfile_history.is_open()) {
        cerr << "无法打开文件。" << endl;
        return;
    }
    //添加时间戳
    auto now_history = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    outfile_history << "time of operation: " << std::ctime(&now_history); // 将当前时间写入文件
    outfile_history << "type of operation:The library of camera calibration images has been emptied";
    outfile_history << "\n\n\n\n";
    // 关闭文件流
    outfile_history.close();
}

void imageOperation::renewAlbum_rebuild(){
    std::string left_dir = "D:/Desktop/code/QTproject/photo/left_rebuild/";
    std::string right_dir = "D:/Desktop/code/QTproject/photo/right_rebuild/";

    removeAllFilesInDirectory(left_dir);
    removeAllFilesInDirectory(right_dir);

    std::cout << "目录清空成功！" << std::endl;

    //记录
    ofstream outfile_history("D:/Desktop/code/QTproject/photo/history_data.txt", ios::app);
    if (!outfile_history.is_open()) {
        cerr << "无法打开文件。" << endl;
        return;
    }
    //添加时间戳
    auto now_history = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    outfile_history << "time of operation: " << std::ctime(&now_history); // 将当前时间写入文件
    outfile_history << "type of operation:The library of camera reconstructed images has been emptied";
    outfile_history << "\n\n\n\n";
    // 关闭文件流
    outfile_history.close();

}
