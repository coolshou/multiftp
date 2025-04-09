#ifndef REMOTELIST_H
#define REMOTELIST_H

#include <QWidget>
#include <QUrl>
#include "CurlEasy.h"

class RemoteList : public QWidget
{
    Q_OBJECT
public:
    explicit RemoteList(QUrl url, QString username, QString passwd, int port=21,
                        QWidget *parent = nullptr);
    void list(QUrl url=QUrl(""), QString username="", QString passwd="", int port=21);
signals:
    void errormsg(QString msg);
private slots:
    void onTransferDone();

private:
    QUrl m_url;
    QString m_username;
    QString m_password;
    int m_port;
    CurlEasy *transfer;
};

#endif // REMOTELIST_H
