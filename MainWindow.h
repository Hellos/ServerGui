#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <ResourcesServer.h>

#include <QMainWindow>
#include <QTextEdit>

QT_BEGIN_NAMESPACE
namespace Ui
{
    class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

public:
    static QTextEdit* log;

private slots:
    void on_switchPortButton_clicked(bool checked);
    void onUsersUpdated();
    void onResourcesUpdated();
    void updateUptime();
    void updateSessionTimes();
    void on_rejectAuthButton_clicked(bool checked);
    void on_rejectRcButton_clicked(bool checked);
    void on_clearResourcesButton_clicked();
    void on_reloadSettings_clicked();

    void on_acceptAccessTime_clicked();

private:
    Ui::MainWindow*  ui;
    ResourcesServer* _rs;
    QTime            _startTime;
    QTimer*          _uptimeTimer;
    QTimer*          _sessionTimer;
};
#endif // MAINWINDOW_H
