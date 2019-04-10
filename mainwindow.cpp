#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    m_chart = nullptr;
    m_chartSeries = nullptr;
}

MainWindow::~MainWindow()
{
    delete ui;
    if(m_chart != nullptr)
        delete m_chart;
    if(m_chartSeries != nullptr)
        delete m_chartSeries;
}

void MainWindow::on_generateButton_pressed()
{
    ui->generateButton->setEnabled(false);

    // Calculating trajectory values

    const double g = 9.81;
    double time_s = ui->timeSpinBox->value()/1000.0;
    double v0 = (ui->heightSpinBox->value()+0.5*g*time_s*time_s)/time_s;

    double t_p = v0/g;
    double h = (v0*t_p-0.5*g*t_p*t_p);
    ui->initialValue->setText(QString("<span style='color:red'>%1</span> m/s").arg(v0));
    ui->heightValue->setText(QString("<span style='color:red'>%1</span> m").arg(QString::number(h, 'f',1)));
    ui->apogeeValue->setText(QString("<span style='color:red'>%1</span> s").arg(t_p));

    // Generating chart

    if(m_chartSeries != nullptr)
        delete m_chartSeries;

    if(m_chart != nullptr)
        delete m_chart;

    m_chartSeries = new QScatterSeries();
    m_chartSeries->append(100, h);


    m_chart = new Chart(m_chartSeries, "The flight path of the object", "x", "height [m]", 200,
                        h+h/4, "Bullet trajectory");

    ui->chartWidget->setWidget(m_chart);


    ui->generateButton->setEnabled(true);
}
