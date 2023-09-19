#include "include/UdpClient.h"

UdpClient::UdpClient(QWidget *parent)noexcept : QWidget(parent)
{
    ReadIniFile();

    setWindowTitle("Udp client");

    m_Socket = new QUdpSocket(this);
    m_Socket->bind(m_Port);

    QFont font_labels;
    font_labels.setPointSize(16);

    m_ConnectionLabel = new QLabel("Connection: Disconnected", this);
    m_ConnectionLabel->setAlignment(Qt::AlignTop | Qt::AlignLeft);
    m_ConnectionLabel->setFont(font_labels);

    m_HeightLabel = new QLabel("Height: ", this);
    m_HeightLabel->setFont(font_labels);

    m_HeightIndicatorWidget = new HeightIndicatorWidget(this);

    m_BoxLayout = new QVBoxLayout(this);
    m_BoxLayout->addWidget(m_ConnectionLabel);
    m_BoxLayout->addWidget(m_HeightLabel);
    m_BoxLayout->addWidget(m_HeightIndicatorWidget);
    m_BoxLayout->addStretch();

    setLayout(m_BoxLayout);

    m_ActivityTimer = new QTimer(this);
    m_ActivityTimer->setInterval(500);

    connect(m_ActivityTimer, &QTimer::timeout, this, &UdpClient::SendActivity);

    m_ActivityTimer->start();

    connect(m_Socket, &QUdpSocket::readyRead, this, &UdpClient::ReceiveHeight);
}

void UdpClient::SendActivity()
{
    Message2 message;

    QByteArray datagram;

    QDataStream stream(&datagram, QIODevice::WriteOnly);
    stream << message.header;

    m_Socket->writeDatagram(datagram, QHostAddress{m_IpAddress}, m_Port);

    if(!m_IsConnected)
        m_ConnectionLabel->setText(QString{"Connection: Disconnected"});

    m_IsConnected = false;
}

void UdpClient::ReceiveHeight()
{
    while(m_Socket->hasPendingDatagrams())
    {
        //Make code as dry as Sahara!
        auto show_message = [](const Message1 &message, bool broken = false)
        {
            broken ? qDebug() << "Client got a broken message" : qDebug() << "Client got a new message";
            qDebug() << "Message:";
            qDebug() << "Header: " << message.header;
            qDebug() << "Height: " << message.height;
        };

        QByteArray datagram;
        Message1 message;

        datagram.resize(m_Socket->pendingDatagramSize());
        m_Socket->readDatagram(datagram.data(), datagram.size());

        QDataStream stream(datagram);

        //There is no point to read height here because we are not sure that message is not broken
        //stream >> message.header >> message.height; //Wrong
        stream >> message.header;

        if (message.header != 0xABCD)
        {
            show_message(message, true);
            return;
        }

        //Read height if message is not broken
        stream >> message.height;

        show_message(message);

        m_HeightIndicatorWidget->SetHeight(message.height);
        m_HeightLabel->setText(QString("Height: %1").arg(message.height));
        m_IsConnected = true;
        m_ConnectionLabel->setText(QString{"Connection: Connected"});
    }
}

void UdpClient::ReadIniFile()
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