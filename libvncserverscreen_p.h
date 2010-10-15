#ifndef LIBVNCSERVER_P_H
#define LIBVNCSERVER_P_H

#include "libvncserverscreen_qws.h"

#include <rfb/rfb.h>

#include <QTimer>

QT_BEGIN_NAMESPACE

class LibVNCServer;

class LibVNCServerScreenPrivate : public QObject
{
Q_OBJECT
public:
    LibVNCServerScreenPrivate( LibVNCServerScreen *parent );
    ~LibVNCServerScreenPrivate();

    void setDirty(const QRect &rect);
    void configure();

    qreal dpiX;
    qreal dpiY;
    int refreshRate;

    LibVNCServer* server;
    LibVNCServerScreen *q_ptr;
};

class LibVNCServer : public QObject
{
    Q_OBJECT
public:
    LibVNCServer( LibVNCServerScreen* screen );
    LibVNCServer( LibVNCServerScreen* screen, int id );
    ~LibVNCServer();

    bool isConnected() const { return m_screenInfo->clientHead != 0; }

    void setKeyboardModifiers( Qt::KeyboardModifiers mod ) { m_keymod = mod; }
    Qt::KeyboardModifiers keyboardModifiers() const { return m_keymod; }

    void setDirty(const QRect &rect);

private Q_SLOTS:
    void processEvents();
    void checkIdle();

private:
    void init( uint port = 0 );

    QTimer* m_timer;
    QTimer* m_timerIdle;
public:
    rfbScreenInfoPtr m_screenInfo;
private:
    LibVNCServerScreen* m_screen;    
    Qt::KeyboardModifiers m_keymod;
};

QT_END_NAMESPACE
#endif // LIBVNCSERVER_P_H
