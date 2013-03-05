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

/*
 * NOTE: Some portion of this code comes from Qt VNC graphics driver, unsure
 * about what license that applies to that code. 
 */

#include "libvncserverscreen_qws.h"
#include "libvncserverscreen_p.h"

#include <QDebug>
#include <QFile>
#include <QHostAddress>
#include <QThread>
#include <QUuid>
#include <QVariant>
#include <QWSServer>

#include <rfb/rfb.h>

static const struct {
    int keysym;
    int keycode;
} keyMap[] = {
    { 0xff08, Qt::Key_Backspace },
    { 0xff09, Qt::Key_Tab       },
    { 0xff0d, Qt::Key_Return    },
    { 0xff1b, Qt::Key_Escape    },
    { 0xff63, Qt::Key_Insert    },
    { 0xffff, Qt::Key_Delete    },
    { 0xff50, Qt::Key_Home      },
    { 0xff57, Qt::Key_End       },
    { 0xff55, Qt::Key_PageUp    },
    { 0xff56, Qt::Key_PageDown  },
    { 0xff51, Qt::Key_Left      },
    { 0xff52, Qt::Key_Up        },
    { 0xff53, Qt::Key_Right     },
    { 0xff54, Qt::Key_Down      },
    { 0xffbe, Qt::Key_F1        },
    { 0xffbf, Qt::Key_F2        },
    { 0xffc0, Qt::Key_F3        },
    { 0xffc1, Qt::Key_F4        },
    { 0xffc2, Qt::Key_F5        },
    { 0xffc3, Qt::Key_F6        },
    { 0xffc4, Qt::Key_F7        },
    { 0xffc5, Qt::Key_F8        },
    { 0xffc6, Qt::Key_F9        },
    { 0xffc7, Qt::Key_F10       },
    { 0xffc8, Qt::Key_F11       },
    { 0xffc9, Qt::Key_F12       },
    { 0xffe1, Qt::Key_Shift     },
    { 0xffe2, Qt::Key_Shift     },
    { 0xffe3, Qt::Key_Control   },
    { 0xffe4, Qt::Key_Control   },
    { 0xffe7, Qt::Key_Meta      },
    { 0xffe8, Qt::Key_Meta      },
    { 0xffe9, Qt::Key_Alt       },
    { 0xffea, Qt::Key_Alt       },

    { 0xffb0, Qt::Key_0         },
    { 0xffb1, Qt::Key_1         },
    { 0xffb2, Qt::Key_2         },
    { 0xffb3, Qt::Key_3         },
    { 0xffb4, Qt::Key_4         },
    { 0xffb5, Qt::Key_5         },
    { 0xffb6, Qt::Key_6         },
    { 0xffb7, Qt::Key_7         },
    { 0xffb8, Qt::Key_8         },
    { 0xffb9, Qt::Key_9         },

    { 0xff8d, Qt::Key_Return    },
    { 0xffaa, Qt::Key_Asterisk  },
    { 0xffab, Qt::Key_Plus      },
    { 0xffad, Qt::Key_Minus     },
    { 0xffae, Qt::Key_Period    },
    { 0xffaf, Qt::Key_Slash     },

    { 0xff95, Qt::Key_Home      },
    { 0xff96, Qt::Key_Left      },
    { 0xff97, Qt::Key_Up        },
    { 0xff98, Qt::Key_Right     },
    { 0xff99, Qt::Key_Down      },
    { 0xff9a, Qt::Key_PageUp    },
    { 0xff9b, Qt::Key_PageDown  },
    { 0xff9c, Qt::Key_End       },
    { 0xff9e, Qt::Key_Insert    },
    { 0xff9f, Qt::Key_Delete    },

    { 0, 0 }
};

void ptrAddEvent( int buttonMask, int x, int y, rfbClientPtr cl )
{
    Qt::MouseButtons buttons = 0;
    if( buttonMask & 1 ) {
        buttons |= Qt::LeftButton;
    }
    if( buttonMask & 2 ) {
        buttons |= Qt::MidButton;
    }
    if( buttonMask & 4 ) {
        buttons |= Qt::RightButton;
    }

    QWSServer::sendMouseEvent( QPoint( x, y ), buttons );

    rfbDefaultPtrAddEvent( buttonMask, x, y, cl );
}

void kbdAddEvent( rfbBool down, rfbKeySym key, rfbClientPtr cl )
{
    int keycode = 0;
    int unicode = 0;
    int i = 0;
    while (keyMap[i].keysym && !keycode) {
        if (keyMap[i].keysym == (int)key)
            keycode = keyMap[i].keycode;
        i++;
    }

    if (keycode >= ' ' && keycode <= '~')
        unicode = keycode;

    if (!keycode) {
        if (key <= 0xff) {
            unicode = key;
            if (key >= 'a' && key <= 'z')
                keycode = Qt::Key_A + key - 'a';
            else if (key >= ' ' && key <= '~')
                keycode = Qt::Key_Space + key - ' ';
        }
    }

    LibVNCServer* server = static_cast< LibVNCServer* >( cl->screen->screenData );
    Qt::KeyboardModifiers keymod = server->keyboardModifiers();
    if (keycode == Qt::Key_Shift)
        keymod = down ? keymod | Qt::ShiftModifier :
                        keymod & ~Qt::ShiftModifier;
    else if (keycode == Qt::Key_Control)
        keymod = down ? keymod | Qt::ControlModifier :
                        keymod & ~Qt::ControlModifier;
    else if (keycode == Qt::Key_Alt)
        keymod = down ? keymod | Qt::AltModifier :
                        keymod & ~Qt::AltModifier;
    if (unicode || keycode)
        QWSServer::sendKeyEvent(unicode, keycode, keymod, down, false);
    
    server->setKeyboardModifiers( keymod );
}

static bool displayHookCalled = false;

void displayHook( rfbClientPtr cl ) {
    Q_UNUSED( cl );
    displayHookCalled = true;
}

static const char* cur=
        "                   "
        " x                 "
        " xx                "
        " xxx               "
        " xxxx              "
        " xxxxx             "
        " xxxxxx            "
        " xxxxxxx           "
        " xxxxxxxx          "
        " xxxxxxxxx         "
        " xxxxxxxxxx        "
        " xxxxx             "
        " xx xxx            "
        " x  xxx            "
        "     xxx           "
        "     xxx           "
        "      xxx          "
        "      xxx          "
        "                   ";

static const char* mask=
        "xx                 "
        "xxx                "
        "xxxx               "
        "xxxxx              "
        "xxxxxx             "
        "xxxxxxx            "
        "xxxxxxxx           "
        "xxxxxxxxx          "
        "xxxxxxxxxx         "
        "xxxxxxxxxxx        "
        "xxxxxxxxxxxx       "
        "xxxxxxxxxx         "
        "xxxxxxxx           "
        "xxxxxxxx           "
        "xx  xxxxx          "
        "    xxxxx          "
        "     xxxxx         "
        "     xxxxx         "
        "      xxx          ";
 
static rfbCursorPtr myCursor;

LibVNCServerScreenPrivate::LibVNCServerScreenPrivate( LibVNCServerScreen *parent )
    : dpiX( 72 ), dpiY( 72 ), refreshRate( 25 ), q_ptr( parent )
{
}

LibVNCServerScreenPrivate::~LibVNCServerScreenPrivate()
{
    if (q_ptr->screen())
        return;
}

void LibVNCServerScreenPrivate::configure()
{
    if (q_ptr->screen())
        return;

    q_ptr->lstep = q_ptr->dw * ((q_ptr->d + 7) / 8);
    q_ptr->size = q_ptr->h * q_ptr->lstep;
    q_ptr->mapsize = q_ptr->size;
    q_ptr->physWidth = qRound(q_ptr->dw * qreal(25.4) / dpiX);
    q_ptr->physHeight = qRound(q_ptr->dh * qreal(25.4) / dpiY);

    switch (q_ptr->d) {
    case 1:
        q_ptr->setPixelFormat(QImage::Format_Mono); //### LSB???
        break;
    case 8:
        q_ptr->setPixelFormat(QImage::Format_Indexed8);
        break;
    case 12:
        q_ptr->setPixelFormat(QImage::Format_RGB444);
        break;
    case 15:
        q_ptr->setPixelFormat(QImage::Format_RGB555);
        break;
    case 16:
        q_ptr->setPixelFormat(QImage::Format_RGB16);
        break;
    case 18:
        q_ptr->setPixelFormat(QImage::Format_RGB666);
        break;
    case 24:
        q_ptr->setPixelFormat(QImage::Format_RGB888);
        break;
    case 32:
        q_ptr->setPixelFormat(QImage::Format_ARGB32_Premultiplied);
        break;
    }
}

LibVNCServer::LibVNCServer( LibVNCServerScreen* screen )
    : m_screen( screen )
{
    init();
}

LibVNCServer::LibVNCServer( LibVNCServerScreen* screen, int id )
    : m_screen( screen )
{
    init( id );
}

void updateServerFormat( rfbScreenInfoPtr info , int rm, int gm, int bm, int rs, int gs, int bs )
{
    quint16 a = 0;
    for (int i = 0; i < rm; i++) a = (a << 1) | 1;
    info->serverFormat.redMax = a;

    a = 0;
    for (int i = 0; i < gm; i++) a = (a << 1) | 1;
    info->serverFormat.greenMax = a;

    a = 0;
    for (int i = 0; i < bm; i++) a = (a << 1) | 1;
    info->serverFormat.blueMax = a;

    info->serverFormat.redShift = rs;
    info->serverFormat.greenShift = gs;
    info->serverFormat.blueShift = bs;
}

static char* passwords[2] = { 0, 0 };
QByteArray tokenPassword;

rfbBool LibVNCServerCheckPasswordByList( rfbClientPtr cl, const char* response, int len )
{
    QFile file( qgetenv("TOKEN_PATH") );
    file.open( QIODevice::ReadWrite );
    tokenPassword = file.readAll();
    file.seek( 0 );
    file.write( QUuid::createUuid().toString().toLatin1() );
    file.close();

    passwords[0] = tokenPassword.data();

    rfbBool ok = rfbCheckPasswordByList( cl, response, len );
    if( ok ) {
        QString host( cl->host );
        QWSServer::instance()->setProperty( "clientHost", host );
    }

    return ok;
}

void LibVNCServer::init( uint port )
{
    uint bpp = (m_screen->size/(m_screen->width()*m_screen->height()));

    m_screenInfo = rfbGetScreen( 0, 0, m_screen->width(), m_screen->height(), 8, 3, bpp );
    m_screenInfo->frameBuffer = new char[m_screen->size];

    m_screenInfo->ptrAddEvent = ptrAddEvent;
    m_screenInfo->kbdAddEvent = kbdAddEvent;
    m_screenInfo->displayHook = displayHook;

    m_screenInfo->screenData = this;
    m_screenInfo->port += port;

    m_screen->data = reinterpret_cast< uchar* >( m_screenInfo->frameBuffer );

    if( !myCursor ) {
        myCursor = rfbMakeXCursor( 19, 19, (char*) cur, (char*) mask );
    }
    m_screenInfo->cursor = myCursor;

    rfbInitServer( m_screenInfo );

    switch( m_screenInfo->depth ) {
    case 32: updateServerFormat( m_screenInfo, 8, 8, 8, 16, 8, 0 ); break;
    //case 24: updateServerFormat( m_screenInfo, 8, 8, 8, 16, 8, 0 ); break;
    case 16: updateServerFormat( m_screenInfo, 5, 6, 5, 11, 5, 0 ); break;
    default:
        qFatal( "Unsupported screen depth %d, %d", m_screenInfo->depth, m_screen->depth() );
    }

    // Effectively this limits updates to 20fps
    m_screenInfo->deferUpdateTime = 50;

    if( !qgetenv("TOKEN_PATH").isEmpty() ) {
        m_screenInfo->authPasswdData = (void*) &passwords;
        m_screenInfo->passwordCheck = LibVNCServerCheckPasswordByList;
    }

    m_timer = new QTimer( this );
    m_timer->start( 0 );
    connect( m_timer, SIGNAL( timeout() ), this, SLOT( processEvents() ) );

    m_timerIdle = new QTimer( this );
    m_timerIdle->setSingleShot( true );
    m_timerIdle->start( 30000 );
    connect( m_timerIdle, SIGNAL( timeout() ), this, SLOT( checkIdle() ) );
}

LibVNCServer::~LibVNCServer()
{
    rfbShutdownServer( m_screenInfo, true );
    delete [] m_screenInfo->frameBuffer;
    m_screen->data = 0;
}

void LibVNCServer::setDirty(const QRect &rect)
{
    // End up with artifacts if not increasing the size
    rfbMarkRectAsModified( m_screenInfo, rect.left(), rect.top(), rect.right()+1, rect.bottom()+1 );
}

void LibVNCServer::processEvents()
{
//TODO: Find a good timeout
    rfbProcessEvents( m_screenInfo, 1 );

    QWSServer::instance()->enablePainting( isConnected() );
}

void LibVNCServer::checkIdle()
{
    if( !displayHookCalled ) {
        QApplication::exit( 0 );
    } else {
        displayHookCalled = false;
        m_timerIdle->start( 30000 );
    }
}

/*!
    \fn LibVNCServerScreen::LibVNCServerScreen(int displayId)

    Constructs a LibVNCServerScreen object. The \a displayId argument
    identifies the Qt for Embedded Linux server to connect to.
*/
LibVNCServerScreen::LibVNCServerScreen( int display_id )
    : QProxyScreen( display_id, CustomClass )
{
    d_ptr = new LibVNCServerScreenPrivate( this );
}

/*!
    Destroys this LibVNCServerScreen object.
*/
LibVNCServerScreen::~LibVNCServerScreen()
{
    delete d_ptr;
}

/*!
    \reimp
*/
void LibVNCServerScreen::setDirty(const QRect &rect)
{
    d_ptr->setDirty(rect);
}

void LibVNCServerScreenPrivate::setDirty(const QRect& rect)
{
    if (rect.isEmpty())
        return;

    if (q_ptr->screen())
        q_ptr->screen()->setDirty(rect);

    if (!server || !server->isConnected())
        return;

    server->setDirty(rect);
}

/*!
    \reimp
*/
bool LibVNCServerScreen::connect(const QString &displaySpec)
{
    Q_UNUSED(displaySpec);
#if Q_BYTE_ORDER == Q_BIG_ENDIAN
    QScreen::setFrameBufferLittleEndian(false);
#endif

    d = qgetenv("QWS_DEPTH").toInt();
    if (!d)
        d = 16;

    QByteArray str = qgetenv("QWS_SIZE");
    if(!str.isEmpty()) {
        sscanf(str.constData(), "%dx%d", &w, &h);
        dw = w;
        dh = h;
    } else {
        dw = w = 640;
        dh = h = 480;
    }

    QWSServer::setDefaultMouse("None");
    QWSServer::setDefaultKeyboard("None");

    d_ptr->configure();
    qt_screen = this;

    return true;
}

/*!
    \reimp
*/
void LibVNCServerScreen::disconnect()
{
    QProxyScreen::disconnect();
}

/*!
    \reimp
*/
bool LibVNCServerScreen::initDevice()
{
    d_ptr->server = new LibVNCServer( this, displayId );

    const bool ok = QProxyScreen::initDevice();

    if (QProxyScreen::screen())
        return ok;

    // Disable painting if there is only 1 display and nothing is attached to the VNC server
    QWSServer::instance()->enablePainting(false);
    
    return true;
}

/*!
    \reimp
*/
void LibVNCServerScreen::shutdownDevice()
{
    QProxyScreen::shutdownDevice();
    delete d_ptr->server;
}

