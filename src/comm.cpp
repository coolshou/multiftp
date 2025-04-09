#include "comm.h"

QString getDateTimeNow(qint64 sec)
{
    QDateTime t = QDateTime::currentDateTime();
    if (sec){
        t= t.addSecs(sec);
    }
    return t.toString("yyyy-MM-dd hh:mm:ss.zzz");
}
