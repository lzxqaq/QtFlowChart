#include <QtWidgets>
#include "node.h"
#include "link.h"

Node::Node()
{
    this->setFlags(ItemIsMovable | ItemIsSelectable | ItemSendsGeometryChanges);
    this->setAcceptHoverEvents(true);
}

Node::~Node()
{
    foreach (Link *link, m_linkList)
        delete link;
}

void Node::setText(const QStringList &text)
{
    m_text = text;

    prepareGeometryChange();
}

void Node::addLink(Link *link)
{
    m_linkList.append(link);
}

void Node::removeLink(Link *link)
{
    m_linkList.removeOne(link);
}

QList<Link *> Node::getLinkList()
{
    return m_linkList;
}

QRectF Node::outlineRect() const
{
    if (m_text.size() <=0) return QRectF();

    QFontMetricsF metrics = (QFontMetricsF)qApp->font();
    QRectF rect = metrics.boundingRect(m_text.at(0));
    rect.adjust(-10, -10, +20, +10);
    rect.translate(-rect.center());

    int size = m_text.size();

    return QRectF(rect.x(), rect.y(), rect.width(), rect.height() * size);
}

QRectF Node::boundingRect() const
{
    return outlineRect().adjusted(-1, -1, +1, +1);
}

QPainterPath Node::shape() const
{
    QRectF rect = outlineRect();

    QPainterPath path;
    path.addRect(rect);

    return path;
}

void Node::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget */*widget*/)
{
    if (option->state & QStyle::State_Selected) {
        painter->setPen(QPen(Qt::red, 1.0, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    }
    else {
        painter->setPen(QPen(Qt::black, 1.0, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    }
    painter->setBrush(Qt::white);

    QRectF rect = outlineRect();
    painter->drawRect(rect);

    int size = m_text.size();

    qreal x = rect.x();
    qreal y = rect.y();
    qreal w = rect.width();
    qreal h = rect.height() / size;

    qreal x2 = x + w;
    rect = QRectF(x, y, w, h);

    for (int i = 0; i < size; i++)
    {
        int yy = y + (i + 1) * h;
        painter->drawLine(x, yy, x2, yy);
        painter->drawText(rect, Qt::AlignCenter, m_text.at(i));
        rect.adjust(0, h, 0, h);
    }

}

QVariant Node::itemChange(GraphicsItemChange change, const QVariant &value)
{
    if (change == ItemPositionHasChanged) {
        foreach (Link *link, m_linkList) {
            link->adjust();
        }
    }

    return QGraphicsItem::itemChange(change, value);
}

void Node::hoverEnterEvent(QGraphicsSceneHoverEvent */*event*/)
{
    setCursor(Qt::PointingHandCursor);
}

void Node::hoverLeaveEvent(QGraphicsSceneHoverEvent */*event*/)
{
    setCursor(Qt::PointingHandCursor);
}

QStringList Node::getText() const
{
    return m_text;
}
