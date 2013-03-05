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

