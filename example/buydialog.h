/*
 * Copyright (C) 2010, Connected Table AB <info@connectedtable.com>
 */

#ifndef BUYDIALOG_H
#define BUYDIALOG_H

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
    QGraphicsProxyWidget* m_dialogItem;
    Ui::BuyDialog* m_ui;
};

#endif /* BUYDIALOG_H */
