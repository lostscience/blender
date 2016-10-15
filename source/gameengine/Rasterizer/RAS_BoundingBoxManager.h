/*
 * ***** BEGIN GPL LICENSE BLOCK *****
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 * The Original Code is: all of this file.
 *
 * Contributor(s): Tristan Porteries.
 *
 * ***** END GPL LICENSE BLOCK *****
 */

/** \file RAS_BoundingBoxManager.h
 *  \ingroup bgerast
 */

#ifndef __RAS_BOUNDING_BOX_MANAGER_H__
#define __RAS_BOUNDING_BOX_MANAGER_H__

#include "RAS_BoundingBox.h"

class RAS_BoundingBoxManager
{
private:
	/// All the existing bounding box for this manager.
	RAS_BoundingBoxList m_boundingBoxList;
	/** All the bounding box used by at least one mesh user.
	 * These bounding box will be updated each frames.
	 */
	RAS_BoundingBoxList m_activeBoundingBoxList;

public:
	RAS_BoundingBoxManager();
	~RAS_BoundingBoxManager();

	/** Create the bounding box from the bounding box manager. The reason to
	 * do that is that the scene owning the bounding box manager will be freed
	 * before the deformers and meshes.
	 */
	RAS_BoundingBox *CreateBoundingBox();
	RAS_BoundingBox *CreateMeshBoundingBox(const RAS_IDisplayArrayList& arrayList);

	void AddActiveBoundingBox(RAS_BoundingBox *boundingBox);
	void RemoveActiveBoundingBox(RAS_BoundingBox *boundingBox);

	void Update(bool force);
	void ClearModified();

	void Merge(RAS_BoundingBoxManager *other);
};

#endif  // __RAS_MESH_BOUNDING_BOX_MANAGER_H__
