#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDir>
#include <QFile>
#include <QByteArray>

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
    connect(m_ftpmanager, &FtpManager::progress, m_ftpmodel, &FtpModel::updateProgress);
    connect(m_ftpmanager, &FtpManager::errormsg, m_ftpmodel, &FtpModel::updateComment);
    connect(m_ftpmanager, &FtpManager::started, this, &MainWindow::onStarted);
    connect(m_ftpmanager, &FtpManager::stoped, this, &MainWindow::onStoped);
    // m_ftpmodel->
    m_ftpmodel->setHeaderData(0, Qt::Horizontal, "ID");
    ui->tableView->setModel(m_ftpmodel);
    ui->tableView->setColumnWidth(FtpModel::Col::Dir, 50);
    ui->tableView->setColumnWidth(FtpModel::Col::Percentage, 50);
    m_dirdelegate= new DirDelegate(ui->tableView);
    ui->tableView->setItemDelegateForColumn(1, m_dirdelegate);
    // ui->tableView->horizontalHeader()->show();
    apppath = qApp->applicationDirPath();
    // decompress 500M.qz
    QString srcfile = apppath+QDir::separator()+"500M.qz";
    QString destfile = apppath+QDir::separator()+"500M";
    decompressFile(srcfile, destfile);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::decompressFile(const QString &sourcePath, const QString &destinationPath)
{
    QFile sourceFile(sourcePath);
    QFile destinationFile(destinationPath);

    if (sourceFile.open(QFile::ReadOnly) && destinationFile.open(QFile::WriteOnly)) {
        QByteArray compressedData = sourceFile.readAll();
        QByteArray decompressedData = qUncompress(compressedData);
        destinationFile.write(decompressedData);
        sourceFile.close();
        destinationFile.close();
    } else {
        qDebug() << "Failed to open files." << sourcePath;
    }
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
    int iloop = ui->sbLoop->value();
    // QDir::absoluteFilePath();
    int id = m_ftpmanager->count();
    QString username = ui->ftpUsername->text();
    QString password = ui->ftpPassword->text();
    QString server = ui->ftpServer->text();
    int serverport = ui->ftpPort->value();
    QString org_localfile = ui->ftpLocalfile->text();
    QString org_localpath = "";
    QString tmp_localpath = ui->ftpLocalPath->text();
    QFileInfo f(org_localfile);
    org_localfile = f.fileName();
    if (!QDir::isAbsolutePath(org_localfile)){
        org_localpath = qApp->applicationDirPath()+QDir::separator();
    }else{
        org_localpath = f.absolutePath();
    }
    QString org_remotefile = ui->ftpRemotefile->text();
    QString remotepath = ui->ftpRemotePath->text();
    for(int i=0;i<num;i++){
        QString localfile="";
        QString remotefile="";

        if (mode == FtpClient::FtpMode::Download){
            localfile = org_localpath + tmp_localpath + QDir::separator() + org_localfile +"_"+QString::number(i+id);
        }else{
            localfile = org_localpath + org_localfile;
        }
        if (mode == FtpClient::FtpMode::Upload){
            remotefile = QString("ftp://%1/%2/%3%4").arg(server, remotepath, org_remotefile, "_"+QString::number(i+id));
        }else{
            remotefile = QString("ftp://%1/%2").arg(server, org_remotefile);
        }
        m_ftpmanager->addFtpClient(i+id, server , serverport,
                                   username, password,
                                   localfile, remotefile, mode, iloop);

        QVector<QVariant> data;
        data << QVariant(localfile);
        data << QVariant(mode);
        data << QVariant(remotefile);
        data << QVariant(""); //status
        data << QVariant(""); //percentage
        data << QVariant(""); //comment
        m_ftpmodel->addData(data);
    }
}

void MainWindow::onClear(bool checked)
{
    Q_UNUSED(checked)
    m_ftpmanager->clear();
    m_ftpmodel->clear();
}

void MainWindow::onStart(bool checked)
{
    Q_UNUSED(checked)
    //start run all ftps
    m_ftpmanager->start();

}

void MainWindow::onStop(bool checked)
{
    Q_UNUSED(checked)
    m_ftpmanager->stop();

}

void MainWindow::setRunStatus(bool start)
{
    ui->actionStart->setEnabled(!start);
    ui->actionStop->setEnabled(start);
    ui->pbAdd->setEnabled(!start);
    ui->pbClear->setEnabled(!start);
}

void MainWindow::onStarted()
{
    setRunStatus(true);
}

void MainWindow::onStoped()
{
    qDebug() << "MainWindow::onStoped";
    setRunStatus(false);
}
