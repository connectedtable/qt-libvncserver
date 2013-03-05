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

#ifndef BUYDIALOG_H
#define BUYDIALOG_H

class QFrame;
#include <QGraphicsProxyWidget>
#include <QGraphicsScene>
#include <QTimer>

namespace Ui { class BuyDialog; }

class BuyDialog : public QObject {
Q_OBJECT
public:
    explicit BuyDialog( QObject* parent=0 );
    virtual ~BuyDialog();

    void setScene( QGraphicsScene* scene );

Q_SIGNALS:
    void cancel();
private Q_SLOTS:
    void slotValueChanged( int val );
private:
    qreal roundSliderValue() const;

    QWidget* m_dialog;
    QWidget* m_dialogWidget;
    QFrame* m_dialogFrame;
    QGraphicsProxyWidget* m_dialogItem;
    QGraphicsProxyWidget* m_dialogBackgroundProxyWidget;
    Ui::BuyDialog* m_ui;
};

#endif /* BUYDIALOG_H */
