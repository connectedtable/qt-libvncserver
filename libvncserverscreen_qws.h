#ifndef QSCREENLIBVNCSERVER_QWS_H
#define QSCREENLIBVNCSERVER_QWS_H

#include <QtGui/qscreenproxy_qws.h>

QT_BEGIN_HEADER

QT_BEGIN_NAMESPACE

QT_MODULE(Gui)

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

QT_END_NAMESPACE

QT_END_HEADER

#endif // QSCREENLIBVNCSERVER_QWS_H
