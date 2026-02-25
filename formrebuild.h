#ifndef FORMREBUILD_H
#define FORMREBUILD_H

#include <QWidget>
#include <form.h>
#include "matchimage.h"

namespace Ui {
class FormRebuild;
}

class FormRebuild : public QWidget
{
    Q_OBJECT

public:
    explicit FormRebuild(QWidget *parent = nullptr);
    ~FormRebuild();

private:
    Ui::FormRebuild *ui;
//    matchimage *seeMatchForm;

signals:
    void back_rebuild();

public slots:
    void renew();
    void renew_match();
    void reBuild();
    void reDeep();
    void depDel();
    void openPointData();
};

#endif // FORMREBUILD_H
