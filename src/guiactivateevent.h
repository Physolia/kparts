/*
    This file is part of the KDE project
    SPDX-FileCopyrightText: 1999 Simon Hausmann <hausmann@kde.org>
    SPDX-FileCopyrightText: 1999 David Faure <faure@kde.org>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#ifndef __kparts_guiactivateevent_h__
#define __kparts_guiactivateevent_h__

#include <kparts/event.h>

namespace KParts
{

class GUIActivateEventPrivate;
/**
 * @class GUIActivateEvent guiactivateevent.h <KParts/GUIActivateEvent>
 *
 * @short This event is sent to a Part when its GUI has been activated or deactivated.
 * This is related to PartActivateEvent, but the difference is that
 * GUIActivateEvent happens later (when the GUI is actually built),
 * only for parts that have GUI elements, and only if using KParts::MainWindow.
 * @see KParts::Part::guiActivateEvent()
 */
class KPARTS_EXPORT GUIActivateEvent : public Event
{
public:
    GUIActivateEvent(bool activated);
    virtual ~GUIActivateEvent();

    bool activated() const;

    static bool test(const QEvent *event);

private:
    GUIActivateEventPrivate *const d;
};

} // namespace

#endif
