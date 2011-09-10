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

#include "iokitprivate.h"

#include <IOKit/IOKitLib.h>
#include <IOKit/network/IOEthernetInterface.h>

namespace Solid { namespace Backends { namespace IOKit {

const char *interfaceToIOClass(Solid::DeviceInterface::Type type)
{
    switch (type) {
    case Solid::DeviceInterface::Unknown:
        return 0;
    case Solid::DeviceInterface::NetworkInterface:
        return kIOEthernetInterfaceClass;
    case Solid::DeviceInterface::Processor:
        return "AppleACPICPU";
    case Solid::DeviceInterface::SerialInterface:
        return "IOSerialBSDClient";
    case Solid::DeviceInterface::Battery:
        return "AppleSmartBattery";
    case Solid::DeviceInterface::StorageVolume:
        return "IOMedia";
    case Solid::DeviceInterface::Block:
        return "IOMedia";
    case Solid::DeviceInterface::StorageAccess:
        return "IOMedia";

    //Solid::DeviceInterface::GenericInterface:
    //Solid::DeviceInterface::StorageDrive:
    //Solid::DeviceInterface::OpticalDrive:
    //Solid::DeviceInterface::OpticalDisc:
    //Solid::DeviceInterface::Camera:
    //Solid::DeviceInterface::PortableMediaPlayer:
    //Solid::DeviceInterface::NetworkInterface:
    //Solid::DeviceInterface::AcAdapter:
    //Solid::DeviceInterface::Button:
    //Solid::DeviceInterface::AudioInterface:
    //Solid::DeviceInterface::DvbInterface:
    //Solid::DeviceInterface::Video:
    }

    return 0;
}

io_registry_entry_t findIORegistryEntrySafe(const QString& udi)
{
    // Try IORegistryEntryFromPath
    const char *target = udi.toLocal8Bit().constData();
    io_registry_entry_t entry = IORegistryEntryFromPath(kIOMasterPortDefault,
        target);
    if (entry != MACH_PORT_NULL)
        return entry;

	// Find a valid parent
    io_string_t path;
    strlcpy(path, target, sizeof(path));
    io_registry_entry_t root = MACH_PORT_NULL;
    while (root == MACH_PORT_NULL) {
        char *sep = strrchr(path, '/');
        if (!sep)
            return MACH_PORT_NULL;
        *sep = '\0';
        root = IORegistryEntryFromPath(kIOMasterPortDefault, path);
    }

	// Walk the hierarchy
    io_iterator_t iter;
    if (IORegistryEntryCreateIterator(root, kIOServicePlane, 0, &iter) != KERN_SUCCESS)
        goto err;
    while ((entry = IOIteratorNext(iter)) != MACH_PORT_NULL) {
        if (IORegistryEntryGetPath(entry, kIOServicePlane, path) != KERN_SUCCESS)
            goto err;
        if (strcmp(target, path) == 0) {
            goto success;
        } else if (strncmp(target, path, strlen(path)) == 0) { // is it a prefix?
            if (IORegistryIteratorEnterEntry(iter) != KERN_SUCCESS)
                goto err;
        }
        IOObjectRelease(entry);
    }

err:
    IOObjectRelease(entry);
    entry = 0;
success:
    IOObjectRelease(iter);
    IOObjectRelease(root);
    return entry;
}

} } } // namespace
