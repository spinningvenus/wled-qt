#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "networkmanager.h"

#include <QObject>
#include <QJsonDocument>

class Controller : public QObject {
    Q_OBJECT

public:
    explicit Controller(QObject *parent = nullptr);

private:
    NetworkManager *network;

public slots:
    void processRequest(const QString &url, const QJsonDocument json);
    void responseReceived(const QString &url, const QByteArray &response);
    
signals:
    void updateUI(const QJsonDocument &doc);
};

#endif