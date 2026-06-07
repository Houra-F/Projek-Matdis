#ifndef DIJKSTRA_H
#define DIJKSTRA_H

#include <QString>
#include <QStringList>
#include <QMap>
#include <QVector>

struct Edge
{
    QString tujuan;
    int jarak;
};

class Dijkstra
{
public:
    Dijkstra();

    void tambahEdge(
        const QString &asal,
        const QString &tujuan,
        int jarak);

    QStringList cariRute(
        const QString &start,
        const QString &finish,
        int &totalJarak);

private:
    QMap<QString, QVector<Edge>> graf;
};

#endif