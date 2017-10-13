/**
 * @file   GLVertexAttributeArray.h
 * @author Sebastian Maisch <sebastian.maisch@googlemail.com>
 * @date   2014.02.16
 *
 * @brief  Contains the definition of GLVertexAttributeArray.
 */

#ifndef GLVERTEXATTRIBUTEARRAY_H
#define GLVERTEXATTRIBUTEARRAY_H

#include "main.h"

namespace cgu {
    /** The type of the vertex attribute inside a shader. */
    enum class VAShaderType
    {
        /** Integer type. */
        INTEGER,
        /** Single precision float type. */
        FLOAT,
        /** Double precision float type. */
        DOUBLE
    };

    /** Describes a general point for a gpu program. */
    struct shader_binding_desc
    {

        /** Unnamed union.*/
        union
        {
            /** Integer binding point. */
            GLint iBinding;
            /** Unsigned int binding point. */
            GLuint uBinding;
        };
    };

    /** The location of a general shader binding point. */
    typedef shader_binding_desc* BindingLocation;

    /** Description of a vertex attribute. */
    struct vertex_attribute_desc
    {
        /** Holds the attribute type inside the shader. */
        VAShaderType shaderType;
        /** Holds the attribute binding location. */
        BindingLocation location;
        /** Holds the number of components in the attribute. */
        int size;
        /** Holds the attributes type in the vertex buffer. */
        GLenum type;
        /** Holds whether the attribute should be normalized. */
        GLboolean normalized;
        /** The distance between 2 attributes of this type in bytes. */
        int stride;
        /** The offset into the vertex to the beginning of this attribute in bytes. */
        unsigned int offset;
    };

    /**
     * @brief
     *
     * @author Sebastian Maisch <sebastian.maisch@googlemail.com>
     * @date   2014.02.16
     */
    class GLVertexAttributeArray
    {
    public:
        GLVertexAttributeArray(GLuint vertexBuffer, GLuint indexBuffer);
        GLVertexAttributeArray(const GLVertexAttributeArray&) = delete;
        GLVertexAttributeArray& operator=(const GLVertexAttributeArray&) = delete;
        GLVertexAttributeArray(GLVertexAttributeArray&& orig);
        GLVertexAttributeArray& operator=(GLVertexAttributeArray&& orig);
        ~GLVertexAttributeArray();

        void StartAttributeSetup() const;
        void EndAttributeSetup() const;
        void AddVertexAttribute(BindingLocation location, int size, GLenum type, GLboolean normalized,
            GLsizei stride, size_t offset);
        void AddVertexAttributeI(BindingLocation location, int size, GLenum type, GLsizei stride,
            size_t offset);
        void AddVertexAttributeL(BindingLocation location, int size, GLenum type, GLsizei stride,
            size_t offset);
        void UpdateVertexAttributes();
        void DisableAttributes();
        void EnableVertexAttributeArray() const;
        void DisableVertexAttributeArray() const;

    private:
        VertexArrayRAII vao;
        GLuint i_buffer;
        GLuint v_buffer;
        std::vector<vertex_attribute_desc> v_desc;

    };
}

#endif /* GLVERTEXATTRIBUTEARRAY_H */
