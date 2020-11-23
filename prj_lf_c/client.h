#ifndef CLIENT_H
#define CLIENT_H

#include<QDialog>
#include <QDataStream>
#include<QLocalSocket>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
};

#endif // CLIENT_H
