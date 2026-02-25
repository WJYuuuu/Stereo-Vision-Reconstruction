#ifndef HISTORYOP_H
#define HISTORYOP_H

#include <QWidget>

namespace Ui {
class HistoryOp;
}

class HistoryOp : public QWidget
{
    Q_OBJECT

public:
    explicit HistoryOp(QWidget *parent = nullptr);
    ~HistoryOp();


private:
    Ui::HistoryOp *ui;
    void renewHistory();
    void delHistory();

signals:
    void back_historyOp();
};

#endif // HISTORYOP_H
