#pragma once
#include <QObject>

enum class ControlSystemMode: quint8 {
    Ruchnoi,
    Auto
};
#pragma pack(push,1)
    //структура данных для отправки из НПА в СУ НУ
    struct ROVData {
        float X=0;           ///---Координата X, м
        float Y=0;           ///---Координата Y, м (глубина)
        float y=0;           ///---Координата y, м (отстояние)
        float Z=0;           ///---Координата Z, м
        float Vx=0;          ///---Скорость по X, м/с
        float Vy=0;          ///---Скорость по Y, м/с
        float Vz=0;          ///---Скорость по Z, м/с
        float psi=0;         ///---Угол курса, град (0...360˚)
        float teta=0;        ///---Угол дифферента, град (-90˚...+90˚)
        float gamma=0;       ///---Угол крена, град (-180˚...+180˚)
        float Wx=0;      ///---Скорость поворота вокруг оси Xa, рад/с
        float Wy=0;      ///---Скорость поворота вокруг оси Ya, рад/с
        float Wz=0;      ///---Скорость поворота вокруг оси Za, рад/с
        ControlSystemMode mode=ControlSystemMode::Ruchnoi; //режим работы СУ
        union { // Установка состояний локальных контуров 1 - замкнут, а 0 - разомкнут
            uchar contours_state_raw = 0;
            struct {
                uchar contours_state_march      : 1; // Маршевый (X)
                uchar contours_state_glubina    : 1; // Вертикальный (Y)
                uchar contours_state_lag        : 1; // Лаговый (Z)
                uchar contours_state_yaw        : 1; // Курсовой
                uchar contours_state_pitch      : 1; // Дифферентный
                uchar contours_state_roll       : 1; // Креновый

            };
        };
    };
#pragma pack (pop)
