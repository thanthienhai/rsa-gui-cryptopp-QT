#include "xacnhanvanban.h"
#include "ui_xacnhanvanban.h"
//---------------------------
#include <cryptopp/sha.h>        // Cho SHA256
#include <cryptopp/hex.h>        // Cho HexEncoder
#include <cryptopp/filters.h>    // Cho HashFilter
#include <cryptopp/osrng.h>
#include <cryptopp/files.h>
#include <cryptopp/rsa.h>
#include <zip.h>
#include <QFileDialog>
#include <QDebug>
#include <QMessageBox>
#include <QTextStream>


using namespace CryptoPP;

std::string content;
std::string chuKyHexx;
std::string giaiMa;
RSA::PublicKey publicKey;
std::string outputx;

int flatInputx = 0;

xacnhanvanban::xacnhanvanban(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::xacnhanvanban)
{
    ui->setupUi(this);
}

xacnhanvanban::~xacnhanvanban()
{
    delete ui;
}

bool VerifySignature(const std::string& message, const std::string& signatureHex, const RSA::PublicKey& publicKey) {
    // Chuyển đổi chữ ký từ hex sang nhị phân
    std::string signature;
    StringSource ss(signatureHex, true, new HexDecoder(new StringSink(signature)));

    // Tạo đối tượng xác minh
    RSASS<PKCS1v15, SHA256>::Verifier verifier(publicKey);

    // Xác minh chữ ký
    //return verifier.GetLastResult();
    //return true;
    return verifier.VerifyMessage((const byte*)message.data(), message.size(), (const byte*)signature.data(), signature.size());
}

void xacnhanvanban::on_pushButton_taiVanBanTXT_clicked()
{
    std::string contentx;
    QString fileName = QFileDialog::getOpenFileName(nullptr, "Chọn tệp", QDir::homePath());
    if (!fileName.isEmpty()) { // Kiểm tra xem có file được chọn không
        std::ifstream file(fileName.toStdString());
        if (file.is_open()) {

            std::string line;
            while (std::getline(file, line)) {
                contentx += line + "\n"; // Đọc từng dòng và nối vào content
            }
            file.close();

            // Hiển thị nội dung lên PlainTextEdit (giả sử bạn có QPlainTextEdit tên là plainTextEdit)
            ui->textEdit_vanBanCanXacNhan->setPlainText(QString::fromStdString(contentx));
            flatInputx = 1;

        } else {
            QMessageBox::critical(this, "Lỗi", "Không thể mở tệp tin!");
        }
    } else{
        QMessageBox::critical(this, "Lỗi", "Tệp tin không được chọn!");
    }
    content = contentx;
}


void xacnhanvanban::on_pushButton_taiChuKy_clicked()
{
    std::string chuKyHex;
    FileSource("/home/thanthien/Coding/ATBM/26-06/rsa-atbm/chuKy.txt", true, new StringSink(chuKyHex));
    chuKyHexx = chuKyHex;
    ui->textBrowser_chuKy->setText(QString::fromStdString(chuKyHex));
}


void xacnhanvanban::on_pushButton_taiKhoaCongKhai_clicked()
{
    std::string publicKeyHex;
    FileSource("/home/thanthien/Coding/ATBM/26-06/rsa-atbm/publicKey.txt", true, new StringSink(publicKeyHex));
    ByteQueue queue; // Tạo ByteQueue để chứa dữ liệu khóa
    StringSource ss(publicKeyHex, true, new HexDecoder(new Redirector(queue))); // Giải mã Hex và đưa vào queue
    publicKey.Load(queue); // Tải khóa riêng từ queue
    ui->textBrowser_khoaCongKhai->setText(QString::fromStdString(publicKeyHex));
}


void xacnhanvanban::on_pushButton_xacNhan_clicked()
{
    if (flatInputx == 0){
        QString text = ui->textEdit_vanBanCanXacNhan->toPlainText();
        content = text.toStdString();
    }
    std::string outputxx;
    //bool result = VerifyMessage(content, chuKyHex, publicKey);
    if (content != ""){
        CryptoPP::SHA256 hash;
        unsigned char digest[CryptoPP::SHA256::DIGESTSIZE];
        hash.CalculateDigest(digest, (const unsigned char*)content.c_str(), content.length());
        CryptoPP::HexEncoder encoder;
        encoder.Attach(new CryptoPP::StringSink(outputxx));
        encoder.Put(digest, sizeof(digest));
        encoder.MessageEnd();
        ui->textBrowser_giaiMa->setPlainText(QString::fromStdString(outputxx));
    }
    outputx = outputxx;
    bool result = VerifySignature(outputx, chuKyHexx, publicKey);
    //ui->textBrowser_giaiMa->setPlainText(QString::fromStdString(recoveredText));

    if (result == true){
        QMessageBox::information(this, "Xác thực", "Văn bản được xác thực!");
    }else {
        QMessageBox::critical(this, "Cảnh báo", "Văn bản không được xác thực!");
    }
}


void xacnhanvanban::on_pushButton_thoat_clicked()
{
    this->close();
}


void xacnhanvanban::on_pushButton_taiVanBanDocx_clicked()
{
    QString docxFilePath = QFileDialog::getOpenFileName(nullptr, "Chọn tệp", QDir::homePath());
    int err = 0;
    zip* archive = zip_open(docxFilePath.toStdString().c_str(), 0, &err);
    if (err != 0) {
        QMessageBox::critical(this, "Lỗi", "Lỗi mở file .docx!");
    }else{
        flatInputx = 1;
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
    ui->textEdit_vanBanCanXacNhan->setPlainText(QString::fromStdString(xmlContent));
    zip_fclose(xmlFile);

    content = xmlContent;
}

