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

/**
 * Default number of cells reserved for key names.
 */
#define ZM_RESOURCE_KEY_SIZE 8      // 32 bytes (including null terminator)

/*____________________________________________________________________________*/

/**
 * Valid data types for entities.
 *
 * TODO: Arrays, handles and function references?
 */
enum ZMEntityDataType
{
    ZMEntityDataType_Cell,
    ZMEntityDataType_Bool,
    ZMEntityDataType_Float,
    ZMEntityDataType_String,
    ZMEntityDataType_Entity
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
 * @param blockSize     Number of cells reserved for each attribute value.
 * @param keySize       Number of cells reserved for attribute names.
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
 *          error when accessed.
 *
 * @param type          Entity type ID.
 */
native ZM_DeleteEntityType(ZMEntityType:type);

/*____________________________________________________________________________*/

/**
 * Creates a new named repository using the specified entity type.
 *
 * @param name          Unique identifier for the repository.
 * @param type          The type of data structure that is stored.
 * @param isPrivate     (Optional) Whether the repository is only accessible by
 *                      the module that created it.
 *
 * @return              Repository ID.
 * @error               Invalid name, type or name already in use.
 */
native ZMRepository:ZM_CreateRepository(
        const String:name[],
        ZMEntityType:type,
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
 *          error when accessed.
 *
 * @param repository    Repository ID.
 *
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
 *          when accessed.
 *
 * @param entity        Entity ID.
 *
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
 */
native ZM_AddEntityAttribute(
        ZMEntityType:type,
        const String:name[],
        ZMEntityDataType:dataType,
        ZMEntityConstraint:constraint = INVALID_ZM_ENTITY_CONSTRAINT);

/*____________________________________________________________________________*/

/*
TODO:
* Constraint builders
* Accessor functions (some of them can be stock functions)
* Validation helpers (to avoid repeated code in accessor functions)
* Load from file (keyvalues)
* Need to handle reloading of modules that attempts to recreate repositories
*/
