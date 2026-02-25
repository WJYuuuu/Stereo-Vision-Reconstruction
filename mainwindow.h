#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <form.h>
#include <formrebuild.h>
#include <historyop.h>
#include <QCameraViewfinder>
#include <imageoperation.h>
#include <QCameraImageCapture>
#include <QPushButton>


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();


private:
    Ui::MainWindow *ui;
    QPushButton *lastClickedButton;
    Form *new_window;
    FormRebuild *new_rebuildWindow;
    HistoryOp *historyOp;
    QCamera *camera1;
    QCamera *camera2;
    QCameraViewfinder *cameraViewfinder1;
    QCameraViewfinder *cameraViewfinder2;
    imageOperation *imageOp;
    void captureAndSaveImages(QCameraImageCapture* imageCapture, const QString& directory);
    void onImageCaptured(int id, const QImage &image);

public slots:
    void history();

signals:

};
#endif // MAINWINDOW_H
