#include "networkmanager.h"
#include "controller.h"
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QUrl>
#include <QDebug>

NetworkManager::NetworkManager(QObject *parent)
    : QObject(parent)
{
    manager = new QNetworkAccessManager(this);    
}

void NetworkManager::sendPostRequest(const QString &url_string, const QJsonDocument &json = {}) {
    QUrl url(url_string);
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QNetworkReply *reply = manager->post(request, json.toJson());
    Controller *controller = qobject_cast<Controller*>(QObject::parent());

    // Wait until network calls are finished before emiting signals
    connect(reply, &QNetworkReply::finished, this, [this, reply, url_string, controller]() {
        if (reply->error() == QNetworkReply::NoError) {
            QByteArray response = reply->readAll();
            controller->responseReceived(url_string, response);
        } else {
            qDebug() << "HTTP Error:" << reply->errorString();
        }
        reply->deleteLater();
    });
}