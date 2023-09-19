#ifndef UDP_SERVER_H
#define UDP_SERVER_H

#include <QtNetwork/QUdpSocket>
#include <QCoreApplication>
#include <QDebug>
#include <QTimer>
#include <QLabel>
#include <QSlider>
#include <QSettings>
#include <QVBoxLayout>
#include "Message1.h"
#include "Message2.h"

class UdpServer : public QWidget
{
Q_OBJECT
private:
    QUdpSocket* m_Socket{nullptr};
    QTimer* m_QTimer{nullptr};
    QVBoxLayout *m_BoxLayout{nullptr};
    QLabel* m_ConnectionLabel{nullptr};
    QLabel* m_HeightLabel{nullptr};
    QSlider* m_QSlider{nullptr};
    QHostAddress m_IpAddress{QHostAddress::Null};
    qint16 m_Port{0};
    quint16 m_Height{0};
    std::chrono::seconds m_ClientTimeOut{std::chrono::seconds(2)};

    void ReadIniFile();

private slots:
    void SendHeight();

    void ReceiveClientActivity();

public:
    explicit UdpServer(QWidget* parent = nullptr) noexcept;

    ~UdpServer() override;
};

#endif // UDP_SERVER_H
