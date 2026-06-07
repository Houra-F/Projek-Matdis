#include "graphcanvas.h"

GraphCanvas::GraphCanvas(QWidget *parent)
    : QWidget(parent)
{
    setMinimumSize(650,550);

    setStyleSheet(
        "background-color:#E2E8F0;"
        "border:1px solid #CBD5E1;"
        "border-radius:20px;"
        );

    m_nodes.append({"A","Gerbang",QPoint(80,250)});
    m_nodes.append({"B","Perpustakaan",QPoint(220,120)});
    m_nodes.append({"C","Gedung A",QPoint(420,120)});
    m_nodes.append({"D","Kantin",QPoint(560,250)});
    m_nodes.append({"E","Masjid",QPoint(420,400)});
    m_nodes.append({"F","Gedung B",QPoint(220,400)});
    m_nodes.append({"G","Lab Komputer",QPoint(320,250)});
    m_nodes.append({"H","Parkiran",QPoint(80,400)});
}

void GraphCanvas::setRuteAktif(
    const QStringList &rute)
{
    m_ruteAktif = rute;
    update();
}

void GraphCanvas::drawEdge(
    QPainter &painter,
    const QPoint &p1,
    const QPoint &p2,
    const QString &jarak,
    bool aktif)
{
    QPen pen;

    pen.setColor(
        aktif
            ? QColor("#3B82F6")
            : QColor("#94A3B8"));

    pen.setWidth(
        aktif ? 5 : 3);

    pen.setCapStyle(Qt::RoundCap);

    painter.setPen(pen);

    painter.drawLine(p1,p2);

    QPoint mid =
        (p1+p2)/2;

    painter.setPen(
        QColor("#1E293B"));

    painter.drawText(
        mid.x()+5,
        mid.y()-5,
        jarak);
}

void GraphCanvas::paintEvent(QPaintEvent *)
{
    QPainter painter(this);

    painter.setRenderHint(
        QPainter::Antialiasing);

    QPoint A = m_nodes[0].posisi;
    QPoint B = m_nodes[1].posisi;
    QPoint C = m_nodes[2].posisi;
    QPoint D = m_nodes[3].posisi;
    QPoint E = m_nodes[4].posisi;
    QPoint F = m_nodes[5].posisi;
    QPoint G = m_nodes[6].posisi;
    QPoint H = m_nodes[7].posisi;

    auto isPathActive =
        [&](QString n1, QString n2)
    {
        for(int i=0;
             i<m_ruteAktif.size()-1;
             i++)
        {
            QString a =
                m_ruteAktif[i];

            QString b =
                m_ruteAktif[i+1];

            if((a==n1 && b==n2)
                ||
                (a==n2 && b==n1))
            {
                return true;
            }
        }

        return false;
    };

    drawEdge(
        painter,A,B,"3",
        isPathActive("A","B"));

    drawEdge(
        painter,B,C,"2",
        isPathActive("B","C"));

    drawEdge(
        painter,C,D,"4",
        isPathActive("C","D"));

    drawEdge(
        painter,D,E,"2",
        isPathActive("D","E"));

    drawEdge(
        painter,E,F,"3",
        isPathActive("E","F"));

    drawEdge(
        painter,F,H,"2",
        isPathActive("F","H"));

    drawEdge(
        painter,B,G,"2",
        isPathActive("B","G"));

    drawEdge(
        painter,G,E,"1",
        isPathActive("G","E"));

    drawEdge(
        painter,F,G,"2",
        isPathActive("F","G"));

    drawEdge(
        painter,C,G,"1",
        isPathActive("C","G"));

    painter.setPen(
        QColor("#1E293B"));

    QFont titleFont;

    titleFont.setPointSize(14);
    titleFont.setBold(true);

    painter.setFont(titleFont);

    painter.drawText(
        20,
        35,
        "Peta Kampus");

    for(const auto &node : m_nodes)
    {
        bool aktif =
            m_ruteAktif.contains(
                node.nama);

        painter.setPen(
            QPen(
                aktif
                    ? QColor("#2563EB")
                    : QColor("#64748B"),
                2));

        painter.setBrush(
            aktif
                ? QColor("#3B82F6")
                : QColor("#FFFFFF"));

        painter.drawEllipse(
            node.posisi,
            22,
            22);

        painter.setPen(
            aktif
                ? Qt::white
                : QColor("#1E293B"));

        painter.drawText(
            QRect(
                node.posisi.x()-20,
                node.posisi.y()-20,
                40,
                40),
            Qt::AlignCenter,
            node.nama);

        painter.setPen(
            QColor("#475569"));

        painter.drawText(
            QRect(
                node.posisi.x()-60,
                node.posisi.y()+25,
                120,
                20),
            Qt::AlignCenter,
            node.keterangan);
    }
}