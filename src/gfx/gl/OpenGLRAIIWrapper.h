/**
 * @file   OpenGLRAIIWrapper.h
 * @author Sebastian Maisch <sebastian.maisch@googlemail.com>
 * @date   2016.01.24
 *
 * @brief  Wrappers for OpenGL object to allow RAII patterns.
 */

#pragma once

#include <glbinding/gl/gl.h>

namespace viscom::enh {

    template<typename T, int N>
    class OpenGLRAIIWrapper
    {
    public:
        OpenGLRAIIWrapper() { T::Create<N>(objs_); }
        OpenGLRAIIWrapper(const OpenGLRAIIWrapper&) = delete;
        OpenGLRAIIWrapper& operator=(const OpenGLRAIIWrapper&) = delete;
        OpenGLRAIIWrapper(OpenGLRAIIWrapper&& rhs) noexcept : objs_(std::move(rhs.objs_)) { for (auto& obj : rhs.objs_) obj = T::null_obj; }
        OpenGLRAIIWrapper& operator=(OpenGLRAIIWrapper&& rhs) noexcept { objs_ = std::move(rhs.objs_); for (auto& obj : rhs.objs_) obj = T::null_obj; return *this; }
        ~OpenGLRAIIWrapper() { T::Destroy<N>(objs_); }

        typename T::value_type operator[](size_t i) const { return objs_[i]; }

    private:
        std::array<typename T::value_type, N> objs_;
    };

    template<typename T>
    class OpenGLRAIIWrapper<T, 1>
    {
    public:
        OpenGLRAIIWrapper() : obj_(T::Create()) {}
        explicit OpenGLRAIIWrapper(typename T::value_type newObj) : obj_(newObj) {}
        OpenGLRAIIWrapper(const OpenGLRAIIWrapper&) = delete;
        OpenGLRAIIWrapper& operator=(const OpenGLRAIIWrapper&) = delete;
        OpenGLRAIIWrapper(OpenGLRAIIWrapper&& rhs) noexcept : obj_(rhs.obj_) { rhs.obj_ = T::null_obj; }
        OpenGLRAIIWrapper& operator=(OpenGLRAIIWrapper&& rhs) noexcept { this->~OpenGLRAIIWrapper(); obj_ = rhs.obj_; rhs.obj_ = T::null_obj; return *this; }
        ~OpenGLRAIIWrapper() { obj_ = T::Destroy(obj_); }

        operator typename T::value_type() const { return obj_; }
        // typename T::value_type get() const { return obj; }
        explicit operator bool() const { return T::null_obj != obj_; }
        bool operator==(const OpenGLRAIIWrapper& rhs) { return rhs.obj_ == obj_; }

        friend bool operator==(typename T::value_type lhs, const OpenGLRAIIWrapper<T, 1>& rhs) { return lhs == rhs.obj_; }
        friend bool operator==(const OpenGLRAIIWrapper<T, 1>& lhs, typename T::value_type rhs) { return lhs.obj_ == rhs; }

        typename T::value_type release() { typename T::value_type tmp = obj_; obj_ = T::null_obj; return tmp; }
        void reset(typename T::value_type newObj = T::null_obj) { obj_ = T::Destroy(obj_); obj_ = newObj; } //-V519
        void swap(OpenGLRAIIWrapper<T, 1>& other) { typename T::value_type tmp = obj_; obj_ = other.obj_; other.obj_ = tmp; }

    private:
        typename T::value_type obj_;
    };

    struct ProgramObjectTraits
    {
        using value_type = gl::GLuint;
        static const value_type null_obj = 0;
        static value_type Create() { return gl::glCreateProgram(); }
        static value_type Destroy(value_type prog) { gl::glDeleteProgram(prog); return null_obj; }
    };

    struct ShaderObjectTraits
    {
        using value_type = gl::GLuint;
        static const value_type null_obj = 0;
        static value_type Create() { return null_obj; }
        static value_type Destroy(value_type shader) { gl::glDeleteShader(shader); return null_obj; }
    };

    struct BufferObjectTraits
    {
        using value_type = gl::GLuint;
        static const value_type null_obj = 0;
        static value_type Create() { value_type buffer; gl::glGenBuffers(1, &buffer); return buffer; }
        template<int N> static void Create(std::array<value_type, N>& buffers) { gl::glGenBuffers(static_cast<gl::GLsizei>(N), buffers.data()); }
        static value_type Destroy(value_type buffer) { gl::glDeleteBuffers(1, &buffer); return null_obj; }
        template<int N> static void Destroy(std::array<value_type, N>& buffers)
        {
            gl::glDeleteBuffers(static_cast<gl::GLsizei>(N), buffers.data());
            for (auto& buffer : buffers) buffer = null_obj;
        }
    };

    struct TextureObjectTraits
    {
        using value_type = gl::GLuint;
        static const value_type null_obj = 0;
        static value_type Create() { value_type texture; gl::glGenTextures(1, &texture); return texture; }
        template<int N> static void Create(std::array<value_type, N>& textures) { gl::glGenTextures(static_cast<gl::GLsizei>(N), textures.data()); }
        static value_type Destroy(value_type texture) { gl::glDeleteTextures(1, &texture); return null_obj; }
        template<int N> static void Destroy(std::array<value_type, N>& textures)
        {
            gl::glDeleteTextures(static_cast<gl::GLsizei>(N), textures.data());
            for (auto& texture : textures) texture = null_obj;
        }
    };

    struct FramebufferObjectTraits
    {
        using value_type = gl::GLuint;
        static const value_type null_obj = 0;
        static value_type Create() { value_type fbo; gl::glGenFramebuffers(1, &fbo); return fbo; }
        template<int N> static void Create(std::array<value_type, N>& fbos) { gl::glGenFramebuffers(static_cast<gl::GLsizei>(N), fbos.data()); }
        static value_type Destroy(value_type fbo) { gl::glDeleteFramebuffers(1, &fbo); return null_obj; }
        template<int N> static void Destroy(std::array<value_type, N>& fbos)
        {
            gl::glDeleteFramebuffers(static_cast<gl::GLsizei>(N), fbos.data());
            for (auto& fbo : fbos) fbo = null_obj;
        }
    };

    struct RenderbufferObjectTraits
    {
        using value_type = gl::GLuint;
        static const value_type null_obj = 0;
        static value_type Create() { value_type rbo; gl::glGenRenderbuffers(1, &rbo); return rbo; }
        template<int N> static void Create(std::array<value_type, N>& rbos) { gl::glGenRenderbuffers(static_cast<gl::GLsizei>(N), rbos.data()); }
        static value_type Destroy(value_type rbo) { gl::glDeleteRenderbuffers(1, &rbo); return null_obj; }
        template<int N> static void Destroy(std::array<value_type, N>& rbos)
        {
            gl::glDeleteRenderbuffers(static_cast<gl::GLsizei>(N), rbos.data());
            for (auto& rbo : rbos) rbo = null_obj;
        }
    };

    struct VertexArrayObjectTraits
    {
        using value_type = gl::GLuint;
        static const value_type null_obj = 0;
        static value_type Create() { value_type vao; gl::glGenVertexArrays(1, &vao); return vao; }
        template<int N> static void Create(std::array<value_type, N>& vaos) { gl::glGenVertexArrays(static_cast<gl::GLsizei>(N), vaos.data()); }
        static value_type Destroy(value_type vao) { gl::glDeleteVertexArrays(1, &vao); return null_obj; }
        template<int N> static void Destroy(std::array<value_type, N>& vaos)
        {
            gl::glDeleteVertexArrays(static_cast<gl::GLsizei>(N), vaos.data());
            for (auto& vao : vaos) vao = null_obj;
        }
    };

    struct SamplerObjectTraits
    {
        using value_type = gl::GLuint;
        static const value_type null_obj = 0;
        static value_type Create() { value_type sampler; gl::glGenSamplers(1, &sampler); return sampler; }
        template<int N> static void Create(std::array<value_type, N>& samplers) { gl::glGenSamplers(static_cast<gl::GLsizei>(N), samplers.data()); }
        static value_type Destroy(value_type sampler) { gl::glDeleteSamplers(1, &sampler); return null_obj; }
        template<int N> static void Destroy(std::array<value_type, N>& samplers)
        {
            gl::glDeleteVertexArrays(static_cast<gl::GLsizei>(N), samplers.data());
            for (auto& sampler : samplers) sampler = null_obj;
        }
    };

    using ProgramRAII = OpenGLRAIIWrapper<ProgramObjectTraits, 1>;
    using ShaderRAII = OpenGLRAIIWrapper<ShaderObjectTraits, 1>;
    template<int N> using BuffersRAII = OpenGLRAIIWrapper<BufferObjectTraits, N>;
    using BufferRAII = OpenGLRAIIWrapper<BufferObjectTraits, 1>;
    template<int N> using TexuturesRAII = OpenGLRAIIWrapper<TextureObjectTraits, N>;
    using TextureRAII = OpenGLRAIIWrapper<TextureObjectTraits, 1>;
    template<int N> using FramebuffersRAII = OpenGLRAIIWrapper<FramebufferObjectTraits, N>;
    using FramebufferRAII = OpenGLRAIIWrapper<FramebufferObjectTraits, 1>;
    template<int N> using RenderbuffersRAII = OpenGLRAIIWrapper<RenderbufferObjectTraits, N>;
    using RenderbufferRAII = OpenGLRAIIWrapper<RenderbufferObjectTraits, 1>;
    template<int N> using VertexArraysRAII = OpenGLRAIIWrapper<VertexArrayObjectTraits, N>;
    using VertexArrayRAII = OpenGLRAIIWrapper<VertexArrayObjectTraits, 1>;

    template<int N> using SamplersRAII = OpenGLRAIIWrapper<SamplerObjectTraits, N>;
    using SamplerRAII = OpenGLRAIIWrapper<SamplerObjectTraits, 1>;
}

