#pragma once

#include "plCommon.h"
#include "plVector3.h"
#include "plVector4.h"
#include "plMatrix44.h"
#include "plTexture2D.h"
#include "plVAO.h"
#include "plVertexFragmentShader.h"


class plRenderComponent
{

    public:

        plRenderComponent(const std::shared_ptr<plVAO>& vao);

        bool operator< (const plRenderComponent& rc) const;

        void attach(const plUniform& uniform);

        void draw(const plShader& shader) const;

    private:

        std::shared_ptr<plVAO >  _vao;
        std::vector<plUniform >  _uniforms;

        void _bindUniforms(const plShader& shader) const;
};

typedef std::map<uint32_t, std::set<plRenderComponent > >  plRenderMap;
