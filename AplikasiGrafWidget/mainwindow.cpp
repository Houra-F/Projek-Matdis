#include "mainwindow.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGroupBox>

// =============================================================================
//                      IMPLEMENTASI CANVAS PETA GRAF
// =============================================================================
GraphCanvas::GraphCanvas(QWidget *parent) : QWidget(parent) {
    setAttribute(Qt::WA_StyledBackground, true);
    // Background melengkung abu-abu soft khas Figma
    setStyleSheet("background-color: #E2E8F0; border: 1px solid #CBD5E1; border-radius: 20px;");
    setMinimumSize(550, 550);

    // Titik koordinat dan keterangan nama tempat sesuai foto/video
    m_nodes.append({"A", "", QPoint(60, 270)});
    m_nodes.append({"B", "Gerbang", QPoint(240, 150)});
    m_nodes.append({"C", "Fakultas", QPoint(420, 270)});
    m_nodes.append({"D", "Kantin (Tujuan)", QPoint(240, 420)});
}

void GraphCanvas::setRuteAktif(const QStringList &rute) {
    m_ruteAktif = rute;
    update(); // Memicu gambar ulang canvas
}

void GraphCanvas::drawEdge(QPainter &painter, const QPoint &p1, const QPoint &p2, const QString &jarakTeks, bool isHighlighted) {
    // 1. Gambar Garis Jalur
    QPen pen;
    pen.setColor(QColor(isHighlighted ? "#3B82F6" : "#94A3B8")); // Biru jika terpilih, Abu jika tidak
    pen.setWidth(isHighlighted ? 5 : 3);
    pen.setCapStyle(Qt::RoundCap);
    painter.setPen(pen);
    painter.drawLine(p1, p2);

    // 2. Gambar Teks Jarak di Tengah-tengah Garis (misal: 3 km)
    QPoint midPoint = (p1 + p2) / 2;
    painter.setPen(QColor("#1E293B"));
    QFont font = painter.font();
    font.setPointSize(10);
    font.setBold(true);
    painter.setFont(font);

    // Atur sedikit offset agar teks tidak menabrak pas di garis
    if (p1.x() == p2.x()) { // Garis Vertikal (B-D)
        painter.drawText(midPoint.x() + 12, midPoint.y() + 5, jarakTeks);
    } else if (p1.y() == p2.y()) { // Garis Horisontal
        painter.drawText(midPoint.x() - 15, midPoint.y() - 10, jarakTeks);
    } else { // Garis Miring
        painter.drawText(midPoint.x() - 20, midPoint.y() - 8, jarakTeks);
    }
}

void GraphCanvas::paintEvent(QPaintEvent *event) {
    QWidget::paintEvent(event);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    // Ambil posisi titik koordinat node
    QPoint pA = m_nodes[0].posisi;
    QPoint pB = m_nodes[1].posisi;
    QPoint pC = m_nodes[2].posisi;
    QPoint pD = m_nodes[3].posisi;

    // Logika jalur aktif berdasarkan rute terpilih saat ini
    bool pathAB = (m_ruteAktif.contains("A") && m_ruteAktif.contains("B") && m_ruteAktif.indexOf("A") + 1 == m_ruteAktif.indexOf("B")) || (m_ruteAktif.contains("B") && m_ruteAktif.contains("A") && m_ruteAktif.indexOf("B") + 1 == m_ruteAktif.indexOf("A"));
    bool pathBC = (m_ruteAktif.contains("B") && m_ruteAktif.contains("C") && m_ruteAktif.indexOf("B") + 1 == m_ruteAktif.indexOf("C")) || (m_ruteAktif.contains("C") && m_ruteAktif.contains("B") && m_ruteAktif.indexOf("C") + 1 == m_ruteAktif.indexOf("B"));
    bool pathBD = (m_ruteAktif.contains("B") && m_ruteAktif.contains("D") && m_ruteAktif.indexOf("B") + 1 == m_ruteAktif.indexOf("D")) || (m_ruteAktif.contains("D") && m_ruteAktif.contains("B") && m_ruteAktif.indexOf("D") + 1 == m_ruteAktif.indexOf("B"));
    bool pathCD = (m_ruteAktif.contains("C") && m_ruteAktif.contains("D") && m_ruteAktif.indexOf("C") + 1 == m_ruteAktif.indexOf("D")) || (m_ruteAktif.contains("D") && m_ruteAktif.contains("C") && m_ruteAktif.indexOf("D") + 1 == m_ruteAktif.indexOf("C"));

    // Judul Pojok Kiri Atap Peta Graf
    painter.setPen(QColor("#1E293B"));
    QFont fontJudul = painter.font();
    fontJudul.setPointSize(14);
    fontJudul.setBold(true);
    painter.setFont(fontJudul);
    painter.drawText(20, 35, "eta Graf");

    // Gambar Edges (Garis + teks km)
    drawEdge(painter, pA, pB, "3 km", pathAB);
    drawEdge(painter, pB, pC, "2 km", pathBC);
    drawEdge(painter, pB, pD, "4 km", pathBD);
    drawEdge(painter, pC, pD, "3 km", pathCD);

    // Gambar Bulatan Kota (Nodes) beserta Keterangan teks bawahnya
    for (int i = 0; i < m_nodes.size(); ++i) {
        const auto &node = m_nodes[i];
        bool isTargetActive = m_ruteAktif.contains(node.nama);

        // Lingkaran luar/Glow tipis
        painter.setPen(Qt::NoPen);
        painter.setBrush(QColor(isTargetActive ? "rgba(59, 130, 246, 0.2)" : "rgba(0, 0, 0, 0.05)"));
        painter.drawEllipse(node.posisi, 28, 28);

        // Lingkaran Utama (Warna hijau jika D aktif, biru jika node lain aktif)
        if (node.nama == "D" && isTargetActive) {
            painter.setBrush(QColor("#22C55E")); // Hijau Kantin Tujuan aktif
            painter.setPen(QPen(QColor("#16A34A"), 2));
        } else {
            painter.setBrush(QColor(isTargetActive ? "#3B82F6" : "#FFFFFF")); // Biru aktif / putih biasa
            painter.setPen(QPen(QColor(isTargetActive ? "#2563EB" : "#94A3B8"), 2));
        }
        painter.drawEllipse(node.posisi, 22, 22);

        // Huruf di dalam lingkaran (A, B, C, D)
        painter.setPen(QColor(isTargetActive ? "#FFFFFF" : "#1E293B"));
        QFont fontNode = painter.font();
        fontNode.setBold(true);
        fontNode.setPointSize(11);
        painter.setFont(fontNode);
        painter.drawText(QRect(node.posisi.x() - 22, node.posisi.y() - 22, 44, 44), Qt::AlignCenter, node.nama);

        // Teks Sub-Keterangan di bawah Bulatan (Gerbang, Fakultas, Kantin)
        if (!node.keterangan.isEmpty()) {
            painter.setPen(QColor("#475569"));
            QFont fontKet = painter.font();
            fontKet.setBold(false);
            fontKet.setPointSize(9);
            painter.setFont(fontKet);
            painter.drawText(QRect(node.posisi.x() - 80, node.posisi.y() + 28, 160, 20), Qt::AlignCenter, node.keterangan);
        }
    }
}

// =============================================================================
//                      IMPLEMENTASI LAYOUT WINDOW UTAMA
// =============================================================================
MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    setWindowTitle("Visualisasi Rute Terpendek (Matematika Diskrit)");
    resize(920, 600);
    setStyleSheet("background-color: #F8FAFC;");

    QWidget *centerWidget = new QWidget(this);
    setCentralWidget(centerWidget);

    // --- PANEL NAVIGASI KANAN ---
    QGroupBox *boxInput = new QGroupBox("Menu Navigasi");
    boxInput->setStyleSheet("QGroupBox { font-size: 18px; font-weight: bold; color: #1E293B; border: none; margin-top: 10px; }");
    QVBoxLayout *boxLayout = new QVBoxLayout(boxInput);

    QLabel *lblTitleAwal = new QLabel("Lokasi Awal (A, B, C, atau D):");
    lblTitleAwal->setStyleSheet("font-size: 11px; font-weight: bold; color: #1E293B;");
    txtAwal = new QLineEdit();
    txtAwal->setText("A");
    txtAwal->setStyleSheet("padding: 8px; font-size: 12px; border: 1px solid #CBD5E1; border-radius: 6px; background: white; color: black;");

    QLabel *lblTitleTujuan = new QLabel("Lokasi Tujuan (A, B, C, atau D):");
    lblTitleTujuan->setStyleSheet("font-size: 11px; font-weight: bold; color: #1E293B; margin-top: 5px;");
    txtTujuan = new QLineEdit();
    txtTujuan->setPlaceholderText("Contoh: D");
    txtTujuan->setStyleSheet("padding: 8px; font-size: 12px; border: 1px solid #CBD5E1; border-radius: 6px; background: white; color: black;");

    QPushButton *btnHitung = new QPushButton("🔍 Hitung Rute");
    btnHitung->setStyleSheet("background-color: #3B82F6; color: white; font-weight: bold; font-size: 12px; padding: 10px; border-radius: 6px; margin-top: 10px;");

    QLabel *lblTitleHasil = new QLabel("Hasil Analisis Graf:");
    lblTitleHasil->setStyleSheet("font-size: 13px; font-weight: bold; color: #1E293B; margin-top: 25px;");

    QLabel *lblSubRute = new QLabel("Rute Terbaik:");
    lblSubRute->setStyleSheet("font-size: 11px; color: #64748B;");
    lblRute = new QLabel("Belum Ada Input");
    lblRute->setStyleSheet("font-size: 18px; font-weight: bold; color: #3B82F6;");

    QLabel *lblSubJarak = new QLabel("Total Jarak Terpendek:");
    lblSubJarak->setStyleSheet("font-size: 11px; color: #64748B; margin-top: 5px;");
    lblJarak = new QLabel("-");
    lblJarak->setStyleSheet("font-size: 22px; font-weight: bold; color: #22C55E;");

    boxLayout->addWidget(lblTitleAwal);
    boxLayout->addWidget(txtAwal);
    boxLayout->addWidget(lblTitleTujuan);
    boxLayout->addWidget(txtTujuan);
    boxLayout->addWidget(btnHitung);
    boxLayout->addWidget(lblTitleHasil);
    boxLayout->addWidget(lblSubRute);
    boxLayout->addWidget(lblRute);
    boxLayout->addWidget(lblSubJarak);
    boxLayout->addWidget(lblJarak);
    boxLayout->addStretch();

    // --- PASANG LAYOUT UTAMA ---
    canvasGraf = new GraphCanvas();
    QHBoxLayout *mainLayout = new QHBoxLayout(centerWidget);
    mainLayout->setContentsMargins(20, 20, 20, 20);
    mainLayout->setSpacing(20);
    mainLayout->addWidget(canvasGraf, 3);
    mainLayout->addWidget(boxInput, 1);

    inisialisasiDataRute();
    connect(btnHitung, &QPushButton::clicked, this, &MainWindow::onHitungRuteClicked);
}

MainWindow::~MainWindow() {}

void MainWindow::inisialisasiDataRute() {
    m_dataRute["A-B"] = {"A ➔ B", 3};
    m_dataRute["B-C"] = {"B ➔ C", 2};
    m_dataRute["B-D"] = {"B ➔ D", 4};
    m_dataRute["C-D"] = {"C ➔ D", 1};
    m_dataRute["A-C"] = {"A ➔ B ➔ C", 5};
    m_dataRute["A-D"] = {"A ➔ B ➔ C ➔ D", 6}; // Rute Terpendek A ke D adalah lewat B lalu C lalu D (3+2+1 = 6km)
}

void MainWindow::onHitungRuteClicked() {
    QString start = txtAwal->text().toUpper().trimmed();
    QString end = txtTujuan->text().toUpper().trimmed();

    if (start.isEmpty() || end.isEmpty()) {
        lblRute->setText("Input Kosong!");
        lblJarak->setText("-");
        canvasGraf->setRuteAktif(QStringList());
        return;
    }

    if (start == end) {
        lblRute->setText(start);
        lblJarak->setText("0 km");
        canvasGraf->setRuteAktif(QStringList() << start);
        return;
    }

    QString key = start + "-" + end;
    if (m_dataRute.contains(key)) {
        lblRute->setText(m_dataRute[key].teksRute);
        lblJarak->setText(QString::number(m_dataRute[key].jarak) + " km");

        // Pengatur sorotan rute aktif ke panel gambar canvas
        QStringList lintasan;
        if (key == "A-C") lintasan << "A" << "B" << "C";
        else if (key == "A-D") lintasan << "A" << "B" << "C" << "D";
        else lintasan << start << end;

        canvasGraf->setRuteAktif(lintasan);
    } else {
        lblRute->setText("Rute Gak Ada");
        lblJarak->setText("-");
        canvasGraf->setRuteAktif(QStringList());
    }
}