#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <dialogt.h>
#include <kyvanban.h>
#include <xacnhanvanban.h>
#include <QFileDialog>
#include <cryptopp/rsa.h>
#include <cryptopp/osrng.h>
#include <cryptopp/files.h>
#include <cryptopp/hex.h>
#include <QString>
#include <string>
#include <zip.h>

using namespace CryptoPP;
using namespace std;

QString khoaCongKhai;
QString khoaBiMat;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    AutoSeededRandomPool prng;
    std::string privateKeyHex;
    std::string publicKeyHex;
    FileSource("/home/thanthien/Coding/ATBM/26-06/rsa-atbm/privateKey.txt", true, new StringSink(privateKeyHex));
    FileSource("/home/thanthien/Coding/ATBM/26-06/rsa-atbm/publicKey.txt", true, new StringSink(publicKeyHex));
    /*
    RSA::PrivateKey privateKey;
    ByteQueue queue; // Tạo ByteQueue để chứa dữ liệu khóa
    StringSource ss(privateKeyHex, true, new HexDecoder(new Redirector(queue))); // Giải mã Hex và đưa vào queue
    privateKey.Load(queue); // Tải khóa riêng từ queue
    */
    ui->textBrowser_khoaBiMat->setText(QString::fromStdString(privateKeyHex));
    ui->textBrowser_khoaCongKhai->setText(QString::fromStdString(publicKeyHex));
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pushButton_taoKhoa_clicked()
{
    Dialogt dialog(this);
    dialog.setModal(true);
    dialog.exec();
}


void MainWindow::on_pushButton_kyVanBan_clicked()
{
    KyVanBan kyvanban(this);
    kyvanban.setModal(true);
    kyvanban.exec();
}


void MainWindow::on_pushButton_xacNhanChuKy_clicked()
{
    xacnhanvanban xnvanban(this);
    xnvanban.setModal(true);
    xnvanban.exec();
}

void MainWindow::on_pushButton_thoat_clicked()
{
    this->close();
}

