/*
 *  Copyright (C) 2010 Connected Table AB <info@connectedtable.com>
 *  All Rights Reserved.
 *
 *  This is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This software is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this software; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307,
 *  USA.
 */

#include <qscreendriverplugin_qws.h>
#include <libvncserverscreen_qws.h>
#include <qstringlist.h>

class GfxLibVncServerDriver : public QScreenDriverPlugin
{
public:
    GfxLibVncServerDriver();

    QStringList keys() const;
    QScreen *create(const QString&, int displayId);
};

GfxLibVncServerDriver::GfxLibVncServerDriver()
: QScreenDriverPlugin()
{
}

QStringList GfxLibVncServerDriver::keys() const
{
    return ( QStringList() << "libvnc" );
}

QScreen* GfxLibVncServerDriver::create(const QString& driver, int displayId)
{
    if (driver.toLower() == "libvnc")
        return new LibVNCServerScreen(displayId);

    return 0;
}

Q_EXPORT_STATIC_PLUGIN(GfxLibVncServerDriver)
Q_EXPORT_PLUGIN2(vncserverscreendriver, GfxLibVncServerDriver)

