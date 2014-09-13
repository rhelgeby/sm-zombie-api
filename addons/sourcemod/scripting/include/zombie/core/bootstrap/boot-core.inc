/*
 * ============================================================================
 *
 *  Zombie API
 *
 *  File:           boot-core.inc
 *  Description:    Bootstrapper for the module and event managers.
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

/*

USAGE

This bootstrapper will notify you when the module and event managers are
available or unavailable. Once the module and event system is running and
available, you should be able to initialize the rest of your module yourself.

To use this bootstrapper, create the following functions in your plugin. These
will be called by this bootstrapper when the module manager is added or removed.

    ZM_OnCoreLoaded
        Create your module in this function. You need to store the module ID
        somewhere, so it can be deleted later.

    ZM_OnCoreUnloaded
        Delete your module in this function.

Call the following functions. The last part of their names tell where you should
call them from:

    ZM_ModuleMgr_AskPluginLoad2
    ZM_ModuleMgr_OnAllPluginsLoaded
    ZM_ModuleMgr_OnPluginEnd
    ZM_ModuleMgr_OnLibraryAdded
    ZM_ModuleMgr_OnLibraryRemoved

*/

/*____________________________________________________________________________*/

new bool:ZM_ModuleManagerLoaded = false;
new bool:ZM_EventManagerLoaded = false;

new bool:ZM_CoreDependenciesLoaded = false;

/*____________________________________________________________________________*/

stock ZM_CheckLibraryStates()
{
    // Check previous state to prevent double events.
    new bool:loadedLastTime = ZM_CoreDependenciesLoaded;
    
    if (ZM_IsCoreDependenciesLoaded && !loadedLastTime)
    {
        LogMessage("Core modules available.");
        
        // Defined in module.
        ZM_OnCoreLoaded();
    }
    else if (loadedLastTime && !ZM_IsCoreDependenciesLoaded)
    {
        LogMessage("Core modules unavailable.");
        
        // Defined in module.
        ZM_OnCoreUnloaded();
    }
}

/*____________________________________________________________________________*/

stock APLRes:ZM_BootMgr_AskPluginLoad2(Handle:myself, bool:late, String:error[], err_max)
{
    ZM_SetModuleManagerOptional();
    ZM_SetEventManagerOptional();
    return APLRes_Success;
}

/*____________________________________________________________________________*/

stock ZM_ModuleMgr_OnAllPluginsLoaded()
{
    ZM_ModuleManagerLoaded = LibraryExists(LIBRARY_ZM_MODULE_MANAGER);
    ZM_EventManagerLoaded = LibraryExists(LIBRARY_ZM_MODULE_MANAGER);
    
    ZM_CheckLibraryStates();
}

/*____________________________________________________________________________*/

stock ZM_ModuleMgr_OnPluginEnd()
{
    ZM_ModuleManagerLoaded = false;
    ZM_EventManagerLoaded = false;
    
    ZM_CheckLibraryStates();
}

/*____________________________________________________________________________*/

stock ZM_ModuleMgr_OnLibraryAdded(const String:name[])
{
    ZM_UpdateLibraryState(name, true);
    ZM_CheckLibraryStates();
}

/*____________________________________________________________________________*/

stock ZM_ModuleMgr_OnLibraryRemoved(const String:name[])
{
    ZM_UpdateLibraryState(name, false);
    ZM_CheckLibraryStates();
}

/*____________________________________________________________________________*/

stock ZM_UpdateLibraryState(const String:libraryName[], bool:loaded)
{
    if (StrEqual(name, LIBRARY_ZM_MODULE_MANAGER))
    {
        ZM_ModuleManagerLoaded = loaded;
    }
    else if (StrEqual(name, LIBRARY_ZM_EVENT_MANAGER))
    {
        ZM_EventManagerLoaded = loaded;
    }
}

/*____________________________________________________________________________*/

stock ZM_IsCoreDependenciesLoaded()
{
    return ZM_ModuleManagerLoaded && ZM_EventManagerLoaded;
}
