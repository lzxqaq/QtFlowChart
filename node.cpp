#include <QtWidgets>
#include "node.h"
#include "link.h"

static const double AddHeight = 100.0;
static const double AddWidth = 100.0;


Node::Node(QGraphicsScene *scene)
    :   m_scene(scene)
    ,   m_width(50)
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
    m_texts = text;

    QString maxString;
    for (QString string : m_texts)
    {
        if (string.size() > maxString.size()) maxString = string;
    }
    QFontMetricsF metrics = (QFontMetricsF)qApp->font();

    m_width = metrics.boundingRect(maxString).width();

    prepareGeometryChange();
}

void Node::addParent(Node *parent)
{
    QPointF thisP = this->pos();
    QPointF nextP;
    nextP.setY(thisP.y() - AddHeight);
    int parentCount = m_parents.size();
    qreal addWidth = (parentCount / 2 + 1) * AddWidth;

    int i = parentCount % 2 == 0 ? -1 : 1;//偶数加在左 奇数加在右

    nextP.setX(thisP.x() + i * addWidth);

    m_parents.append(parent);
    if (!parent->m_childs.contains(this))
    {
        parent->m_childs.append(this);
    }

    parent->setPos(nextP);

    if (!m_scene->collidingItems(parent).isEmpty() || m_scene->itemAt(nextP, QTransform()) != nullptr)
    {
        if (i == -1)
        {
            nextP.setX(thisP.x() + addWidth);
        }
        else
        {
            nextP.setX(thisP.x() - addWidth - AddWidth);

        }
        QPointF pos = this->pos();
        this->setPos(pos.x() - i * 2 * AddWidth, pos.y());
    }

    parent->setPos(nextP);



    m_scene->addItem(parent);

    Link *newLink = new Link(this, parent);
    m_scene->addItem(newLink);
}

void Node::addChild(Node *child)
{
    QPointF next = nextChildP();

    m_childs.append(child);
    if (!child->m_parents.contains(this))
    {
        child->m_parents.append(this);
    }

    child->setPos(next);
    m_scene->addItem(child);

    Link *newLink = new Link(child, this);
    m_scene->addItem(newLink);
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
    if (m_texts.size() <=0) return QRectF();


    QFontMetricsF metrics = (QFontMetricsF)qApp->font();
    QString maxString;
    for (QString string : m_texts)
    {
        if (string.size() > maxString.size()) maxString = string;
    }

    QRectF rect = metrics.boundingRect(maxString);
    rect.adjust(-10, -10, +20, +10);
    rect.translate(-rect.center());

    int size = m_texts.size();

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

    int size = m_texts.size();

    qreal x = rect.x();
    qreal y = rect.y();
    qreal w = rect.width();
    qreal h = rect.height();

    h =  h / size;

    qreal x2 = x + w;
    rect = QRectF(x, y, w, h);

    for (int i = 0; i < size; i++)
    {
        int yy = y + (i + 1) * h;
        painter->drawLine(x, yy, x2, yy);
        painter->drawText(rect, Qt::AlignCenter, m_texts.at(i));
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

QPointF Node::nextChildP()
{
    QPointF thisP = this->pos();
    QPointF nextP;
    nextP.setY(thisP.y() + AddHeight);
    int childCount = m_childs.size();
    qreal addWidth = (childCount / 2 + 1) * AddWidth;
    if (childCount % 2 == 0)
    {
        nextP.setX(thisP.x() - addWidth);
        if (m_scene->itemAt(nextP, QTransform()) != nullptr)
        {
            nextP.setX(nextP.x() - AddWidth);
            Node *parent = m_parents.at(0);
            QPointF parentP = parent->pos();
            parent->setPos(parentP.x() - AddWidth, parentP.y());
        }
    }
    else
    {
        nextP.setX(thisP.x() + addWidth);
        if (m_scene->itemAt(nextP, QTransform()) != nullptr)
        {
            nextP.setX(nextP.x() + AddWidth);
            Node *parent = m_parents.at(0);
            QPointF parentP = parent->pos();
            parent->setPos(parentP.x() + AddWidth, parentP.y());
        }
    }

    return nextP;
}

QPointF Node::nextParentP()
{
    QPointF thisP = this->pos();
    QPointF nextP;
    nextP.setY(thisP.y() - AddHeight);
    int parentCount = m_parents.size();
    qreal addWidth = (parentCount / 2 + 1) * AddWidth;
    if (parentCount % 2 == 0)
    {
        nextP.setX(thisP.x() - addWidth);
        if (m_scene->itemAt(nextP, QTransform()) != nullptr)
        if (!m_scene->collidingItems(this).isEmpty())
        {
            nextP.setX(thisP.x() + addWidth);
            QPointF pos = this->pos();
            this->setPos(pos.x() + 2 * AddWidth, pos.y());
        }
    }
    else
    {
        nextP.setX(thisP.x() + addWidth);
        if (m_scene->itemAt(nextP, QTransform()) != nullptr)
        {
            nextP.setX(thisP.x() - addWidth - AddWidth);
            QPointF pos = this->pos();
            this->setPos(pos.x() - 2 * AddWidth, pos.y());
        }
    }
    return nextP;
}

QStringList Node::getText() const
{
    return m_texts;
}
