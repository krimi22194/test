#ifndef CLIENT_H
#define CLIENT_H

#include <QDialog>
#include <QDataStream>
#include<QLocaleSocket>

QT_BEGIN_NAMESPACE
class QLabel;
class QLineEdit;
class QPushButton;
QT_END_NAMESPACE

class Client : public QDialog
{
    Q_OBJECT

public:
    explicit Client(QWidget *parent = nullptr);

private slots:
    void requestNewFortune();
    void readFortune();
    void displayError(QLocalSocket::LocalSockettError socketError);
    void enableGetFortuneButton();

   private:
    QLineEdit *hostLineEdit;
    QPushButton *getFortuneButton;
    QLabel *statusLabel;

    QLocalSocket *socket;
    QDataStream in;
    quint32 blockSize;

    QString currentFortune;
};

#endif
