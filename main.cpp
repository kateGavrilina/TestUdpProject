#include <QCoreApplication>
#include "pultcontrolsystemprotocols.h"
#include <QDebug>

//тут приведен пример, где создаются 2 обмена: объект обмена, который будет создан в программе Северова (controlProtocol)
//и обмен, который будет создан в программе Пульта (pultProtocol)
//структуры данных, которые передаются, описаны в "pultcontrolsystemprotocols.h"
//ToPult - то что передается от СУ Северова в Пульт
//FromPult - то, что передается от Пульта в СУ Северова
int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    qDebug()<<"create Pult protocol";
    Pult::PC_Protocol *pultProtocol = new Pult::PC_Protocol (QHostAddress("127.0.0.1"), 13021,QHostAddress::LocalHost,13020,10);
    qDebug()<<"create Control System protocol";
    ControlSystem::PC_Protocol *controlProtocol = new ControlSystem::PC_Protocol(QHostAddress("127.0.0.1"), 13020,QHostAddress::LocalHost,13021,10);

    QObject::connect(pultProtocol,&Pult::PC_Protocol::dataReceived,[&](){
        qDebug()<<"Pult received correct data from Control System";
        qDebug()<<"X = "<< pultProtocol->rec_data._data.X;
    });

    QObject::connect(controlProtocol,&ControlSystem::PC_Protocol::dataReceived,[&](){
        qDebug()<<"Control system received correct data from Pult";
        qDebug()<<"Control mode = "<< static_cast<int>(controlProtocol->rec_data.mode);
    });

    qDebug()<<"-----start exchange";
    //изменяют что-то в отправляемой из СУ в Пульт структуре
    controlProtocol->send_data._data.X = 10;
    //и в структуре из Пульта в СУ
    pultProtocol->send_data.mode = ControlSystemMode::Auto;
    //отправляю разово
    controlProtocol->sendData();
    pultProtocol->sendData();

    //или запускаю обмен
//    controlProtocol->send_data._data.X = 14;
//    controlProtocol->startExchange();


    return a.exec();
}
