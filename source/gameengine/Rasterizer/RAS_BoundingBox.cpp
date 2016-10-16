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

/** \file RAS_BoundingBox.cpp
 *  \ingroup bgerast
 */

#include "RAS_BoundingBox.h"
#include "RAS_BoundingBoxManager.h"

RAS_BoundingBox::RAS_BoundingBox(RAS_BoundingBoxManager *manager)
	:m_modified(false),
	m_aabbMin(0.0f, 0.0f, 0.0f),
	m_aabbMax(0.0f, 0.0f, 0.0f),
	m_users(0),
	m_manager(manager)
{
}

RAS_BoundingBox::~RAS_BoundingBox()
{
}

RAS_BoundingBox *RAS_BoundingBox::GetReplica()
{
	RAS_BoundingBox *boundingBox = new RAS_BoundingBox(*this);
	boundingBox->m_users = 0;
	return boundingBox;
}

void RAS_BoundingBox::AddUser()
{
	++m_users;
	/* No one was using this bounding box previously. Then add it to the active
	 * bounding box list in the manager.*/
	if (m_users == 1) {
		m_manager->AddActiveBoundingBox(this);
	}
}

void RAS_BoundingBox::RemoveUser()
{
	--m_users;
	BLI_assert(m_users >= 0);

	/* Some one was using this bounding box previously. Then remove it from the
	 * active bounding box list. */
	if (m_users == 0) {
		m_manager->RemoveActiveBoundingBox(this);
	}
}

void RAS_BoundingBox::SetManager(RAS_BoundingBoxManager *manager)
{
	m_manager = manager;
}

bool RAS_BoundingBox::GetModified() const
{
	return m_modified;
}

void RAS_BoundingBox::ClearModified()
{
	m_modified = false;
}

void RAS_BoundingBox::GetAabb(MT_Vector3& aabbMin, MT_Vector3& aabbMax) const
{
	aabbMin = m_aabbMin;
	aabbMax = m_aabbMax;
}

void RAS_BoundingBox::SetAabb(const MT_Vector3& aabbMin, const MT_Vector3& aabbMax)
{
	m_aabbMin = aabbMin;
	m_aabbMax = aabbMax;
	m_modified = true;
}

void RAS_BoundingBox::ExtendAabb(const MT_Vector3& aabbMin, const MT_Vector3& aabbMax)
{
	m_aabbMin.x() = std::min(m_aabbMin.x(), aabbMin.x());
	m_aabbMin.y() = std::min(m_aabbMin.y(), aabbMin.y());
	m_aabbMin.z() = std::min(m_aabbMin.z(), aabbMin.z());
	m_aabbMax.x() = std::max(m_aabbMax.x(), aabbMax.x());
	m_aabbMax.y() = std::max(m_aabbMax.y(), aabbMax.y());
	m_aabbMax.z() = std::max(m_aabbMax.z(), aabbMax.z());
	m_modified = true;
}

void RAS_BoundingBox::Update(bool force)
{
}

RAS_MeshBoundingBox::RAS_MeshBoundingBox(RAS_BoundingBoxManager *manager, const RAS_IDisplayArrayList displayArrayList)
	:RAS_BoundingBox(manager),
	m_displayArrayList(displayArrayList)
{
}

RAS_MeshBoundingBox::~RAS_MeshBoundingBox()
{
}

RAS_BoundingBox *RAS_MeshBoundingBox::GetReplica()
{
	RAS_MeshBoundingBox *boundingBox = new RAS_MeshBoundingBox(*this);
	boundingBox->m_users = 0;
	return boundingBox;
}

void RAS_MeshBoundingBox::Update(bool force)
{
	bool modified = false;
	// Detect if a display array was modified.
	for (RAS_IDisplayArrayList::iterator it = m_displayArrayList.begin(), end = m_displayArrayList.end(); it != end; ++it) {
		if ((*it)->GetModifiedFlag() & RAS_IDisplayArray::AABB_MODIFIED) {
			modified = true;
			break;
		}
	}

	if (!modified && !force) {
		return;
	}

	for (unsigned short i = 0, size = m_displayArrayList.size(); i < size; ++i) {
		RAS_IDisplayArray *displayArray = m_displayArrayList[i];
		// For each vertex.
		for (unsigned int j = 0, size = displayArray->GetVertexCount(); j < size; ++j) {
			RAS_ITexVert *v = displayArray->GetVertex(j);
			MT_Vector3 vertpos = v->xyz();

			// For the first vertex of the mesh, only initialize AABB.
			if (i == 0 && j == 0) {
				m_aabbMin = m_aabbMax = vertpos;
			}
			else {
				m_aabbMin.x() = std::min(m_aabbMin.x(), vertpos.x());
				m_aabbMin.y() = std::min(m_aabbMin.y(), vertpos.y());
				m_aabbMin.z() = std::min(m_aabbMin.z(), vertpos.z());
				m_aabbMax.x() = std::max(m_aabbMax.x(), vertpos.x());
				m_aabbMax.y() = std::max(m_aabbMax.y(), vertpos.y());
				m_aabbMax.z() = std::max(m_aabbMax.z(), vertpos.z());
			}
		}
	}

	m_modified = true;
}
