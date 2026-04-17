#include "monitor.h"
#include "ui_monitor.h"

Monitor::Monitor(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Monitor)
  ,main_ui(nullptr)
{

    ui->setupUi(this);
}

Monitor::~Monitor()
{
    delete ui;
}

void Monitor::on_pushButton_clicked()
{
    this->hide();

    if(!main_ui){

        main_ui->show();
    }
    else{


        }

    }


