#ifndef HUECONTROLWINDOW_H
#define HUECONTROLWINDOW_H

#include <QMainWindow>
#include <QNetworkAccessManager>
#include <QDebug>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QJsonObject>
#include <QJsonDocument>
#include <QTimer>
#include <QCheckBox>


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

    //ui slots
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_horizontalSlider_sliderMoved(int position);

    void on_horizontalSlider_valueChanged(int value);

    void on_pushButton_4_clicked();

    //custom slots
    void syncRequestFinished(QNetworkReply* reply);
    void updateLightStatus();

    void on_pushButton_SelAll_clicked();

    void on_pushButton_SelNone_clicked();

private:
    Ui::HueControlWindow *ui;
    QNetworkAccessManager *manager;

    void setLightOn(bool isOn, int idx);
    void setBrightness(int bri, int idx);
    void queryLight(int idx);

    void selectAllChecks(bool checked);

    QCheckBox** lightCheckList;


    QTimer *timer;
    int t_count;

};

#endif // HUECONTROLWINDOW_H
