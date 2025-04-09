#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDir>
#include <QFile>
#include <QByteArray>
#include <QMessageBox>
#include <QFileDialog>
#include <QStandardPaths>

#include "ftpclient.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    tmppath = QStandardPaths::writableLocation(QStandardPaths::TempLocation)+
              QDir::separator();
    oldfilepath = "";
    cfg = new QSettings(QSettings::IniFormat, QSettings::UserScope,
                        APP_ORG, APP_NAME);
    ui->setupUi(this);
    loadcfg();
    ui->actionSave->setVisible(false);
    connect(ui->actionAbout, &QAction::triggered, this, &MainWindow::onShowAbout);
    connect(ui->actionQuit, &QAction::triggered, this, &MainWindow::onQuit);
    connect(ui->pbAdd, &QPushButton::clicked, this, &MainWindow::onAdd);
    connect(ui->pbClear, &QPushButton::clicked, this, &MainWindow::onClear);
    connect(ui->pbSelLocalPath, &QPushButton::clicked, this, &MainWindow::onSelectLocalPath);
    connect(ui->pbAddLocal, &QPushButton::clicked, this, &MainWindow::onAddLocal);
    connect(ui->pbAddRemote, &QPushButton::clicked, this, &MainWindow::onAddRemote);
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
    savecfg();
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

void MainWindow::loadcfg()
{
    cfg->beginGroup("main");
    ui->ftpServer->setText(cfg->value("ftpServer", "192.168.70.21").toString());
    ui->ftpPort->setValue(cfg->value("ftpPort", 21).toInt());
    ui->ftpUsername->setText(cfg->value("ftpUsername", "1").toString());
    ui->ftpPassword->setText(cfg->value("ftpPassword", "1").toString());
    ui->ftpLocalfile->setText(cfg->value("ftpLocalfile", "500M").toString());
    ui->ftpRemotefile->setText(cfg->value("ftpRemotefile", "500M").toString());
    QString localpath = tmppath+QDir::separator()+"tmp";
    ui->ftpLocalPath->setText(cfg->value("ftpLocalPath", localpath).toString());
    ui->ftpRemotePath->setText(cfg->value("ftpRemotePath", "tmp").toString());
    ui->sbNum->setValue(cfg->value("num", 1).toInt());
    ui->sbLoop->setValue(cfg->value("loop", 0).toInt());
    cfg->endGroup();
}

void MainWindow::savecfg()
{
    cfg->beginGroup("main");
    cfg->setValue("ftpServer", ui->ftpServer->text());
    cfg->setValue("ftpPort", ui->ftpPort->value());
    cfg->setValue("ftpUsername", ui->ftpUsername->text());
    cfg->setValue("ftpPassword", ui->ftpPassword->text());
    cfg->setValue("ftpLocalfile", ui->ftpLocalfile->text());
    cfg->setValue("ftpRemotefile", ui->ftpRemotefile->text());
    cfg->setValue("ftpLocalPath", ui->ftpLocalPath->text());
    cfg->setValue("ftpRemotePath", ui->ftpRemotePath->text());
    cfg->setValue("num", ui->sbNum->value());
    cfg->setValue("loop", ui->sbLoop->value());
    cfg->endGroup();
    cfg->sync();
}

void MainWindow::onShowAbout(bool checked)
{
    Q_UNUSED(checked)
    //TODO: show about
    QString msg = QString("%1\n use curl 8.13 do ftp upload/download").arg(APP_NAME);
    QMessageBox::information(this, tr("About"), msg, QMessageBox::Ok);
}

void MainWindow::onQuit(bool checked)
{
    Q_UNUSED(checked)
    qApp->quit();
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
    QString org_localfilepath = ui->ftpLocalfile->text();
    QString org_localpath = "";
    QString tmp_localpath = ui->ftpLocalPath->text();
    QFileInfo f(org_localfilepath);
    QString org_localfile = f.fileName();
    if (!QDir::isAbsolutePath(tmp_localpath)){
        org_localpath = qApp->applicationDirPath()+QDir::separator()+tmp_localpath + QDir::separator() ;
    }else{
        if (mode == FtpClient::FtpMode::Download){
            org_localpath = tmp_localpath + QDir::separator();
        }else{
            org_localpath = qApp->applicationDirPath() + QDir::separator();
        }
    }
    QString org_remotefile = ui->ftpRemotefile->text();
    QString remotepath = ui->ftpRemotePath->text();
    for(int i=0;i<num;i++){
        QString localfile="";
        QString remotefile="";

        if (mode == FtpClient::FtpMode::Download){
            localfile = org_localpath + org_remotefile +"_"+QString::number(i+id);
        }else{
            // upload local file
            if(QDir::isAbsolutePath(f.path())){
                localfile = org_localfilepath;
            }else{
                localfile = org_localpath + org_localfile;
            }
        }
        if (mode == FtpClient::FtpMode::Upload){
            remotefile = QString("ftp://%1/%2/%3%4").arg(server, remotepath, org_localfile, "_"+QString::number(i+id));
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

void MainWindow::onSelectLocalPath(bool checked)
{
    Q_UNUSED(checked)
    QString folder = QFileDialog::getExistingDirectory(this, "Select Folder for save the download file");
    if (!folder.isEmpty()) {
        ui->ftpLocalPath->setText(folder);
    }
}

void MainWindow::onAddLocal(bool checked)
{
    Q_UNUSED(checked)
    if (oldfilepath.isEmpty()){
        oldfilepath = apppath;
    }
    QString filename = QFileDialog::getOpenFileName(this, "Select a file for upload", oldfilepath);
    if (!filename.isEmpty()){
        ui->ftpLocalfile->setText(filename);
        QFileInfo f(filename);
        oldfilepath = f.absolutePath();
    }
}

void MainWindow::onAddRemote(bool checked)
{
    Q_UNUSED(checked)
    QUrl url(QString("ftp://%1/").arg(ui->ftpServer->text()));
    QString username = ui->ftpUsername->text();
    QString password = ui->ftpPassword->text();
    int port = ui->ftpPort->value();
    m_remotelist= new RemoteList(url, username, password, port, this);
    m_remotelist->show();
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
