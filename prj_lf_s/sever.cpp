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
         << tr("you might have mail.")
         << tr("you connot kill time without injuring eternity.")
         << tr("computers are not intelligent. they only think they are.");

QPushButton *quitButton = new QPushButton(tr("quit"));
quitButton->setAutoDefault(false);
connect(quitButton, &QPushButton::clicked,this,&Server::close);
connect(server,&QLocalServeur::newConnection, this, &Server::sendFortune);

QHBoxLayout *buttonLayout = new QHBoxLayout;
buttonLayout->addStretch(1);
buttonLayout->addWidget(quitButton);
buttonLayout->addStretch(1);

setWindowTitle(QGuiApplication::applicationDisplayName());
}

void Server::sendFortune()
{
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_10);
    const int fortuneIndex = QRandomGenerator::global()->bounded(0,fortunes.size());
    const QString &message = fortunes.at(fortuneIndex);
    out <<quint32(message.size());
    out <<message;

    QLocalSocket *clientConnection = server->nextPendingConnection();
    connect(clientConnection, &QLocalSocket::disconnected,
            clientConnection, &QLocalSocket::deleteLater);

    clientConnection->write(block);
    clientConnection->flush();
    clientConnection->disconectFromServer();

}
