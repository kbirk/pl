#pragma once

#include "plCommon.h"
#include "plBufferObject.h"

#include <epoxy/gl.h>

class plSSBO : public plBufferObject
{

    public:

        plSSBO();
        plSSBO(uint32_t numBytes, const void *buffer = nullptr);

        void bind(uint32_t location) const   { glBindBufferBase(GL_SHADER_STORAGE_BUFFER, location, _id); }
        void unbind(uint32_t location) const { glBindBufferBase(GL_SHADER_STORAGE_BUFFER, location, 0); }

        // set
        template <typename T>
        void set(std::vector<T> &ts, uint32_t count, uint32_t index = 0, uint32_t ssboIndex = 0);

        template <typename T>
        void setBytes(T *ts, uint32_t numBytes, uint32_t byteOffset = 0, uint32_t ssboByteOffset = 0);

        // read
        template <typename T>
        void read(std::vector<T> &ts, uint32_t count, uint32_t index = 0, uint32_t ssboIndex = 0) const;

        template <typename T>
        void readBytes(T *ts, uint32_t numBytes, uint32_t byteOffset = 0, uint32_t ssboByteOffset = 0) const;

    private:

        void _create(uint32_t numBytes, const void *buffer = nullptr);
};


template <typename T>
void plSSBO::set(std::vector<T> &ts, uint32_t count, uint32_t index, uint32_t ssboIndex)
{
    if (count == 0)
    {
        std::cerr << "plSSBO::set() warning: write count is 0, function ignored" << std::endl;
        return;
    }

    setBytes(&ts[0], count*sizeof(T), index*sizeof(T), ssboIndex*sizeof(T));
}


template <typename T>
void plSSBO::setBytes(T *ts, uint32_t numBytes, uint32_t byteOffset, uint32_t ssboByteOffset)
{
    if (numBytes == 0)
    {
        std::cerr << "plSSBO::setBytes() warning: numBytes is 0, function ignored" << std::endl;
        return;
    }

    if (!_id || numBytes> _numBytes)
    {
        // ssbo has not been generated yet or is too small to map properly
        _create(numBytes, (void*)ts);
        return; // no need to map
    }

    glBindBuffer(GL_SHADER_STORAGE_BUFFER, _id);
    T *mappedBuffer = (T*) glMapBufferRange(
        GL_SHADER_STORAGE_BUFFER,
        ssboByteOffset,
        numBytes,
        GL_MAP_WRITE_BIT);

    memcpy(mappedBuffer, reinterpret_cast<uint8_t*>(ts)+byteOffset, numBytes); // cast to char array to ensure bytewise increments
    glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
}


template <typename T>
void plSSBO::read(std::vector<T> &ts, uint32_t count, uint32_t index, uint32_t ssboIndex) const
{
    if (!_id)
    {
        std::cerr << "plSSBO::read() error: SSBO buffer has not been generated" << std::endl;
        return;
    }

    if (count == 0)
    {
        std::cerr << "plSSBO::read() warning: read count is 0, function ignored" << std::endl;
        return;
    }

    if (count*sizeof(T)> _numBytes)
    {
        std::cerr << "plSSBO::read() error: requested read size larger than SSBO, function ignored" << std::endl;
        return;
    }

    if (ts.size() < count)
         ts.resize(count);

    readBytes(&ts[0], count*sizeof(T), index*sizeof(T), ssboIndex*sizeof(T));
}


template <typename T>
void plSSBO::readBytes(T *ts, uint32_t numBytes, uint32_t byteOffset, uint32_t ssboByteOffset) const
{
    if (!_id)
    {
        std::cerr << "plSSBO::readBytes() error: SSBO buffer has not been generated" << std::endl;
        return;
    }

    if (numBytes == 0)
    {
        std::cerr << "plSSBO::readBytes() warning: numBytes is 0, function ignored" << std::endl;
        return;
    }

    if (numBytes> _numBytes)
    {
        std::cerr << "plSSBO::readBytes() error: requested read size larger than SSBO, function ignored" << std::endl;
        return;
    }

    glBindBuffer(GL_SHADER_STORAGE_BUFFER, _id);
    T *mappedBuffer = (T*) glMapBufferRange(GL_SHADER_STORAGE_BUFFER,
        ssboByteOffset,
        numBytes,
        GL_MAP_READ_BIT);

    memcpy(reinterpret_cast<uint8_t*>(ts)+byteOffset, &mappedBuffer[0], numBytes);
    glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
}
