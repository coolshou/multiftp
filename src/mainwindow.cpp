#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "ftpclient.h"

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
    m_ftpmanager->addFtpClient(ui->ftpServer->text(), ui->ftpPort->value(),
                               ui->ftpUsername->text(),ui->ftpPassword->text(),
                               ui->ftpLocalfile->text(), ui->ftpRemotefile->text(),
                               mode);
    QVector<QVariant> data;
    data << QVariant(ui->ftpLocalfile->text());
    data << QVariant(mode);
    data << QVariant(ui->ftpRemotefile->text());
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
