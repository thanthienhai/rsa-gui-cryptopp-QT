#ifndef DIALOGT_H
#define DIALOGT_H

#include <QDialog>

namespace Ui {
class Dialogt;
}

class Dialogt : public QDialog
{
    Q_OBJECT

public:
    explicit Dialogt(QWidget *parent = nullptr);
    ~Dialogt();

private slots:
    void on_pushButton_deleteUI_clicked();

    void on_pushButton_chonSoNgTo_clicked();

    void on_pushButton_cancel_clicked();

private:
    Ui::Dialogt *ui;
};

#endif // DIALOGT_H
