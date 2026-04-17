#include "widget.h"
#include "ui_widget.h"

#include <QDebug>
#include<fsmpBeeper.h>
#include<QPixmap>
#include"monitor.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
    ,time(new QTimer(this))
    ,monitor_ui(nullptr)
{
    ui->setupUi(this);

    ui->horizontalSlider->setRange(0,100);


    QPixmap pix;
    pix.load(":/zjj.jpg");
    ui->label_3->setPixmap(pix);


    connect(&myevent,&fsmpEvents::keyPressed,this,&Widget:: pushbutton);

    connect(time,&QTimer :: timeout,this,&Widget ::timout);
    time->start(1000);

}

Widget::~Widget()
{
    delete ui;
}


void Widget::on_pushButton_clicked()
{
    if(ui->pushButton->text()=="开灯"){

        myled.on(fsmpLeds::LED1);
        ui->pushButton->setText("关灯");
    }
    else{
        myled.off(fsmpLeds::LED1);
        ui->pushButton->setText("开灯");
    }
}


void Widget::on_pushButton_2_clicked()
{

}


void Widget::on_checkBox_stateChanged(int arg1)
{
    if(arg1){

        myled.on(fsmpLeds::LED2);    }
    else
    {

        myled.off(fsmpLeds::LED2);
    }
}



void Widget::on_checkBox_2_stateChanged(int arg1)
{
    if(arg1){

        mybeeper.setRate(5000);
        mybeeper.start();
    }
    else
    {
        mybeeper.stop();
    }
}


void Widget::on_horizontalSlider_valueChanged(int value)
{
    qDebug() << ui->horizontalSlider->value();

    mybeeper.setRate(value);
    mybeeper.start();

    ui->progressBar->setValue(ui->horizontalSlider->value());
}


void Widget::on_checkBox_3_stateChanged(int arg1)
{
    if(arg1){

            myfan.setSpeed(100);
            myfan.start();
        }
        else
        {
            myfan.stop();
        }
}
void Widget:: pushbutton(int num){

  switch(num){

  case 1:
      myled.on(fsmpLeds::LED1);
      break;
  case 2:
      myled.on(fsmpLeds::LED2);
      break;
  case 3:
      myled.on(fsmpLeds::LED3);
      break;
  default:
      break;
  }

};

void Widget::timout(){

    ui->doubleSpinBox->setValue(myth.temperature());
    ui->doubleSpinBox_2->setValue(myth.humidity());

};


void Widget::on_pushButton_3_clicked()
{
    if(!monitor_ui){

        monitor_ui=new Monitor(nullptr);

    }


    this->hide();

    monitor_ui->show();
}

