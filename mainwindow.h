#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "chart.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_generateButton_pressed();

private:
    Ui::MainWindow *ui;
    Chart *m_chart;
    QScatterSeries *m_chartSeries;
};

#endif // MAINWINDOW_H
