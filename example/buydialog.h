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
