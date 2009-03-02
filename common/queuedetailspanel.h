/* XIVO CTI clients
 * Copyright (C) 2007-2009  Proformatique
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License version 2 for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA.
 *
 * Linking the Licensed Program statically or dynamically with other
 * modules is making a combined work based on the Licensed Program. Thus,
 * the terms and conditions of the GNU General Public License version 2
 * cover the whole combination.
 *
 * In addition, as a special exception, the copyright holders of the
 * Licensed Program give you permission to combine the Licensed Program
 * with free software programs or libraries that are released under the
 * GNU Library General Public License version 2.0 or GNU Lesser General
 * Public License version 2.1 or any later version of the GNU Lesser
 * General Public License, and with code included in the standard release
 * of OpenSSL under a version of the OpenSSL license (with original SSLeay
 * license) which is identical to the one that was published in year 2003,
 * or modified versions of such code, with unchanged license. You may copy
 * and distribute such a system following the terms of the GNU GPL
 * version 2 for the Licensed Program and the licenses of the other code
 * concerned, provided that you include the source code of that other code
 * when and as the GNU GPL version 2 requires distribution of source code.
 */

/* $Revision$
 * $Date$
 */

#ifndef __QUEUEDETAILSPANEL_H__
#define __QUEUEDETAILSPANEL_H__

#include <QDateTime>
#include <QHash>
#include <QObject>
#include <QVariant>
#include <QWidget>

class QGridLayout;
class QLabel;
class QPushButton;
class QScrollArea;
class QVBoxLayout;

class BaseEngine;
class UserInfo;

/*! \brief Display details about a queue
 *
 * The agents in this queue and details are displayed
 */
class QueuedetailsPanel : public QWidget
{
    Q_OBJECT
        public:
    QueuedetailsPanel(BaseEngine *,
                      const QVariant &,
                      QWidget * parent = 0);
    ~QueuedetailsPanel();
 signals:
    void changeWatchedAgent(const QString &, bool); //!< select an agent to watch
 protected:
    void timerEvent(QTimerEvent *);
    public slots:
    void setGuiOptions(const QVariant &);
    void setUserInfo(const UserInfo *);
    void newQueue(double, const QString &, const QString &, const QVariant &);
    void setAgentList(double, const QVariant &);
    void updatePeerAgent(double,
                         const QString &,
                         const QString &,
                         const QVariant &);
    private slots:
    void agentClicked();
 private:
    void update();
        
    double m_timesrv;
    QDateTime m_timeclt;
        
    BaseEngine * m_engine;        //!< BaseEngine object
    QGridLayout * m_gridlayout; //!< Layout
    //QVBoxLayout * m_layout; //
    //    QScrollArea * m_scrollarea;
    //QWidget * m_widget;
        
    QString m_astid;    //!< asterisk id
    QString m_queueid;  //!< queue id
    QLabel * m_queuelegend_agentid; //!< "Agent" label
    QLabel * m_queuelegend_status;  //!< "Status" label
    QLabel * m_queuelegend_paused;  //!< "Paused" label
    QLabel * m_queuelegend_callstaken;  //!< "Call Taken" label
    QLabel * m_label;       //!< queue name label
    QHash<QString, QLabel *> m_agentlabels; //!< agents name
    QHash<QString, QPushButton *> m_agentmore;  //!< "+" buttons
    QHash<QString, QLabel *> m_agentstatus; //!< agents status
    QHash<QString, QLabel *> m_agentpaused; //!< agents paused
    QHash<QString, QLabel *> m_agentncalls; //!< agents number of calls
        
    QVariantMap m_agentlist;    //!< list of agents in queue
    QVariantMap m_agentlists;   //!< list of agents in asterisk instance
};

#endif /* __QUEUEDETAILSPANEL_H__ */
