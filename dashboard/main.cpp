# include   <QtGui/QGuiApplication>
# include   <QtQml/QQmlApplicationEngine>
# include   <QtGui/QFont>
# include   <QtGui/QFontDatabase>
# include   <QtDBus/QtDBus>

# define    FAILURE -1
# define    SUCCESS 0

int         main(
    int     _arc,
    char*   _arv[]
) {
    // application define
    QGuiApplication         _app(_arc, _arv);

    // font setting
    QFontDatabase::addApplicationFont(":/asset/fonts/DejaVuSans.ttf");
    _app.setFont(QFont("DejaVu Sans"));

    // dbus connection test
    if (!QDBusConnection::sessionBus().isConnected())
    {
        fprintf(
            stderr,
            "[Error] [Dashboard] [DBus Connection]  Cannot connect to the D-Bus session bus.\n",
            "                                       To start, run : [eval `dbus-launch --auto-syntax]"
        );
        return ;
    }

    // dbus signal receiver
    DBusSignalHandler       _signal_handler;

    // connect dbus signal to slot
    QDBusConnection::sessionBus().connect(
        QString(),              // Service
        "/org/example/Test",    // Path
        "org.example.Test"      // Interface
        "TestSignal",           // Signal Name
        &_signal_handler,
        SLOT(handleSignal(QDBusMessage))
    );

    // qml on
    QQmlApplicationEngine   _engine(QUrl("qrc:/asset/qml/dashboard.qml"));
    if (_engine.rootObjects().isEmpty())
        return (FAILURE);

    return (_app.exec());
}
