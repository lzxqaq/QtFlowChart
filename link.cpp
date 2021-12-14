#include <QtWidgets>
#include "node.h"
#include "link.h"

static const double Pi = 3.14159265358979323846264338327950288419717;
static double TwoPi = 2.0 * Pi;

Link::Link(Node *fromNode, Node *toNode)
{
    m_fromNode = fromNode;
    m_toNode = toNode;

    m_fromNode->addLink(this);
    m_toNode->addLink(this);

    this->setZValue(-1);
    adjust();
}

Link::~Link()
{
    m_fromNode->removeLink(this);
    m_toNode->removeLink(this);
}

void Link::adjust()
{

    QPointF from = m_fromNode->pos();
    QPointF to = m_toNode->pos();

    int size = m_toNode->getText().size();
    if (size <= 0) return ;
    qreal height = m_toNode->boundingRect().height() / (2 * size);

    m_fromPoint = from;

    if (from.y() < to.y())
    {
        m_toPoint = QPointF(to.x(), to.y() - height);
    }
    else
    {
        m_toPoint = QPointF(to.x(), to.y() + (2 * size - 1) * height);
    }

    QLineF line(m_fromPoint, m_toPoint);

    double angle = ::acos(line.dx() / line.length());
    if (line.dy() >= 0) angle = TwoPi - angle;

    m_fromArrow = m_toPoint + QPointF(sin(angle - Pi / 3) * 8, cos(angle - Pi /3) * 8);
    m_toArrow = m_toPoint + QPointF(sin(angle - Pi + Pi / 3) * 8, cos(angle - Pi + Pi /3) * 8);


    prepareGeometryChange();
}

Node *Link::getFromNode() const
{
    return m_fromNode;
}

Node *Link::getToNode() const
{
    return m_toNode;
}

QRectF Link::boundingRect() const
{
    qreal extra = (1.5 + 8) / 2.0;
    return QRectF(m_fromPoint, QSizeF(m_toPoint.x() - m_fromPoint.x(), m_toPoint.y() - m_fromPoint.y())).normalized().adjusted(-extra, -extra, extra, extra);
}

void Link::paint(QPainter *painter, const QStyleOptionGraphicsItem */*option*/, QWidget */*widget*/)
{
//    painter->setPen(QPen(Qt::black, 1.0, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    painter->setRenderHint(QPainter::Antialiasing);
    QPen p(QColor::fromRgb(79, 136, 187));
    painter->setBrush(QBrush(QColor::fromRgb(79, 136, 187)));
    p.setWidthF(1.5);
    painter->setPen(p);

    painter->drawLine(m_fromPoint, m_toPoint);
    painter->drawPolygon(QPolygonF() << m_toPoint << m_fromArrow << m_toArrow);
}























