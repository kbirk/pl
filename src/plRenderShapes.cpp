#include "plRenderShapes.h"

namespace plRenderShapes {

std::shared_ptr<plVAO> sphereVAO(float32_t radius, int32_t slices, int32_t stacks)
{
    float32_t rho, theta;
    float32_t x, y, z;
    int32_t base;

    float32_t drho = PL_PI / stacks;
    float32_t dtheta = 2.0f * PL_PI / slices;

    std::vector<plVector3> vertices;
    std::vector<uint32_t> indices;

    // draw +Z end as a triangle fan
    // centre of triangle fan
    vertices.push_back(plVector3(0.0f, 0.0f, radius)); // position
    vertices.push_back(plVector3(0.0f, 0.0f, 1.0f)); // normal
    for (int32_t j = 0; j <= slices; j++) {
        theta = (j == slices) ? 0.0f : j * dtheta;
        x = -sin(theta) * sin(drho);
        y = cos(theta) * sin(drho);
        z = cos(drho);

        vertices.push_back(plVector3(x * radius, y * radius, z * radius)); // position
        vertices.push_back(plVector3(x, y, z)); // normal
    }

    for (int32_t j = 1; j <= slices; j++) {
        indices.push_back(0);
        indices.push_back(j);
        indices.push_back(j + 1);
    }

    int32_t imin = 1;
    int32_t imax = stacks - 1;

    // draw intermediate stacks as quad strips
    for (int32_t i = imin; i < imax; i++) {
        base = vertices.size() / 2;

        rho = i * drho;

        for (int32_t j = 0; j <= slices; j++) {
            theta = (j == slices) ? 0.0f : j * dtheta;

            x = -sin(theta) * sin(rho);
            y = cos(theta) * sin(rho);
            z = cos(rho);

            vertices.push_back(plVector3(x * radius, y * radius, z * radius)); // position
            vertices.push_back(plVector3(x, y, z)); // normal

            x = -sin(theta) * sin(rho + drho);
            y = cos(theta) * sin(rho + drho);
            z = cos(rho + drho);

            vertices.push_back(plVector3(x * radius, y * radius, z * radius)); // position
            vertices.push_back(plVector3(x, y, z)); // normal
        }

        for (int32_t j = 0; j < slices * 2; j += 2) {
            indices.push_back(base + j);
            indices.push_back(base + j + 1);
            indices.push_back(base + j + 2);

            indices.push_back(base + j + 1);
            indices.push_back(base + j + 3);
            indices.push_back(base + j + 2);
        }
    }

    // draw -Z end as a triangle fan
    base = vertices.size() / 2;

    vertices.push_back(plVector3(0.0f, 0.0f, -radius)); // position
    vertices.push_back(plVector3(0.0f, 0.0f, -1.0f)); // normal

    rho = PL_PI - drho;

    for (int32_t j = slices; j >= 0; j--) {
        theta = (j == slices) ? 0.0f : j * dtheta;
        x = -sin(theta) * sin(rho);
        y = cos(theta) * sin(rho);
        z = cos(rho);

        vertices.push_back(plVector3(x * radius, y * radius, z * radius)); // position
        vertices.push_back(plVector3(x, y, z)); // normal
    }

    for (int32_t j = 1; j <= slices; j++) {
        indices.push_back(base + 0);
        indices.push_back(base + j);
        indices.push_back(base + j + 1);
    }

    // set vbo and attach attribute pointers
    auto vbo = std::make_shared<plVBO>();
    vbo->set(vertices);
    vbo->set(plVertexAttributePointer(PL_POSITION_ATTRIBUTE, 32, 0));
    vbo->set(plVertexAttributePointer(PL_NORMAL_ATTRIBUTE, 32, 16));
    // set eabo
    auto eabo = std::make_shared<plEABO>();
    eabo->set(indices);
    // create and attach to vao
    std::shared_ptr<plVAO> vao = std::make_shared<plVAO>();
    vao->attach(vbo);
    vao->attach(eabo);
    // upload to gpu
    vao->upload();
    return vao;
}

std::shared_ptr<plVAO> cylinderVAO(float32_t baseRadius, float32_t topRadius, float32_t height, int32_t slices, int32_t stacks)
{
    float32_t da = 2.0f * PL_PI / slices;
    float32_t dr = (topRadius - baseRadius) / stacks;
    float32_t dz = height / stacks;
    float32_t nz = (baseRadius - topRadius) / height;

    float32_t sa, ca;
    float32_t z = 0.0f;
    float32_t r = baseRadius;

    std::vector<plVector3> vertices;
    std::vector<uint32_t> indices;

    for (int32_t j = 0; j < stacks; j++) {
        int32_t base = vertices.size() / 2;

        for (int32_t i = 0; i <= slices; i++) {
            float32_t a = (i == slices) ? 0.0f : i * da;

            sa = sin(a);
            ca = cos(a);

            vertices.push_back(plVector3(sa * r, ca * r, z)); // position
            vertices.push_back(plVector3(sa, ca, nz)); // normal

            vertices.push_back(plVector3(sa * (r + dr), ca * (r + dr), z + dz)); // position
            vertices.push_back(plVector3(sa, ca, nz)); // normal
        }

        for (int32_t i = 0; i < slices * 2; i += 2) {
            indices.push_back(base + i);
            indices.push_back(base + i + 1);
            indices.push_back(base + i + 2);

            indices.push_back(base + i + 1);
            indices.push_back(base + i + 3);
            indices.push_back(base + i + 2);
        }
        r += dr;
        z += dz;
    }

    // set vbo and attach attribute pointers
    auto vbo = std::make_shared<plVBO>();
    vbo->set(vertices);
    vbo->set(plVertexAttributePointer(PL_POSITION_ATTRIBUTE, 32, 0));
    vbo->set(plVertexAttributePointer(PL_NORMAL_ATTRIBUTE, 32, 16));
    // set eabo
    auto eabo = std::make_shared<plEABO>();
    eabo->set(indices);
    // create and attach to vao
    std::shared_ptr<plVAO> vao = std::make_shared<plVAO>();
    vao->attach(vbo);
    vao->attach(eabo);
    // upload to gpu
    vao->upload();
    return vao;
}

std::shared_ptr<plVAO> diskVAO(float32_t innerRadius, float32_t outerRadius, int32_t slices, int32_t loops, bool up)
{
    plVector3 normal = (up) ? plVector3(0.0f, 0.0f, 1.0f) : plVector3(0.0f, 0.0f, -1.0f);

    float32_t da = 2.0f * PL_PI / slices;
    float32_t dr = (outerRadius - innerRadius) / loops;

    float32_t sa, ca;
    float32_t r1 = innerRadius;

    std::vector<plVector3> vertices;
    std::vector<uint32_t> indices;

    for (int32_t l = 0; l < loops; l++) {
        float32_t r2 = r1 + dr;

        int32_t base = vertices.size() / 2;

        if (up) {
            for (int32_t s = 0; s <= slices; s++) {
                float32_t a = (s == slices) ? 0.0f : s * da;

                sa = sin(a);
                ca = cos(a);

                vertices.push_back(plVector3(r2 * sa, r2 * ca, 0.0f)); // position
                vertices.push_back(normal); // normal

                vertices.push_back(plVector3(r1 * sa, r1 * ca, 0.0f)); // position
                vertices.push_back(normal); // normal
            }
        } else {
            for (int32_t s = slices; s >= 0; s--) {
                float32_t a = (s == slices) ? 0.0f : s * da;

                sa = sin(a);
                ca = cos(a);

                vertices.push_back(plVector3(r2 * sa, r2 * ca, 0.0f)); // position
                vertices.push_back(normal); // normal

                vertices.push_back(plVector3(r1 * sa, r1 * ca, 0.0f)); // position
                vertices.push_back(normal); // normal
            }
        }

        r1 = r2;

        for (int32_t i = 0; i < slices * 2; i += 2) {
            indices.push_back(base + i);
            indices.push_back(base + i + 1);
            indices.push_back(base + i + 2);

            indices.push_back(base + i + 1);
            indices.push_back(base + i + 3);
            indices.push_back(base + i + 2);
        }
    }

    // set vbo and attach attribute pointers
    auto vbo = std::make_shared<plVBO>();
    vbo->set(vertices);
    vbo->set(plVertexAttributePointer(PL_POSITION_ATTRIBUTE, 32, 0));
    vbo->set(plVertexAttributePointer(PL_NORMAL_ATTRIBUTE, 32, 16));
    // set eabo
    auto eabo = std::make_shared<plEABO>();
    eabo->set(indices);
    // create and attach to vao
    std::shared_ptr<plVAO> vao = std::make_shared<plVAO>();
    vao->attach(vbo);
    vao->attach(eabo);
    // upload to gpu
    vao->upload();
    return vao;
}

std::shared_ptr<plVAO> coneVAO(float32_t baseRadius, float32_t topRadius, float32_t height, int32_t slices, int32_t stacks)
{
    float32_t da = 2.0f * PL_PI / slices;
    float32_t dr = (topRadius - baseRadius) / stacks;
    float32_t dz = height / stacks;
    float32_t nz = (baseRadius - topRadius) / height;

    float32_t sa, ca;
    float32_t z = 0.0f;
    float32_t r = baseRadius;

    std::vector<plVector3> vertices;
    std::vector<uint32_t> indices;

    for (int32_t j = 0; j < stacks; j++) {
        int32_t base = vertices.size() / 2;

        for (int32_t i = 0; i <= slices; i++) {
            float32_t a = (i == slices) ? 0.0f : i * da;

            sa = sin(a);
            ca = cos(a);

            vertices.push_back(plVector3(sa * r, ca * r, z)); // position
            vertices.push_back(plVector3(sa, ca, nz)); // normal

            vertices.push_back(plVector3(sa * (r + dr), ca * (r + dr), z + dz)); // position
            vertices.push_back(plVector3(sa, ca, nz)); // normal
        }

        for (int32_t i = 0; i < slices * 2; i += 2) {
            indices.push_back(base + i);
            indices.push_back(base + i + 1);
            indices.push_back(base + i + 2);

            indices.push_back(base + i + 1);
            indices.push_back(base + i + 3);
            indices.push_back(base + i + 2);
        }
        r += dr;
        z += dz;
    }

    // set vbo and attach attribute pointers
    auto vbo = std::make_shared<plVBO>();
    vbo->set(vertices);
    vbo->set(plVertexAttributePointer(PL_POSITION_ATTRIBUTE, 32, 0));
    vbo->set(plVertexAttributePointer(PL_NORMAL_ATTRIBUTE, 32, 16));
    // set eabo
    auto eabo = std::make_shared<plEABO>();
    eabo->set(indices);
    // create and attach to vao
    std::shared_ptr<plVAO> vao = std::make_shared<plVAO>();
    vao->attach(vbo);
    vao->attach(eabo);
    // upload to gpu
    vao->upload();
    return vao;
}

std::shared_ptr<plVAO> quadVAO()
{
    std::vector<plVector3> vertices;
    vertices.reserve(8);
    std::vector<uint32_t> indices;
    indices.reserve(6);

    // position                                   // normals
    vertices.push_back(plVector3(-1, -1, 0));
    vertices.push_back(plVector3(0, 0, 1));
    vertices.push_back(plVector3(1, -1, 0));
    vertices.push_back(plVector3(0, 0, 1));
    vertices.push_back(plVector3(1, 1, 0));
    vertices.push_back(plVector3(0, 0, 1));
    vertices.push_back(plVector3(-1, 1, 0));
    vertices.push_back(plVector3(0, 0, 1));

    // indices
    indices.push_back(0);
    indices.push_back(1);
    indices.push_back(2);

    indices.push_back(0);
    indices.push_back(2);
    indices.push_back(3);

    // set vbo and attach attribute pointers
    auto vbo = std::make_shared<plVBO>();
    vbo->set(vertices);
    vbo->set(plVertexAttributePointer(PL_POSITION_ATTRIBUTE, 32, 0));
    vbo->set(plVertexAttributePointer(PL_NORMAL_ATTRIBUTE, 32, 16));
    // set eabo
    auto eabo = std::make_shared<plEABO>();
    eabo->set(indices);
    // create and attach to vao
    std::shared_ptr<plVAO> vao = std::make_shared<plVAO>();
    vao->attach(vbo);
    vao->attach(eabo);
    // upload to gpu
    vao->upload();
    return vao;
}

std::shared_ptr<plVAO> lineVAO(const plVector3& p0, const plVector3& p1)
{
    std::vector<plVector3> vertices;
    vertices.reserve(2);
    std::vector<uint32_t> indices;
    indices.reserve(2);

    // position
    vertices.push_back(p0);
    vertices.push_back(p1);

    // indices
    indices.push_back(0);
    indices.push_back(1);

    // set vbo and attach attribute pointers
    auto vbo = std::make_shared<plVBO>();
    vbo->set(vertices);
    vbo->set(plVertexAttributePointer(PL_POSITION_ATTRIBUTE, 16, 0));
    // set eabo
    auto eabo = std::make_shared<plEABO>();
    eabo->set(indices, GL_LINES);
    // create and attach to vao
    std::shared_ptr<plVAO> vao = std::make_shared<plVAO>();
    vao->attach(vbo);
    vao->attach(eabo);
    // upload to gpu
    vao->upload();
    return vao;
}
}
