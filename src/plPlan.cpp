#include "plPlan.h"

plPlan::plPlan(plString filename)
{
    if (filename.compare(".csv", filename.size()-4, 4))
    {
        std::cerr << "Unrecognized suffix on filename '" << filename
                  << "'. plPlan filenames should have suffix .csv" << std::endl;
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
            _models.push_back(new plModel(filename, PL_MODEL_DEFAULT_OCTREE_DEPTH));
        }
        else
        {
            std::cerr << "Unrecognized suffix on filename '" << filename
                      << "'. plPlan filenames should have suffix .csv, "
                      << "plModel filenames should have suffix .stl" << std::endl;
            exit(1);
        }
    }
}


plPlan::~plPlan()
{
    for (uint32_t i = 0; i < _defectSites.size(); i++) delete _defectSites[i];
    for (uint32_t i = 0; i < _donorSites.size();  i++) delete _donorSites[i];
    for (uint32_t i = 0; i < _grafts.size();      i++) delete _grafts[i];
    for (uint32_t i = 0; i < _models.size();      i++) delete _models[i];
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
    for (uint32_t i =0; i < _models.size(); i++)
    {
        plPickingStack::loadRed(PL_PICKING_TYPE_BONE);
        plPickingStack::loadGreen(i);
        plPickingStack::loadBlue(-1); // unused by models
        plColourStack::load(PL_MODEL_COLOUR);
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
        return;

    _defectSites.push_back(new plDefectSite(_models[modelIndex]->mesh()));
}


void plPlan::addDonorSite(uint32_t modelIndex)
{
    if (_modelIndexErrorCheck("addDonorSite", modelIndex))
        return;

    _donorSites.push_back(new plDonorSite(_models[modelIndex]->mesh()));
}


void plPlan::addGraft(const plPlug &harvest, const plPlug &recipient, float32_t radius, float32_t length)
{
    _grafts.push_back(new plGraft(harvest, recipient, radius, length));
}


void plPlan::removeDefectSite(uint32_t index)
{
    delete _defectSites[index];
    _defectSites.erase(_defectSites.begin()+index);
}


void plPlan::removeDonorSite(uint32_t index)
{
    delete _donorSites[index];
    _donorSites.erase(_donorSites.begin()+index);
}


void plPlan::removeGraft(uint32_t index)
{
    delete _grafts[index];
    _grafts.erase(_grafts.begin()+index);
}


void plPlan::importFile(const plString &filename)
{
    plCSV csv(filename);

    for (uint32_t i = 0; i < csv.data.size(); i++)
    {
        plString field = csv.data[i][0];    // get field name

        if (field.compareCaseInsensitive("model"))
        {
            plString modelFile (csv.data[++i][1]);

            _models.push_back(new plModel(modelFile, PL_MODEL_DEFAULT_OCTREE_DEPTH));
        }
        else if (field.compareCaseInsensitive("defect_site"))
        {
            std::cout << "Loading defect site...";

            uint32_t     splineModelID  (std::stoi(csv.data[++i][1]));
            plSpline   spline         (_models[splineModelID]->mesh(), csv.data[++i]);

            _defectSites.push_back(new plDefectSite(spline, csv.data[++i]));
            std::cout << "\t\t\t\t\tComplete." << std::endl;

        }
        else if (field.compareCaseInsensitive("donor_site"))
        {
            std::cout << "Loading donor site...";

            uint32_t     boundaryModelID(std::stoi(csv.data[++i][1]));
            plBoundary boundary       (PL_PICKING_TYPE_DONOR_BOUNDARY, _models[boundaryModelID]->mesh(), csv.data[++i]);

            _donorSites.push_back(new plDonorSite(boundary));
            std::cout << "\t\t\t\t\tComplete." << std::endl;

        }
        else if (field.compareCaseInsensitive("graft"))
        {
            std::cout << "Loading graft...";

            uint32_t      recipientSiteID    (std::stoi(csv.data[++i][1]));
            plTransform recipientTransform (csv.data[++i]);
            plTransform recipientRotation  (csv.data[++i]);
            uint32_t      harvestModelID     (std::stoi(csv.data[++i][1]));
            plTransform harvestTransform   (csv.data[++i]);
            plTransform harvestRotation    (csv.data[++i]);
            float32_t     radius             (std::stof(csv.data[++i][1]));
            float32_t     length             (std::stof(csv.data[++i][1]));
            plVector3   markDirection      (csv.data[++i][1]);

            plPlug recipientPlug (_defectSites[recipientSiteID]->spline.surfaceMesh(), PL_PICKING_INDEX_GRAFT_DEFECT, recipientTransform, recipientRotation);
            plPlug harvestPlug   (_models[harvestModelID]->mesh(),                     PL_PICKING_INDEX_GRAFT_DONOR,  harvestTransform,   harvestRotation);

            _grafts.push_back(new plGraft(harvestPlug, recipientPlug, radius, length, markDirection));
            std::cout << "\t\t\t\t\tComplete." << std::endl;
        }
        else
        {
            std::cerr << "Error in '" << filename << "': Unrecognized word '" << field << "' in first column." << std::endl;
        }
    }
}


void plPlan::exportFile(const plString &filename)
{
    std::ofstream out((filename + plString(".csv")).c_str());

    if (!out)
    {
        std::cerr << "Could not open '" << filename << "' to store the plan." << std::endl;
    }
    else
    {
        // models
        for (uint32_t i=0; i<_models.size(); i++)
        {
            out << "model"              << std::endl
                << "    filename,      " << _models[i]->filename  << std::endl
                << std::endl;
        }

        // defect sites
        for (uint32_t i=0; i<_defectSites.size(); i++)
        {
            out << "defect_site"  << std::endl
                << "    spline_model_id,   " << getModelIndex(_defectSites[i]->spline) << std::endl
                << "    spline,            " << _defectSites[i]->spline                   << std::endl
                << "    boundary,          " << _defectSites[i]->boundary                 << std::endl

                << std::endl;
        }

        // donor sites
        for (uint32_t i=0; i<_donorSites.size(); i++)
        {
            out << "donor_site"   << std::endl
                << "    boundary_model_id, " << getModelIndex(_donorSites[i]->boundary) << std::endl
                << "    boundary,          " << _donorSites[i]->boundary                   << std::endl
                << std::endl;
        }

        // grafts
        for (uint32_t i=0; i<_grafts.size(); i++)
        {
            out << "graft"                          << std::endl
                << "    recipient_defect_site_id, " << _getDefectSiteIndex(_grafts[i]->recipient())     << std::endl
                << "    recipient_transform,      " << _grafts[i]->recipient().surfaceTransform()       << std::endl
                << "    recipient_rotation,       " << _grafts[i]->recipient().offsetTransform()        << std::endl
                << "    harvest_model_id,         " << getModelIndex(_grafts[i]->harvest())             << std::endl
                << "    harvest_transform,        " << _grafts[i]->harvest().surfaceTransform()         << std::endl
                << "    harvest_rotation,         " << _grafts[i]->harvest().offsetTransform()          << std::endl
                << "    radius,                   " << _grafts[i]->radius()                             << std::endl
                << "    length,                   " << _grafts[i]->length()                             << std::endl
                << "    mark_direction,           " << _grafts[i]->markDirection()                      << std::endl
                << std::endl;
        }

        std::cout << "Exported plan to file: '" << filename << ".csv'." << std::endl;
    }
}


void plPlan::updateGraftMarkerPositions()
{
    for (plGraft* graft : _grafts)
    {
        graft->snapMarkDirection();
    }
}


void plPlan::clear()
{
    for (uint32_t i = 0; i < _defectSites.size(); i++) delete _defectSites[i];
    for (uint32_t i = 0; i < _donorSites.size();  i++) delete _donorSites[i];
    for (uint32_t i = 0; i < _grafts.size();      i++) delete _grafts[i];
    _defectSites.clear();
    _donorSites.clear();
    _grafts.clear();
}


int32_t plPlan::getModelIndex(const plMeshSpecific& mesh) const
{
    for (uint32_t i = 0; i < _models.size(); i++)
    {
        if (&_models[i]->mesh() == &mesh.mesh())
            return i;
    }
    return -1;
}


int32_t plPlan::_getDefectSiteIndex(const plMeshSpecific& mesh) const
{
    for (uint32_t i = 0; i < _defectSites.size(); i++)
    {
        if (&_defectSites[i]->spline.surfaceMesh() == &mesh.mesh())
            return i;
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
