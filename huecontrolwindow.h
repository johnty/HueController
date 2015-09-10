#ifndef HUECONTROLWINDOW_H
#define HUECONTROLWINDOW_H

#include <QMainWindow>
#include <QNetworkAccessManager>
#include <QDebug>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QJsonObject>
#include <QJsonDocument>

namespace Ui {
class HueControlWindow;
}

class HueControlWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit HueControlWindow(QWidget *parent = 0);
    ~HueControlWindow();

private slots:
    void on_pushButton_clicked();
    void syncRequestFinished(QNetworkReply* reply);

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_horizontalSlider_sliderMoved(int position);

    void on_horizontalSlider_valueChanged(int value);

    void on_pushButton_4_clicked();

private:
    Ui::HueControlWindow *ui;
    QNetworkAccessManager *manager;

    void setLightOn(bool isOn, int idx);
    void setBrightness(int bri, int idx);
};

#endif // HUECONTROLWINDOW_H
