#ifndef NETWORKMANAGER_H
#define NETWORKMANAGER_H

#include <QObject>
#include <QJsonDocument>
#include <QNetworkAccessManager>

class NetworkManager : public QObject {
    Q_OBJECT

public:
    explicit NetworkManager(QObject *parent = nullptr);
    void sendPostRequest(const QString &url, const QJsonDocument &json);

private:
    QNetworkAccessManager *manager;
};

#endif