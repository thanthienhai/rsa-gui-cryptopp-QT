#ifndef XACNHANVANBAN_H
#define XACNHANVANBAN_H

#include <QDialog>

namespace Ui {
class xacnhanvanban;
}

class xacnhanvanban : public QDialog
{
    Q_OBJECT

public:
    explicit xacnhanvanban(QWidget *parent = nullptr);
    ~xacnhanvanban();

private slots:
    void on_pushButton_taiVanBanTXT_clicked();

    void on_pushButton_taiChuKy_clicked();

    void on_pushButton_taiKhoaCongKhai_clicked();

    void on_pushButton_xacNhan_clicked();

    void on_pushButton_thoat_clicked();

    void on_pushButton_taiVanBanDocx_clicked();

private:
    Ui::xacnhanvanban *ui;
};

#endif // XACNHANVANBAN_H
