/*
 * Copyright (C) 2010, Connected Table AB <info@connectedtable.com>
 */

#ifndef PAINTERPATHWIDGET_H
#define PAINTERPATHWIDGET_H

#include <QWidget>

class PainterPathWidget : public QWidget {
Q_OBJECT
public:
    explicit PainterPathWidget( QWidget* parent=0 );
    virtual ~PainterPathWidget();

    /*reimp*/QSize minimumSizeHint() const;
    /*reimp*/QSize sizeHint() const;

    QString text() const;
public Q_SLOTS:
    void setText( const QString& str );
    void setPen( const QPen& pen );
    void setBrush( const QBrush& brush );
    void setTrueOutline( bool on );
protected:
    /*reimp*/ void paintEvent( QPaintEvent* ev );
private:
    struct Private;
    Private* d;
};

#endif /* PAINTERPATHWIDGET_H */

