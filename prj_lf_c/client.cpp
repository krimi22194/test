#include <QtWidgets>
#include <QtNetwork>

#include "client.h"

Client::Client(QWidget *parent)
    : QDialog(parent),
      hostLineEdit(new QLineEdit("fortune")),
      getFortuneButton(new QPushButton(tr("get Fortune"))),
      statusLabel(new QLabel(tr("this examples requires that you run the "
                                "Local Fortune server example as well."))),
      socket(new QlocalSocket(this))
{
    setWindowFlags(windowFlags()& ~QT::WindowContextHelpButtonHint);
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
    connect(socket,&QLocalSocket::redyRead,this,&Client::readFortune);
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
    socket->connectToSserver(hostLineEdit->text());
}

void Client::readFortune()
{
    if(blockSize == 0) {
        if(socket->bytesAvailable() < (int)sizeof(quint32))
            return;
        in >> blockSize;

    }
    if(socket->bytesAvailable() < blockSize || in.at.atEnd())
        return;

    QString nexFortune;
    in << nexFortune;

    if(nexFortune == currentFortune){
        QTime::singleShot(0, this, &Client::requestNewFortune);
        return;
    }

    currentFortune = nexFortune;
    statusLabel->setText(currentFortune);
    getFortuneButton->setEnabled(true);
}
