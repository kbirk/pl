#include "plPlan.h"

plPlan::plPlan(plString filename)
{
    if (filename.compare(".csv", filename.size()-4, 4))
    {
        std::cerr << "Unrecognized suffix on filename '"
            << filename
            << "'. plPlan filenames should have suffix .csv"
            << std::endl;
        exit(1);
    }
    importFile(filename);
}


plPlan::plPlan(int32_t argc, char **argv)
{
    if (argc == 1)
    {
        // 0 arguments
        std::cerr << "plPlan::plPlan() error: No arguments provided" << std::endl;
        exit(1);
    }
    if (argc >= 2)
    {
        plString filename(argv[1]) ;

        if (filename.compare(".csv", filename.size()-4, 4))
        {
            // load plan
            importFile(filename);
        }
        else if (filename.compare(".stl", filename.size()-4, 4))
        {
            // model input order: bone, cartilage, bone, cartilage, etc...
            _models.push_back(std::make_shared<plModel>(filename, PL_MODEL_DEFAULT_OCTREE_DEPTH));
        }
        else
        {
            std::cerr << "Unrecognized suffix on filename '"
                << filename
                << "'. plPlan filenames should have suffix .csv, "
                << "plModel filenames should have suffix .stl"
                << std::endl;
            exit(1);
        }
    }
}

plPlan::~plPlan()
{
}

void plPlan::extractRenderComponents(plRenderMap& renderMap, uint32_t technique) const
{
    if (!_isVisible)
        return;

    // Draw defect boundary
    for (uint32_t i = 0; i < _defectSites.size(); i++)
    {
        plPickingStack::loadGreen(i);
        _defectSites[i]->extractRenderComponents(renderMap, technique);
    }

    // Draw donor boundaries
    for (uint32_t i = 0; i < _donorSites.size(); i++)
    {
        plPickingStack::loadGreen(i);
        _donorSites[i]->extractRenderComponents(renderMap, technique);
    }

    // Draw grafts
    for (uint32_t i = 0; i < _grafts.size(); i++)
    {
        plPickingStack::loadGreen(i);
        _grafts[i]->extractRenderComponents(renderMap, technique);
    }

    // draw models
    for (uint32_t i = 0; i < _models.size(); i++)
    {
        plPickingStack::loadRed(PL_PICKING_TYPE_BONE);
        plPickingStack::loadGreen(i);
        plPickingStack::loadBlue(-1); // unused by models
        plColorStack::load(PL_MODEL_COLOR);
        _models[i]->extractRenderComponents(renderMap, technique);
    }
}


void plPlan::extractRenderComponents(plRenderMap& renderMap) const
{
    extractRenderComponents(renderMap, PL_PLAN_TECHNIQUE);
}


void plPlan::addDefectSite(uint32_t modelIndex)
{
    if (_modelIndexErrorCheck("addDefectSite", modelIndex))
    {
        return;
    }
    _defectSites.push_back(std::make_shared<plDefectSite>(_models[modelIndex]->mesh()));
}


void plPlan::addDonorSite(uint32_t modelIndex)
{
    if (_modelIndexErrorCheck("addDonorSite", modelIndex))
    {
        return;
    }
    _donorSites.push_back(std::make_shared<plDonorSite>(_models[modelIndex]->mesh()));
}


void plPlan::addGraft(std::shared_ptr<plPlug>harvest, std::shared_ptr<plPlug>recipient, float32_t radius, float32_t length)
{
    _grafts.push_back(std::make_shared<plGraft>(harvest, recipient, radius, length));
}


void plPlan::removeDefectSite(uint32_t index)
{
    _defectSites.erase(_defectSites.begin() + index);
}


void plPlan::removeDonorSite(uint32_t index)
{
    _donorSites.erase(_donorSites.begin() + index);
}


void plPlan::removeGraft(uint32_t index)
{
    _grafts.erase(_grafts.begin() + index);
}


void plPlan::importFile(const plString &filename)
{
    plCSV csv(filename);

    if (!csv.good())
    {
        std::cerr << "Could not open '" << filename << "'." << std::endl;
        exit(1);
    }

    while (!csv.eof()) {

        auto field = csv.getRow().getCol(0);

        if (field.compareCaseInsensitive("model"))
        {
            // model data
            plString modelFile = csv.getRow().getCol(1);

            std::cout << "Loading model `" << modelFile << "`...";

            // model
            auto model = std::make_shared<plModel>(
                modelFile,
                PL_MODEL_DEFAULT_OCTREE_DEPTH);
            _models.push_back(model);

            std::cout << " Complete." << std::endl;
        }
        else if (field.compareCaseInsensitive("defect_site"))
        {
            std::cout << "Loading defect site " << _defectSites.size() << "...";

            // spline data
            uint32_t splineModelID = std::stoi(csv.getRow().getCol(1));
            auto spline = std::make_shared<plSpline>(
                _models[splineModelID]->mesh(),
                csv.getRow().getCols());

            // defect site
            auto defectSite = std::make_shared<plDefectSite>(
                spline,
                csv.getRow().getCols());
            _defectSites.push_back(defectSite);

            std::cout << " Complete." << std::endl;

        }
        else if (field.compareCaseInsensitive("donor_site"))
        {
            std::cout << "Loading donor site " << _donorSites.size() << "...";

            // boundary data
            uint32_t boundaryModelID = std::stoi(csv.getRow().getCol(1));
            auto boundary = std::make_shared<plBoundary>(
                PL_PICKING_TYPE_DONOR_BOUNDARY,
                _models[boundaryModelID]->mesh(),
                csv.getRow().getCols());

            // donor site
            auto donorSite = std::make_shared<plDonorSite>(boundary);
            _donorSites.push_back(donorSite);

            std::cout << " Complete." << std::endl;

        }
        else if (field.compareCaseInsensitive("graft"))
        {
            std::cout << "Loading graft " << _grafts.size() << "...";

            // recipient data
            uint32_t recipientSiteID = std::stoi(csv.getRow().getCol(1));
            plTransform recipientTransform(csv.getRow().getCols());
            plTransform recipientRotation(csv.getRow().getCols());

            // recipient plug
            auto recipientPlug = std::make_shared<plPlug>(
                _defectSites[recipientSiteID]->spline->surfaceMesh(),
                recipientTransform,
                recipientRotation);

            // harvest data
            uint32_t harvestModelID = std::stoi(csv.getRow().getCol(1));
            plTransform harvestTransform(csv.getRow().getCols());
            plTransform harvestRotation(csv.getRow().getCols());

            // harvest plug
            auto harvestPlug = std::make_shared<plPlug>(
                _models[harvestModelID]->mesh(),
                harvestTransform,
                harvestRotation);

            // graft data
            float32_t radius = std::stof(csv.getRow().getCol(1));
            float32_t length = std::stof(csv.getRow().getCol(1));
            plVector3 markDirection(csv.getRow().getCol(1));

            // graft
            auto graft = std::make_shared<plGraft>(
                harvestPlug,
                recipientPlug,
                radius,
                length,
                markDirection);
            _grafts.push_back(graft);

            std::cout << " Complete." << std::endl;
        }
        else
        {
            std::cerr << "Error in '"
                << filename
                << "': Unrecognized word '"
                << field
                << "' in first column."
                << std::endl;
            exit(1);
        }
    }
}


void plPlan::exportFile(const plString& filename)
{
    std::ofstream out(filename + ".csv");

    if (!out)
    {
        std::cerr << "Could not open '"
            << filename
            << "' to store the plan."
            << std::endl;
    }
    else
    {
        // models
        for (auto model : _models)
        {
            out << "model"               << std::endl
                << "    filename,      " << model->filename  << std::endl
                << std::endl;
        }

        // defect sites
        for (auto defectSite : _defectSites)
        {
            out << "defect_site"                                                     << std::endl
                << "    spline_model_id,   " << getModelIndex(defectSite->spline)    << std::endl
                << "    spline,            " << *defectSite->spline                  << std::endl
                << "    boundary,          " << *defectSite->boundary                << std::endl

                << std::endl;
        }

        // donor sites
        for (auto donorSite : _donorSites)
        {
            out << "donor_site"                                                    << std::endl
                << "    boundary_model_id, " << getModelIndex(donorSite->boundary) << std::endl
                << "    boundary,          " << *donorSite->boundary               << std::endl
                << std::endl;
        }

        // grafts
        for (auto graft : _grafts)
        {
            out << "graft"                                                                     << std::endl
                << "    recipient_defect_site_id, " << _getDefectSiteIndex(graft->recipient()) << std::endl
                << "    recipient_transform,      " << graft->recipient()->surfaceTransform()  << std::endl
                << "    recipient_rotation,       " << graft->recipient()->offsetTransform()   << std::endl
                << "    harvest_model_id,         " << getModelIndex(graft->harvest())         << std::endl
                << "    harvest_transform,        " << graft->harvest()->surfaceTransform()    << std::endl
                << "    harvest_rotation,         " << graft->harvest()->offsetTransform()     << std::endl
                << "    radius,                   " << graft->radius()                         << std::endl
                << "    length,                   " << graft->length()                         << std::endl
                << "    mark_direction,           " << graft->markDirection()                  << std::endl
                << std::endl;
        }

        std::cout << "Exported plan to file: '"
            << filename
            << ".csv'."
            << std::endl;
    }
}


void plPlan::updateGraftMarkerPositions()
{
    for (auto graft : _grafts)
    {
        graft->snapMarkDirection();
    }
}


void plPlan::clear()
{
    _defectSites.clear();
    _donorSites.clear();
    _grafts.clear();
}


int32_t plPlan::getModelIndex(std::shared_ptr<plMeshSpecific> mesh) const
{
    int32_t index = 0;
    for (auto model : _models)
    {
        if (model->mesh() == mesh->mesh())
        {
            return index;
        }
        index++;
    }
    return -1;
}


int32_t plPlan::_getDefectSiteIndex(std::shared_ptr<plMeshSpecific> mesh) const
{
    int32_t index = 0;
    for (auto defectSite : _defectSites)
    {
        if (defectSite->spline->surfaceMesh() == mesh->mesh())
        {
            return index;
        }
        index++;
    }
    return -1;
}


bool plPlan::_modelIndexErrorCheck(const std::string& callingFunction, int32_t modelIndex) const
{
    if (modelIndex == -1)
    {
        std::cerr << "plPlan::" << callingFunction << " error: model has not been selected" << std::endl;
        return true;
    }

    if (_models.size() < static_cast<uint32_t>(modelIndex+1))
    {
        std::cerr << "plPlan::" << callingFunction << " error: model ID does not exist" << std::endl;
        return true;
    }
    return false;
}
