#ifndef MATCHIMAGE_H
#define MATCHIMAGE_H

#include <QWidget>

namespace Ui {
class matchImage;
}

class matchImage : public QWidget
{
    Q_OBJECT

public:
    explicit matchImage(QWidget *parent = nullptr);
    ~matchImage();

private:
    Ui::matchImage *ui;
};

#endif // MATCHIMAGE_H
