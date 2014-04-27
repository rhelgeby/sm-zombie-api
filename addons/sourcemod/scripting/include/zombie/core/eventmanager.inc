/*
 * ============================================================================
 *
 *  Zombie Mod API
 *
 *  File:           eventmanager.inc
 *  Description:    Create and manage abstract events.
 *
 *  Copyright (C) 2014  Richard Helgeby
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * ============================================================================
 */

#if defined _zm_eventmanager_included
 #endinput
#endif
#define _zm_eventmanager_included

/*____________________________________________________________________________*/

/*

IMPLEMENTATION GUIDELINES

Event owner:
The plugin that creates an event is considered the event owner. Only that plugin
is allowed to fire the event.

Event hooks:
Modules must be looked up by their plugin handle. Use ZM_GetModuleByPlugin in
modulemanager.inc. That way the plugins doesn't have to pass their own module
ID each time, and we avoid that they pass an invalid ID.

In addition modules must be authorized before an event hook is allowed. Use
ZM_SecurityCheck in auth.inc. Cache the result and update within
OnAuthSettingsChanged event. When security settings have changed, event hooks
that modules previously had access to, but no longer, should be removed (maybe
we also need an event to notify the module).

New and unloaded plugins:
The event manager must update its list of event callbacks when a plugin is
unloaded, so that any event it may own is unregistered and properly removed.
Event hooks that other mouldes made to that event should be unhooked.

*/

/*____________________________________________________________________________*/

/**
 * Event ID.
 */
enum ZMEvent
{
    INVALID_ZM_EVENT;
}

/*____________________________________________________________________________*/

/**
 * @section Predefined bootstrap events created by the event manager.
 */

/**
 * Called when events are being created. Modules that create their own events
 * should create them in this event.
 */
#define ZM_ON_EVENTS_CREATE "OnEventsCreate"
functag ZM_OnEventsCreate();

/**
 * All events are created. Modules should register event hooks in this event.
 */
#define ZM_ON_EVENTS_CREATED "OnEventsCreated"
functag ZM_OnEventsCreated();

/** 
 * @endsection
 */

/*____________________________________________________________________________*/

/**
 * Creates a new named event.
 *
 * @param eventName     Name of new event (case sensitive).
 *
 * @return              Event ID if successful, otherwise INVALID_ZM_EVENT.
 */
native ZMEvent:ZM_CreateEvent(const String:eventName);

/*____________________________________________________________________________*/

/**
 * Deletes an existing event from the event manager.
 *
 * @param eventID       Event to delete.
 *
 * @return              True if deleted, otherwise false.
 */
native bool:ZM_DeleteEvent(ZMEvent:eventID);

/*____________________________________________________________________________*/

/**
 * Validates an event ID.
 *
 * @param eventID       Event to validate.
 *
 * @return              True if event exists, otherwise false.
 */
native bool:ZM_IsValidEvent(ZMEvent:eventID);

/*____________________________________________________________________________*/

/**
 * Gets an event by its name.
 *
 * @param eventName     Event name to look up.
 *
 * @return              Event id if successful, otherwise INVALID_ZM_EVENT.
 */
native ZMEvent:ZM_GetEventByName(const String:eventName[]);

/*____________________________________________________________________________*/

/**
 * Removes an event hook.
 *
 * @param event         Event ID.
 * @param callback      Callback to unhook.
 *
 * @noreturn
 * @error               Invalid event ID or callback.
 */
native ZM_UnhookEvent(ZMEvent:event, Function:callback);

/*____________________________________________________________________________*/

/**
 * Registers an event hook for the OnEventsCreate event.
 *
 * @param callback      Function to call when event is fired.
 *
 * @return              Event ID.
 * @error               Event is already hooked by the module.
 */
native ZMEvent:ZM_HookEventsCreate(ZM_OnEventsCreate:callback);

/*____________________________________________________________________________*/

/**
 * Registers an event hook for the OnEventsCreated event.
 *
 * @param callback      Function to call when event is fired.
 *
 * @return              Event ID.
 * @error               Event is already hooked by the module.
 */
native ZMEvent:ZM_HookEventsCreated(ZM_OnEventsCreated:callback);
