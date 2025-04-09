#ifndef COMM_H
#define COMM_H

#define APP_DOMAIN "alphanetworks.com"
#define APP_ORG "alphanetworks"
#define APP_NAME "multiftp"

#include <QString>
#include <QDateTime>

QString getDateTimeNow(qint64 sec=0)
{
    QDateTime t = QDateTime::currentDateTime();
    if (sec){
        t= t.addSecs(sec);
    }
    return t.toString("yyyy-MM-dd hh:mm:ss.zzz");
}

#endif // COMM_H
