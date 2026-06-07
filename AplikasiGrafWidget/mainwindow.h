#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QComboBox>
#include <QLabel>

#include "graphcanvas.h"
#include "dijkstra.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void hitungRute();

private:

    void inisialisasiGraf();

    GraphCanvas *canvasGraf;

    QComboBox *cmbAwal;
    QComboBox *cmbTujuan;

    QLabel *lblRute;
    QLabel *lblJarak;

    Dijkstra dijkstra;
};

#endif