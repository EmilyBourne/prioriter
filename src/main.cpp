#include <QtGui>
#include "window.h"
#include <QMessageBox>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    Q_INIT_RESOURCE(resources);
    app.setWindowIcon(QIcon(":/MainIcon"));

    if (!QSystemTrayIcon::isSystemTrayAvailable()) {
        QMessageBox::critical(0, QObject::tr("Prioriter"),
                              QObject::tr("I couldn't detect any system tray "
                                          "on this system."));
        return 1;
    }
    QApplication::setQuitOnLastWindowClosed(false);

    Window fenetre;
    fenetre.show();

    if (argc>1) {
        for (int i(1); i<argc; ++i) {
            if (strcmp(argv[i],"-start_hidden")==0) {
                fenetre.setVisible(false);
            }
        }
    }

    return app.exec();
}

