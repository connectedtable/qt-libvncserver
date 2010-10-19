/*
 * Copyright (C) 2010, Connected Table AB <info@connectedtable.com>
 */

#include <QApplication>
#include <QGraphicsScene>
#include <QGraphicsView>

#include "buydialog.h"

int main( int argc, char** argv ) {
    QApplication app( argc, argv );

    QGraphicsScene scene;
    QGraphicsView view( &scene );
    view.setFixedSize( 1024, 768 );
    view.showFullScreen();
    view.adjustSize();

    BuyDialog dialog;
    dialog.setScene( &scene );

    return app.exec();
} 
