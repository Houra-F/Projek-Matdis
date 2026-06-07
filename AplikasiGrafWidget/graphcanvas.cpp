#include "graphcanvas.h"
#include <QPainter>

GraphCanvas::GraphCanvas(QWidget *parent)
    : QWidget(parent)
{
    setMinimumSize(750, 650);

    // KANVAS PUTIH CERAH
    setStyleSheet(
        "background-color: #FFFFFF;"
        "border: 1px solid #E2E8F0;"
        "border-radius: 15px;"
        );

    // --- DEKLARASI KOORDINAT NODE (A sampai N) ---
    // Sisi Kiri
    m_nodes.append({"A", "Pintu Masuk", QPoint(230, 560)});
    m_nodes.append({"B", "Gedung A",    QPoint(170, 490)});
    m_nodes.append({"C", "Musholla",    QPoint(110, 420)});
    m_nodes.append({"D", "Parkiran L",  QPoint(60,  310)});
    m_nodes.append({"E", "USU Press",   QPoint(60,  150)});

    // Sisi Kanan
    m_nodes.append({"F", "Keluar",      QPoint(350, 560)});
    m_nodes.append({"G", "Gedung B",    QPoint(410, 490)});
    m_nodes.append({"H", "Pendopo",     QPoint(470, 420)});
    m_nodes.append({"I", "Gedung C",    QPoint(470, 310)});
    m_nodes.append({"J", "Gedung D",    QPoint(470, 210)});
    m_nodes.append({"K", "Oktagon",     QPoint(470, 130)});

    // Area Tengah & Atas (L, M, N)
    m_nodes.append({"L", "Wifi Corner", QPoint(310, 130)});
    m_nodes.append({"M", "PSI",         QPoint(310, 310)});
    m_nodes.append({"N", "Ten Cafe",    QPoint(310, 50)});
}

void GraphCanvas::setRuteAktif(const QStringList &rute)
{
    m_ruteAktif = rute;
    update();
}

void GraphCanvas::drawEdge(QPainter &painter, const QPoint &p1, const QPoint &p2, const QString &jarak, bool aktif)
{
    QPen pen;
    pen.setColor(aktif ? QColor("#3B82F6") : QColor("#94A3B8")); // Biru jika aktif, abu-abu jika biasa
    pen.setWidth(aktif ? 5 : 3);
    pen.setCapStyle(Qt::RoundCap);

    painter.setPen(pen);
    painter.drawLine(p1, p2);

    // Cetak angka bobot/jarak di tengah jalur
    QPoint mid = (p1 + p2) / 2;
    painter.setPen(QColor("#0F172A"));
    painter.setFont(QFont("Arial", 10, QFont::Bold));
    painter.drawText(mid.x() + 8, mid.y() - 4, jarak);
}

void GraphCanvas::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    // Paksa background berwarna putih bersih
    painter.fillRect(this->rect(), QColor("#FFFFFF"));

    // Ambil koordinat untuk pembuatan jalur logika pembantu
    QPoint pA = m_nodes[0].posisi;
    QPoint pB = m_nodes[1].posisi;
    QPoint pC = m_nodes[2].posisi;
    QPoint pD = m_nodes[3].posisi;
    QPoint pE = m_nodes[4].posisi;
    QPoint pF = m_nodes[5].posisi;
    QPoint pG = m_nodes[6].posisi;
    QPoint pH = m_nodes[7].posisi;
    QPoint pI = m_nodes[8].posisi;
    QPoint pJ = m_nodes[9].posisi;
    QPoint pK = m_nodes[10].posisi;
    QPoint pL = m_nodes[11].posisi;
    QPoint pM = m_nodes[12].posisi;
    QPoint pN = m_nodes[13].posisi;

    // Lambda untuk cek status keaktifan rute
    auto isPathActive = [&](QString n1, QString n2) {
        for(int i = 0; i < m_ruteAktif.size() - 1; i++) {
            QString a = m_ruteAktif[i];
            QString b = m_ruteAktif[i+1];
            if((a == n1 && b == n2) || (a == n2 && b == n1)) return true;
        }
        return false;
    };

    // --- MENGGAMBAR KESELURUHAN JALUR (EDGES) ---
    // Jalur Sisi Kiri
    drawEdge(painter, pA, pB, "4", isPathActive("A","B"));
    drawEdge(painter, pB, pC, "2", isPathActive("B","C"));
    drawEdge(painter, pC, pD, "3", isPathActive("C","D"));
    drawEdge(painter, pD, pE, "2", isPathActive("D","E"));

    // Jalur Sisi Kanan (Pendopo ke Gedung C sudah tersambung lurus)
    drawEdge(painter, pF, pG, "2", isPathActive("F","G"));
    drawEdge(painter, pG, pH, "7", isPathActive("G","H"));
    drawEdge(painter, pH, pI, "6", isPathActive("H","I"));
    drawEdge(painter, pI, pJ, "1", isPathActive("I","J"));
    drawEdge(painter, pJ, pK, "2", isPathActive("J","K"));

    // Jalur Sektor Tengah & Atas
    drawEdge(painter, pK, pN, "7", isPathActive("K","N"));
    drawEdge(painter, pK, pL, "1", isPathActive("K","L"));
    drawEdge(painter, pL, pM, "1", isPathActive("L","M"));
    drawEdge(painter, pM, pI, "3", isPathActive("M","I"));

    // === JALUR-JALUR PERMINTAAN BARU ===
    drawEdge(painter, pA, pF, "3", isPathActive("A","F")); // Jalur langsung Pintu Masuk (A) ke Keluar (F)
    drawEdge(painter, pE, pN, "4", isPathActive("E","N")); // Jalur akses dari USU Press (E) ke Ten Cafe (N)
    drawEdge(painter, pB, pM, "5", isPathActive("B","M")); // Jalur akses dari Gedung A (B) ke PSI (M)


    // --- MENGGAMBAR BENTUK NODE DAN LABEL HURUFNYA ---
    for(const auto &node : m_nodes)
    {
        bool aktif = m_ruteAktif.contains(node.nama);

        // Styling Pena Border Node
        painter.setPen(QPen(aktif ? QColor("#2563EB") : QColor("#334155"), 2));
        painter.setBrush(QBrush(aktif ? QColor("#DBEAFE") : QColor("#F8FAFC")));

        // Desain bentuk kustom berdasarkan tipe tempat
        if(node.nama == "N") { // Ten Cafe (Kotak Horizontal)
            painter.drawRect(node.posisi.x() - 55, node.posisi.y() - 20, 110, 40);
        } else if(node.nama == "M") { // PSI (Kotak Vertikal)
            painter.drawRect(node.posisi.x() - 30, node.posisi.y() - 35, 60, 70);
        } else if(node.nama == "L") { // Wifi Corner (Oval Horizontal)
            painter.drawEllipse(node.posisi.x() - 45, node.posisi.y() - 20, 90, 40);
        } else { // Node Utama Lainnya Lingkaran Sempurna
            painter.drawEllipse(node.posisi, 22, 22);
        }

        // --- ATURAN TEKS LABEL (Penyebab Utama Error Sebelumnya Sudah Dibenahi) ---
        painter.setFont(QFont("Arial", 9, QFont::Bold));

        if(node.nama == "L" || node.nama == "M" || node.nama == "N") {
            // Tampilkan Huruf Kode + Nama Tempat di dalam kotak/oval agar seragam!
            painter.setPen(QColor("#1E293B"));
            QString labelKotak = QString("[%1]\n%2").arg(node.nama).arg(node.keterangan);
            painter.drawText(QRect(node.posisi.x() - 55, node.posisi.y() - 18, 110, 36), Qt::AlignCenter, labelKotak);
        } else {
            // Tampilkan Kode Huruf di dalam lingkaran node
            painter.setPen(aktif ? QColor("#2563EB") : QColor("#0F172A"));
            painter.drawText(QRect(node.posisi.x() - 20, node.posisi.y() - 20, 40, 40), Qt::AlignCenter, node.nama);

            // Tampilkan deskripsi lokasi tepat di bawah lingkaran node
            painter.setPen(QColor("#475569"));
            painter.setFont(QFont("Arial", 9));
            painter.drawText(QRect(node.posisi.x() - 60, node.posisi.y() + 25, 120, 20), Qt::AlignCenter, node.keterangan);
        }
    }
}