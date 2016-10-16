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

/** \file RAS_BoundingBoxManager.cpp
 *  \ingroup bgerast
 */

#include "RAS_BoundingBoxManager.h"

#include <algorithm>

RAS_BoundingBoxManager::RAS_BoundingBoxManager()
{
}

RAS_BoundingBoxManager::~RAS_BoundingBoxManager()
{
	for (RAS_BoundingBoxList::iterator it = m_boundingBoxList.begin(), end = m_boundingBoxList.end(); it != end; ++it) {
		delete *it;
	}

	BLI_assert(m_activeBoundingBoxList.size() == 0);
}

RAS_BoundingBox *RAS_BoundingBoxManager::CreateBoundingBox()
{
	RAS_BoundingBox *boundingBox = new RAS_BoundingBox(this);
	m_boundingBoxList.push_back(boundingBox);

	return boundingBox;
}

RAS_BoundingBox *RAS_BoundingBoxManager::CreateMeshBoundingBox(const RAS_IDisplayArrayList& arrayList)
{
	RAS_BoundingBox *boundingBox = new RAS_MeshBoundingBox(this, arrayList);
	m_boundingBoxList.push_back(boundingBox);

	return boundingBox;
}

void RAS_BoundingBoxManager::AddActiveBoundingBox(RAS_BoundingBox *boundingBox)
{
	m_activeBoundingBoxList.push_back(boundingBox);
}

void RAS_BoundingBoxManager::RemoveActiveBoundingBox(RAS_BoundingBox *boundingBox)
{
	RAS_BoundingBoxList::iterator it = std::find(m_activeBoundingBoxList.begin(), m_activeBoundingBoxList.end(), boundingBox);
	if (it != m_activeBoundingBoxList.end()) {
		m_activeBoundingBoxList.erase(it);
	}
}

void RAS_BoundingBoxManager::Update(bool force)
{
	for (RAS_BoundingBoxList::iterator it = m_activeBoundingBoxList.begin(), end = m_activeBoundingBoxList.end(); it != end; ++it) {
		(*it)->Update(force);
	}
}

void RAS_BoundingBoxManager::ClearModified()
{
	for (RAS_BoundingBoxList::iterator it = m_activeBoundingBoxList.begin(), end = m_activeBoundingBoxList.end(); it != end; ++it) {
		(*it)->ClearModified();
	}
}

void RAS_BoundingBoxManager::Merge(RAS_BoundingBoxManager *other)
{
	for (RAS_BoundingBoxList::iterator it = other->m_boundingBoxList.begin(), end = other->m_boundingBoxList.end(); it != end; ++it) {
		RAS_BoundingBox *boundingBox = *it;
		boundingBox->SetManager(this);
		m_boundingBoxList.push_back(boundingBox);
	}

	other->m_boundingBoxList.clear();

	for (RAS_BoundingBoxList::iterator it = other->m_activeBoundingBoxList.begin(), end = other->m_activeBoundingBoxList.end(); it != end; ++it) {
		m_activeBoundingBoxList.push_back(*it);
	}

	other->m_activeBoundingBoxList.clear();
}
