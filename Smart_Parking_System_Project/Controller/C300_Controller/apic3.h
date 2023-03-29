#ifndef APIC3_H
#define APIC3_H

#include <QObject>
#include <QTcpSocket>
#include <QUdpSocket>
#include <QAbstractSocket>
#include <QDebug>
#include <QDateTime>
#include <QThread>
#include <QTimer>
#include <qdatetime.h>

#define devIDdefault 0x01
#define header QByteArray::fromHex("AA")
#define footer QByteArray::fromHex("55")

class ApiC3 : public QObject
{
    Q_OBJECT
public:
    explicit ApiC3(QObject *parent = nullptr);

    quint16 crc16(QByteArray msg);
    void sendCMD(quint8 devID,quint8 cmdID,QByteArray payload);

    Q_INVOKABLE void connect(QString address, quint16 port);
    Q_INVOKABLE void disconect();
    Q_INVOKABLE void openDoor(quint8 doorID, quint8 state);
    Q_INVOKABLE void getRTlog();

    Q_INVOKABLE void searchDev();
    Q_INVOKABLE void modifyIP(QString oldAddress,QString newAddress);


    int decode(QByteArray data);
    QString decodeTime(QByteArray time);
    int decodepayload(QByteArray payload);

signals:
    Q_INVOKABLE void completeSearchDev();

public slots:
    void connected();
    void disconnected();
    void bytesWritten(qint64 bytes);
    void readyRead();

    void readyReadUDP();

private:
    QTcpSocket *socket = new QTcpSocket(this);
    QUdpSocket *udp = new QUdpSocket(this);
    QTimer *timer = new QTimer(this);
    QDateTime dateTime;

    //decode frame
    uchar cmdID;
    uchar devID;
    quint16 lenPayload;
    QByteArray payload;

    //decode payload getRTlog
    quint32 cardNo;
    quint32 pin;
    uchar verifyMode;
    uchar doorID;
    uchar eventType;
    QString time;

    // UDP
    QHostAddress sender;
    quint16 senderPort;

    //searchDev
    QList<QByteArray> listMAC;
    QList<QByteArray> listIP;
    QList<QByteArray> listNetMask;
    QList<QByteArray> listGATEIPAddress;
    QList<QByteArray> listSN;
    QList<quint16> listPort;


    enum CMDID{
        connectID=0x76,
        disconnectID=0x02,
        openDoorID=0x05,
        getRTlogID=0x0B,
        searchDevID=0x14,
        modifyIPID=0x15,
        responseSuccessID=0xC8
    };

    //payload
    QByteArray openDoorPayload = QByteArray::fromHex("01 01 01 01 00");
    QByteArray connectPayload = QByteArray::fromHex("00 00 01 00");
    QByteArray getRTlogPayload = "";
    QByteArray disConnectPayload = "";
};

#endif // APIC3_H
