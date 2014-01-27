/*
 * ============================================================================
 *
 *  Zombie API
 *
 *  File:           modulemanager.inc
 *  Description:    API for registering and managing modules.
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

#if defined _zm_modulemanager_included
 #endinput
#endif
#define _zm_modulemanager_included

/*____________________________________________________________________________*/

/*

Context awareness:
Some natives are context aware, so that only the owner plugin can do actions
on its own module. We don't need to pass the module ID, because registered
modules can be looked up by the plugin handle of the caller.

Dependencies:
Module dependencies might not be a good idea, because it creates a coupling to
a complete module while you may just need a small feature. A better approach
might be to declare dependencies on specific features, so it doesn't matter
which module that implements it.

*/

/*____________________________________________________________________________*/

// TODO: Events for initializing module on late load.

// TODO: Add natives for enabling and disabling modules?

/*____________________________________________________________________________*/

/**
 * Module ID.
 */
enum ZMModule
{
    INVALID_ZM_MODUlE = -1
}

/*____________________________________________________________________________*/

/**
 * Feature ID.
 *
 * TODO: Should possibly be moved to its own feature API, but the feature
 *       information might be stored along with modules since they're related.
 */
enum ZMFeature
{
    INVALID_ZM_FEATURE = -1
}

/*____________________________________________________________________________*/

/**
 * Creates a new module. A plugin is only allowed to create one module.
 *
 * @param name          Unique module name. Used by lookup functions.
 *
 * @return              Module ID.
 * @error               Plugin has already created a module, id is already in
 *                      use.
 */
native ZMModule:ZM_CreateModule(const String:name[]);

/*____________________________________________________________________________*/

/**
 * Looks up a module by the specified plugin handle value.
 *
 * @param plugin        Handle to plugin representing a moudle.
 *
 * @return              Module ID, or INVALID_ZM_MODULE if not found.
 */
native ZMModule:ZM_GetModuleByPlugin(Handle:plugin);

/*____________________________________________________________________________*/

/**
 * Looks up a module by the specified name.
 *
 * @param name          Name to search for.
 *
 * @return              Module ID, or INVALID_ZM_MODULE if not found.
 */
native ZMModule:ZM_GetModuleByName(const String:name[]);

/*____________________________________________________________________________*/

/**
 * Gets the module description.
 *
 * @param moduleID      Module ID.
 * @param buffer        Output buffer.
 * @param maxlen        Size of output buffer.
 *
 * @return              Number of characters written.
 */
native ZM_GetModuleDescription(ZMModule:moduleID, String:buffer[], maxlen);

/*____________________________________________________________________________*/

/**
 * Sets the module brief description. This may be disabled by administration
 * commands.
 *
 * @param description   Description to set.
 */
native ZM_SetModuleDescription(const String:description[]);

/*____________________________________________________________________________*/

/**
 * Declares a dependency on the specified module. If a dependency is
 * unavailable, the module will be notified.
 *
 * @param dependency    Module that the plugin depends on.
 *
 * @error               Invalid dependency, or plugin is not registered as a
 *                      module.
 */
native ZM_AddModuleDependency(ZMModule:dependency);

/*____________________________________________________________________________*/

/**
 * Removes a dependency on the specified module.
 *
 * @param dependency    Module that the plugin depends on.
 *
 * @error               Invalid dependency, or plugin is not registered as a
 *                      module.
 */
native ZM_RemoveModuleDependency(ZMModule:dependency);
