#include <QtWidgets>
#include "qrgchartdialog.h"
#include "node.h"
#include "link.h"

QrgChartDialog::QrgChartDialog(QWidget *parent) :
    QDialog(parent)
{
    this->setWindowFlags(Qt::Window);
    this->resize(1268, 744);

    m_leftCount = 0;
    m_rightCount = 0;

    initOrgChartDialog();
    createActions();

    onSceneSelectionChanged();
}

QrgChartDialog::~QrgChartDialog()
{

}

void QrgChartDialog::initOrgChartDialog()
{
    m_scene = new QGraphicsScene(0, 0, 1500, 1600);
    connect(m_scene, SIGNAL(selectionChanged()), this, SLOT(onSceneSelectionChanged()));
    m_view = new QGraphicsView(this);
    m_view->setScene(m_scene);
    m_view->setDragMode(QGraphicsView::RubberBandDrag);
    m_view->setContextMenuPolicy(Qt::ActionsContextMenu);

    Node *node = new Node(m_scene);
    node->setText({"Nodsssse", "Nasdfasdfsafddddddddode2"});
    node->setPos(m_scene->width() / 2.0, m_scene->height() / 2.0 - 100);
    m_scene->addItem(node);

    Node *node1 = new Node(m_scene);
    Node *node2 = new Node(m_scene);
    node1->setText({"node1", "Nasdfasdfsafddddddddode2"});
    node2->setText({"node2", "Nasdfasdfsafddddddddode2"});
    Node *node3 = new Node(m_scene);
    Node *node4 = new Node(m_scene);
    Node *node5 = new Node(m_scene);
    Node *node6 = new Node(m_scene);
    Node *node7 = new Node(m_scene);
    Node *node8 = new Node(m_scene);
    node3->setText({"node3", "Nasdfasdfsafddddddddode2"});
    node4->setText({"node4", "Nasdfasdfsafddddddddode2"});
    node5->setText({"node5", "Nasdfasdfsafddddddddode2"});
    node6->setText({"node6", "Nasdfasdfsafddddddddode2"});
    node7->setText({"node7", "Nasdfasdfsafddddddddode2"});
    node8->setText({"node8", "Nasdfasdfsafddddddddode2"});

    node->addParent(node1);
    node->addParent(node2);
    node->addParent(node3);


//    node1->addParent(node3);
    node1->addParent(node5);
    node2->addParent(node4);
    node2->addParent(node6);
    node2->addParent(node7);
    node2->addParent(node8);

    QHBoxLayout *hLayout = new QHBoxLayout();
    hLayout->addWidget(m_view);
    this->setLayout(hLayout);
}

void QrgChartDialog::createActions()
{
    m_topMenu = new QMenu(this);

    m_actAdd = new QAction("Add node", this);
    connect(m_actAdd, SIGNAL(triggered(bool)), this, SLOT(onAdd()));
    m_topMenu->addAction(m_actAdd);

    m_actDel = new QAction("Delete", this);
    connect(m_actDel, SIGNAL(triggered(bool)), this, SLOT(onDelete()));
    m_topMenu->addAction(m_actDel);
}

Node *QrgChartDialog::getSelectedNode()
{
    QList<QGraphicsItem *> items = m_scene->selectedItems();
    if (items.count() == 1) {
        return dynamic_cast<Node *>(items.first());
    }
    else {
        return 0;
    }
}

void QrgChartDialog::onSceneSelectionChanged()
{
    foreach (QAction *action, m_view->actions())
        m_view->removeAction(action);

    foreach (QAction *action, m_topMenu->actions())
        m_view->addAction(action);
}

void QrgChartDialog::onAdd()
{
    Node *fromNode = getSelectedNode();
    if (!fromNode)
        return;

    Node *node = new Node(m_scene);
    node->setText({"Node", "Nsssssssssssssode2", "Noddddddddddddddddde3", "Node3"});
    QList<Link *> linkList = fromNode->getLinkList();
    if (linkList.isEmpty()) {
        node->setPos(m_scene->width() / 2.0, m_scene->height() / 2.0 + 100);
        m_scene->addItem(node);
    }
    else {
        if (linkList.size() % 2) {
            //左边的节点
            m_leftCount++;
            node->setPos(m_scene->width() / 2.0 - 140 * m_leftCount, m_scene->height() / 2.0 + 100);
        }
        else {
            //右边的节点
            m_rightCount++;
            node->setPos(m_scene->width() / 2.0 + 140 * m_rightCount, m_scene->height() / 2.0 + 100);
        }
        m_scene->addItem(node);
    }

}

void QrgChartDialog::onDelete()
{
    QList<QGraphicsItem *> items = m_scene->selectedItems();
    QMutableListIterator<QGraphicsItem *> it(items);
    while (it.hasNext()) {
        Link *link = dynamic_cast<Link *>(it.next());
        if (link) {
            delete link;
            it.remove();
        }
    }

    qDeleteAll(items);
}
