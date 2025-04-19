#ifndef UDP_SERVER_H
#define UDP_SERVER_H

#include <QUdpSocket>
#include <QHostAddress>
#include <QDebug>
#include  <qbuffer.h>
#include <qimage.h>

class UdpServer : public QObject {
    Q_OBJECT
public:
    UdpServer() {
        udpSocket = new QUdpSocket(this);
        udpSocket->bind(QHostAddress::Any, 12345);
        connect(udpSocket, &QUdpSocket::readyRead, this, &UdpServer::onReadyRead);
    }

    void sendData(const QString &message) {
        QByteArray data = message.toUtf8();
        udpSocket->writeDatagram(data, QHostAddress("192.168.137.1"), 12346);
    }

    void sendImage(const QImage& qimage){
        QByteArray imageData;
        QBuffer buffer(&imageData);
        buffer.open(QIODevice::WriteOnly);
        qimage.save(&buffer, "JPEG",45);
        udpSocket->writeDatagram(imageData, QHostAddress("192.168.137.1"), 12346);
    }

private slots:
    void onReadyRead() {
        while (udpSocket->hasPendingDatagrams()) {
            QByteArray datagram;
            datagram.resize(udpSocket->pendingDatagramSize());
            QHostAddress sender;
            quint16 senderPort;
            udpSocket->readDatagram(datagram.data(), datagram.size(), &sender, &senderPort);
        }
    }
private:
    QUdpSocket *udpSocket;
};

#endif // UDP_SERVER_H
