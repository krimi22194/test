#include <QtWidgets>
#include <QtNetwork>

#include "client.h"

Client::Client(QWidget *parent)
    : QDialog(parent),
      hostLineEdit(new QLineEdit("fortune")),
      getFortuneButton(new QPushButton(tr("get Fortune"))),
      statusLabel(new QLabel(tr("this examples requires that you run the "
                                "Local Fortune server example as well."))),
      socket(new QLocalSocket(this))
{
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
    QLabel *hostLabel = new QLabel(tr("&Server name"));
    hostLabel->setBuddy(hostLineEdit);

    statusLabel->setWordWrap(true);

    getFortuneButton->setDefault(true);
    QPushButton *quitButton = new QPushButton(tr("quit"));

    QDialogButtonBox *buttonBox = new QDialogButtonBox;
    buttonBox->addButton(getFortuneButton, QDialogButtonBox::ActionRole);
    buttonBox->addButton(quitButton, QDialogButtonBox::RejectRole);

    in.setDevice(socket);
    in.setVersion(QDataStream::Qt_5_10);

    connect(hostLineEdit,&QLineEdit::textChanged,
            this, &Client::enableGetFortuneButton);
    connect(getFortuneButton,&QPushButton::clicked,
            this,&Client::requestNewFortune);
    connect(quitButton,&QPushButton::clicked,this, &Client::close);
    connect(socket,&QLocalSocket::readyRead,this,&Client::readFortune);
    connect(socket, QOverload<QLocalSocket::LocalSocketError>::of(&QLocalSocket::error),
            this,&Client::displayError);

    QGridLayout *mainLayout = new QGridLayout(this);
    mainLayout->addWidget(hostLabel,0,0);
    mainLayout->addWidget(hostLineEdit,0,1);
    mainLayout->addWidget(statusLabel,2,0,1,2);
    mainLayout->addWidget(buttonBox,3,0,1,2);

    setWindowTitle(QGuiApplication::applicationDisplayName());
    hostLineEdit->setFocus();
}

void Client::requestNewFortune()
{
    getFortuneButton->setEnabled(false);
    blockSize = 0;
    socket->abort();
    socket->connectToServer(hostLineEdit->text());
}

void Client::readFortune()
{
    if(blockSize == 0) {
        if(socket->bytesAvailable() < (int)sizeof(quint32))
            return;
        in >> blockSize;

    }
    if(socket->bytesAvailable() < blockSize || in.atEnd())
        return;

    QString nexFortune;
    in << nexFortune;

    if(nexFortune == currentFortune){
        QTimer::singleShot(0, this, &Client::requestNewFortune);
        return;
    }

    currentFortune = nexFortune;
    statusLabel->setText(currentFortune);
    getFortuneButton->setEnabled(true);
}
void Client::displayError(QLocalSocket::LocalSocketError socketError)
{
    switch (socketError){
    case QLocalSocket::ServerNotFoundError:
        QMessageBox::information(this, tr("local forrtune Client"),
                                 tr("the host was not found. please make sure"
                                    "that the server name is correct."));

        break;
    case QLocalSocket::ConnectionRefusedError:
        QMessageBox::information(this, tr("Local Fortune Client"),
                                 tr("the connection was refused by the peer."
                                    "make sure the fortune server is running",
                                    "is correct"));
        break;
    case QLocalSocket::PeerClosedError:
        break;
    default:
        QMessageBox::information(this, tr("Local Fortune Client"),
                                 tr("The following error occured: %1.")
                                 .arg(socket->errorString()));
    }

    getFortuneButton->setEnabled(true);
}
void Client::enableGetFortuneButton()
{
    getFortuneButton->setEnabled(!hostLineEdit->text().isEmpty());

}
