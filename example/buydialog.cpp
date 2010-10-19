/*
 * Copyright (C) 2010, Connected Table AB <info@connectedtable.com>
 */

#include "buydialog.h"
#include "ui_buydialog.h"

#include <QPushButton>
#include <QPen>
#include <QSlider>

#include <cmath>

BuyDialog::BuyDialog( QObject* parent ) : QObject( parent )
{
    m_dialog = new QWidget;
    m_dialogItem = new QGraphicsProxyWidget();
    m_dialogItem->setWidget( m_dialog );
    m_dialogWidget = new QWidget();
    m_dialogWidget->setObjectName( "DialogWidget" );

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
    m_dialog->adjustSize();

    QRect sceneRect( scene->sceneRect().toRect() );
    m_dialogWidget->setGeometry( sceneRect );
    const QSize dialogFrameSize = m_dialog->size();
    qint32 xPosition;
    qint32 yPosition;
    xPosition = ( sceneRect.width() - dialogFrameSize.width() ) / 2;
    yPosition = ( sceneRect.height() - dialogFrameSize.height() ) / 2;
    QPoint dialogFramePosition( xPosition, yPosition );
    m_dialog->setGeometry( QRect( dialogFramePosition, dialogFrameSize ) );
    scene->addItem( m_dialogItem );
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

