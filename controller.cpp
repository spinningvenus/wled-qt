#include "controller.h"
#include "networkmanager.h"
#include "mainwindow.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QString>

Controller::Controller(QObject *parent) : QObject(parent) {
    // Spawn a network controller and link it to this class
    network = new NetworkManager(this);
}

QJsonDocument buildResponseJSON(const QString &url, const QByteArray &response) {
    QString type;
    bool sync;
    if (url.contains("presets.json")) {
        type = "presets";
        sync = true;
    }
    else if (url.contains("/json/state")) {
        type = "state";
        QString responseString = QString(response);
        if (responseString.contains("success")) {
            sync = false;
        } else if (responseString.contains("error")) {
            sync = false;
        } else {
            sync = true;
        }
    }
    else {
        type = "unknown";
        sync = false;
    }

    QJsonObject json;
    json["response"] = QJsonDocument::fromJson(response).object();
    json["type"] = type;
    json["sync"] = sync;

    QJsonDocument doc(json);
    return doc;
}

void Controller::processRequest(const QString &url, const QJsonDocument json) {
    // Uses callback instead of signal/slots for efficiency as these two classes are already hard coupled
    network->sendPostRequest(url, json);
}

void Controller::responseReceived(const QString &url, const QByteArray &response) {
    emit updateUI(buildResponseJSON(url, response));
}