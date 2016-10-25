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
 * Contributor(s): Tristan Porteries.
 *
 * ***** END GPL LICENSE BLOCK *****
 */

/** \file CM_Message.h
 *  \ingroup common
 */


#ifndef __CM_MESSAGE_H__
#define __CM_MESSAGE_H__

#include "../../blender/python/generic/py_capi_utils.h" /* for PyC_FileAndNum only

/*CM_LogicBrickWarning/Error(object, brick, message):
Warning/Error: objectname(brickname), message.
*/

#define CM_Warning(msg) std::cout << "Warning: " << msg << std::endl;
#define CM_Error(msg) std::cout << "Error: " << msg << std::endl;

#define _CM_PythonMsg(prefix, msg) \
{ \
	int line; \
	const char *file; \
	PyC_FileAndNum(&line, &file) \
	if (file) { \
		std::cout << prefix << ": " << file << "(" << line << "), " << msg << std::endl; \
	} \
	else { \
		std::cout << prefix << ": " << msg << std::endl; \
	} \
}

#define CM_PythonWarning(msg) _CM_PythonMsg("Warning", msg)
#define CM_PythonError(msg) _CM_PythonMsg("Error", msg)

#define _CM_PythonMsgClass(prefix, class, msg) \
{ \
	int line; \
	const char *file; \
	PyC_FileAndNum(&line, &file) \
	if (file) { \
		std::cout << prefix << ": " << file << "(" << line << "): " << class << ", " << msg << std::endl; \
	} \
	else { \
		std::cout << prefix << ": " << class << ", " << msg << std::endl; \
	} \
}

#define CM_PythonWarningClass(class, msg) _CM_PythonMsgClass("Warning", class, msg)
#define CM_PythonErrorClass(class, msg) _CM_PythonMsgClass("Error", class, msg)

#define _CM_PythonMsgAttribut(prefix, class, attribut, msg) \
{ \
	int line; \
	const char *file; \
	PyC_FileAndNum(&line, &file) \
	if (file) { \
		std::cout << prefix << ": " << file << "(" << line << "): " << class << "." << attribut << ", " << msg << std::endl; \
	} \
	else { \
		std::cout << prefix << ": " << class << "." << attribut << ", " << msg << std::endl; \
	} \
}

#define CM_PythonWarningAttribut(class, attribut, msg) _CM_PythonMsgAttribut("Warning", class, attribut, msg)
#define CM_PythonErrorAttribut(class, attribut, msg) _CM_PythonMsgAttribut("Error", class, attribut, msg)

#endif  // __CM_MESSAGE_H__
