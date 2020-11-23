#ifndef SERVER_H
#define SERVER_H

#include <QDialog>

QT_BEGIN_NAMESPACE
class QLabel;
class QPushButton;
class QLocalServeur;
QT_END_NAMESPACE
class serveur : public QDialog
{
    Q_OBJECT

public:
    explicit serveur(QWidget *parent = nullptr);

private slots:
    void sendFortune();

private:
    QLocalServeur *serveur;
    QStringList fortunes;
};

#endif
