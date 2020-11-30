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

}



