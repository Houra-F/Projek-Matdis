#include "mainwindow.h"

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGroupBox>
#include <QPushButton>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle(
        "Visualisasi Rute Terpendek (Dijkstra)");

    resize(1100,700);

    setStyleSheet(
        "background:#F8FAFC;"
        );

    QWidget *center =
        new QWidget(this);

    setCentralWidget(center);

    canvasGraf =
        new GraphCanvas();

    QGroupBox *menu =
        new QGroupBox("Menu Navigasi");

    menu->setFixedWidth(280);

    QVBoxLayout *menuLayout =
        new QVBoxLayout(menu);

    QLabel *lblAwal =
        new QLabel("Lokasi Awal");

    cmbAwal =
        new QComboBox();

    cmbAwal->addItems({
        "A","B","C","D",
        "E","F","G","H"
    });

    QLabel *lblTujuan =
        new QLabel("Lokasi Tujuan");

    cmbTujuan =
        new QComboBox();

    cmbTujuan->addItems({
        "A","B","C","D",
        "E","F","G","H"
    });

    QPushButton *btnHitung =
        new QPushButton(
            "Hitung Rute");

    btnHitung->setStyleSheet(
        "background:#3B82F6;"
        "color:white;"
        "font-weight:bold;"
        "padding:10px;"
        "border-radius:8px;");

    QLabel *judulHasil =
        new QLabel(
            "Hasil Analisis");

    lblRute =
        new QLabel(
            "Belum Ada Input");

    lblJarak =
        new QLabel("-");

    lblRute->setStyleSheet(
        "font-size:18px;"
        "font-weight:bold;"
        "color:#2563EB;");

    lblJarak->setStyleSheet(
        "font-size:22px;"
        "font-weight:bold;"
        "color:#22C55E;");

    menuLayout->addWidget(lblAwal);
    menuLayout->addWidget(cmbAwal);

    menuLayout->addWidget(lblTujuan);
    menuLayout->addWidget(cmbTujuan);

    menuLayout->addSpacing(10);

    menuLayout->addWidget(btnHitung);

    menuLayout->addSpacing(20);

    menuLayout->addWidget(judulHasil);

    menuLayout->addWidget(lblRute);

    menuLayout->addWidget(lblJarak);

    menuLayout->addStretch();

    QHBoxLayout *layout =
        new QHBoxLayout(center);

    layout->addWidget(
        canvasGraf,
        3);

    layout->addWidget(
        menu,
        1);

        inisialisasiGraf();

    connect(
        btnHitung,
        &QPushButton::clicked,
        this,
        &MainWindow::hitungRute);
}

MainWindow::~MainWindow()
{
}

void MainWindow::inisialisasiGraf()
{
    dijkstra.tambahEdge(
        "A","B",3);

    dijkstra.tambahEdge(
        "B","C",2);

    dijkstra.tambahEdge(
        "C","D",4);

    dijkstra.tambahEdge(
        "D","E",2);

    dijkstra.tambahEdge(
        "E","F",3);

    dijkstra.tambahEdge(
        "F","H",2);

    dijkstra.tambahEdge(
        "B","G",2);

    dijkstra.tambahEdge(
        "G","E",1);

    dijkstra.tambahEdge(
        "F","G",2);

    dijkstra.tambahEdge(
        "C","G",1);
}

void MainWindow::hitungRute()
{
    QString awal =
        cmbAwal->currentText();

    QString tujuan =
        cmbTujuan->currentText();

    int jarak;

    QStringList path =
        dijkstra.cariRute(
            awal,
            tujuan,
            jarak);

    lblRute->setText(
        path.join(
            " → "));

    lblJarak->setText(
        QString::number(
            jarak)
        + " km");

    canvasGraf->setRuteAktif(
        path);
}