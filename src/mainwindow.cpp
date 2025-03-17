#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "ftpclient.h"

#include <QDir>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->pbAdd, &QPushButton::clicked, this, &MainWindow::onAdd);
    connect(ui->actionStart, &QAction::triggered, this, &MainWindow::onStart);
    connect(ui->actionStop, &QAction::triggered, this, &MainWindow::onStop);

    m_ftpmanager = new FtpManager();
    m_ftpmodel = new FtpModel(this);
    // m_ftpmodel->
    m_ftpmodel->setHeaderData(0, Qt::Horizontal, "ID");
    ui->tableView->setModel(m_ftpmodel);
    ui->tableView->setColumnWidth(1, 50);
    // ui->tableView->horizontalHeader()->show();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onAdd(bool checked)
{
    Q_UNUSED(checked)
    FtpClient::FtpMode mode;
    if (ui->ftpMode->currentText().contains("Download")){
        mode = FtpClient::FtpMode::Download;
    }else{
        mode = FtpClient::FtpMode::Upload;
    }
    // QDir::absoluteFilePath();
    int id = m_ftpmanager->count();
    QString username = ui->ftpUsername->text();
    QString password = ui->ftpPassword->text();
    QString server = ui->ftpServer->text();
    int serverport = ui->ftpPort->value();
    QString localfile = ui->ftpLocalfile->text();
    if (!QDir::isAbsolutePath(localfile)){
        localfile = qApp->applicationDirPath()+QDir::separator()+localfile;
    }
    QString remotefile = ui->ftpRemotefile->text();

    m_ftpmanager->addFtpClient(id, server , serverport,
                               username, password,
                               localfile, remotefile, mode);
    remotefile = QString("ftp://%1:%2@%3:%4/%5").arg(username, password, server,
                                                     QString::number(serverport),
                                                     remotefile);
    QVector<QVariant> data;
    data << QVariant(localfile);
    data << QVariant(mode);
    data << QVariant(remotefile);
    data << QVariant("");
    qDebug() << "data:" << data;
    m_ftpmodel->addData(data);
}

void MainWindow::onStart(bool checked)
{
    Q_UNUSED(checked)
    //start run all ftps
    m_ftpmanager->start();
    setRunStatus(true);
}

void MainWindow::onStop(bool checked)
{
    Q_UNUSED(checked)
    m_ftpmanager->stop();
    setRunStatus(false);
}

void MainWindow::setRunStatus(bool start)
{
    ui->actionStart->setEnabled(!start);
    ui->actionStop->setEnabled(start);
}
