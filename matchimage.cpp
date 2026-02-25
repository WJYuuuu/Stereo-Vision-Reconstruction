#include "matchimage.h"
#include "ui_matchimage.h"

matchImage::matchImage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::matchImage)
{
    ui->setupUi(this);
}

matchImage::~matchImage()
{
    delete ui;
}
