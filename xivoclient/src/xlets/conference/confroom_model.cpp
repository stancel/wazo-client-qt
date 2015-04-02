/* XiVO Client
 * Copyright (C) 2007-2014 Avencall
 *
 * This file is part of XiVO Client.
 *
 * XiVO Client is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version, with a Section 7 Additional
 * Permission as follows:
 *   This notice constitutes a grant of such permission as is necessary
 *   to combine or link this software, or a modified version of it, with
 *   the OpenSSL project's "OpenSSL" library, or a derivative work of it,
 *   and to copy, modify, and distribute the resulting work. This is an
 *   extension of the special permission given by Trolltech to link the
 *   Qt code with the OpenSSL library (see
 *   <http://doc.trolltech.com/4.4/gpl.html>). The OpenSSL library is
 *   licensed under a dual license: the OpenSSL License and the original
 *   SSLeay license.
 *
 * XiVO Client is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with XiVO Client.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <QTimer>

#include "confroom_model.h"


enum ColOrder {
    ID, ACTION_MUTE, ACTION_KICK, ACTION_TALK_TO,
    ACTION_ALLOW_IN, ACTION_RECORD, ADMIN,
    NAME, NUMBER, SINCE, NB_COL
};

static QVariant COL_TITLE[NB_COL];

ConfRoomModel::ConfRoomModel(ConfTab *tab, QWidget *parent, const QString &number, const QVariantMap &members)
    : QAbstractTableModel(parent), m_tab(tab), m_parent(parent), m_admin(0),
      m_authed(0), m_number(number), m_members(members)
{
    connect(b_engine, SIGNAL(meetmeUpdate(const QVariantMap &)),
            this, SLOT(updateMeetmeConfig(const QVariantMap &)));

    extractRow2IdMap();

    COL_TITLE[ID] = tr("ID");
    COL_TITLE[NUMBER] = tr("Number");
    COL_TITLE[NAME] = tr("Name");
    COL_TITLE[SINCE] = tr("Since");
    COL_TITLE[ADMIN] = tr("Admin");
    COL_TITLE[ACTION_KICK] = tr("K");
    COL_TITLE[ACTION_RECORD] = tr("R");
    COL_TITLE[ACTION_ALLOW_IN] = tr("A");
    COL_TITLE[ACTION_TALK_TO] = tr("T");
    COL_TITLE[ACTION_MUTE] = tr("M");

    QTimer * join_time_timer = new QTimer(this);
    connect(join_time_timer, SIGNAL(timeout()),
            this, SLOT(updateJoinTime()));
    join_time_timer->start(1000);
}

void ConfRoomModel::updateMeetmeConfig(const QVariantMap &config)
{
    beginResetModel();
    m_members = config[m_number].toMap()["members"].toMap();
    extractRow2IdMap();
    endResetModel();
}

void ConfRoomModel::extractRow2IdMap()
{
    m_row2number = m_members.keys();
}

void ConfRoomModel::sort(int column, Qt::SortOrder order)
{
    struct {
        static bool ascending(const QPair<QString, QString> &a,
                              const QPair<QString, QString> &b) {
            return QString::localeAwareCompare(a.second, b.second) < 0 ?
                                               true : false;
        }
        static bool descending(const QPair<QString, QString> &a,
                               const QPair<QString, QString> &b) {
            return QString::localeAwareCompare(a.second, b.second) < 0 ?
                                               false : true;
        }
    } sFun;

    QList<QPair<QString, QString> > toSort;

    int count = rowCount(QModelIndex());
    beginResetModel();
    for (int i = 0; i < count; i++) {
        toSort.append(QPair<QString, QString>(index(i, ID).data().toString(),
                                              index(i, column).data().toString()));
    }

    qSort(toSort.begin(), toSort.end(), (order == Qt::AscendingOrder) ?
                                         sFun.ascending :
                                         sFun.descending);

    for (int i = 0; i < count; i++) {
        m_row2number.insert(i, QString(toSort[i].first));
    }
    endResetModel();
}

int ConfRoomModel::rowCount(const QModelIndex &) const
{
    return m_members.size();
}

int ConfRoomModel::columnCount(const QModelIndex&) const
{
    return NB_COL;
}

bool ConfRoomModel::isRowMuted(int row) const
{
    const QVariantMap &member = m_members[m_row2number[row]].toMap();
    return member["muted"].toString() == "Yes";
}

int ConfRoomModel::userNumberFromRow(int row) const
{
    const QString &number = m_row2number[row];
    return number.toInt();
}

void ConfRoomModel::updateJoinTime()
{
    QModelIndex first = createIndex(0, SINCE);
    QModelIndex last = createIndex(m_members.size() - 1, SINCE);

    emit dataChanged(first, last);
}

QVariant ConfRoomModel::data(const QModelIndex & index, int role) const
{
    int row = index.row();
    int col = index.column();
    const QString &number = m_row2number[row];
    const QVariantMap &member = m_members[number].toMap();
    int join_sequence = member["join_order"].toInt();
    bool isMe = b_engine->isMeetmeMember(m_number, join_sequence);

    if (role != Qt::DisplayRole) {
        if (role == Qt::TextAlignmentRole) {
            return Qt::AlignCenter;
        } else if (role == Qt::DecorationRole) {
            if (col == ACTION_MUTE && isMe) {
                return QPixmap(":images/conference/mute.png").scaledToHeight(16, Qt::SmoothTransformation);
            }
        } else if (role == Qt::ToolTipRole) {
            if (col == ACTION_MUTE) {
                return tr("Mute/UnMute");
            }
        }
        return QVariant();
    }

    int started_since = member["join_time"].toInt();

    switch (col) {
    case ID:
        return member["join_order"].toInt();
    case NUMBER:
        return member["number"].toString();
    case ACTION_RECORD:
        return tr("No");
    case ADMIN:
        return tr("No");
    case NAME:
        return member["name"].toString();
    case ACTION_ALLOW_IN:
        return tr("Yes");
    case SINCE:
        if (started_since == -1)
            return tr("Unknown");
        else if (started_since == 0)
            return tr("Not started");
        return QDateTime::fromTime_t(
            QDateTime::currentDateTime().toTime_t()
            - started_since
            - b_engine->timeDeltaServerClient()).toUTC().toString("hh:mm:ss");
    default:
        break;
    }
    return QVariant();
}

QVariant ConfRoomModel::headerData(int section,
                                   Qt::Orientation orientation,
                                   int role) const
{
    if (role != Qt::DisplayRole)
        return QVariant();

    if (orientation == Qt::Horizontal) {
        return COL_TITLE[section];
    }

    return QVariant();
}

Qt::ItemFlags ConfRoomModel::flags(const QModelIndex &index) const
{
    int col = index.column();
    if (col != ACTION_MUTE) return Qt::NoItemFlags;

    int row = index.row();
    const QString &number = m_row2number[row];
    const QVariantMap &member = m_members[number].toMap();
    bool isMuted = member["muted"] == "Yes";
    bool isMe = b_engine->isMeetmeMember(m_number, number.toInt());

    if (isMe && col == ACTION_MUTE && isMuted) {
        return Qt::ItemIsEnabled;
    }
    return Qt::NoItemFlags;
}

