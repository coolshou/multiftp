#ifndef FTPOPTION_H
#define FTPOPTION_H

#include <QWidget>
#include <QSettings>

#include "remotelist.h"
#include "ftpclient.h"

namespace Ui {
class FtpOption;
}

class FtpOption : public QWidget
{
    Q_OBJECT

public:
    explicit FtpOption(QSettings *cfg, QString apppath, QWidget *parent = nullptr);
    ~FtpOption();
public slots:
    void loadcfg();
    void savecfg();
    void onAdd(int id);
signals:
    void addFtpClient(int id, QString server, int port,
                      QString username, QString password,
                      QString localfile, QString remotefile,
                      FtpClient::FtpMode mode, int iloop);
    void addFtpModel(QVector<QVariant> data);

protected:
    void changeEvent(QEvent *e);
private slots:
    void onSelectLocalPath(bool checked);
    void onAddLocal(bool checked);
    void onAddRemote(bool checked);

private:
    QSettings *m_cfg;
    QString m_apppath;
    Ui::FtpOption *ui;
    RemoteList *m_remotelist;
    QString oldfilepath;
    QString tmppath;
};

#endif // FTPOPTION_H
