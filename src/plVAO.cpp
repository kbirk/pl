#include "plVAO.h"

plVAO::plVAO()
    : _id(0)
{
}


plVAO::~plVAO()
{
    _destroy();
}


void plVAO::attach(std::shared_ptr<plVBO> vbo)
{
    _vbos.push_back(vbo);
}


void plVAO::attach(std::shared_ptr<plEABO> eabo)
{
    _eabo = eabo;
}


void plVAO::clear()
{
    _eabo = nullptr;
    _vbos.clear();
}


void plVAO::draw() const
{
    if (_eabo && !_vbos.empty())
    {
        // bind vertex array object
        glBindVertexArray(_id);
        LOG_OPENGL("glBindVertexArray");
        // draw
        _eabo->drawElements();
        // unbind vao
        glBindVertexArray(0);
        LOG_OPENGL("glBindVertexArray");
    }
}


void plVAO::upload()
{
    if (!_eabo)
    {
        LOG_WARN("Cannot upload, VAO has no EABO attached, command ignored");
        return;
    }

    if (_vbos.empty())
    {
        LOG_WARN("Cannot upload, VAO has no VBOs attached, command ignored");
        return;
    }

    // create and bind VAO
    if (!_id)
    {
        glGenVertexArrays(1, &_id);
        LOG_OPENGL("glGenVertexArrays");
    }

    glBindVertexArray(_id);
    LOG_OPENGL("glBindVertexArray");

    for (auto& vbo : _vbos)
    {
        vbo->upload();
    }

    _eabo->upload();

    glBindVertexArray(0);
    LOG_OPENGL("glBindVertexArray");
}


void plVAO::_destroy()
{
    glDeleteVertexArrays(1, &_id) ;    // delete vao
    LOG_OPENGL("glDeleteVertexArrays");
    _id = 0;
}
