//  -------------------------------------------------------------------------
//  Copyright (C) 2015 BMW Car IT GmbH
//  -------------------------------------------------------------------------
//  This Source Code Form is subject to the terms of the Mozilla Public
//  License, v. 2.0. If a copy of the MPL was not distributed with this
//  file, You can obtain one at https://mozilla.org/MPL/2.0/.
//  -------------------------------------------------------------------------

#ifndef RAMSES_TEXTURESAMPLERIMPL_H
#define RAMSES_TEXTURESAMPLERIMPL_H

#include "ramses-client-api/TextureEnums.h"

// internal
#include "Resource/TextureMetaInfo.h"
#include "SceneObjectImpl.h"

// ramses framework
#include "SceneAPI/Handles.h"
#include "SceneAPI/ResourceContentHash.h"
#include "SceneAPI/TextureSamplerStates.h"

namespace ramses
{
    class Texture2D;
    class Resource;

    class TextureSamplerImpl final : public SceneObjectImpl
    {
    public:
        /**
         * @brief This creates a Sampler.
         */
        TextureSamplerImpl(SceneImpl& scene, const char* name);
        virtual ~TextureSamplerImpl();

        void             initializeFrameworkData(
            const ramses_internal::TextureSamplerStates& samplerStates,
            ERamsesObjectType textureType,
            ramses_internal::ResourceContentHash textureHash,
            ramses_internal::MemoryHandle contentHandle);

        virtual void     deinitializeFrameworkData() override;
        virtual status_t serialize(ramses_internal::IOutputStream& outStream, SerializationContext& serializationContext) const override;
        virtual status_t deserialize(ramses_internal::IInputStream& inStream, DeserializationContext& serializationContext) override;
        virtual status_t validate(uint32_t indent) const override;


        ETextureAddressMode getWrapUMode() const;
        ETextureAddressMode getWrapVMode() const;
        ETextureAddressMode getWrapRMode() const;
        ETextureSamplingMethod getSamplingMethod() const;
        uint32_t getAnisotropyLevel() const;

        ramses_internal::TextureSamplerHandle getTextureSamplerHandle() const;
        ERamsesObjectType getTextureType() const;

    private:
        status_t validateRenderBuffer(ramses_internal::RenderBufferHandle renderBufferHandle, uint32_t indent) const;
        status_t validateStreamTexture(ramses_internal::StreamTextureHandle streamTextureHandle, uint32_t indent) const;
        status_t validateTextureBuffer(ramses_internal::TextureBufferHandle textureBufferHandle, uint32_t indent) const;
        status_t validateResource(const Resource* resource, uint32_t indent) const;

        ERamsesObjectType m_textureType;

        ramses_internal::TextureSamplerHandle m_textureSamplerHandle;
    };
}

#endif
