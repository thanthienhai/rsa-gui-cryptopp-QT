#include <zip.h>
#include <pugixml.hpp>
#include <iostream>
#include <string>

using namespace std;

int main() {
    // Mở file .docx như một file zip
    int err = 0;
    zip* z = zip_open("your_file.docx", 0, &err);
    if (err != 0) {
        cout << "Lỗi mở file .docx: " << err << std::endl;
        return 1;
    }
    // Tìm file document.xml trong zip
    zip_file* f = zip_fopen(z, "word/document.xml", 0);
    if (!f) {
        cout << "Không tìm thấy document.xml" << std::endl;
        zip_close(z);
        return 1;
    }

    // Đọc nội dung document.xml
    std::string xmlContent;
    char buf[1024];
    int len;
    while ((len = zip_fread(f, buf, sizeof(buf))) > 0) {
        xmlContent.append(buf, len);
    }

    zip_fclose(f);
    // ...
}
