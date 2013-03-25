#ifndef __AGENT_STATUS_DELEGATE__
#define __AGENT_STATUS_DELEGATE__

#include <QAbstractItemDelegate>

class AgentStatusWidgetStorage;

class AgentStatusDelegate: public QAbstractItemDelegate
{
 public:
    AgentStatusDelegate(AgentStatusWidgetStorage & widget_storage);
    ~AgentStatusDelegate();
    void paint(QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index) const;
    QSize sizeHint(const QStyleOptionViewItem & option, const QModelIndex & index) const;
 private:
    AgentStatusWidgetStorage & m_widget_storage;
};

#endif
