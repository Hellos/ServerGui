#include <QApplication>

#include "MainWindow.h"

void logOutput(QtMsgType type, const QMessageLogContext& context, const QString& msg);

int main(int argc, char* argv[])
{
    qInstallMessageHandler(logOutput);
    QApplication a(argc, argv);
    MainWindow   w;
    w.show();
    return a.exec();
}

void logOutput(QtMsgType type, const QMessageLogContext& context, const QString& msg)
{
    switch (type)
    {
        case QtDebugMsg:
            MainWindow::log->append("qDebug " + QTime::currentTime().toString("[hh:mm:ss.zzz] ") + msg);
            break;
        case QtWarningMsg:
            break;
        case QtInfoMsg:
            MainWindow::log->append(QTime::currentTime().toString("[hh:mm:ss.zzz] ") + msg);
            break;
        case QtCriticalMsg:
            break;
        case QtFatalMsg:
            abort();
            break;
    }
}
