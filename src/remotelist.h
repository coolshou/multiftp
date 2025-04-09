#ifndef REMOTELIST_H
#define REMOTELIST_H

#include <QDialog>
#include <QList>

#include "CurlEasy.h"

namespace Ui {
class RemoteList;
}

class RemoteList : public QDialog
{
    Q_OBJECT

public:
    enum cols{
        TPY,
        NAME
    };
    Q_ENUM(cols)

    explicit RemoteList(QUrl url, QString username, QString passwd, int port=21, QWidget *parent = nullptr);
    ~RemoteList();
    void list(QUrl url=QUrl(""), QString username="", QString passwd="", int port=21);
    static size_t writecallback(char *data, size_t size);
    void clear();
    QString getRemoteFileName();
    // TODO: step in/out folders
signals:
    void errormsg(QString msg);
private slots:
    void onTransferDone();
    void onTransferAborted();
    void onErrormsg(QString msg);

protected:
    void changeEvent(QEvent *e);

private:
    Ui::RemoteList *ui;
    QUrl m_url;
    QString m_username;
    QString m_password;
    int m_port;
    CurlEasy *transfer;
    static QList<QList<QString>> m_dirdatas;
    static QList<QList<QString>> m_datas;

};


#endif // REMOTELIST_H
