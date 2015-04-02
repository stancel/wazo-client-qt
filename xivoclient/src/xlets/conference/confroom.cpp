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

#include "confroom.h"


ConfRoom::ConfRoom(QWidget *parent, ConfTab *tab, const QString &number, const QVariantMap &members)
    : QWidget(parent), m_number(number)
{
    QVBoxLayout *vBox = new QVBoxLayout(this);
    setLayout(vBox);
    QHBoxLayout *hBox = new QHBoxLayout();
    m_model = new ConfRoomModel(tab, this, number, members);
    QLabel *redondant = new QLabel(tr(" Conference room %1").arg(number));
    setProperty("id", number);

    hBox->addStretch(1);
    hBox->addWidget(redondant, 6);
    hBox->addStretch(1);
    vBox->addLayout(hBox);

    hBox = new QHBoxLayout();

    m_view = new ConfRoomView(this);
    m_view->setModel(m_model);
    m_view->updateHeadersView();

    m_view->setStyleSheet("ConfRoomView {"
                            "border: none;"
                            "background:transparent;"
                            "color:black;"
                        "}");

    m_view->verticalHeader()->hide();

    hBox->addWidget(m_view);

    vBox->addLayout(hBox);
}
