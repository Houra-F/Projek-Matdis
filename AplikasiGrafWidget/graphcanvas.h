#ifndef GRAPHCANVAS_H
#define GRAPHCANVAS_H

#include <QWidget>
#include <QPainter>

struct NodeData
{
    QString nama;
    QString keterangan;
    QPoint posisi;
};

class GraphCanvas : public QWidget
{
    Q_OBJECT

public:
    explicit GraphCanvas(QWidget *parent = nullptr);

    void setRuteAktif(const QStringList &rute);

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    QList<NodeData> m_nodes;
    QStringList m_ruteAktif;

    void drawEdge(
        QPainter &painter,
        const QPoint &p1,
        const QPoint &p2,
        const QString &jarak,
        bool aktif);
};

#endif