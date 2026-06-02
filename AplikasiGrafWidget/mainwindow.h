#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QWidget>
#include <QPainter>
#include <QPoint>
#include <QVector>
#include <QString>
#include <QMap>
#include <QLineEdit>
#include <QLabel>
#include <QPushButton>

// --- CANVAS UNTUK MENGGAMBAR GRAF INTERAKTIF ---
class GraphCanvas : public QWidget {
    Q_OBJECT
public:
    explicit GraphCanvas(QWidget *parent = nullptr);
    void setRuteAktif(const QStringList &rute);

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    struct Node {
        QString nama;
        QString keterangan;
        QPoint posisi;
    };
    QVector<Node> m_nodes;
    QStringList m_ruteAktif;

    void drawEdge(QPainter &painter, const QPoint &p1, const QPoint &p2, const QString &jarakTeks, bool isHighlighted);
};

// --- JENDELA UTAMA APLIKASI ---
class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onHitungRuteClicked();

private:
    GraphCanvas *canvasGraf;
    QLineEdit *txtAwal;
    QLineEdit *txtTujuan;
    QLabel *lblRute;
    QLabel *lblJarak;

    struct RuteData {
        QString teksRute;
        int jarak;
    };
    QMap<QString, RuteData> m_dataRute;
    void inisialisasiDataRute();
};

#endif // MAINWINDOW_H