#ifndef NODE_H
#define NODE_H

#include <QGraphicsItem>

class Link;
class Node : public QGraphicsItem
{
public:
    Node();
    ~Node();

    void setText(const QStringList &text);

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
    QStringList       m_text;
    QList<Link *> m_linkList;

};

#endif // NODE_H
