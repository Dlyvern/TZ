#include "include/UdpServer.h"

UdpServer::UdpServer(QWidget *parent) noexcept : QWidget(parent)
{
    ReadIniFile();

    setWindowTitle("Udp server");

    m_BoxLayout = new QVBoxLayout(this);

    m_HeightLabel = new QLabel(QString{"Height: %1"}.arg(m_Height), this);
    m_HeightLabel->setAlignment(Qt::AlignCenter);

    m_ConnectionLabel = new QLabel("Connected with client: Disconnected", this);
    m_ConnectionLabel->setAlignment(Qt::AlignCenter);

    m_QSlider = new QSlider(Qt::Horizontal, this);
    m_QSlider->setRange(0, 9999);

    connect(m_QSlider, &QSlider::valueChanged, this, [this](int value){m_HeightLabel->setText(QString("Height: %1").arg(m_Height = value));});

    m_QTimer = new QTimer(this);
    m_QTimer->setInterval(40);//1000ms / 25hz = 40ms
    connect(m_QTimer, SIGNAL(timeout()), this, SLOT(SendHeight()));
    m_QTimer->start();

    m_BoxLayout->addWidget(m_QSlider);
    m_BoxLayout->addWidget(m_HeightLabel);
    m_BoxLayout->addWidget(m_ConnectionLabel);

    setLayout(m_BoxLayout);

    m_Socket = new QUdpSocket(this);

    connect(m_Socket, &QUdpSocket::readyRead, this, &UdpServer::ReceiveClientActivity);

    qDebug() << "Server started";
    qDebug() << "Ip address:" << m_IpAddress.toString();
    qDebug() << "Port:" <<  m_Port;
}

void UdpServer::SendHeight()
{
    Message1 message;

    message.height = m_Height;

    QByteArray datagram;

    QDataStream stream(&datagram, QIODevice::WriteOnly);
    stream << message.header << message.height;

    m_Socket->writeDatagram(datagram, QHostAddress{m_IpAddress}, m_Port);
}

void UdpServer::ReadIniFile()
{
    QSettings settings("../settings.ini", QSettings::IniFormat);

    if (settings.status() != QSettings::NoError)
    {
        settings.setValue("clientAddress", "127.0.0.1");
        settings.setValue("clientPort", 8080);
    }

    settings.beginGroup("NETWORK");

    QString address_in_string = settings.value("IpAddress").toString();

    if(address_in_string.isEmpty() || address_in_string.isNull())
        address_in_string = settings.value("clientAddress", "127.0.0.1").toString();

    m_IpAddress.setAddress(address_in_string);

    m_Port = settings.value("Port").toInt();

    if(m_Port == 0 || m_Port < 0)
        m_Port = settings.value("clientPort", 8080).toInt();

    settings.endGroup();
}

UdpServer::~UdpServer()
{
    delete(m_Socket);
    delete(m_ConnectionLabel);
    delete(m_HeightLabel);
    delete(m_QSlider);
    delete(m_QTimer);
    delete(m_BoxLayout);
}

void UdpServer::ReceiveClientActivity()
{
    while (m_Socket->hasPendingDatagrams())
    {
        QByteArray datagram;
        datagram.resize(m_Socket->pendingDatagramSize());
        m_Socket->readDatagram(datagram.data(), datagram.size());

        QDataStream stream(datagram);
        Message2 message;
        stream >> message.header;

        m_ConnectionLabel->setText(QString{"Connection: Connected"});
    }
}