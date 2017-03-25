#pragma once

#include "plCommon.h"
#include "plRenderable.h"
#include "plEditable.h"
#include "plVector3.h"
#include "plTriangle.h"
#include "plPicking.h"
#include "plVAO.h"
#include "plPlug.h"
#include "plPolygon.h"
#include "plUtility.h"
#include "plRenderer.h"
#include "plGraftCap.h"

class plGraft : public plRenderable, public plEditable
{
    public:

        plGraft();
        plGraft(
            std::shared_ptr<plPlug> harvest,
            std::shared_ptr<plPlug> recipient,
            float32_t radius,
            float32_t length,
            const plVector3& markDirection = plVector3(0, 0, 1));

        virtual ~plGraft();

        float32_t radius() const { return _radius; }
        float32_t length() const { return _length; }
        const plVector3& markDirection() const { return _markDirection; }
        const plVector3& markPositions(uint32_t index) const { return _markPositions[index]; }

        std::shared_ptr<plPlug> plug(uint32_t type) const;
        std::shared_ptr<plPlug> harvest() const { return _harvest; }
        std::shared_ptr<plPlug> recipient() const { return _recipient; }

        void move(uint32_t type, const plVector3& origin, const plVector3& y);
        void rotate(uint32_t type, const plVector3& y);
        void rotate(uint32_t type, float32_t angleDegrees);

        void setMarkDirection(const plVector3& direction);
        void snapMarkDirection();

        void extractRenderComponents(plRenderMap& renderMap) const;
        void extractRenderComponents(plRenderMap& renderMap, uint32_t technique) const;

    private:

        std::shared_ptr<plPlug> _recipient;
        std::shared_ptr<plPlug> _harvest;

        float32_t _radius;
        float32_t _length;

        plVector3 _markDirection; // marker direction in graft space
        plVector3 _markPositions[4];

        plBoneCap _boneCap;
        plCartilageCap _cartilageCap;

        void _extractGraftRenderComponents(plRenderMap& renderMap, uint32_t technique) const;
        void _generateCaps();
        void _generateMarkPositions();

};
