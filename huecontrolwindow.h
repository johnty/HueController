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

private:
    Ui::HueControlWindow *ui;
    QNetworkAccessManager *manager;
};

#endif // HUECONTROLWINDOW_H
