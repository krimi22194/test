#include "server.h"
#include <QtWidgets>
#include <QtNetwork>

Server::Server(QWidget *parent)
    : QDialog(parent)
{

setWindowFlags(windowFlags() & -Qt::WindowContextHelpButtonHint);

server = new QLocalServer(this);
if(!server->listen("fortune")){
    QMessageBox::critical(this, tr("local Fortune server"),
                          tr("Unable to start the serveur:%1.")
                          .arg(server->errorString()));

    close();
    return;
}

QLabel *statusLabel = new QLabel;
statusLabel->setWordWrap(true);
statusLabel->setText(tr("the server is running.\n"
                        "run the Local Fortune Client example now."));

fortunes << tr("you've been leading a dog's life. Stay off the furniture")
         << tr("you've got to think about tomorrow.")
         << tr("you will be surprised by a loud noise.")
         << tr("you will feel hungry again in another hour.")
         << tr("")
