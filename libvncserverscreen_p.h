/*
 *  Copyright (C) 2010 Connected Table AB <info@connectedtable.com>
 *  All Rights Reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */

#ifndef LIBVNCSERVER_P_H
#define LIBVNCSERVER_P_H

#include "libvncserverscreen_qws.h"

#include <rfb/rfb.h>

#include <QTimer>

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

#endif // LIBVNCSERVER_P_H
