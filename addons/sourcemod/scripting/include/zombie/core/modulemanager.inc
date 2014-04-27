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

GOALS

Technical user stories:

* As a plugin I need to register myself as a module in the module manager so I
  can become an authenticated module in the zombie framework.

* As a module I need to have a unique ID so I can be identified by the zombie
  framework.

* As a module I need to have a name so I can be identified by server admins.

* As a module I need to have a description so server admins know what I do.

* As a module I need to declare unique named features that other modules can
  declare dependency on, to avoid explicit dependency on the module itself.

* As a module I need to declare dependency on a named feature.

* As a module I need to be notified when a dependent feature is unavailable so I
  can do cleanup before disabling myself.

* As a module I need to be notified when a dependent feature is available so I
  can re-initialize myself.


Context awareness:
Some natives are context aware, so that only the owner plugin can do actions
on its own module. We don't need to pass the module ID, because registered
modules can be looked up by the plugin handle of the caller.

Dependencies:
The module manager doesn't support module dependencies. Instead, modules declare
dependencies on specific features, regardless which module that implements it.

Unique module ID:
Since a module can be unloaded and reloaded, it must re-register itself as a
module and will get another module ID. The old module ID will not be used again,
until the server is restarted.

Bootstrap event:
The module manager should make use of RegPluginLibrary so plugins can listen for
availability of the module manager. The ideal solution is that even the module
manager can be reloaded in the middle of the game, and that modules will
dynamically react to this event. It will obviously cause some interruptions in
the game, but modules should be flexible enough to start over after resetting
the round.

*/

/*____________________________________________________________________________*/

// TODO: Events for initializing module on late load.

// TODO: Add natives for enabling and disabling modules?

/*____________________________________________________________________________*/

/**
 * @section Events
 */

functag OnFeatureAdded(ZMFeature:featureID);
functag OnFeatureRemoved(ZMFeature:featureID);

/**
 * @endsection
 */

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
 */
enum ZMFeature
{
    INVALID_ZM_FEATURE = -1
}

/*____________________________________________________________________________*/

/**
 * Creates a new module. The new module is bound to the calling plugin.
 *
 * Note: A plugin is only allowed to create one module. The module will remain
 *       registered until the plugin is unloaded.
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
 * @param plugin        Handle to plugin representing a module.
 *
 * @return              Module ID, or INVALID_ZM_MODULE if not found.
 */
native ZMModule:ZM_GetModuleByPlugin(Handle:plugin);

/*____________________________________________________________________________*/

/**
 * Gets the plugin that created the specified module.
 *
 * @param module        Module to look up.
 *
 * @return              Handle to owner plugin, or INVALID_ZM_MODULE if not
 *                      found.
 */
native Handle:ZM_GetModuleOwner(ZMModule:module);

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
 * @param module        Module to read from.
 * @param buffer        Output buffer.
 * @param maxlen        Size of output buffer.
 *
 * @return              Number of characters written.
 */
native ZM_GetModuleDescription(ZMModule:module, String:buffer[], maxlen);

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
 * Declares a feature identifier and assigns it to the caller module.
 *
 * @param identifier    Global and unique name of the feature. Other modules
 *                      may declare dependency on this feature.
 *
 * @return              Feature ID.
 * @error               Name is blank or already in use.
 */
native ZMFeature:ZM_CreateFeature(const String:identifier[]);

/*____________________________________________________________________________*/

/**
 * Deletes and unregisters a feature in the caller module.
 *
 * @param feature       Feature to delete.
 */
native ZM_DeleteFeature(ZMFeature:feature);

/*____________________________________________________________________________*/

/**
 * Gets a feature.
 *
 * @param identifier    Feature identifier string.
 *
 * @return              Feature ID if found, otherwise INVALID_ZM_FEATURE.
 */
native ZMFeature:ZM_GetFeature(const String:identifier[]);

/*____________________________________________________________________________*/

/**
 * Returns whether the specified feature exists.
 *
 * @param identifier    Feature identifier to search for.
 *
 * @return              True if feature exists, false otherwise.
 */
stock bool:ZM_FeatureExists(const String:identifier[])
{
    return ZM_GetFeature(identifier) != INVALID_ZM_FEATURE;
}

/*____________________________________________________________________________*/

/**
 * Gets the specified feature's owner module.
 *
 * @parma feature       Feature to look up.
 *
 * @return              Module ID if found, otherwise INVALID_ZM_MODULE.
 */
native ZMModule:ZM_GetFeatureOwner(ZMFeature:feature);
