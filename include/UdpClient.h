#ifndef UDP_CLIENT_H
#define UDP_CLIENT_H

#include <QtNetwork/QUdpSocket>
#include <QCoreApplication>
#include <QDebug>
#include <QTimer>
#include <QVBoxLayout>
#include <QLabel>
#include <QSettings>
#include "Message2.h"
#include "Message1.h"
#include "HeightIndicatorWidget.h"

class UdpClient : public QWidget
{
    Q_OBJECT
private:
    QUdpSocket* m_Socket{nullptr};
    QTimer* m_ActivityTimer{nullptr};
    QLabel* m_ConnectionLabel{nullptr};
    QLabel* m_HeightLabel{nullptr};
    QVBoxLayout *m_BoxLayout{nullptr};
    HeightIndicatorWidget *m_HeightIndicatorWidget{nullptr};
    bool m_IsConnected{false};

    QHostAddress m_IpAddress{QHostAddress::Null};
    qint16 m_Port{0};

    QHostAddress m_IpAddressServer{QHostAddress::Null};
    qint16 m_PortServer{0};

    void ReadIniFile();

private slots:
    void SendActivity();

    void ReceiveHeight();

public:

    explicit UdpClient(QWidget *parent = nullptr) noexcept;

    ~UdpClient() override;
};

#endif //UDP_CLIENT_H
