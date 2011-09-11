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

#include "iokitstorageaccess.h"
#include "iokitdevice.h"

#include <QtCore/qdebug.h>

#include <sys/param.h> // for MAXPATHLEN

#include <CoreFoundation/CoreFoundation.h>
#include <DiskArbitration/DADisk.h>

using namespace Solid::Backends::IOKit;

// TODO: emit signals

StorageAccess::StorageAccess(IOKitDevice *device)
    : Block(device)
{
}

StorageAccess::~StorageAccess()
{

}

bool StorageAccess::isAccessible() const
{
    return !diskProp(kDADiskDescriptionVolumePathKey).isNull();
}

QString StorageAccess::filePath() const
{
    QString urlStr = diskProp(kDADiskDescriptionVolumePathKey).toString();

    if (!urlStr.isEmpty()) {
        // Don't try to figure out ourselves what local path this URL-string
        // represents. Ask CFURL instead.
        QByteArray urlBytes = urlStr.toUtf8();
        CFURLRef url = CFURLCreateWithBytes(kCFAllocatorDefault,
            reinterpret_cast<const UInt8*>(urlBytes.constData()),
            urlBytes.size(), kCFStringEncodingUTF8, NULL);
        if (url) {
            UInt8 buf[MAXPATHLEN];
            bool ok = CFURLGetFileSystemRepresentation(url, false, buf,
                sizeof(buf));
            if (ok) {
                return QString::fromUtf8(reinterpret_cast<char*>(buf));
            }
            CFRelease(url);
        }
    }

    return QString();
}

bool StorageAccess::isIgnored() const
{
    return false; // TODO: Check statfs() for MNT_DONTBROWSE?
}

bool StorageAccess::setup()
{
    return false; // TODO
}

bool StorageAccess::teardown()
{
    return false; // TODO
}

#include "backends/iokit/iokitstorageaccess.moc"
