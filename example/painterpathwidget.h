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

