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

    m_Socket = new QUdpSocket(this);
    m_Socket->bind( m_IpAddress, m_Port);
    connect(m_Socket, SIGNAL(readyRead()), this, SLOT(ReceiveClientActivity()));

    m_QTimer = new QTimer(this);
    m_QTimer->setInterval(40);//1000ms / 25hz = 40ms
    connect(m_QTimer, SIGNAL(timeout()), this, SLOT(SendHeight()));
    m_QTimer->start();

    m_BoxLayout->addWidget(m_QSlider);
    m_BoxLayout->addWidget(m_HeightLabel);
    m_BoxLayout->addWidget(m_ConnectionLabel);

    setLayout(m_BoxLayout);
}

void UdpServer::SendHeight()
{
    Message1 message{m_Height};

    QByteArray datagram;

    QDataStream stream(&datagram, QIODevice::WriteOnly);
    stream << message.header << message.height;

    m_Socket->writeDatagram(datagram, QHostAddress{m_IpAddressClient}, m_PortClient);
}

void UdpServer::ReadIniFile()
{
    QSettings settings("../settings.ini", QSettings::IniFormat);

    if (settings.status() != QSettings::NoError)
        throw std::runtime_error("Could not read settings.ini");

    settings.beginGroup("NETWORK_SERVER");

    m_IpAddress.setAddress(QString{settings.value("IpAddress").toString()});

    m_Port = settings.value("Port").toInt();

    settings.endGroup();


    settings.beginGroup("NETWORK_CLIENT");

    if (settings.status() != QSettings::NoError)
        throw std::runtime_error("Could not read settings.ini");

    m_IpAddressClient.setAddress(QString{settings.value("IpAddress").toString()});

    m_PortClient = settings.value("Port").toInt();

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
    if (!m_Socket->waitForReadyRead(2000))
    {
        m_ConnectionLabel->setText(QString{"Connection: Disconnected"});
        return;
    }

    while (m_Socket->hasPendingDatagrams())
    {
        QByteArray datagram;
        datagram.resize(m_Socket->pendingDatagramSize());

        m_Socket->readDatagram(datagram.data(), datagram.size());

        QDataStream stream(datagram);
        Message2 message;
        stream >> message.header;

        if(message.header != 0x1234)
            break;

        m_ConnectionLabel->setText(QString{"Connection: Connected"});
    }

}