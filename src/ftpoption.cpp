#include "ftpoption.h"
#include "ui_ftpoption.h"

#include <QFileDialog>
#include <QStandardPaths>
#include <QDir>


FtpOption::FtpOption(QSettings *cfg, QString apppath, QWidget *parent)
    : QWidget(parent), m_cfg(cfg), m_apppath(apppath)
    , ui(new Ui::FtpOption)
{
    tmppath = QStandardPaths::writableLocation(QStandardPaths::TempLocation)+
              QDir::separator();
    ui->setupUi(this);
    connect(ui->pbSelLocalPath, &QPushButton::clicked, this, &FtpOption::onSelectLocalPath);
    connect(ui->pbAddLocal, &QPushButton::clicked, this, &FtpOption::onAddLocal);
    connect(ui->pbAddRemote, &QPushButton::clicked, this, &FtpOption::onAddRemote);
}

FtpOption::~FtpOption()
{
    delete ui;
}

void FtpOption::loadcfg()
{
    m_cfg->beginGroup("main");
    ui->ftpServer->setText(m_cfg->value("ftpServer", "192.168.70.21").toString());
    ui->ftpPort->setValue(m_cfg->value("ftpPort", 21).toInt());
    ui->ftpUsername->setText(m_cfg->value("ftpUsername", "1").toString());
    ui->ftpPassword->setText(m_cfg->value("ftpPassword", "1").toString());
    QString lfile;
#if defined(Q_OS_LINUX)
    lfile = tmppath+"500M";
#else
    lfile = "500M";
#endif
    ui->ftpLocalfile->setText(m_cfg->value("ftpLocalfile", lfile).toString());
    ui->ftpRemotefile->setText(m_cfg->value("ftpRemotefile", "500M").toString());
    QString localpath = tmppath+"tmp";
    ui->ftpLocalPath->setText(m_cfg->value("ftpLocalPath", QDir::toNativeSeparators(localpath)).toString());
    ui->ftpRemotePath->setText(m_cfg->value("ftpRemotePath", "tmp").toString());
    ui->sbNum->setValue(m_cfg->value("num", 1).toInt());
    ui->sbLoop->setValue(m_cfg->value("loop", 0).toInt());
    m_cfg->endGroup();
}

void FtpOption::savecfg()
{
    m_cfg->beginGroup("main");
    m_cfg->setValue("ftpServer", ui->ftpServer->text());
    m_cfg->setValue("ftpPort", ui->ftpPort->value());
    m_cfg->setValue("ftpUsername", ui->ftpUsername->text());
    m_cfg->setValue("ftpPassword", ui->ftpPassword->text());
    m_cfg->setValue("ftpLocalfile", ui->ftpLocalfile->text());
    m_cfg->setValue("ftpRemotefile", ui->ftpRemotefile->text());
    m_cfg->setValue("ftpLocalPath", ui->ftpLocalPath->text());
    m_cfg->setValue("ftpRemotePath", ui->ftpRemotePath->text());
    m_cfg->setValue("num", ui->sbNum->value());
    m_cfg->setValue("loop", ui->sbLoop->value());
    m_cfg->endGroup();
    m_cfg->sync();
}

void FtpOption::onAdd(int id)
{
    FtpClient::FtpMode mode;
    if (ui->ftpMode->currentText().contains("Download")){
        mode = FtpClient::FtpMode::Download;
    }else{
        mode = FtpClient::FtpMode::Upload;
    }
    int num = ui->sbNum->value();
    int iloop = ui->sbLoop->value();
    // QDir::absoluteFilePath();
    // int id = m_ftpmanager->count();
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
        localfile = QDir::toNativeSeparators(localfile);
        if (mode == FtpClient::FtpMode::Upload){
            remotefile = QString("ftp://%1/%2/%3%4").arg(server, remotepath, org_localfile, "_"+QString::number(i+id));
        }else{
            remotefile = QString("ftp://%1/%2").arg(server, org_remotefile);
        }
        // m_ftpmanager->addFtpClient(i+id, server , serverport,
        //                            username, password,
        //                            localfile, remotefile, mode, iloop);
        emit addFtpClient(i+id, server , serverport,
                          username, password,
                          localfile, remotefile, mode, iloop);
        QVector<QVariant> data;
        data << QVariant(localfile);
        data << QVariant(mode);
        data << QVariant(remotefile);
        data << QVariant(""); //progress
        data << QVariant(""); //percentage
        data << QVariant(""); //TP
        data << QVariant(""); //comment
        // m_ftpmodel->addData(data);
        emit addFtpModel(data);
    }
}

void FtpOption::changeEvent(QEvent *e)
{
    QWidget::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}
void FtpOption::onSelectLocalPath(bool checked)
{
    Q_UNUSED(checked)
    QString folder = QFileDialog::getExistingDirectory(this, "Select Folder for save the download file");
    if (!folder.isEmpty()) {
        ui->ftpLocalPath->setText(QDir::toNativeSeparators(folder));
    }
}

void FtpOption::onAddLocal(bool checked)
{
    Q_UNUSED(checked)
    if (oldfilepath.isEmpty()){
        oldfilepath = m_apppath;
    }
    QString filename = QFileDialog::getOpenFileName(this, "Select a file for upload", oldfilepath);
    if (!filename.isEmpty()){
        ui->ftpLocalfile->setText(filename);
        QFileInfo f(filename);
        oldfilepath = f.absolutePath();
    }
}

void FtpOption::onAddRemote(bool checked)
{
    Q_UNUSED(checked)
    QUrl url(QString("ftp://%1/").arg(ui->ftpServer->text()));
    QString username = ui->ftpUsername->text();
    QString password = ui->ftpPassword->text();
    int port = ui->ftpPort->value();
    m_remotelist= new RemoteList(url, username, password, port, this);
    int rc = m_remotelist->exec();
    if (rc == QDialog::Accepted){
        QString rfilename = m_remotelist->getRemoteFileName();
        if (!rfilename.isEmpty()){
            ui->ftpRemotefile->setText(rfilename);
        }
    }

}
