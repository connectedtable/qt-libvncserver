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

#ifndef LIBVNCSERVERSCREEN_QWS_H
#define LIBVNCSERVERSCREEN_QWS_H

#include <QtGui/qscreenproxy_qws.h>

class LibVNCServer;
class LibVNCServerCursor;
class LibVNCServerScreenPrivate;

class LibVNCServerScreen : public QProxyScreen
{
public:
    explicit LibVNCServerScreen(int display_id);
    virtual ~LibVNCServerScreen();

    bool initDevice();
    bool connect(const QString &displaySpec);
    void disconnect();
    void shutdownDevice();

    void setDirty(const QRect&);

private:
    friend class LibVNCServer;
    friend class LibVNCServerCursor;
    friend class LibVNCServerScreenPrivate;

    LibVNCServerScreenPrivate *d_ptr;
};

#endif // LIBVNCSERVERSCREEN_QWS_H
