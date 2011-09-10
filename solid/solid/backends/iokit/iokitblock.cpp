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

#include "iokitblock.h"
#include "iokitdevice.h"
#include "iokitprivate.h"

#include <QtCore/qdebug.h>

#include <IOKit/IOBSD.h>
#include <DiskArbitration/DiskArbitration.h>

using namespace Solid::Backends::IOKit;

Block::Block(IOKitDevice *device)
    : DeviceInterface(device), m_diskDescription(0)
{
}

Block::~Block()
{
    if (m_diskDescription) {
        CFRelease(m_diskDescription);
    }
}

QString Block::device() const
{
    return QString("/dev/") + m_device->property(kIOBSDNameKey).toString();
}

int Block::deviceMajor() const
{
    return m_device->property(kIOBSDMajorKey).toInt();
}

int Block::deviceMinor() const
{
    return m_device->property(kIOBSDMinorKey).toInt();
}

QVariant Block::diskProp(const CFStringRef key) const
{
    // Cache the properties
    if (!m_diskDescription) {
        // FIXME: Should we use a global session?
        DASessionRef session = DASessionCreate(kCFAllocatorDefault);
        if (session) {
            QByteArray name = m_device->property(kIOBSDNameKey).toByteArray();
            DADiskRef disk = DADiskCreateFromBSDName(kCFAllocatorDefault,
                session, name.constData());
            if (disk) {
                const_cast<Block*>(this)->m_diskDescription =
                    DADiskCopyDescription(disk);
                CFRelease(disk);
            }
            CFRelease(session);
        }
    }

    if (m_diskDescription) {
        CFTypeRef result = CFDictionaryGetValue(m_diskDescription, key);
        if (result) {
            return q_toVariant(result);
        }
    }
    return QVariant();
}

#include "backends/iokit/iokitblock.moc"
