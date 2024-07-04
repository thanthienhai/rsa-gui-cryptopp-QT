#ifndef KYVANBAN_H
#define KYVANBAN_H

#include <QDialog>

namespace Ui {
class KyVanBan;
}

class KyVanBan : public QDialog
{
    Q_OBJECT

public:
    explicit KyVanBan(QWidget *parent = nullptr);
    ~KyVanBan();

private slots:
    void on_pushButton_thoat_clicked();

    void on_pushButton_taiVanBanTXT_clicked();

    void on_pushButton_taiVanBanDOCX_clicked();

    void on_pushButton_luuChuKy_clicked();

private:
    Ui::KyVanBan *ui;
};

#endif // KYVANBAN_H
