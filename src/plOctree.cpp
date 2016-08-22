#include "plOctree.h"

plOctree::plOctree()
    : _children(8, nullptr)
{
    _isVisible = false;
}


plOctree::plOctree(const plVector3 &min, const plVector3 &max, const std::vector<plTriangle> &triangles, uint32_t depth)
    : _children(8, nullptr)
{
    // build from root
    build(min, max, triangles, depth);
    _isVisible = false;
}


plOctree::plOctree(const plVector3 &centre, float32_t halfWidth, uint32_t depth)
    : _depth(depth),
      _centre(centre),
      _halfWidth(halfWidth),
      _children(8, nullptr)
{
    _isVisible = false;
}


plOctree::plOctree (const plOctree& octree)
    : _children(8, (plOctree*)(nullptr))
{
    _copy(octree);
}


plOctree::plOctree (plOctree&& octree)
    : _children(8, (plOctree*)(nullptr))
{
    _move(std::move(octree));
}


plOctree::~plOctree ()
{
    clear();
}


plOctree& plOctree::operator= (const plOctree& octree)
{
    _copy(octree);
    return *this;
}


plOctree& plOctree::operator= (plOctree&& octree)
{
    _move(std::move(octree));
    return *this;
}


void plOctree::clear()
{
    for (uint32_t i=0; i < 8; i++)
    {
        if (_children[i])
        {
            // delete child
            delete _children[i];
            _children[i] = nullptr;
        }
    }
}


void plOctree::build(const plVector3 &min, const plVector3 &max, const std::vector<plTriangle> &triangles, uint32_t depth, bool verbose)
{
    if (verbose)
        std::cout << "Building octree for " << triangles.size() << " triangles (depth = " << depth << ")...";

    // centre point of octree
    _centre = 0.5f * (min+max);
    _depth = depth;

    // find largest distance component, becomes half width
    plVector3 minDiff = min - _centre;
    plVector3 maxDiff = max - _centre;
    float32_t minMax = PL_MAX_OF_3(fabs(minDiff.x), fabs(minDiff.y), fabs(minDiff.z));
    float32_t maxMax = PL_MAX_OF_3(fabs(maxDiff.x), fabs(maxDiff.y), fabs(maxDiff.z));

    // half width of octree cell
    _halfWidth = PL_MAX_OF_2(minMax, maxMax);

    // clear just incase
    clear();

    // insert triangles into octree
    for (const plTriangle& triangle : triangles)
    {
        _insert(triangle);
    }

    if (verbose)
        std::cout << " Complete." << std::endl;
}


void plOctree::extractRenderComponents(plRenderMap& renderMap, uint32_t technique) const
{
    if (!_isVisible)
        return;

    static std::shared_ptr<plVAO> vao = std::make_shared<plVAO>(_generateVAO(1.0f));

    int32_t count = 0;
    // draw child nodes
    for (uint32_t i=0; i < 8; i++)
    {
        if (_children[i])
        {
            _children[i]->extractRenderComponents(renderMap);
            count++;
        }
    }

    // draw current node
    if (_contained.size() > 0 || count > 0)    // only draw if contains objects, or has children that contain
    {
        plColorStack::load(PL_PURPLE_COLOR);

        plModelStack::push();
        plModelStack::translate(_centre);
        plModelStack::scale(plVector3(_halfWidth, _halfWidth, _halfWidth));

        // create render component
        plRenderComponent component(vao);
        // attached uniforms
        component.attach(plUniform(PL_MODEL_MATRIX_UNIFORM,      plModelStack::top()));
        component.attach(plUniform(PL_VIEW_MATRIX_UNIFORM,       plCameraStack::top()));
        component.attach(plUniform(PL_PROJECTION_MATRIX_UNIFORM, plProjectionStack::top()));
        component.attach(plUniform(PL_COLOR_UNIFORM,             plColorStack::top()));
        // insert into render map
        renderMap[technique].insert(component);

        plModelStack::pop();
    }
}


void plOctree::extractRenderComponents(plRenderMap& renderMap) const
{
    extractRenderComponents(renderMap, PL_MINIMAL_TECHNIQUE);
}


void plOctree::toggleVisibility()
{
    _isVisible = !_isVisible;
    for (uint32_t i=0; i < 8; i++)
    {
        if (_children[i])
        {
            _children[i]->toggleVisibility();
        }
    }
}


plVAO plOctree::_generateVAO(float32_t halfWidth) const
{
    std::vector<plVector3> vertices;    vertices.reserve(8);
    std::vector<uint32_t>    indices;     indices.reserve(8*3);

    // front face
    vertices.emplace_back(plVector3(-halfWidth, -halfWidth, halfWidth));
    vertices.emplace_back(plVector3(halfWidth, -halfWidth, halfWidth));
    vertices.emplace_back(plVector3(halfWidth, halfWidth, halfWidth));
    vertices.emplace_back(plVector3(-halfWidth, halfWidth, halfWidth));

    // back face
    vertices.emplace_back(plVector3(-halfWidth, -halfWidth, -halfWidth));
    vertices.emplace_back(plVector3(halfWidth, -halfWidth, -halfWidth));
    vertices.emplace_back(plVector3(halfWidth,  halfWidth, -halfWidth));
    vertices.emplace_back(plVector3(-halfWidth,  halfWidth, -halfWidth));

    // front
    indices.push_back(0);   indices.push_back(1);
    indices.push_back(1);   indices.push_back(2);
    indices.push_back(2);   indices.push_back(3);
    indices.push_back(3);   indices.push_back(0);

    // sides
    indices.push_back(0);   indices.push_back(4);
    indices.push_back(1);   indices.push_back(5);
    indices.push_back(2);   indices.push_back(6);
    indices.push_back(3);   indices.push_back(7);

    // back
    indices.push_back(4);   indices.push_back(5);
    indices.push_back(5);   indices.push_back(6);
    indices.push_back(6);   indices.push_back(7);
    indices.push_back(7);   indices.push_back(4);

    // set vbo and attach attribute pointers
    std::shared_ptr<plVBO> vbo = std::make_shared<plVBO>();
    vbo->set(vertices);
    vbo->set(plVertexAttributePointer(PL_POSITION_ATTRIBUTE, 16, 0));
    // set eabo
    std::shared_ptr<plEABO> eabo = std::make_shared<plEABO>();
    eabo->set(indices, GL_LINES);
    // create and attach to vao
    plVAO vao;
    vao.attach(vbo);
    vao.attach(eabo);
    // upload to gpu
    vao.upload();

    return vao;
}


void plOctree::_insert(const plTriangle &tri)
{
    // only add triangle if leaf node
    if (_depth == 0)
        _contained.push_back(&tri);

    // distance from each axis
    float32_t dx = tri.centroid().x - _centre.x;
    float32_t dy = tri.centroid().y - _centre.y;
    float32_t dz = tri.centroid().z - _centre.z;

    // if distance is less than radius, then the triangle straddles a boundary
    if (fabs(dx) < tri.radius() || fabs(dy) < tri.radius() || fabs(dz) < tri.radius())
    {
        // straddles a boundary try to add to intersected children
        for (uint32_t i=0; i<8; i++)
        {
            // check if triangle bounding sphere intersects this child
            if (_sphereCheck(tri.centroid(), tri.radius(), i))
            {
                // part of bounding sphere intersects child, insert
                _insertIntoChild(i, tri);
            }
        }
    }
    else
    {
        // fully contained in a single child, find child index
        // contains the 0-7 index of the child, determined using bit wise addition
        int32_t child = 0;
        if (dx > 0) child += 1;
        if (dy > 0) child += 2;
        if (dz > 0) child += 4;

        _insertIntoChild(child, tri);
    }
}


void plOctree::_insertIntoChild(uint32_t index, const plTriangle &tri)
{
    if (_children[index])
    {
        // child already exists, recursively insert
        _children[index]->_insert(tri);
    }
    else
    {
        // child does not exist, if terminal depth has not been reached, create child node
        if (_depth > 0)
        {
            plVector3 offset;
            float32_t step = _halfWidth * 0.5f;
            offset.x = ((index & 1) ? step : -step);
            offset.y = ((index & 2) ? step : -step);
            offset.z = ((index & 4) ? step : -step);
            _children[index] = new plOctree(_centre + offset, step, _depth-1);
            _children[index]->_insert(tri);
        }
    }
}


float32_t plOctree::_sqrDistFromPoint(const plVector3 &point, int32_t child) const
{
    // shift AABB dimesions based on which child cell is begin tested
    plVector3 offsetCentre = _centre;
    float32_t step = 0.5f * _halfWidth;
    offsetCentre.x += ((child & 1) ? step : -step);
    offsetCentre.y += ((child & 2) ? step : -step);
    offsetCentre.z += ((child & 4) ? step : -step);
    plVector3 minAABB = plVector3(offsetCentre.x - step, offsetCentre.y - step, offsetCentre.z - step);
    plVector3 maxAABB = plVector3(offsetCentre.x + step, offsetCentre.y + step, offsetCentre.z + step);

    // For each axis count any excess distance outside box extents
    float32_t sqrDist = 0.0f;
    // x
    if (point.x < minAABB.x) sqrDist += (minAABB.x - point.x) * (minAABB.x - point.x);
    if (point.x > maxAABB.x) sqrDist += (point.x - maxAABB.x) * (point.x - maxAABB.x);
    // y
    if (point.y < minAABB.y) sqrDist += (minAABB.y - point.y) * (minAABB.y - point.y);
    if (point.y > maxAABB.y) sqrDist += (point.y - maxAABB.y) * (point.y - maxAABB.y);
    // z
    if (point.z < minAABB.z) sqrDist += (minAABB.z - point.z) * (minAABB.z - point.z);
    if (point.z > maxAABB.z) sqrDist += (point.z - maxAABB.z) * (point.z - maxAABB.z);

    return sqrDist;
}


plVector3 plOctree::_closestPointInBox(const plVector3 &point, int32_t child) const
{
    // shift AABB dimesions based on which child cell is begin tested
    float32_t step = 0.5f * _halfWidth;
    plVector3 offsetCentre = _centre;
    offsetCentre.x += ((child & 1) ? step : -step);
    offsetCentre.y += ((child & 2) ? step : -step);
    offsetCentre.z += ((child & 4) ? step : -step);
    plVector3 minAABB = plVector3(offsetCentre.x - step, offsetCentre.y - step, offsetCentre.z - step);
    plVector3 maxAABB = plVector3(offsetCentre.x + step, offsetCentre.y + step, offsetCentre.z + step);

    plVector3 closestPoint;
    closestPoint.x = (point.x < minAABB.x)? minAABB.x : (point.x > maxAABB.x)? maxAABB.x : point.x;
    closestPoint.y = (point.y < minAABB.y)? minAABB.y : (point.y > maxAABB.y)? maxAABB.y : point.y;
    closestPoint.z = (point.z < minAABB.z)? minAABB.z : (point.z > maxAABB.z)? maxAABB.z : point.z;

    return closestPoint;
}


bool plOctree::_sphereCheck(const plVector3 &centre, float32_t radius, int32_t child) const
{
    // compute squared distance between sphere centre and AABB
    float32_t dist = _sqrDistFromPoint(centre, child);
    // sphere and AABB intersect if the distance is less than the sphere radius
    return dist <= radius*radius;
}


bool plOctree::rayIntersect(std::set<const plTriangle*> &triangles, const plVector3 &rayOrigin, const plVector3 &rayDirection, float32_t rayRadius, bool ignoreBehindRay) const
{
    // box inflation is used to intersect cylinder with box rather than ray, used for grafts
    float32_t boxExtents = _halfWidth + rayRadius;

    // check if ray origin is inside box
    plVector3 diff = rayOrigin - _centre;

    if(ignoreBehindRay)
    {
        if(fabs(diff.x) > boxExtents && diff.x*rayDirection.x >= 0.0f)    return false;
        if(fabs(diff.y) > boxExtents && diff.y*rayDirection.y >= 0.0f)    return false;
        if(fabs(diff.z) > boxExtents && diff.z*rayDirection.z >= 0.0f)    return false;
    }

    plVector3 absDirection(fabs(rayDirection.x), fabs(rayDirection.y), fabs(rayDirection.z));

    float32_t f;
    f = rayDirection.y * diff.z - rayDirection.z * diff.y;    if(fabs(f) > boxExtents*absDirection.z + boxExtents*absDirection.y) return false;
    f = rayDirection.z * diff.x - rayDirection.x * diff.z;    if(fabs(f) > boxExtents*absDirection.z + boxExtents*absDirection.x) return false;
    f = rayDirection.x * diff.y - rayDirection.y * diff.x;    if(fabs(f) > boxExtents*absDirection.y + boxExtents*absDirection.x) return false;

    // intersection exists, recurse further
    uint32_t childCount = 0;

    for (uint32_t i=0; i<8; i++)
    {
        if (_children[i])
        {
            // not a leaf, recurse
            _children[i]->rayIntersect(triangles, rayOrigin, rayDirection, rayRadius, ignoreBehindRay);
            childCount++;
        }
    }

    if (childCount == 0)
    {
        // leaf node, add triangles to collision set
        for (const plTriangle* tri : _contained)
        {
            triangles.insert(tri);
        }
    }

    return true;
}


void plOctree::_move(plOctree &&octree)
{
    for (uint32_t i=0; i < 8; i++)
    {
        _children[i] = octree._children[i];
        octree._children[i] = 0;
    }
    _depth     = octree._depth;
    _centre    = octree._centre;
    _halfWidth = octree._halfWidth;
    _contained = octree._contained;
    _isVisible = octree._isVisible;
}


void plOctree::_copy(const plOctree& octree)
{
    // clear this entire octree
    clear();

    // copy data
    _depth     = octree._depth;
    _centre    = octree._centre;
    _halfWidth = octree._halfWidth;
    _contained = octree._contained;
    _isVisible = octree._isVisible;

    // copy children
    for (uint32_t i=0; i < 8; i++)
    {
        if (octree._children[i])
        {
            // this child exists, allocate if needed
            if (!_children[i])
                _children[i] = new plOctree();

            // copy node
            _children[i]->_copy(*(octree._children[i]));

        }
        else
        {
            // nothign to copy, free this child
            delete _children[i];
        }

    }
}
