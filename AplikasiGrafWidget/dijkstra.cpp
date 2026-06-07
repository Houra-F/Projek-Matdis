#include "dijkstra.h"

#include <climits>

Dijkstra::Dijkstra()
{
}

void Dijkstra::tambahEdge(
    const QString &asal,
    const QString &tujuan,
    int jarak)
{
    graf[asal].append({tujuan, jarak});
    graf[tujuan].append({asal, jarak});
}

QStringList Dijkstra::cariRute(
    const QString &start,
    const QString &finish,
    int &totalJarak)
{
    QMap<QString,int> distance;
    QMap<QString,QString> previous;

    QStringList unvisited;

    for(const QString &node : graf.keys())
    {
        distance[node] = INT_MAX;
        unvisited.append(node);
    }

    distance[start] = 0;

    while(!unvisited.isEmpty())
    {
        QString current;
        int minDistance = INT_MAX;

        for(const QString &node : unvisited)
        {
            if(distance[node] < minDistance)
            {
                minDistance = distance[node];
                current = node;
            }
        }

        if(current.isEmpty())
            break;

        unvisited.removeOne(current);

        if(current == finish)
            break;

        for(const Edge &edge : graf[current])
        {
            if(distance[current] == INT_MAX)
                continue;

            int alt =
                distance[current]
                + edge.jarak;

            if(alt < distance[edge.tujuan])
            {
                distance[edge.tujuan] = alt;
                previous[edge.tujuan] = current;
            }
        }
    }

    QStringList path;

    QString current = finish;

    while(!current.isEmpty())
    {
        path.prepend(current);
        current = previous[current];
    }

    totalJarak = distance[finish];

    return path;
}