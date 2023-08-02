#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QTimer>
#include "speedometer.h"
//#include <wiringPiSPI.h>
#include <unistd.h>

static const int CHANNEL = 1;

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);

    // to connect to qml, we need a qml RegisterType with tpy of class Speedometer
    qmlRegisterType<Speedometer>("piracer.speedometer",1,0,"Speedometer");

    //
    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    QObject *object = engine.rootObjects()[0];
    QObject *speedometer = object->findChild<QObject*>("speedoMeter");

    Speedometer *ptrSpeedometer = qobject_cast<Speedometer*>(speedometer);
    //Speedometer *ptrSpeedometer = dynamic_cast<Speedometer*>(speedometer);



    // connector (for testing)
    qreal val = 0;
    float volt = 0;
    QTimer timer;

    QObject::connect(&timer, &QTimer::timeout, [&]()
    {
        //LAMBDA

        if(abs(volt-val) <=10)
            val = volt;
        else if(volt > val)
            val = val + 10;
        else if( volt < val)
            val = val - 10;

        // set speed in speedometer
        ptrSpeedometer->setSpeed(val);

        // change color of outer ring depending on speed

        if(val < 1600)
                ptrSpeedometer->setOuterColor(QColor(128,255,0));
        else if(val > 1600 && val < 3200)
                ptrSpeedometer->setOuterColor(QColor(255,255,0));
        else if(val > 3200)
                ptrSpeedometer->setOuterColor(QColor(255,0,0));

    }
    );

    // connector (for speed sensor)
    QTimer timer2;

    QObject::connect(&timer2,&QTimer::timeout, [&]()
    {
        /*connect sensor data from can bus here*/
    });

    timer2.start(180);
    timer.start(10);

    if (engine.rootObjects().isEmpty())
        return -1;

    // run application
    return app.exec();
}
