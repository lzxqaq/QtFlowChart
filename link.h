#ifndef LINK_H
#define LINK_H

#include <QGraphicsItem>

class Node;
class Link : public QGraphicsItem
{
public:
    Link(Node *fromNode, Node *toNode);
    ~Link();

    void adjust();

    Node *getFromNode() const;
    Node *getToNode() const;

protected:
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

private:
    Node    *m_fromNode;
    Node    *m_toNode;

    QPointF m_fromArrow;
    QPointF m_toArrow;

    QPointF  m_fromPoint;
    QPointF  m_toPoint;

};

#endif // LINK_H
