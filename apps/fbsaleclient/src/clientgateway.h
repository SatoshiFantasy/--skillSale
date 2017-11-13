#ifndef CLIENTGATEWAY_H
#define CLIENTGATEWAY_H

#include <QObject>

class ClientGateway : public QObject
{
    Q_OBJECT
public:
    explicit ClientGateway(QObject *parent = 0);

signals:

public slots:
};

#endif // CLIENTGATEWAY_H