#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDir>
#include <QFile>
#include <QByteArray>
#include <QMessageBox>
#include <QFileDialog>
#include <QStandardPaths>

#include "ftpclient.h"
#include "versions.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    QString tmppath = QStandardPaths::writableLocation(QStandardPaths::TempLocation)+
              QDir::separator();
    apppath = qApp->applicationDirPath();
    cfg = new QSettings(QSettings::IniFormat, QSettings::UserScope,
                        APP_ORG, APP_NAME);
    ui->setupUi(this);
    m_ftpmanager = new FtpManager();
    m_ftpmodel = new FtpModel(this);
    m_ftpoption = new FtpOption(cfg, apppath);
    connect(m_ftpoption, &FtpOption::addFtpClient, m_ftpmanager, &FtpManager::onAddFtpClient);
    connect(m_ftpoption, &FtpOption::addFtpModel, m_ftpmodel, &FtpModel::addData);
    ui->vlCentral->insertWidget(0, m_ftpoption);
    loadcfg();
    ui->actionSave->setVisible(false);
    connect(ui->actionAbout, &QAction::triggered, this, &MainWindow::onShowAbout);
    connect(ui->actionQuit, &QAction::triggered, this, &MainWindow::onQuit);
    connect(ui->pbAdd, &QPushButton::clicked, this, &MainWindow::onAdd);
    connect(ui->pbClear, &QPushButton::clicked, this, &MainWindow::onClear);
    connect(ui->actionStart, &QAction::triggered, this, &MainWindow::onStart);
    connect(ui->actionStop, &QAction::triggered, this, &MainWindow::onStop);

    connect(m_ftpmanager, &FtpManager::progress, m_ftpmodel, &FtpModel::updateProgress);
    connect(m_ftpmanager, &FtpManager::throughput, m_ftpmodel, &FtpModel::updateThroughput);
    connect(m_ftpmanager, &FtpManager::errormsg, m_ftpmodel, &FtpModel::updateComment);
    connect(m_ftpmanager, &FtpManager::started, this, &MainWindow::onStarted);
    connect(m_ftpmanager, &FtpManager::stoped, this, &MainWindow::onStoped);
    // m_ftpmodel->
    m_ftpmodel->setHeaderData(0, Qt::Horizontal, "ID");
    ui->tableView->setModel(m_ftpmodel);
    ui->tableView->setColumnWidth(FtpModel::Col::Dir, 50);
    ui->tableView->setColumnWidth(FtpModel::Col::Percentage, 50);
    ui->tableView->setColumnWidth(FtpModel::Col::Throughput, 60);
    m_dirdelegate= new DirDelegate(ui->tableView);
    ui->tableView->setItemDelegateForColumn(1, m_dirdelegate);
    // ui->tableView->horizontalHeader()->show();

    // decompress 500M.qz
#if defined(Q_OS_LINUX)
// TODO: under linux the /opt/multiftp/bin/500M.qz can not be read!!
    qDebug() << "copy 500M.qz to " << tmppath << "500M.qz";
    QFile::copy(":/500M", tmppath+"500M.qz");
    decompressFile(tmppath+"500M.qz", tmppath+"500M");

#elif defined(Q_OS_WIN32)
    QString srcfile = apppath+QDir::separator()+"500M.qz";
    QString destfile = apppath+QDir::separator()+"500M";
    decompressFile(srcfile, destfile);
#endif
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
    m_ftpoption->loadcfg();
}

void MainWindow::savecfg()
{
    m_ftpoption->savecfg();
}

void MainWindow::onShowAbout(bool checked)
{
    Q_UNUSED(checked)
    //TODO: show about
    QString msg = QString("%1 %2\n use curl 8.13 do ftp upload/download").arg(APP_NAME, MULTIFTPFILE_VERSION);
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
    m_ftpoption->onAdd(m_ftpmanager->count());
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
    setRunStatus(false);
}

