#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_generateButton_pressed()
{
    ui->generateButton->setEnabled(false);
    const double g = 9.81;
    double time_s = ui->timeSpinBox->value()/1000.0;
    double v0 = (ui->heightSpinBox->value()+0.5*g*time_s*time_s)/time_s;

    double t_p = v0/g;
    double h = (v0*t_p-0.5*g*t_p*t_p);
    ui->initialValue->setText(QString("<span style='color:red'>%1</span>m/s").arg(v0));
    ui->heightValue->setText(QString("<span style='color:red'>%1</span>m").arg(h));
    ui->apogeeValue->setText(QString("<span style='color:red'>%1</span>s").arg(t_p));

    ui->generateButton->setEnabled(true);
}
