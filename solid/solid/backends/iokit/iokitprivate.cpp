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

    //Solid::DeviceInterface::GenericInterface:
    //Solid::DeviceInterface::Block:
    //Solid::DeviceInterface::StorageAccess:
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

} } } // namespace
