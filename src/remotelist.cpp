#include "remotelist.h"
#include "comm.h"

#include <QDebug>

RemoteList::RemoteList(QUrl url, QString username, QString passwd, int port,
                       QWidget *parent)
    : QWidget{parent},
    m_url(url), m_username(username), m_password(passwd) , m_port(port)
{
    transfer = CurlEasy(this);
    connect(transfer, &CurlEasy::done, this, &RemoteList::onTransferDone);
}

void RemoteList::list(QUrl url, QString username, QString passwd, int port)
{
    if (!url.isEmpty()){
        m_url = url;
    }
    if (!username.isEmpty()){
        m_username = username;
    }
    if (!passwd.isEmpty()){
        m_password = passwd;
    }
    if (port){
        m_port = port;
    }
    transfer->set(CURLOPT_URL, m_url);
    transfer->set(CURLOPT_PORT, m_port);
    transfer->set(CURLOPT_USERNAME, m_username);
    transfer->set(CURLOPT_PASSWORD, m_password);
    transfer->set(CURLOPT_DIRLISTONLY, 1L);
    transfer->perform();
}

void RemoteList::onTransferDone()
{
    if (transfer->result() != CURLE_OK) {
        QString msg = QString(" Transfer failed with curl error '%1'")
                          .arg(curl_easy_strerror(transfer->result()));
        emit errormsg( getDateTimeNow() + msg);
    }else{
        qDebug() << "perform list down";
    }
}
