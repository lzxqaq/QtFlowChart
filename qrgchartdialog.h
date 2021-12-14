#ifndef QRGCHARTDIALOG_H
#define QRGCHARTDIALOG_H

#include <QDialog>

class QGraphicsView;
class QGraphicsScene;
class QMenu;
class Node;
class QrgChartDialog : public QDialog
{
    Q_OBJECT

public:
    explicit QrgChartDialog(QWidget *parent = 0);
    ~QrgChartDialog();

    void initOrgChartDialog();
    void createActions();

    Node *getSelectedNode();

private:
    QGraphicsView  *m_view;
    QGraphicsScene *m_scene;

    QMenu          *m_topMenu;
    QAction        *m_actAdd;
    QAction        *m_actDel;

    int             m_leftCount;
    int             m_rightCount;

public slots:
    void onSceneSelectionChanged();
    void onAdd();
    void onDelete();

};

#endif // QRGCHARTDIALOG_H
