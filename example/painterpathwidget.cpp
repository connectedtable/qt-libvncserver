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

#include "painterpathwidget.h"

#include <QString>
#include <QPainterPath>
#include <QPainter>
#include <QPen>
#include <QBrush>

struct PainterPathWidget::Private {
    QString text;
    QPen pen;
    QBrush brush;
    QPainterPath path;
    bool useTrueOutline;
};

PainterPathWidget::PainterPathWidget( QWidget* parent )
    : QWidget( parent ),
      d( new Private )
{
    setSizePolicy( QSizePolicy::Fixed, QSizePolicy::Fixed );
    d->pen = QPen( palette().color( QPalette::Foreground ) );
    d->brush = palette().brush( QPalette::Foreground );
    d->useTrueOutline = false;
}

PainterPathWidget::~PainterPathWidget()
{
    delete d; d=0;
}

QSize PainterPathWidget::minimumSizeHint() const
{
    return sizeHint();
}

QSize PainterPathWidget::sizeHint() const
{
    const int penOffset = static_cast< int >( d->pen.width() + 5 );
    return d->path.boundingRect().size().toSize() + QSize( penOffset, penOffset );
}

QString PainterPathWidget::text() const
{
    return d->text;
}

QPainterPath makePath( const QFont& f, const QString& str )
{
    QPainterPath path;
    const QStringList lines = str.split( "\n" );
    const QFontMetricsF fm( f );

    qreal offset = 0.0;
    Q_FOREACH( QString line, lines ) {
        const qreal lineWidth = fm.width( line ) / 2.0;
        if ( lineWidth > offset ) {
            offset = lineWidth;
        }
    }

    qreal y = 0.0;
    Q_FOREACH( QString line, lines ) {
        const qreal x = offset - ( fm.width( line ) / 2.0 );
        path.addText( x, y, f, line );
        y += fm.height();
    }
    return path;
}

void PainterPathWidget::setText( const QString& text )
{
    if ( d->text == text ) return;
    d->text = text;
    d->path = makePath( font(), d->text );
    updateGeometry();
    update();
}

void PainterPathWidget::setPen( const QPen& pen )
{
    d->pen = pen;
    updateGeometry();
    update();
}

void PainterPathWidget::setBrush( const QBrush& brush )
{
    d->brush = brush;
    update();
}

void PainterPathWidget::setTrueOutline( bool on )
{
    d->useTrueOutline = on;
    update();
}

void PainterPathWidget::paintEvent( QPaintEvent* )
{
    QPainter painter( this );
    painter.setPen( d->pen );
    painter.setBrush( d->brush );
    painter.translate( -d->path.boundingRect().left() + d->pen.width(), -d->path.boundingRect().top() + d->pen.width() );
    if ( d->useTrueOutline ) {
        painter.strokePath( d->path, painter.pen() );
        painter.fillPath( d->path, painter.brush() );
    }
    else painter.drawPath( d->path );
}
