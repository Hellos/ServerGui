#include "MainWindow.h"

#include "ui_MainWindow.h"

QTextEdit* MainWindow::log = 0;

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    _rs = new ResourcesServer(this);

    ui->portSpinBox->setValue(_rs->port());
    ui->accessTimeSpinBox->setValue(_rs->zTime() / 1000);
    ui->userCount->setText(QString("%1/%2").arg(_rs->clients().size()).arg(_rs->clientLimit()));
    connect(_rs, &ResourcesServer::usersUpdated, this, &MainWindow::onUsersUpdated);
    connect(_rs, &ResourcesServer::resourcesUpdated, this, &MainWindow::onResourcesUpdated);

    ui->settingsPathLabel->setText(QCoreApplication::applicationDirPath() + "/settings.ini");

    log = ui->logBox;

    _sessionTimer = new QTimer(this);
    connect(_sessionTimer, &QTimer::timeout, this, &MainWindow::updateSessionTimes);
    _sessionTimer->start(1000);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_switchPortButton_clicked(bool checked)
{
    if (!_rs->isRunning())
    {
        _rs->setPort(ui->portSpinBox->value());
        _rs->run();
        ui->switchPortButton->setText("Закрыть порт");
        ui->switchPortButton->setChecked(true);

        _startTime = QTime::currentTime();

        _uptimeTimer = new QTimer(this);
        connect(_uptimeTimer, &QTimer::timeout, this, &MainWindow::updateUptime);
        _uptimeTimer->start(1000);
    }
    else
    {
        _rs->stop();
        ui->switchPortButton->setText("Открыть порт");
        ui->switchPortButton->setChecked(false);
        delete _uptimeTimer;
    }
}

void MainWindow::onUsersUpdated()
{
    ui->connectedClientsList->clear();
    ui->connectedClientsList->addItems(_rs->clients().values());
    ui->userCount->setText(QString("%1/%2").arg(_rs->clients().size()).arg(_rs->clientLimit()));
}

void MainWindow::onResourcesUpdated()
{
    QMap<int, QString> resources = _rs->resources();

    ui->resource1->setText(resources[0].isEmpty() ? "Ресурс свободен" : resources[0]);
    ui->resource2->setText(resources[1].isEmpty() ? "Ресурс свободен" : resources[1]);
    ui->resource3->setText(resources[2].isEmpty() ? "Ресурс свободен" : resources[2]);
    ui->resource4->setText(resources[3].isEmpty() ? "Ресурс свободен" : resources[3]);
}

void MainWindow::updateUptime()
{
    QTime currentTime    = QTime::currentTime();
    int   elapsedSeconds = _startTime.secsTo(currentTime);

    int hours   = elapsedSeconds / 3600;
    int minutes = (elapsedSeconds % 3600) / 60;
    int seconds = elapsedSeconds % 60;

    ui->uptime->setText(QString("%1:%2:%3")
                            .arg(hours, 2, 10, QChar('0'))
                            .arg(minutes, 2, 10, QChar('0'))
                            .arg(seconds, 2, 10, QChar('0')));
}

void MainWindow::updateSessionTimes()
{
    ui->connectedClientsList->clear();
    QMap<QTcpSocket*, QString> clients       = _rs->clients();
    QMap<QString, QTime>       resourceTimes = _rs->resourceTimes();
    QMap<int, QString>         resources     = _rs->resources();
    QTime                      currentTime   = QTime::currentTime();

    for (auto it = clients.begin(); it != clients.end(); ++it)
    {
        QString username = it.value();
        if (resourceTimes.contains(username) && resources.values().contains(username))
        {
            int elapsedSeconds = resourceTimes[username].secsTo(currentTime);
            int hours          = elapsedSeconds / 3600;
            int minutes        = (elapsedSeconds % 3600) / 60;
            int seconds        = elapsedSeconds % 60;

            QString sessionTime = QString("%1:%2:%3")
                                      .arg(hours, 2, 10, QChar('0'))
                                      .arg(minutes, 2, 10, QChar('0'))
                                      .arg(seconds, 2, 10, QChar('0'));

            QString resourcesTaken = "";
            for (int i = 0; i < ResourcesServer::RESOURCE_QUANTITY; i++)
            {
                if (resources[i] == username)
                {
                    resourcesTaken.append(QString("#%1 ").arg(i + 1));
                }
            }

            ui->connectedClientsList->addItem(QString("%1 %2 %3").arg(username).arg(resourcesTaken).arg(sessionTime));
        }
        else
        {
            ui->connectedClientsList->addItem(username);
        }
    }
    ui->userCount->setText(QString("%1/%2").arg(clients.size()).arg(_rs->clientLimit()));
}

void MainWindow::on_rejectAuthButton_clicked(bool checked)
{
    _rs->rejectAuthRequests(checked);
}

void MainWindow::on_rejectRcButton_clicked(bool checked)
{
    _rs->rejectRcRequests(checked);
}

void MainWindow::on_clearResourcesButton_clicked()
{
    _rs->freeResources();
}

void MainWindow::on_reloadSettings_clicked()
{
    _rs->loadSettings();
    ui->portSpinBox->setValue(_rs->port());
    ui->accessTimeSpinBox->setValue(_rs->zTime());
    ui->userCount->setText(QString("%1/%2").arg(_rs->clients().size()).arg(_rs->clientLimit()));
}

void MainWindow::on_acceptAccessTime_clicked()
{
    _rs->setZTime(ui->accessTimeSpinBox->value() * 1000);
}
