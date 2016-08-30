#include "plBoundary.h"


plBoundary::plBoundary(uint32_t type, std::shared_ptr<plMesh> mesh)
    : plMeshSpecific(mesh),
      _type(type)
{
}


plBoundary::plBoundary(uint32_t type, std::shared_ptr<plMesh> mesh, const std::vector<plString>& row)
    : plMeshSpecific(mesh),
      _type(type)
{
    // assumes points are counter-clockwise
    for (uint32_t j = 1; j < row.size(); j+=2)
    {
        _points.push_back(plVector3(row[j]));
        _normals.push_back(plVector3(row[j+1]));
    }
    // construct mesh
    _generateVAO();
}


void plBoundary::extractRenderComponents(plRenderMap& renderMap, uint32_t technique) const
{
    if (!_isVisible)
        return;

    plPickingStack::loadRed(_type);
    plColorStack::load(_getColor());

    // draw walls
    if (_points.size() > 1)
    {
        plPickingStack::loadBlue(-1); // draw walls with index of -1

        // create render component
        auto component = std::make_shared<plRenderComponent>(_vao);
        // attached uniforms
        component->attach(PL_MODEL_MATRIX_UNIFORM, std::make_shared<plUniform>(plMatrix44()));
        component->attach(PL_VIEW_MATRIX_UNIFORM, std::make_shared<plUniform>(plCameraStack::top()));
        component->attach(PL_PROJECTION_MATRIX_UNIFORM, std::make_shared<plUniform>(plProjectionStack::top()));
        component->attach(PL_COLOR_UNIFORM, std::make_shared<plUniform>(plColorStack::top()));
        component->attach(PL_PICKING_UNIFORM, std::make_shared<plUniform>(plPickingStack::top()));
        component->attach(PL_LIGHT_POSITION_UNIFORM, std::make_shared<plUniform>(plVector3(PL_LIGHT_POSITION)));
        // insert into render map
        renderMap[technique].push_back(component);
    }

    // draw points
    _extractPointRenderComponents(renderMap, technique);
}


void plBoundary::extractRenderComponents(plRenderMap& renderMap) const
{
    extractRenderComponents(renderMap, PL_PLAN_TECHNIQUE);
}


void plBoundary::_extractPointRenderComponents(plRenderMap& renderMap, uint32_t technique) const
{
    // draw points
    for (uint32_t i=0; i<_points.size(); i++)
    {
        plPickingStack::loadBlue(i);

        if (_isSelected && _selectedValue == i)   // is the current point selected?
        {
            // scale larger
            plRenderer::queueSphere(technique, _points[i], PL_BOUNDARY_SELECTED_POINT_RADIUS);
        }
        else
        {
            // regular size
            plRenderer::queueSphere(technique, _points[i], PL_BOUNDARY_POINT_RADIUS);
        }
    }
}


plVector3 plBoundary::getAverageNormal() const
{
    plVector3 n(0, 0, 0);
    for (uint32_t i=0; i < _normals.size(); i++)
    {
        n = n + _normals[i];
    }
    return n.normalize();
}


plVector3 plBoundary::getCentroid() const
{
    plVector3 p(0, 0, 0);
    for (uint32_t i=0; i < _points.size(); i++)
    {
        p = p + _points[i];
    }
    return p * (1.0f/(float32_t)_points.size());
}


uint32_t plBoundary::addPointAndNormal(const plVector3& point, const plVector3& normal)
{
    if (_points.size() < 2)
    {
        // 0 or 1 _points, doesnt matter, just add
        _points.push_back(point);
        _normals.push_back(normal);
        _generateVAO();
        return _points.size()-1;
    }
    else if (_points.size() == 2)
    {
        // 2 _points, ensure third is counter clockwise
        plVector3 n = 0.5f * (_normals[1] + _normals[0]);

        if (((_points[1] - _points[0]) ^ (point - _points[0])) * n > 0)
        {
            // already counter clockwise
            _points.push_back(point);
            _normals.push_back(normal);

            _generateVAO();
            return 2;
        }
        else
        {
            // clock-wise, add new point between existing two
            _points.insert (_points.begin()+1, point);
            _normals.insert(_normals.begin()+1, normal);

            _generateVAO();
            return 1;
        }
    }
    else
    {
        // Find the closest edge to the new point
        float32_t minDist = FLT_MAX;
        int32_t shift_i = 0;

        const float32_t EPSILON = 0.000001f;

        for (uint32_t i = 0; i < _points.size(); i++)
        {
            int32_t j = (i+1) % _points.size();   // next point
            // get closest point on current edge
            plVector3 closest = plMath::closestPointOnSegment(point, _points[i], _points[j]);
            // compare distance to current minimum distance
            float32_t distSquared = (closest - point).squaredLength();

            // check if same point as previous edge, occurs when edges are at acute angles, making a pointy "v"
            if (fabs(distSquared - minDist) < EPSILON)
            {
                // same point as previous edge,
                int32_t h = (i == 0) ? _points.size()-1 : i-1; // previous index
                // get current and previous wall segments
                plVector3 previous_segment = (_points[i] - _points[h]);
                plVector3 current_segment  = (_points[j] - _points[i]);
                // get current wall tangent
                plVector3 current_tangent  = current_segment ^ (0.5f * (_normals[j] + _normals[i]));

                // check if current "v" shape is pointing inwards or outwards
                if ((current_segment ^ previous_segment) * _normals[i] < 0)
                {
                    // outward pointing "v"
                    if ((point - closest) * current_tangent  > 0)
                    {
                        // not behind previous edge
                        minDist = distSquared;
                        shift_i = j;
                    }
                }
                else
                {
                    // inward pointing "v"
                    if ((point - closest) * current_tangent  < 0)
                    {
                        // in front of current edge
                        minDist = distSquared;
                        shift_i = j;
                    }
                }
            }
            else if (distSquared < minDist)
            {
                // new minimum, store index
                minDist = distSquared;
                shift_i = j;
            }
        }

        _points.insert (_points.begin()+shift_i, point);
        _normals.insert(_normals.begin()+shift_i, normal);

        _generateVAO();
        return shift_i;
    }
}


void plBoundary::movePointAndNormal(uint32_t index, const plVector3& point, const plVector3& normal)
{
    _points[index] = point;
    _normals[index] = normal;
    _generateVAO();
}


void plBoundary::removePointAndNormal(uint32_t index)
{

    _points.erase (_points.begin()+index);
    _normals.erase(_normals.begin()+index);
    _generateVAO();
}


void plBoundary::clear()
{
    _points.clear();
    _normals.clear();
    _generateVAO();
}


plVector4 plBoundary::_getColor() const
{
    // not selected
    switch (_type)
    {
        case PL_PICKING_TYPE_DEFECT_CORNERS:
        case PL_PICKING_TYPE_DEFECT_SPLINE:
            // defect spline / corners
            return plVector4(PL_BOUNDARY_DEFECT_CORNER_COLOR);

        case PL_PICKING_TYPE_DEFECT_BOUNDARY:
            // defect boundary
            return plVector4(PL_BOUNDARY_DEFECT_BOUNDARY_COLOR);

        case PL_PICKING_TYPE_DONOR_BOUNDARY:
        default:
            // donor boundary
            return plVector4(PL_BOUNDARY_DONOR_COLOR);
    }

}


void plBoundary::_generateVAO()
{
    if (_points.size() < 2)
    {
        _vao = nullptr;
        return;
    }

    plVector3 n = getAverageNormal();

    std::vector<plVector3> vertices;
    vertices.reserve(_points.size() * 10);

    std::vector<uint32_t> indices;
    indices.reserve (_points.size() * 6 * 4);

    for (uint32_t i = 0; i < _points.size(); i++)
    {
        int32_t j = (i+1) % _points.size();            // next index
        int32_t k = (i+2) % _points.size();            // next next index
        int32_t l = (i == 0) ? _points.size()-1 : i-1; // previous index

        // tangent vectors of previous, current, and next segments
        plVector3 t0 = ((_points[i] - _points[l]) ^ n).normalize(); // previous tangent
        plVector3 t1 = ((_points[j] - _points[i]) ^ n).normalize(); // current tangent
        plVector3 t2 = ((_points[k] - _points[j]) ^ n).normalize(); // next tangent

        plVector3 x0, x1;
        if (_points.size() == 2)
        {
            // single wall, only use
            x0 = t1;
            x1 = t1;
        }
        else
        {
            // average tangent vectors of current segment end points
            x0 = (t0 + t1).normalize();
            x1 = (t1 + t2).normalize();
        }

        plVector3 a = _points[i] + PL_BOUNDARY_MESH_WIDTH_HALF * x0;
        plVector3 b = _points[j] + PL_BOUNDARY_MESH_WIDTH_HALF * x1;
        plVector3 c = b + PL_BOUNDARY_MESH_HEIGHT * n;
        plVector3 d = a + PL_BOUNDARY_MESH_HEIGHT * n;

        plVector3 e = _points[i] - PL_BOUNDARY_MESH_WIDTH_HALF * x0;
        plVector3 f = _points[j] - PL_BOUNDARY_MESH_WIDTH_HALF * x1;
        plVector3 g = f + PL_BOUNDARY_MESH_HEIGHT * n;
        plVector3 h = e + PL_BOUNDARY_MESH_HEIGHT * n;

        uint32_t base = vertices.size()/2;

        // "right" quad
        vertices.push_back(a);    // position
        vertices.push_back(t1);   // normal

        vertices.push_back(b);    // position
        vertices.push_back(t1);   // normal

        vertices.push_back(c);    // position
        vertices.push_back(t1);   // normal

        vertices.push_back(d);    // position
        vertices.push_back(t1);   // normal

        // top ridge
        vertices.push_back(0.5 * (d+h) + PL_BOUNDARY_MESH_CURVE_HEIGHT * n);    // position
        vertices.push_back(n);    // normal

        vertices.push_back(0.5 * (c+g) + PL_BOUNDARY_MESH_CURVE_HEIGHT * n);    // position
        vertices.push_back(n);    // normal

        // "left" quad
        vertices.push_back(e);    // position
        vertices.push_back(-t1);  // normal

        vertices.push_back(f);    // position
        vertices.push_back(-t1);  // normal

        vertices.push_back(g);    // position
        vertices.push_back(-t1);  // normal

        vertices.push_back(h);    // position
        vertices.push_back(-t1);  // normal

        // triangle indices
        indices.push_back(base+0);    indices.push_back(base+1);    indices.push_back(base+2);
        indices.push_back(base+0);    indices.push_back(base+2);    indices.push_back(base+3);

        indices.push_back(base+3);    indices.push_back(base+2);    indices.push_back(base+5);
        indices.push_back(base+3);    indices.push_back(base+5);    indices.push_back(base+4);

        indices.push_back(base+4);    indices.push_back(base+5);    indices.push_back(base+8);
        indices.push_back(base+4);    indices.push_back(base+8);    indices.push_back(base+9);

        indices.push_back(base+7);    indices.push_back(base+6);    indices.push_back(base+9);
        indices.push_back(base+7);    indices.push_back(base+9);    indices.push_back(base+8);

        if (_points.size() == 2 && i == 0)
        {
            // only 2 points, add faces on ends
            plVector3 backNormal = (_points[i] - _points[l]).normalize();
            plVector3 frontNormal = -backNormal;

            // "back" wall ridge
            vertices.push_back(0.5f * (a+e) + PL_BOUNDARY_MESH_CURVE_HEIGHT * backNormal);    // position
            vertices.push_back(backNormal);   // normal

            vertices.push_back(0.5f * (d+h) + PL_BOUNDARY_MESH_CURVE_HEIGHT * backNormal);    // position
            vertices.push_back(backNormal);   // normal

            // "front" wall ridge
            vertices.push_back(0.5f * (b+f) + PL_BOUNDARY_MESH_CURVE_HEIGHT * frontNormal);    // position
            vertices.push_back(frontNormal);   // normal

            vertices.push_back(0.5f * (c+g) + PL_BOUNDARY_MESH_CURVE_HEIGHT * frontNormal);    // position
            vertices.push_back(frontNormal);   // normal

            // "back" indices
            indices.push_back(base+6);    indices.push_back(base+10);    indices.push_back(base+11);
            indices.push_back(base+6);    indices.push_back(base+11);    indices.push_back(base+9);

            indices.push_back(base+10);   indices.push_back(base+0);    indices.push_back(base+3);
            indices.push_back(base+10);   indices.push_back(base+3);    indices.push_back(base+11);

            indices.push_back(base+9);    indices.push_back(base+11);    indices.push_back(base+4);
            indices.push_back(base+11);   indices.push_back(base+3);    indices.push_back(base+4);

            // "front" indices
            indices.push_back(base+1);    indices.push_back(base+12);   indices.push_back(base+13);
            indices.push_back(base+1);    indices.push_back(base+13);   indices.push_back(base+2);

            indices.push_back(base+12);   indices.push_back(base+7);    indices.push_back(base+8);
            indices.push_back(base+12);   indices.push_back(base+8);    indices.push_back(base+13);

            indices.push_back(base+2);    indices.push_back(base+13);   indices.push_back(base+5);
            indices.push_back(base+13);   indices.push_back(base+8);    indices.push_back(base+5);

            break; // break out of loop, a 2 point wall mesh is complete from the first iteration
        }
    }

    // set vbo and attach attribute pointers
    auto vbo = std::make_shared<plVBO>();
    vbo->set(vertices);
    vbo->set(plVertexAttributePointer(PL_POSITION_ATTRIBUTE, 32, 0));
    vbo->set(plVertexAttributePointer(PL_NORMAL_ATTRIBUTE,   32, 16)); // FOUR COMPONENT FLOATING POINT OFFSET
    // set eabo
    auto eabo = std::make_shared<plEABO>();
    eabo->set(indices);
    // attach to vao
    _vao = std::make_shared<plVAO>();
    _vao->attach(vbo);
    _vao->attach(eabo);
    // upload to gpu
    _vao->upload();
}


std::ostream& operator<< (std::ostream& out, const plBoundary& b)
{
    for (uint32_t j=0; j<b.size(); j++)
    {
        if (j != 0)
        {
            out << ",";
        }
        out << b.points(j) << "," << b.normals(j);
    }
    return out;
}
