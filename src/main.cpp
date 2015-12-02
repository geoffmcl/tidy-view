#include <QtGui/QApplication>
#include <QSettings>

#include "mainwindow.h"

// check if file exists and if yes: Is it really a file and not a directory?
bool m_fileExists(QString path) 
{
    QFileInfo checkFile(path);
    if (path.size() && checkFile.exists() && checkFile.isFile()) {
        return true;
    }
    return false;
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QApplication::setOrganizationDomain(APP_SETD);
    // store the UNIX file as $HOME/.config/geoffair/Qt_OSM_Map.conf - nav: . hconf; cd geoffair
    QApplication::setOrganizationName(APP_SETO);
    QApplication::setApplicationName(APP_SETN);
    QApplication::setApplicationVersion(APP_SETV);

    QSettings startsettings;
    MainWindow w;
    QStringList args = a.arguments();
    for (int i = 1; i < args.size(); ++i) {
        QString s = args.at(i);
        if (s.at(0) == QChar('-')) {
            printf("Only argument is an input file name!\n");
            return 1;
        } else if (!m_fileExists(s)) {
            printf("Unable to find file '%s'!\n", s.toStdString().c_str());
            return 1;
        }
        w.openFile(s);
    }
    w.show();
    
    return a.exec();
}
