#include <QApplication>
#include "include/UdpServer.h"
#include "include/UdpClient.h"

int main(int argc, char *argv[])
{
    QApplication q_application(argc, argv);

    UdpServer udp_server;
    UdpClient udp_client;

    udp_server.show();
    udp_client.show();

    return q_application.exec();
}