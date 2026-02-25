#ifndef FORM_H
#define FORM_H

#include <QWidget>

namespace Ui {
class Form;
}

class Form : public QWidget
{
    Q_OBJECT

public:
    explicit Form(QWidget *parent = nullptr);
    ~Form();

private:
    Ui::Form *ui;

signals:
    void back();

public slots:
    void on_pushButton_clicked();
    void delete_biaodData();
};

#endif // FORM_H
