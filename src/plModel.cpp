#include "plModel.h"

plModel::plModel(const std::vector<plTriangle> &triangles, const plString &file, uint32_t octreeDepth)
    : filename(file)
{
    if (octreeDepth > 1)
    {
        // use octree mesh
        _mesh = std::make_shared<plOctreeMesh>(std::move(triangles), octreeDepth, true);
    }
    else
    {
        // use non-octree mesh
        _mesh = std::make_shared<plMesh>(std::move(triangles));
    }

    _generateVAO();
}


plModel::plModel(const plString &file, uint32_t octreeDepth)
    : filename(file)
{
    std::vector<plTriangle > triangles;

    // import triangles from STL file
    if (!plSTL::importFile(triangles, filename, true))
        return;

    if (octreeDepth > 1)
    {
        // use octree mesh
        _mesh = std::make_shared<plOctreeMesh>(std::move(triangles), octreeDepth, true);
    }
    else
    {
        // use non-octree mesh
        _mesh = std::make_shared<plMesh>(std::move(triangles));
    }

    _generateVAO();
}

plModel::~plModel()
{
}

void plModel::extractRenderComponents(plRenderMap& renderMap, uint32_t technique) const
{
    // render octree
    if (std::dynamic_pointer_cast<plOctreeMesh>(_mesh))
    {
        std::dynamic_pointer_cast<plOctreeMesh>(_mesh)->octree().extractRenderComponents(renderMap);
    }

    if (!_isVisible)
        return;

    // create render component
    plRenderComponent component(_vao);
    // attached uniforms
    component.attach(plUniform(PL_MODEL_MATRIX_UNIFORM,      plModelStack::top()));
    component.attach(plUniform(PL_VIEW_MATRIX_UNIFORM,       plCameraStack::top()));
    component.attach(plUniform(PL_PROJECTION_MATRIX_UNIFORM, plProjectionStack::top()));
    component.attach(plUniform(PL_LIGHT_POSITION_UNIFORM, plVector3(PL_LIGHT_POSITION)));
    component.attach(plUniform(PL_PICKING_UNIFORM,           plPickingStack::top()));

    if (!_isTransparent)
    {
        component.attach(plUniform(PL_COLOUR_UNIFORM, plColourStack::top()));
        // insert into render map
        renderMap[technique].insert(component);
    }
    else
    {
        // Sort by distance
        plVector3 viewDir = plCameraStack::direction();

        std::vector<plOrderPair> order;     order.reserve(_mesh->triangles().size());
        uint32_t index = 0;
        for (const plTriangle& triangle : _mesh->triangles())
        {
            order.emplace_back(plOrderPair(index++, triangle.centroid() * viewDir));
        }
        std::sort(order.begin(), order.end());

        std::vector<uint32_t> indices;    indices.reserve(_mesh->triangles().size()*3);
        for (uint32_t i = 0; i < order.size(); i++)
        {
            indices.push_back(order[i].index*3);
            indices.push_back(order[i].index*3+1);
            indices.push_back(order[i].index*3+2);
        }

        // dirty const_cast
        const_cast< std::shared_ptr<plVAO >&>(_vao)->eabo()->set(indices);
        const_cast< std::shared_ptr<plVAO >&>(_vao)->upload();

        component.attach(plUniform(PL_COLOUR_UNIFORM, plVector4(plColourStack::top().x, plColourStack::top().y, plColourStack::top().z, 0.7f)));

        // insert into render map
        if (technique == PL_PLAN_TECHNIQUE)
        {
            // if originally meant to be rendered using plan technique
            renderMap[PL_TRANSPARENCY_TECHNIQUE].insert(component);
        }
        else
        {
            renderMap[technique].insert(component);
        }
    }

}


void plModel::extractRenderComponents(plRenderMap& renderMap) const
{
    extractRenderComponents(renderMap, PL_PLAN_TECHNIQUE);
}


void plModel::toggleOctreeVisibility()
{
    if (std::dynamic_pointer_cast<plOctreeMesh>(_mesh))
    {
        std::dynamic_pointer_cast<plOctreeMesh>(_mesh)->toggleOctreeVisibility();
    }
}

void plModel::_generateVAO()
{
    // convert to interleaved format
    std::vector<plVector3> vertices;    vertices.reserve(_mesh->triangles().size() * 3 * 2);
    std::vector<uint32_t>    indices;     indices.reserve (_mesh->triangles().size() * 3);

    int32_t indexCount = 0;
    for (const plTriangle& triangle : _mesh->triangles())
    {
        // p1
        vertices.emplace_back(triangle.point0());    // position
        vertices.emplace_back(triangle.normal());    // normal
        indices.emplace_back(indexCount++);
        // p2
        vertices.emplace_back(triangle.point1());
        vertices.emplace_back(triangle.normal());
        indices.emplace_back(indexCount++);
        // p3
        vertices.emplace_back(triangle.point2());
        vertices.emplace_back(triangle.normal());
        indices.emplace_back(indexCount++);
    }

    // set vbo and attach attribute pointers
    std::shared_ptr<plVBO > vbo = std::make_shared<plVBO>();
    vbo->set(vertices);
    vbo->set(plVertexAttributePointer(PL_POSITION_ATTRIBUTE, 32, 0));
    vbo->set(plVertexAttributePointer(PL_NORMAL_ATTRIBUTE,   32, 16));
    // set eabo
    std::shared_ptr<plEABO> eabo = std::make_shared<plEABO>();
    eabo->set(indices);
    // create vao, attach eabo and vbo, upload to gpu
    _vao = std::make_shared<plVAO>();
    _vao->attach(vbo);
    _vao->attach(eabo);
    // upload to gpu
    _vao->upload();
}


plVector3 plModel::getCentroid() const
{
    plVector3 min, max;

    _mesh->getMinMax(min, max);

    return 0.5f * (max + min);
}
