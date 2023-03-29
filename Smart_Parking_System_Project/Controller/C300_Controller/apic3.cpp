#include "apic3.h"

ApiC3::ApiC3(QObject *parent)
    : QObject{parent}
{

}

quint16 ApiC3::crc16(QByteArray msg)
{
    quint16 crc = 0;
    for(int i=0; i<msg.length(); i++) {
        crc ^= uchar(msg[i]);
        for(int j=0; j<8;j++) {
            if (crc & 1) {
                crc = (crc >>1) ^ 0xa001;
            }
            else {
                crc = crc >>1;
            }
        }
    }
    return crc;
}

void ApiC3::sendCMD(quint8 devID, quint8 cmdID, QByteArray payload)
{
    QByteArray message;
    message.append(devID);
    message.append(cmdID);
    quint16 len = payload.length();
    message.append((const char*)&len, sizeof(len));
    message.append(payload);
    quint16 crc = crc16(message);
    message.append((const char*)&crc, sizeof(crc));
    message = header + message + footer;
    socket->write(message);
}

void ApiC3::connect(QString address, quint16 port)
{

    QObject::connect(socket, SIGNAL(connected()),this, SLOT(connected()));
    QObject::connect(socket, SIGNAL(disconnected()),this, SLOT(disconnected()));
    QObject::connect(socket, SIGNAL(bytesWritten(qint64)),this, SLOT(bytesWritten(qint64)));
    QObject::connect(socket, SIGNAL(readyRead()),this, SLOT(readyRead()));

    qDebug() << "connecting...";

    socket->connectToHost(address,port);

    if(!socket->waitForConnected(5000))
    {
        qDebug() << "Error: " << socket->errorString();
    }
    sendCMD(devIDdefault,connectID,connectPayload);
}

void ApiC3::disconect()
{
    sendCMD(devIDdefault,disconnectID,disConnectPayload);
}

void ApiC3::openDoor(quint8 doorID, quint8 state)
{
    QByteArray payload = openDoorPayload;
    payload[1] = doorID;
    payload[3] = state;
    sendCMD(devIDdefault,openDoorID,payload);
}

void ApiC3::getRTlog()
{
    sendCMD(devIDdefault,getRTlogID,getRTlogPayload);
}

void ApiC3::searchDev()
{
    QObject::connect(udp, SIGNAL(readyRead()),this,SLOT(readyReadUDP()));
    QByteArray searchDevFrame = QByteArray::fromStdString("CallSecurityDevice");
    listMAC.clear();
    listIP.clear();
    listNetMask.clear();
    listGATEIPAddress.clear();
    listSN.clear();
    listPort.clear();

    QByteArray message;
    message.append(devIDdefault);
    message.append(0x14);
    quint16 len = searchDevFrame.length();
    message.append((const char*)&len, sizeof(len));
    message.append(searchDevFrame);
    quint16 crc = crc16(message);
    message.append((const char*)&crc, sizeof(crc));
    message = header + message + footer;

    for(int i=65535;i>0;i--) {
        udp->writeDatagram(message.data(),message.size(),QHostAddress::Broadcast, i);
        timer->start(1);
//        qDebug() <<i;
    }

    emit(completeSearchDev());
}

void ApiC3::modifyIP(QString oldAddress, QString newAddress)
{
    int i =0;
    int j;
    if (!listIP.empty()) {
        qDebug() <<listIP.length();
        for(i=0; i<listIP.length(); i++) {
            if(oldAddress.toUtf8()==listIP[i]) {
                j=i;
                break;
            }
            j = -1;
        }

        if (j == -1) {
            //no find IP in list old IP
            qDebug() << "no find IP in list old IP";
        }
        else {
            QByteArray message;
            message.append(devIDdefault);
            message.append(modifyIPID);
            QByteArray payload;
            payload = QByteArray::fromStdString("MAC=") + listMAC[j] + QByteArray::fromStdString(",IPAddress=") + newAddress.toUtf8();
            quint16 len = payload.length();
            message.append((const char*)&len, sizeof(len));
            message.append(payload);
            quint16 crc = crc16(message);
            message.append((const char*)&crc, sizeof(crc));
            message = header + message + footer;
            udp->writeDatagram(message.data(),message.size(),QHostAddress::Broadcast, listPort[j]);
        }
    }
    else {
        qDebug() << "don't see any data";
    }

}


int ApiC3::decode(QByteArray data)
{
    if (data[0] != header[0] || data[data.length()-1] != footer[0] ) {  //check header and footer
        return 0;
    }
    data.remove(data.length()-1,1).remove(0,1);

    if((((static_cast<unsigned int>(data[data.length()-1]) & 0xFF) << 8) +
        (static_cast<unsigned int>(data[data.length()-2]) & 0xFF)) != crc16(data.remove(data.length()-2,2))) { // check sum
        return 0;
    }
    devID = uchar(data[0]);
    cmdID = uchar(data[1]);
    lenPayload = (static_cast<quint16>(data[2]) & 0xFF) + ((static_cast<quint16>(data[3]) & 0xFF) << 8);
    payload = data.remove(0,4);

    return 1;
}

QString ApiC3::decodeTime(QByteArray time)
{
    QString mTime =""; // 2022-12-13 00:42:45
    //    quint32 seconds = (static_cast<quint32>(payload[0]) & 0xFF) + ((static_cast<quint32>(payload[1]) & 0xFF) << 8) + ((static_cast<quint32>(payload[2]) & 0xFF) <<16) + ((static_cast<quint32>(payload[3]) & 0xFF) << 24);
    //    mTime = QString(seconds/31557600) + "-";
    //    seconds = seconds%31667600;
    return mTime;
}

int ApiC3::decodepayload(QByteArray payload)
{
    cardNo = (static_cast<quint32>(payload[0]) & 0xFF) + ((static_cast<quint32>(payload[1]) & 0xFF) << 8) + ((static_cast<quint32>(payload[2]) & 0xFF) <<16) + ((static_cast<quint32>(payload[3]) & 0xFF) << 24);
    payload.remove(0,4);

    pin = (static_cast<quint32>(payload[0]) & 0xFF) + ((static_cast<quint32>(payload[1]) & 0xFF) << 8) + ((static_cast<quint32>(payload[2]) & 0xFF) <<16) + ((static_cast<quint32>(payload[3]) & 0xFF) << 24);
    payload.remove(0,4);

    verifyMode = uchar(payload[0]);
    payload.remove(0,1);

    doorID = uchar(payload[0]);
    payload.remove(0,1);

    eventType = uchar(payload[0]);
    payload.remove(0,1);

    time = dateTime.toString("yyyy-MM-dd hh:mm:ss");

    return 1;
}

void ApiC3::connected()
{
    qDebug() << "connected...";
}

void ApiC3::disconnected()
{
    qDebug() << "disconnected...";
}

void ApiC3::bytesWritten(qint64 bytes)
{
    qDebug() << bytes << " bytes written...";
}

void ApiC3::readyReadUDP()
{
    QByteArray buffer;
    QList<QByteArray> filed;
    buffer.resize(udp->pendingDatagramSize());

    udp->readDatagram(buffer.data(), buffer.size(),&sender, &senderPort);

    qDebug() << "Message from: " << sender.toString();
    qDebug() << "Message port: " << senderPort;
    qDebug() << "Message: " << buffer;
    if(decode(buffer)) {
        if(payload[0]=='M'&&payload[1]=='A'&&payload[2]=='C') {
            quint8 k =0;
            QList<QByteArray> sub_str = payload.split(',');
            filed = sub_str[k++].split('=');
            listMAC.append(filed[1]);

            filed = sub_str[k++].split('=');
            listIP.append(filed[1]);

            filed = sub_str[k++].split('=');
            listNetMask.append(filed[1]);

            filed = sub_str[k++].split('=');
            listGATEIPAddress.append(filed[1]);

            filed = sub_str[k++].split('=');
            listSN.append(filed[1]);

            listPort.append(senderPort);
        }
        else if (cmdID==responseSuccessID&&payload =="") {
            qDebug() <<"modify IP success";
            listMAC.clear();
            listIP.clear();
            listNetMask.clear();
            listGATEIPAddress.clear();
            listSN.clear();
            listPort.clear();
        }
    }
}

void ApiC3::readyRead()
{
    if(decode(socket->readAll())) {
        if (cmdID == uchar(0xC9) && payload == QByteArray::fromHex("F3")) {
            qDebug() << "success connect  step 1";
            sendCMD(devIDdefault,0x01,"");
            cmdID = 0;
        }
        else if (cmdID == uchar(0xC8) && lenPayload == 0) {
            qDebug() << "success connect/disconnect/open door";
            cmdID = 0;
        }
        else if (cmdID == uchar(0xC8) && lenPayload == 16) {
            qDebug() << "getRtlog";
            decodepayload(payload);
        }
    }
    else {
        qDebug() << "decode error";
    }
}
