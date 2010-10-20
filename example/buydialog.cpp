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

#include "buydialog.h"
#include "ui_buydialog.h"

#include <QFrame>
#include <QPushButton>
#include <QPen>
#include <QSlider>

#include <cmath>

BuyDialog::BuyDialog( QObject* parent ) : QObject( parent )
{
    m_dialog = new QWidget;
    m_dialog->setAttribute( Qt::WA_NoSystemBackground );

    m_dialogItem = new QGraphicsProxyWidget();
    m_dialogItem->setWidget( m_dialog );
    m_dialogItem->setZValue( 2 );

    m_dialogWidget = new QWidget();
    m_dialogWidget->setObjectName( "DialogWidget" );
    m_dialogWidget->setAttribute( Qt::WA_NoSystemBackground );

    m_dialogFrame = new QFrame( m_dialogWidget );
    m_dialogFrame->setObjectName( "DialogFrame" );
    m_dialogFrame->setFrameStyle( QFrame::StyledPanel );
    m_dialogFrame->setFrameShadow( QFrame::Raised );
    m_dialogFrame->setStyleSheet( "background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 #FFFFFF, stop: 1 #848383); border-style: outset; border-width: 2px; border-radius: 10px; border-color: beige;" );

    m_dialogBackgroundProxyWidget = new QGraphicsProxyWidget;
    m_dialogBackgroundProxyWidget->setWidget( m_dialogWidget );
    m_dialogBackgroundProxyWidget->setZValue( 1 );

    m_ui = new Ui::BuyDialog;
    m_ui->setupUi( m_dialog );

    m_ui->buyinSlider->setSingleStep( 100*1 );
    m_ui->buyinSlider->setPageStep( 100*2 );
    m_ui->buyinSlider->setRange( static_cast< int >( 20 * 100 ), static_cast< int >( 200 * 100 ) );
    m_ui->buyinSlider->setValue( m_ui->buyinSlider->minimum() );

    QPushButton* cancelButton = new QPushButton( tr( "Cancel" ), m_dialog );
    m_ui->buttonLayout->addWidget( cancelButton );

    connect( cancelButton, SIGNAL( clicked() ), qApp, SLOT( quit() ) );
    connect( m_ui->buyinSlider, SIGNAL( valueChanged( int ) ), this, SLOT( slotValueChanged( int ) ) );

    m_ui->amountWidget->setStyleSheet( "font-size: 60px" );
    m_ui->amountWidget->setPen( QPen( Qt::black, 4 ) );
    m_ui->amountWidget->setBrush( QColor( 0xff, 0xf6, 0x5e ) );
    m_ui->amountWidget->setTrueOutline( true );
}

BuyDialog::~BuyDialog()
{
    delete m_ui;
}

void BuyDialog::setScene( QGraphicsScene* scene )
{
    const QObjectList& widgets = m_dialog->children();
    for( qint32 i = 0; i < widgets.size(); i++) {
        QWidget* widget = qobject_cast<QWidget*>( widgets.at( i ) );
        if ( widget )
            widget->show();
    }
    m_dialog->adjustSize();

    QSize sceneSize( 1024, 768 );
    m_dialogWidget->setFixedSize( sceneSize );
    const QSize dialogFrameSize = m_dialog->size();
    qint32 xPosition;
    qint32 yPosition;
    xPosition = ( sceneSize.width() - dialogFrameSize.width() ) / 2;
    yPosition = ( sceneSize.height() - dialogFrameSize.height() ) / 2;
    QPoint dialogFramePosition( xPosition, yPosition );
    m_dialogFrame->setGeometry( QRect( dialogFramePosition, dialogFrameSize ) );
    m_dialog->setGeometry( m_dialogFrame->geometry() );

    scene->addItem( m_dialogItem );
    scene->addItem( m_dialogBackgroundProxyWidget );
}

void BuyDialog::slotValueChanged( int )
{
    m_ui->amountWidget->setText( QString::number( roundSliderValue(), 'f', 0 ) );
}

qreal BuyDialog::roundSliderValue() const
{
    qreal factor = m_ui->buyinSlider->singleStep();
    qreal value = m_ui->buyinSlider->value();

    if( m_ui->buyinSlider->value() == m_ui->buyinSlider->maximum() || m_ui->buyinSlider->value() == m_ui->buyinSlider->minimum() ) {
        return value / 100;
    } else {
        return ( qRound( ::floor( value ) / factor ) * factor ) / 100;
    }
}

