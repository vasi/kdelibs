/*
    Copyright 2011 Dave Vasilevsky <dave@vasilevsky.ca>

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) version 3, or any
    later version accepted by the membership of KDE e.V. (or its
    successor approved by the membership of KDE e.V.), which shall
    act as a proxy defined in Section 6 of version 3 of the license.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library. If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef SOLID_BACKENDS_IOKIT_PRIVATE_H
#define SOLID_BACKENDS_IOKIT_PRIVATE_H

#include <solid/ifaces/device.h>

#include <QtCore/qvariant.h>

#include <CoreFoundation/CoreFoundation.h>
#include <IOKit/IOKitLib.h>

namespace Solid { namespace Backends { namespace IOKit {

QVariant q_toVariant(const CFTypeRef &obj);
QMap<QString, QVariant> q_toVariantMap(const CFMutableDictionaryRef &dict);

// Which IOKit class provides the given DeviceInterface?
const char *interfaceToIOClass(Solid::DeviceInterface::Type type);

// Work around IORegistryEntryFromPath bug
io_registry_entry_t findIORegistryEntrySafe(const QString& udi);

} } } // namespace

#endif // SOLID_BACKENDS_IOKIT_PRIVATE_H
