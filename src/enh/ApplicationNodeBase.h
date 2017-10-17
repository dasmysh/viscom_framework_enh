/**
 * @file   ApplicationNodeImplementation.h
 * @author Sebastian Maisch <sebastian.maisch@uni-ulm.de>
 * @date   2016.11.30
 *
 * @brief  Declaration of the application node implementation common for master and slave nodes.
 */

#pragma once

#include "core/ApplicationNodeInternal.h"
#include "core/ApplicationNodeBase.h"
#include "enh/gfx/gl/ShaderBufferBindingPoints.h"

namespace viscom::enh {

    class SimpleMeshRenderer;

    class ApplicationNodeBase : public viscom::ApplicationNodeBase
    {
    public:
        explicit ApplicationNodeBase(ApplicationNodeInternal* appNode);
        ApplicationNodeBase(const ApplicationNodeBase&) = delete;
        ApplicationNodeBase(ApplicationNodeBase&&) = delete;
        ApplicationNodeBase& operator=(const ApplicationNodeBase&) = delete;
        ApplicationNodeBase& operator=(ApplicationNodeBase&&) = delete;
        virtual ~ApplicationNodeBase() override;

        virtual void InitOpenGL() override;

        ShaderBufferBindingPoints* GetUBOBindingPoints() { return &uniformBindingPoints_; }
        ShaderBufferBindingPoints* GetSSBOBindingPoints() { return &shaderStorageBindingPoints_; }
        const SimpleMeshRenderer* GetSimpleMeshes() const { return simpleMeshes_.get(); }

    private:
        /** Holds the uniform binding points. */
        ShaderBufferBindingPoints uniformBindingPoints_;
        /** Holds the shader storage buffer object binding points. */
        ShaderBufferBindingPoints shaderStorageBindingPoints_;
        /** Holds the simple meshes renderer. */
        std::unique_ptr<SimpleMeshRenderer> simpleMeshes_;
    };
}
