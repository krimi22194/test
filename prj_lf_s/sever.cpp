#include "server.h"
#include <QtWidgets>
#include <QtNetwork>

Server::server(QWidget *parent)
    : QDialog(parent)
{

setWindowFlags(windowFlags() & -Qt::WindowContextHelpButtonHint);
