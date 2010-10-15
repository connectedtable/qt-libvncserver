#include <qscreendriverplugin_qws.h>
#include <libvncserverscreen_qws.h>
#include <qstringlist.h>

QT_BEGIN_NAMESPACE

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

QT_END_NAMESPACE
