#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include <QJsonDocument>

class QLineEdit;
class QSlider;
class QLabel;
class QComboBox;
class QPushButton;

class MainWindow : public QWidget {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    void setPresets();

private:
    QLineEdit *addressInputField;
    QSlider *slider;
    QLabel *sliderLabel;
    QComboBox *dropdown;
    QPushButton *lightButton;
    QPushButton *webButton;
    void updateConfig();
    void loadConfig();
    QJsonDocument buildRequestJSON(int sliderValue, const int &dropdownValue);

public slots:
    void updateUI(const QJsonDocument &json);

signals:
    void processRequest(const QString &url, const QJsonDocument json);
};

#endif