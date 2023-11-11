#include "passwordmanager.h"
#include <QtWidgets/QApplication>
#include "lang.h"

int main(int argc, char *argv[])
{
    // 默认中文
    lang_util::set_lang(ZH);


    QApplication a(argc, argv);
    passwordmanager w;

    w.setWindowTitle(lang_util::get_by_code(TITLE));
    w.show();
    return a.exec();
}
