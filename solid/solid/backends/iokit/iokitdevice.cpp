/*
    Copyright 2009 Harald Fernengel <harry@kdevelop.org>

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


#include "iokitdevice.h"
#include "iokitprivate.h"
#include "iokitgenericinterface.h"
#include "iokitprocessor.h"
#include "iokitbattery.h"
#include "iokitnetworkinterface.h"
#include "iokitserialinterface.h"
#include "iokitvolume.h"
#include "iokitblock.h"
#include "iokitstorageaccess.h"

#include <QtCore/qdebug.h>

#include <IOKit/IOKitLib.h>
#include <IOKit/usb/IOUSBLib.h>
#include <IOKit/network/IOEthernetInterface.h>

#include <CoreFoundation/CoreFoundation.h>

namespace Solid { namespace Backends { namespace IOKit {

// gets all properties from an entry into a QMap
static QMap<QString, QVariant> getProperties(const io_registry_entry_t &entry)
{
    CFMutableDictionaryRef propertyDict = 0;

    if (IORegistryEntryCreateCFProperties(entry, &propertyDict, kCFAllocatorDefault, kNilOptions) != KERN_SUCCESS) {
        return QMap<QString, QVariant>();
    }

    QMap<QString, QVariant> result = q_toVariantMap(propertyDict);

    CFRelease(propertyDict);

    return result;
}

// gets the parent's Udi from an entry
static QString getParentDeviceUdi(const io_registry_entry_t &entry)
{
    io_registry_entry_t parent = 0;
    kern_return_t ret = IORegistryEntryGetParentEntry(entry, kIOServicePlane, &parent);
    if (ret != KERN_SUCCESS) {
        // don't release parent here - docs say only on success
        return QString();
    }

    QString result;
    io_string_t pathName;
    ret = IORegistryEntryGetPath(parent, kIOServicePlane, pathName);
    if (ret == KERN_SUCCESS)
        result = QString::fromUtf8(pathName);

    // now we can release the parent
    IOObjectRelease(parent);

    return result;
}


const char *const PropertyClassName = "className";

class IOKitDevicePrivate
{
public:
    inline IOKitDevicePrivate()
    {}

    void init(const QString &udiString, const io_registry_entry_t & entry);

    QString udi;
    QString parentUdi;
    QMap<QString, QVariant> properties;
};

void IOKitDevicePrivate::init(const QString &udiString, const io_registry_entry_t &entry)
{
    Q_ASSERT(entry != MACH_PORT_NULL);

    udi = udiString;

    properties = getProperties(entry);

    io_name_t className;
    IOObjectGetClass(entry, className);
    properties[PropertyClassName] = QString::fromUtf8(className);

    parentUdi = getParentDeviceUdi(entry);

    IOObjectRelease(entry);
}

IOKitDevice::IOKitDevice(const QString &udi, const io_registry_entry_t &entry)
    : d(new IOKitDevicePrivate)
{
    d->init(udi, entry);
}

IOKitDevice::IOKitDevice(const QString &udi)
    : d(new IOKitDevicePrivate)
{
    io_registry_entry_t entry = IORegistryEntryFromPath(
            kIOMasterPortDefault,
            udi.toLocal8Bit().constData());

    if (entry == MACH_PORT_NULL) {
        qDebug() << Q_FUNC_INFO << "Tried to create Device from invalid UDI" << udi;
        return;
    }

    d->init(udi, entry);
}

IOKitDevice::~IOKitDevice()
{
    delete d;
}

QString IOKitDevice::udi() const
{
    return d->udi;
}

QString IOKitDevice::parentUdi() const
{
    return d->parentUdi;
}

QString IOKitDevice::vendor() const
{
    return QString(); // TODO
}

QString IOKitDevice::product() const
{
    return QString(); // TODO
}

QString IOKitDevice::icon() const
{
    return QString(); // TODO
}

QStringList IOKitDevice::emblems() const
{
    return QStringList(); // TODO
}

QString IOKitDevice::description() const
{
    return product(); // TODO
}

QVariant IOKitDevice::property(const QString &key) const
{
    return d->properties.value(key);
}

QMap<QString, QVariant> IOKitDevice::allProperties() const
{
    return d->properties;
}

bool IOKitDevice::propertyExists(const QString &key) const
{
    return d->properties.contains(key);
}

bool IOKitDevice::queryDeviceInterface(const Solid::DeviceInterface::Type &type) const
{
    if (type == Solid::DeviceInterface::GenericInterface)
        return true;

    const char *targetStr = interfaceToIOClass(type);
    if (!targetStr)
        return false;


    // Walk the IOKit class hierarchy
    bool result = false;
    CFStringRef target = CFStringCreateWithCStringNoCopy(kCFAllocatorDefault,
        targetStr, kCFStringEncodingASCII, kCFAllocatorNull);

    QString qCurrent = property(PropertyClassName).toString();
    CFStringRef current = CFStringCreateWithCharacters(kCFAllocatorDefault,
        reinterpret_cast<const UniChar*>(qCurrent.unicode()),
        qCurrent.length());

    while (current) {
        if (CFStringCompare(target, current, 0) == kCFCompareEqualTo) {
            result = true;
            break;
        }
        CFStringRef next = IOObjectCopySuperclassForClass(current);
        CFRelease(current);
        current = next;
    }

    if (current)
        CFRelease(current);
    CFRelease(target);

    return result;
}

QObject *IOKitDevice::createDeviceInterface(const Solid::DeviceInterface::Type &type)
{
    if (!queryDeviceInterface(type))
        return 0;

    switch (type)
    {
    case Solid::DeviceInterface::GenericInterface:
        return new GenericInterface(this);
    case Solid::DeviceInterface::Processor:
        return new Processor(this);
    case Solid::DeviceInterface::NetworkInterface:
        return new NetworkInterface(this);
    case Solid::DeviceInterface::SerialInterface:
        return new SerialInterface(this);
    case Solid::DeviceInterface::Battery:
        return new Battery(this);
    case Solid::DeviceInterface::StorageVolume:
        return new Volume(this);
    case Solid::DeviceInterface::Block:
        return new Block(this);
    case Solid::DeviceInterface::StorageAccess:
        return new StorageAccess(this);
    // the rest is TODO
    }

    return 0;
}


} } } // namespaces

