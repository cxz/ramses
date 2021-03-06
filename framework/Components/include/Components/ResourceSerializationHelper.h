//  -------------------------------------------------------------------------
//  Copyright (C) 2017 BMW Car IT GmbH
//  -------------------------------------------------------------------------
//  This Source Code Form is subject to the terms of the Mozilla Public
//  License, v. 2.0. If a copy of the MPL was not distributed with this
//  file, You can obtain one at https://mozilla.org/MPL/2.0/.
//  -------------------------------------------------------------------------

#ifndef RAMSES_RESOURCESERIALIZATIONHELPER_H
#define RAMSES_RESOURCESERIALIZATIONHELPER_H

#include "PlatformAbstraction/PlatformTypes.h"
#include "Resource/EResourceCompressionStatus.h"
#include "SceneAPI/SceneResourceData.h"

namespace ramses_internal
{
    class IResource;
    class IOutputStream;
    class IInputStream;
    class String;

    namespace ResourceSerializationHelper
    {
        struct DeserializedResourceHeader
        {
            IResource* resource;
            EResourceCompressionStatus compressionStatus;
            UInt32 decompressedSize;
            UInt32 compressedSize;
        };

        void SerializeResourceMetadata(IOutputStream& output, const IResource& resource);
        UInt32 ResourceMetadataSize(const IResource& resource);

        DeserializedResourceHeader ResourceFromMetadataStream(IInputStream& input);

        void SetInvalidCreateResourceFromMetadataStreamFunction(IResource*(*fun)(IInputStream&, ResourceCacheFlag, const String&));
    }
}

#endif
