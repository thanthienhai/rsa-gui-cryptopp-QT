#include "dialogt.h"
#include "ui_dialogt.h"
#include <QFileDialog>
#include <cryptopp/rsa.h>
#include <cryptopp/osrng.h>
#include <cryptopp/files.h>
#include <cryptopp/hex.h>
#include <QString>
#include <QMessageBox>
#include <string>
#include <zip.h>
#include <fstream>

using namespace CryptoPP;
using namespace std;

RSA::PrivateKey g_privateKey;
RSA::PublicKey g_publicKey;

Dialogt::Dialogt(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialogt)
{
    ui->setupUi(this);
}

Dialogt::~Dialogt()
{
    delete ui;
}


void SaveKey(const RSA::PrivateKey& key, const std::string& filename) {
    FileSink file(filename.c_str());
    key.DEREncode(file);
}

void SaveKey(const RSA::PublicKey& key, const std::string& filename) {
    FileSink file(filename.c_str());
    key.DEREncode(file);
}

void Dialogt::on_pushButton_deleteUI_clicked()
{
    this->close();
}


void Dialogt::on_pushButton_chonSoNgTo_clicked()
{
    int bitKey = ui->comboBox_bitKhoa->currentText().toInt();
    //cout << bitKey << "\n";
    AutoSeededRandomPool prng;

    // Tạo cặp khóa RSA
    InvertibleRSAFunction params;
    params.GenerateRandomWithKeySize(prng, bitKey); // Bạn có thể thay đổi kích thước khóa (ví dụ: 2048, 4096)
    // Lấy 2 số nguyên tố P và Q
    const Integer& p = params.GetPrime1();
    const Integer& q = params.GetPrime2();
    string p_string = IntToString(p);
    string q_string = IntToString(q);

    //Lấy khóa private và publix
    //const RSA::PrivateKey privateKey(params);
    //const RSA::PublicKey publicKey(params);
    g_privateKey = RSA::PrivateKey(params);
    g_publicKey = RSA::PublicKey(params);



    //Lấy các giá trị E, N khóa công khai và D, N của khóa bí mật
    const Integer& e = g_publicKey.GetPublicExponent();
    const Integer& n = g_publicKey.GetModulus();
    const Integer& d = g_privateKey.GetPrivateExponent();
    string e_string = IntToString(e);
    string n_string = IntToString(n);
    string d_string = IntToString(d);

    //Hiển thị các giá trị e, n, d
    ui->textBrowser_e->setText(QString::fromStdString(e_string));
    ui->textBrowser_n->setText(QString::fromStdString(n_string));
    ui->textBrowser_d->setText(QString::fromStdString(d_string));

    string plaintext = "Hello, world!";
    string ciphertext;
    StringSource ss1(plaintext, true,
        new PK_EncryptorFilter(prng,
            *new RSAES_OAEP_SHA_Encryptor(g_publicKey),
            new StringSink(ciphertext)
        ) // PK_EncryptorFilter
    ); // StringSource
    string encodedCiphertext;
    StringSource ss2(ciphertext, true, new HexEncoder(new StringSink(encodedCiphertext)));
    //ui->textBrowser_P->setText(QString::fromStdString(encodedCiphertext));
    ui->textBrowser_P->setText(QString::fromStdString(p_string));

    string privateKeyString;
    StringSink ssPrivateKey(privateKeyString);
    g_privateKey.Save(ssPrivateKey);
    //ui->textBrowser_Q->setText(QString::fromStdString(privateKeyString));
    ui->textBrowser_Q->setText(QString::fromStdString(q_string));

    string publicKeyString;
    StringSink ssPublicKey(publicKeyString);
    g_publicKey.Save(ssPublicKey);

}


void Dialogt::on_pushButton_cancel_clicked()
{
    std::string filePathPrivate = "/home/thanthien/Coding/ATBM/26-06/rsa-atbm/privateKey.txt";
    std::string filePathPublic = "/home/thanthien/Coding/ATBM/26-06/rsa-atbm/publicKey.txt";

    std::ofstream outFile(filePathPrivate, std::ios::out | std::ios::trunc);
    if (outFile.is_open()) {
        HexEncoder hexEncoder(new FileSink(outFile));
        g_privateKey.Save(hexEncoder);
        outFile.close();
    }
    std::ofstream outFileP(filePathPublic, std::ios::out | std::ios::trunc);
    if (outFileP.is_open()) {
        HexEncoder hexEncoder(new FileSink(outFileP));
        g_publicKey.Save(hexEncoder);
        outFileP.close();
        QMessageBox::information(this, "Thành công", "Key đã được lưu thành công!");
    }
}

