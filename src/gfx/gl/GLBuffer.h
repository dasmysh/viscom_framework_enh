/**
 * @file   GLBuffer.h
 * @author Sebastian Maisch <sebastian.maisch@uni-ulm.de>
 * @date   2016.05.08
 *
 * @brief  Declaration of an OpenGL buffer.
 */

#pragma once

#include "core/type_traits.h"
#include "gfx/gl/OpenGLRAIIWrapper.h"
#include <glbinding/gl/gl.h>

namespace viscom::enh {

    class GLBuffer final
    {
    public:
        explicit GLBuffer(gl::GLenum usage);
        ~GLBuffer();
        GLBuffer(const GLBuffer&);
        GLBuffer& operator=(const GLBuffer&);
        GLBuffer(GLBuffer&&) noexcept;
        GLBuffer& operator=(GLBuffer&&) noexcept;

        void InitializeData(std::size_t size, const void* data);
        void UploadData(std::size_t offset, std::size_t size, const void* data);
        void DownloadData(std::size_t size, void* data) const;

        std::size_t GetBufferSize() const { return bufferSize_; }
        const BufferRAII& GetBuffer() const { return buffer_; }

        template<class T> std::enable_if_t<has_contiguous_memory<T>::value> InitializeData(const T& data);
        template<class T> std::enable_if_t<has_contiguous_memory<T>::value> UploadData(std::size_t offset, const T& data);
        template<class T> std::enable_if_t<has_contiguous_memory<T>::value>  DownloadData(T& data) const;

    private:
        /** Holds the buffer object name. */
        BufferRAII buffer_;
        /** Holds the current size of the buffer in bytes. */
        std::size_t bufferSize_ = 0;
        /** Holds the buffer usage. */
        gl::GLenum usage_ = gl::GL_STATIC_DRAW;
    };

    template <class T> std::enable_if_t<has_contiguous_memory<T>::value> GLBuffer::InitializeData(const T& data)
    {
        InitializeData(sizeof(typename T::value_type) * data.size(), data.data());
    }

    template <class T> std::enable_if_t<has_contiguous_memory<T>::value> GLBuffer::UploadData(std::size_t offset, const T& data)
    {
        UploadData(offset, sizeof(typename T::value_type) * data.size(), data.data());
    }

    template <class T> std::enable_if_t<has_contiguous_memory<T>::value> GLBuffer::DownloadData(T& data) const
    {
        DownloadData(sizeof(typename T::value_type) * data.size(), data.data());
    }
}
