/*
 * ============================================================================
 *
 *  Zombie API
 *
 *  File:           resourcemanager.inc
 *  Description:    API for managing general data resources.
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

#if defined _zm_resourcemanager_included
 #endinput
#endif
#define _zm_resourcemanager_included

#include <zombie/core/resourcemanager/resourceaccessors>

/*____________________________________________________________________________*/

/*

RESOURCE MANAGER

The resource manager is a common module that stores user defined entities and
repositories. It's quite similar to a memory-database, but entities in the
resource manager is accessed directly instead of through database queries.

It supports references between entities and enables data to be stored in a
organized and typesafe manner.

Don't expect the resource manager to be super efficient due to run-time lookup
of attributes and validation. Otherwise the performance depends on the
implementation, but the goal to be good enough. It's recommended to cache data
that's accessed frequently, such as in damage/bullet events, fast timers or
other busy events.

Feature modules will be responsible for declaring entity types and repositories.
The resource manager may help with loading KeyValue files into repositories. If
modules assign constraints it will also validate the data to ensure only valid
data is written to the repositories.

For live debugging purposes an inspector tool may be available, but this is
optional and may not be implemented.


TERMINOLOGY

Entity:
A simple data structure with named attributes of specific types; an object.

Entity type:
Meta data about an entity. Stores the actual attribute names, data types and
constraints. Shared between entities of the same type.

Constraint:
A validation rule applied to a single value. Constraints may be assigned to
entity attributes to ensure data integrity.

Repository:
A collection of entities of a certain type.

*/

/*____________________________________________________________________________*/

/**
 * Default number of cells reserved for key names.
 */
#define ZM_RESOURCE_KEY_SIZE 8      // 32 bytes (including null terminator)

/*____________________________________________________________________________*/

/**
 * Valid data types for entities.
 */
enum ZMEntityDataType
{
    ZMEntityDataType_Cell,          /** Cell or any singular value. */
    ZMEntityDataType_Bool,
    ZMEntityDataType_Float,
    ZMEntityDataType_Array,         /** One-dimensional array. */
    ZMEntityDataType_String,
    ZMEntityDataType_Reference,     /** Reference to another entity. */
    ZMEntityDataType_ReferenceType  /** Reference to an entity type. */
}

/*____________________________________________________________________________*/

/**
 * Entity type ID.
 */
enum ZMEntityType
{
    INVALID_ZM_ENTITY_TYPE = -1
}

/*____________________________________________________________________________*/

/**
 * Entity attribute ID.
 */
enum ZMEntityAttribute
{
    INVALID_ZM_ENTITY_ATTRIBUTE = -1;
}

/*____________________________________________________________________________*/

/**
 * Entity constraint ID.
 */
enum ZMEntityConstraint
{
    INVALID_ZM_ENTITY_CONSTRAINT = -1
}

/*____________________________________________________________________________*/

/**
 * Entity ID.
 */
enum ZMEntity
{
    INVALID_ZM_ENTITY = -1
}

/*____________________________________________________________________________*/

/**
 * Repository ID.
 */
enum ZMRepository
{
    INVALID_ZM_REPOSITORY = -1
}

/*____________________________________________________________________________*/

/**
 * Creates a new named entity type.
 *
 * @param name          Unique identifier for the entity type.
 * @param blockSize     Number of cells reserved for each attribute value. This
 *                      defines the maximum size of arrays or strings that can
 *                      be stored in attributes.
 * @param keySize       (Optional) Number of cells reserved for attribute names.
 *                      Default is 8 cells (32 bytes).
 *
 * @return              Entity type ID. Must be deleted with ZM_DeleteEntityType
 *                      when no longer in use.
 * @error               Invalid parameter value, or name already in use.
 */
native ZMEntityType:ZM_CreateEntityType(
        const String:name[],
        blockSize = 1,
        keySize = ZM_RESOURCE_KEY_SIZE);

/*____________________________________________________________________________*/

/**
 * Gets an entity type by name.
 *
 * @param name          Name of entity type.
 *
 * @return              Entity type ID, or INVALID_ZM_ENTITY_TYPE if not found.
 */
native ZMEntityType:ZM_GetEntityType(const String:name[]);

/*____________________________________________________________________________*/

/**
 * Deletes the specified entity type.
 *
 * Warning: All references to this entity type will become invalid and cause an
 *          error if accessed.
 *
 * @param type              Entity type ID.
 * @param resetReference    (Optional) Reset type to INVALID_ZM_ENTITY_TYPE when
 *                          deleted. Default is true.
 * @param deleteConstraints (Optional) Delete constraint definitions attached to
 *                          attributes. Default is false.
 *
 * @noreturn
 */
native ZM_DeleteEntityType(
        ZMEntityType:type,
        bool:resetReference = true,
        bool:deleteConstraints = false);

/*____________________________________________________________________________*/

/**
 * Creates a new named repository capable of storing entities of the specified
 * type.
 *
 * @param name          Unique identifier for the repository.
 * @param entityType    The type of data structure that is stored.
 * @param isPrivate     (Optional) Whether the repository is only accessible by
 *                      the module (plugin) that created it.
 *
 * @return              Repository ID.
 * @error               Invalid name, type or name already in use.
 */
native ZMRepository:ZM_CreateRepository(
        const String:name[],
        ZMEntityType:entityType,
        bool:isPrivate = false);

/*____________________________________________________________________________*/

/**
 * Gets a repository by name.
 *
 * @param name          Repository name.
 *
 * @return              Repository ID or INVALID_ZM_REPOSITORY if not found.
 */
native ZMRepository:ZM_GetRepository(const String:name[]);

/*____________________________________________________________________________*/

/**
 * Deletes the specified repository.
 *
 * Warning: All references to this repository will become invalid and cause an
 *          error if accessed.
 *
 * @param repository    Repository ID.
 *
 * @noreturn
 * @error               Invalid repository ID.
 */
native ZM_DeleteRepository(ZMRepository:repository);

/*____________________________________________________________________________*/

/**
 * Creates a new entity of the specified type with null values.
 *
 * @param type          Entity type ID.
 *
 * @return              Entity ID.
 */
native ZMEntity:ZM_CreateEntity(ZMEntityType:type);

/*____________________________________________________________________________*/

/**
 * Deletes the specified entity.
 *
 * Warning: All references to this entity will become invalid and cause an error
 *          if accessed.
 *
 * @param entity        Entity ID.
 *
 * @noreturn
 * @error               Invalid entity ID.
 */
native ZM_DeleteEntity(ZMEntity:entity);

/*____________________________________________________________________________*/

/**
 * Adds an attribute to the specified entity type.
 *
 * @param type          Entity type to add attribute to.
 * @param name          Name of attribute. Must be unique for the entity type.
 * @param dataType      Data type of attribute.
 * @param constraint    (Optional) Attribute constraints (max/min, etc).
 *
 * @return              Attribute ID.
 * @error               Invalid entity type, data type, or name is already in
 *                      use for this type.
 */
native ZMEntityAttribute:ZM_AddEntityAttribute(
        ZMEntityType:type,
        const String:name[],
        ZMEntityDataType:dataType,
        ZMEntityConstraint:constraint = INVALID_ZM_ENTITY_CONSTRAINT);

/*____________________________________________________________________________*/

/**
 * Returns the ID of an attribute in the specified entity type.
 *
 * @param type          Entity type to inspect.
 * @param name          Name of attribute to look up.
 *
 * @return              Attribute ID if found, otherwise
 *                      INVALID_ZM_ENTITY_ATTRIBUTE.
 * @error               Invalid entity type.
 */
native ZMEntityAttribute:ZM_GetAttributeID(
        ZMEntityType:type,
        const String:name[]);

/*____________________________________________________________________________*/

/*
TODO:
* Constraint builders
* Validation helpers (to avoid repeated code in accessor functions)
* Load from file (keyvalues)
* Need to handle reloading of modules that attempts to recreate repositories
* Resource events: entity modified, added, removed, etc.
* Event listeners for single entites (may be expensive).
*/
