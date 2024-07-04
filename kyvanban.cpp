#include "kyvanban.h"
#include "ui_kyvanban.h"
#include <cryptopp/cryptlib.h>
#include <cryptopp/sha.h>        // Cho SHA256
#include <cryptopp/hex.h>        // Cho HexEncoder
#include <cryptopp/filters.h>    // Cho HashFilter
#include <cryptopp/osrng.h>
#include <cryptopp/files.h>
#include <cryptopp/rsa.h>
#include <cryptopp/config.h>
#include <cryptopp/pssr.h>
#include <zip.h>
//#include <tinyxml2.h>
#include <tinyxml2.h>
#include <iostream>
#include <vector>
//-----------------------------------------
#include <fstream>
#include <string>
#include <QFileDialog>
#include <QDebug>
#include <QMessageBox>
#include <QTextStream>
#include <QXmlStreamReader>

using namespace CryptoPP;
using namespace std;

//Khai báo hàm băm
std::string output;
int flatInput = 0;

KyVanBan::KyVanBan(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::KyVanBan)
{
    ui->setupUi(this);
}

KyVanBan::~KyVanBan()
{
    delete ui;
}


std::string SignMessage(const std::string& message, const RSA::PrivateKey& privateKey) {
    AutoSeededRandomPool rng;
    RSASS<PKCS1v15, SHA256>::Signer signer(privateKey);

    std::string signature;
    StringSource(message, true,
        new SignerFilter(rng, signer,
            new StringSink(signature)
        ) // SignerFilter
    ); // StringSource

    return signature;
}


void KyVanBan::on_pushButton_thoat_clicked()
{
    this->close();
}


void KyVanBan::on_pushButton_taiVanBanTXT_clicked()
{
    std::string content = "";
    QString fileName = QFileDialog::getOpenFileName(nullptr, "Chọn tệp", QDir::homePath());
    //std::string stdString = fileName.toStdString();
    //cout << stdString << "\n";
    if (!fileName.isEmpty()) { // Kiểm tra xem có file được chọn không
        std::ifstream file(fileName.toStdString());
        if (file.is_open()) {

            std::string line;
            while (std::getline(file, line)) {
                content += line + "\n"; // Đọc từng dòng và nối vào content
            }
            file.close();

            // Hiển thị nội dung lên PlainTextEdit (giả sử bạn có QPlainTextEdit tên là plainTextEdit)
            ui->textBrowser_vanBanCanKy->setPlainText(QString::fromStdString(content));

        } else {
            QMessageBox::critical(this, "Lỗi", "Không thể mở tệp tin!");
        }
    } else{
        QMessageBox::critical(this, "Lỗi", "Tệp tin không được chọn!");
    }
    if (content != ""){
        CryptoPP::SHA256 hash;
        unsigned char digest[CryptoPP::SHA256::DIGESTSIZE];
        hash.CalculateDigest(digest, (const unsigned char*)content.c_str(), content.length());
        CryptoPP::HexEncoder encoder;
        encoder.Attach(new CryptoPP::StringSink(output));
        encoder.Put(digest, sizeof(digest));
        encoder.MessageEnd();
        ui->textBrowser_hamBam->setPlainText(QString::fromStdString(output));
    }
    flatInput = 1;
}


void KyVanBan::on_pushButton_taiVanBanDOCX_clicked()
{
    std::string content = "";
    QString docxFilePath = QFileDialog::getOpenFileName(nullptr, "Chọn tệp", QDir::homePath());
    int err = 0;
    zip* archive = zip_open(docxFilePath.toStdString().c_str(), 0, &err);
    if (err != 0) {
        QMessageBox::critical(this, "Lỗi", "Lỗi mở file .docx!");
    }
    // Tìm và mở file document.xml trong zip
    zip_file* xmlFile = zip_fopen(archive, "word/document.xml", 0);
    if (!xmlFile) {
        QMessageBox::critical(this, "Lỗi", "Không tìm thấy document.xml!");
        zip_close(archive);
    }
    // Đọc nội dung document.xml
    std::string xmlContent;
    char buffer[1024];
    int bytesRead;
    while ((bytesRead = zip_fread(xmlFile, buffer, sizeof(buffer))) > 0) {
        xmlContent.append(buffer, bytesRead);
    }

    zip_fclose(xmlFile);

    content = xmlContent;
    if (content != ""){
        CryptoPP::SHA256 hash;
        unsigned char digest[CryptoPP::SHA256::DIGESTSIZE];
        hash.CalculateDigest(digest, (const unsigned char*)content.c_str(), content.length());
        CryptoPP::HexEncoder encoder;
        encoder.Attach(new CryptoPP::StringSink(output));
        encoder.Put(digest, sizeof(digest));
        encoder.MessageEnd();
        ui->textBrowser_hamBam->setPlainText(QString::fromStdString(output));
    }
    flatInput = 1;

    ui->textBrowser_vanBanCanKy->setText(QString::fromStdString(xmlContent));
}

void KyVanBan::on_pushButton_luuChuKy_clicked()
{
    if (flatInput == 0){
        std::string content;
        QString text = ui->textBrowser_vanBanCanKy->toPlainText();
        content = text.toStdString();

        if (content != ""){
            CryptoPP::SHA256 hash;
            unsigned char digest[CryptoPP::SHA256::DIGESTSIZE];
            hash.CalculateDigest(digest, (const unsigned char*)content.c_str(), content.length());
            CryptoPP::HexEncoder encoder;
            encoder.Attach(new CryptoPP::StringSink(output));
            encoder.Put(digest, sizeof(digest));
            encoder.MessageEnd();
            ui->textBrowser_hamBam->setPlainText(QString::fromStdString(output));
        }
    }
    AutoSeededRandomPool prng;
    std::string privateKeyHex;
    FileSource("/home/thanthien/Coding/ATBM/26-06/rsa-atbm/privateKey.txt", true, new StringSink(privateKeyHex));
    RSA::PrivateKey privateKey;
    ByteQueue queue; // Tạo ByteQueue để chứa dữ liệu khóa
    StringSource ss(privateKeyHex, true, new HexDecoder(new Redirector(queue))); // Giải mã Hex và đưa vào queue
    privateKey.Load(queue); // Tải khóa riêng từ queue
    std::string signature = SignMessage(output, privateKey);
    std::string signatureHex;
    StringSink ss3(signatureHex);
    HexEncoder encoder(new Redirector(ss3));
    encoder.Put((const CryptoPP::byte*)signature.data(), signature.size());
    encoder.MessageEnd();
    ui->textBrowser_chuKy->setText(QString::fromStdString(signatureHex));

    std::string filePathPrivate = "/home/thanthien/Coding/ATBM/26-06/rsa-atbm/chuKy.txt";
    std::string filePathPublic = "/home/thanthien/Coding/ATBM/26-06/rsa-atbm/publicKey.txt";

    std::ofstream outFile(filePathPrivate, std::ios::out | std::ios::trunc);
    if (outFile.is_open()) {
        outFile << signatureHex;
        QMessageBox::information(this, "Lưu chữ ký thành công", "Chữ ký được lưu vào file chuKy.txt!");
        outFile.close();
    }
}

