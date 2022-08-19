#ifndef PULTCONTROLSYSTEMPROTOCOLS_H
#define PULTCONTROLSYSTEMPROTOCOLS_H
#include "rovdata.h"
#include "udp_protocol.h"

#pragma pack(push,1)
//структура данных, которая передается из Северова в Пульт
//тут описаны данные, которые Пульт принимает от Северова
struct ToPult {
    ToPult(){
    }
    ROVData _data;
    uint checksum;
};

//структура данных, которая передается из плнировщика в АНПА
struct FromPult {
    FromPult(){

    }
    ControlSystemMode mode; // режим работы СУ
    uint checksum;
};
#pragma pack (pop)

//класс обмена Северов - Пульт, который создается в Пульте
namespace Pult {
class PC_Protocol : public QObject, public MetaUdpProtocol {
    Q_OBJECT
public:
    //создание объекта, инициализация портов, но при этом отправка не запущена
    explicit PC_Protocol(QHostAddress _receiverIP, int _receiverPort, QHostAddress _senderIP,\
                         int _senderPort,int freq, QObject *parent = 0){
        udpProtocol = new UdpProtocol <ToPult, FromPult> (_receiverIP,_receiverPort, _senderIP,_senderPort,\
                                                          freq, parent);
        connect(timer,SIGNAL(timeout()),SLOT(sendData()));
        connect(udpProtocol->getReceiveSocket(),SIGNAL(readyRead()),SLOT(receiveData()));
        set_ip_receiver(udpProtocol->ip_receiver());
        set_ip_sender (udpProtocol->ip_sender());
        set_port_receiver(udpProtocol->port_receiver());
        set_port_sender (udpProtocol->port_sender());
    }

signals:
    void dataReceived();

public slots:

    //запуск обмена
    void startExchange(){
        timer->start(1000/udpProtocol->getFrequency());
    }
    //остановить обмен
    void stopExhange(){
        timer->stop();
    }
    void sendData(){
        udpProtocol->send_data = send_data;
        udpProtocol->sendData();
    }
    void receiveData(){
        udpProtocol->receiveData();
        rec_data = udpProtocol->rec_data;
        emit dataReceived();
    }
public:
    FromPult send_data;
    ToPult rec_data;
    UdpProtocol <ToPult, FromPult> *udpProtocol;
    bool bindState(){return udpProtocol->bindState();}

};
} //namespace Pult

//класс обмена АНПА- планировщик, который создается в АНПА
namespace ControlSystem {
class PC_Protocol : public QObject, public MetaUdpProtocol {
    Q_OBJECT
public:
    explicit PC_Protocol(QHostAddress _receiverIP, int _receiverPort, QHostAddress _senderIP,\
                         int _senderPort,int freq, QObject *parent = 0){
        udpProtocol = new UdpProtocol <FromPult, ToPult> (_receiverIP,_receiverPort, _senderIP,_senderPort,\
                                                          freq, parent);
        connect(timer,SIGNAL(timeout()),SLOT(sendData()));
        connect(udpProtocol->getReceiveSocket(),SIGNAL(readyRead()),SLOT(receiveData()));
        set_ip_receiver(udpProtocol->ip_receiver());
        set_ip_sender (udpProtocol->ip_sender());
        set_port_receiver(udpProtocol->port_receiver());
        set_port_sender (udpProtocol->port_sender());
    }
signals:
    void dataReceived();
public slots:
    //запуск обмена
    void startExchange(){
        timer->start(1000/udpProtocol->getFrequency());
    }
    //остановить обмен
    void stopExhange(){
        timer->stop();
    }
    void sendData(){
 //       qDebug()<<"send data";
        udpProtocol->send_data = send_data;
        udpProtocol->sendData();
    }
    void receiveData(){
        udpProtocol->receiveData();
        rec_data = udpProtocol->rec_data;
        emit dataReceived();
    }
public:
    ToPult send_data;
    FromPult rec_data;
    UdpProtocol <FromPult, ToPult> *udpProtocol;

    bool bindState(){return udpProtocol->bindState();}
};
} //namespace ControlSystem


#endif // PULTCONTROLSYSTEMPROTOCOLS_H
