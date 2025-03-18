#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDir>
#include "ftpclient.h"



MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->pbAdd, &QPushButton::clicked, this, &MainWindow::onAdd);
    connect(ui->pbClear, &QPushButton::clicked, this, &MainWindow::onClear);

    connect(ui->actionStart, &QAction::triggered, this, &MainWindow::onStart);
    connect(ui->actionStop, &QAction::triggered, this, &MainWindow::onStop);

    m_ftpmanager = new FtpManager();
    m_ftpmodel = new FtpModel(this);
    // m_ftpmodel->
    m_ftpmodel->setHeaderData(0, Qt::Horizontal, "ID");
    ui->tableView->setModel(m_ftpmodel);
    ui->tableView->setColumnWidth(1, 50);
    m_dirdelegate= new DirDelegate(ui->tableView);
    ui->tableView->setItemDelegateForColumn(1, m_dirdelegate);
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
    int num = ui->sbNum->value();
    // QDir::absoluteFilePath();
    int id = m_ftpmanager->count();
    QString username = ui->ftpUsername->text();
    QString password = ui->ftpPassword->text();
    QString server = ui->ftpServer->text();
    int serverport = ui->ftpPort->value();
    QString org_localfile = ui->ftpLocalfile->text();
    if (!QDir::isAbsolutePath(org_localfile)){
        org_localfile = qApp->applicationDirPath()+QDir::separator()+org_localfile;
    }
    QString org_remotefile = ui->ftpRemotefile->text();
    for(int i=0;i<num;i++){
        QString localfile="";
        QString remotefile="";
        localfile = org_localfile;
        if (mode == FtpClient::FtpMode::Download){
            localfile = localfile+"_"+QString::number(i);
        }

        remotefile = QString("ftp://%1:%2@%3:%4/%5").arg(username, password, server,
                                                         QString::number(serverport),
                                                         org_remotefile);
        if (mode == FtpClient::FtpMode::Upload){
            remotefile = remotefile.append("_"+QString::number(i));
        }
        m_ftpmanager->addFtpClient(id, server , serverport,
                                   username, password,
                                   localfile, remotefile, mode);
        connect(m_ftpmanager, &FtpManager::progress, m_ftpmodel, &FtpModel::updateProgress);
        connect(m_ftpmanager, &FtpManager::errormsg, m_ftpmodel, &FtpModel::updateComment);
        QVector<QVariant> data;
        data << QVariant(localfile);
        data << QVariant(mode);
        data << QVariant(remotefile);
        data << QVariant(""); //status
        data << QVariant(""); //comment
        m_ftpmodel->addData(data);
    }
}

void MainWindow::onClear(bool checked)
{
    Q_UNUSED(checked)
    qDebug() << "//TODO: clear setting";

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
