#ifndef NODE_H
#define NODE_H

#include <QGraphicsItem>

class Link;
class Node : public QGraphicsItem
{
public:
    Node(QGraphicsScene *scene);
    ~Node();

    void setText(const QStringList &text);

    void addParent(Node *parent);
    void addChild(Node *child);

    void addLink(Link *link);
    void removeLink(Link *link);
    QList<Link *> getLinkList();

    QRectF outlineRect() const;
    QRectF boundingRect() const;
    QPainterPath shape() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

    QStringList getText() const;

protected:
    QVariant itemChange(GraphicsItemChange change, const QVariant &value);
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event);
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event);

private:
    QPointF nextChildP();
    QPointF nextParentP();

private:
    QGraphicsScene *m_scene;
    QStringList       m_texts;
    qreal m_width;

    QList<Link *> m_linkList;

    QList<Node *> m_parents;
    QList<Node *> m_childs;


};

#endif // NODE_H
