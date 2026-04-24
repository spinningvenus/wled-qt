#include "mainwindow.h"

// Widgets
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QLabel>
#include <QSlider>
#include <QComboBox>

// Libaries
#include <QDebug>
#include <QSettings>
#include <QDesktopServices>
#include <QMetaObject>
#include <QStandardPaths>
#include <QDir>

// Varible types
// Varible types
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QString>

// Declare this object globally so that it can be filled when preset data is pulled
QJsonObject dropdownItems;

// Default url scheme. Gets overided by config file.
QString urlScheme = "http://";

// Config Dir/File
const QString configDir = QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation);
//const QString configPath = "./wled-qt.conf";
const QString configPath = configDir + "/wled-qt/wled-qt.conf";
QSettings settings(configPath, QSettings::IniFormat);

MainWindow::MainWindow(QWidget *parent) : QWidget(parent) {
    setWindowTitle("WLED-Qt");

    addressInputField = new QLineEdit();
    addressInputField->setPlaceholderText("WLED IP Address");

    slider = new QSlider(Qt::Horizontal);
    slider->setRange(1, 255);
    slider->setValue(128);

    sliderLabel = new QLabel("Brightness: 128");

    // Update the value when slider moves
    connect(slider, &QSlider::valueChanged, [&](int value) {
        sliderLabel->setText(QString("Brightness: %1").arg(value));
    });

    dropdown = new QComboBox();

    lightButton = new QPushButton("Toggle Light");
    webButton = new QPushButton("Open Webpage");

    QVBoxLayout *layout = new QVBoxLayout();
    layout->addWidget(addressInputField);
    layout->addWidget(sliderLabel);
    layout->addWidget(slider);
    layout->addWidget(dropdown);
    layout->addWidget(lightButton);
    layout->addWidget(webButton);
    setLayout(layout);

    // Send out a HTTP request on button press
    connect(lightButton, &QPushButton::clicked, [&]() {
        updateConfig();
        int preset = dropdownItems[dropdown->currentText()].toInt();
        QString url_string = urlScheme + addressInputField->text() + "/json/state";
        QJsonDocument json = buildRequestJSON(slider->value(), preset);
        emit processRequest(url_string, json);
    });

    connect(webButton, &QPushButton::clicked, [&]() {
        QString url_string = urlScheme + addressInputField->text() + "/";
        QDesktopServices::openUrl(QUrl(url_string));
    });

    // Delay functions from firing utill after all signals/slots are connect to prevent dropped calls
    QMetaObject::invokeMethod(this, [this]() {
        loadConfig();
        setPresets();
    }, Qt::QueuedConnection);
}

void MainWindow::setPresets() {
    // Get presets.json
    QString url_string;
    url_string = urlScheme + addressInputField->text() + "/presets.json";
    emit processRequest(url_string, {});

    // Get current state information
    url_string = urlScheme + addressInputField->text() + "/json/state";
    QJsonObject json;
    json["v"] = true;
    QJsonDocument doc(json);
    emit processRequest(url_string, doc);
}

QJsonDocument MainWindow::buildRequestJSON(int sliderValue, const int &dropdownValue) {
    // Create JSON payload
    QJsonObject json;
    json["bri"] = sliderValue;
    json["ps"] = dropdownValue;
    json["on"] = "t";

    QJsonDocument doc(json);
    return doc;
}

void MainWindow::loadConfig() {
    QDir().mkpath(configDir);
    qDebug() << "Using config at:" << settings.fileName();
    if (settings.contains("ip")) {
        addressInputField->setText(settings.value("ip").toString());
    }
    if (settings.contains("enable_ssl")) {
        if (settings.value("enable_ssl").toBool() == true) {
            urlScheme = "https://";
        }
    }
}

void MainWindow::updateConfig() {
    QString fieldIP = addressInputField->text();
    QString configIP = settings.value("ip").toString();

    if (!settings.contains("ip") || fieldIP != configIP) {
        settings.setValue("ip", fieldIP);
    }

    if (!settings.contains("enable_ssl")) {
        settings.setValue("enable_ssl", false);
    }
}



void MainWindow::updateUI(const QJsonDocument &json) {
    const QJsonObject obj = json.object();
    const bool isSync = obj.value("sync").toBool();
    const QString type = obj.value("type").toString();
    const QJsonObject response = obj.value("response").toObject();

    // End function is sync is false
    if (!isSync) { return; }

    if (type == "presets") {
        dropdownItems = {};
        dropdownItems["none"] = -1;
        for (auto it = response.constBegin(); it != response.constEnd(); ++it) {
            const QJsonObject preset = it.value().toObject();
            // Skip check if preset is empty or doesn't contain the value we want
            if (preset.contains("n")) {
                dropdownItems[preset.value("n").toString()] = it.key().toInt();
            }
        }

        // Disable built-in signals while updating for performace 
        dropdown->blockSignals(true);

        // Add extracted presets to the UI
        dropdown->clear();
        QStringList items;
        items.reserve(dropdownItems.size());

        for (auto it = dropdownItems.constBegin(); it != dropdownItems.constEnd(); ++it) {
            items.append(it.key());
        }

        dropdown->addItems(items);

        const int index = dropdown->findText("none");
        if (index != -1) {
            dropdown->setCurrentIndex(index);
        }
        // Re-enable signals
        dropdown->blockSignals(false);
    } else if (type == "state") {
        slider->setValue(response.value("bri").toInt());
    }
}

