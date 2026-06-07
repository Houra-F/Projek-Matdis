#include "mainwindow.h"

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGroupBox>
#include <QPushButton>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle("Visualisasi Rute Terpendek (Dijkstra) - Sektor Lengkap");
    resize(1150, 720);

    setStyleSheet("background: #F8FAFC; color: #000000;");

    QWidget *center = new QWidget(this);
    setCentralWidget(center);

    canvasGraf = new GraphCanvas();

    QGroupBox *menu = new QGroupBox("Menu Navigasi");
    menu->setFixedWidth(280);
    menu->setStyleSheet("QGroupBox { font-weight: bold; color: #0F172A; }");

    QVBoxLayout *menuLayout = new QVBoxLayout(menu);

    QLabel *lblAwal = new QLabel("Lokasi Awal");
    cmbAwal = new QComboBox();
    // Dropdown lengkap berisi node A sampai N
    cmbAwal->addItems({"A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L", "M", "N"});
    cmbAwal->setStyleSheet("padding: 4px; background: white;");

    QLabel *lblTujuan = new QLabel("Lokasi Tujuan");
    cmbTujuan = new QComboBox();
    cmbTujuan->addItems({"A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L", "M", "N"});
    cmbTujuan->setStyleSheet("padding: 4px; background: white;");

    QPushButton *btnHitung = new QPushButton("Hitung Rute");
    btnHitung->setStyleSheet(
        "background:#3B82F6;"
        "color:white;"
        "font-weight:bold;"
        "padding:10px;"
        "border-radius:8px;");

    QLabel *judulHasil = new QLabel("Hasil Analisis");

    lblRute = new QLabel("Belum Ada Input");
    lblJarak = new QLabel("-");

    lblRute->setStyleSheet("font-size:16px; font-weight:bold; color:#2563EB;");
    lblJarak->setStyleSheet("font-size:22px; font-weight:bold; color:#22C55E;");

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

    QHBoxLayout *layout = new QHBoxLayout(center);
    layout->addWidget(canvasGraf, 3);
    layout->addWidget(menu, 1);

    inisialisasiGraf();

    connect(btnHitung, &QPushButton::clicked, this, &MainWindow::hitungRute);
}

MainWindow::~MainWindow()
{
}

void MainWindow::inisialisasiGraf()
{
    // --- DATABASE LOGIKA DIJKSTRA (A SAMPAI N) ---
    // Jalur Sisi Kiri
    dijkstra.tambahEdge("A", "B", 4); // Pintu Masuk -> Gedung A
    dijkstra.tambahEdge("B", "C", 2); // Gedung A -> Musholla
    dijkstra.tambahEdge("C", "D", 3); // Musholla -> Parkiran L
    dijkstra.tambahEdge("D", "E", 2); // Parkiran L -> USU Press

    // Jalur Sisi Kanan
    dijkstra.tambahEdge("F", "G", 2); // Keluar -> Gedung B
    dijkstra.tambahEdge("G", "H", 7); // Gedung B -> Pendopo
    dijkstra.tambahEdge("H", "I", 6); // Pendopo -> Gedung C
    dijkstra.tambahEdge("I", "J", 1); // Gedung C -> Gedung D
    dijkstra.tambahEdge("J", "K", 2); // Gedung D -> Oktagon

    // Jalur Akses Penghubung Tengah
    dijkstra.tambahEdge("K", "N", 7); // Oktagon -> Ten Cafe
    dijkstra.tambahEdge("K", "L", 1); // Oktagon -> Wifi Corner
    dijkstra.tambahEdge("L", "M", 1); // Wifi Corner -> PSI
    dijkstra.tambahEdge("M", "I", 3); // PSI -> Gedung C

    // === DEKLARASI HUBUNGAN LOGIKA BARU ===
    dijkstra.tambahEdge("A", "F", 3); // Jalur Pintu Masuk (A) bisa tembus ke Keluar (F) langsung
    dijkstra.tambahEdge("E", "N", 4); // Jalur Atas: USU Press (E) tembus ke Ten Cafe (N)
    dijkstra.tambahEdge("B", "M", 5); // Jalur Tengah-Bawah: Gedung A (B) tembus ke PSI (M)
}

void MainWindow::hitungRute()
{
    QString awal = cmbAwal->currentText();
    QString tujuan = cmbTujuan->currentText();
    int jarak;

    QStringList path = dijkstra.cariRute(awal, tujuan, jarak);

    lblRute->setText(path.join(" → "));
    lblJarak->setText(QString::number(jarak) + " m");

    canvasGraf->setRuteAktif(path);
}