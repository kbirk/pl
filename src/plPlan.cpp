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


plPlan::plPlan(int argc, char **argv)
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
    for (PLuint i = 0; i < _defectSites.size(); i++) delete _defectSites[i];
    for (PLuint i = 0; i < _donorSites.size();  i++) delete _donorSites[i];
    for (PLuint i = 0; i < _grafts.size();      i++) delete _grafts[i];
    for (PLuint i = 0; i < _iGuideSites.size(); i++) delete _iGuideSites[i];
    for (PLuint i = 0; i < _models.size();      i++) delete _models[i];
}


void plPlan::toggleArthroView()
{
    plArthroViewable::toggleArthroView();
    for (plGraft* graft : _grafts)
    {
        graft->toggleArthroView();
    }
    for (plModel* model : _models)
    {
        model->toggleArthroView();
    }
}


void plPlan::extractRenderComponents(plRenderMap& renderMap, PLuint technique) const
{
    if (!_inArthroView)
    {
        if (!_isVisible)
            return;

        // Draw defect boundary
        for (PLuint i = 0; i < _defectSites.size(); i++)
        {
            plPickingStack::loadGreen(i);
            _defectSites[i]->extractRenderComponents(renderMap, technique);
        }

        // Draw donor boundaries
        for (PLuint i = 0; i < _donorSites.size(); i++)
        {
            plPickingStack::loadGreen(i);
            _donorSites[i]->extractRenderComponents(renderMap, technique);
        }

        // Draw iGuideSites
        for (PLuint i = 0; i < _iGuideSites.size(); i++)
        {
            plPickingStack::loadGreen(i);
            _iGuideSites[i]->extractRenderComponents(renderMap, technique);
        }

        // Draw iGuides
        for (PLuint i = 0; i < _iGuides.size(); i++)
        {
            plPickingStack::loadGreen(i);
            _iGuides[i]->extractRenderComponents(renderMap, technique);
        }

        // Draw grafts
        for (PLuint i = 0; i < _grafts.size(); i++)
        {
            plPickingStack::loadGreen(i);
            _grafts[i]->extractRenderComponents(renderMap, technique);
        }

        // draw models
        for (PLuint i =0; i < _models.size(); i++)
        {
            plPickingStack::loadRed(PL_PICKING_TYPE_BONE);
            plPickingStack::loadGreen(i);
            plPickingStack::loadBlue(-1); // unused by models
            plColourStack::load(PL_MODEL_COLOUR);
            _models[i]->extractRenderComponents(renderMap, technique);
        }
    }
    else
    {
        // Draw grafts
        for (PLuint i = 0; i < _grafts.size(); i++)
        {
            plPickingStack::loadGreen(i);
            _grafts[i]->extractRenderComponents(renderMap, technique);
        }

        // draw models
        for (PLuint i =0; i < _models.size(); i++)
        {
            plPickingStack::loadRed(PL_PICKING_TYPE_BONE);
            plPickingStack::loadGreen(i);
            plPickingStack::loadBlue(-1); // unused by models
            _models[i]->extractRenderComponents(renderMap, technique);
        }
    }

}


void plPlan::extractRenderComponents(plRenderMap& renderMap) const
{
    extractRenderComponents(renderMap, PL_PLAN_TECHNIQUE);
}


void plPlan::addDefectSite(PLuint modelIndex)
{
    if (_modelIndexErrorCheck("addDefectSite", modelIndex))
        return;

    _defectSites.push_back(new plDefectSite(_models[ modelIndex ]->mesh()));
}


void plPlan::addDonorSite(PLuint modelIndex)
{
    if (_modelIndexErrorCheck("addDonorSite", modelIndex))
        return;

    _donorSites.push_back(new plDonorSite(_models[ modelIndex ]->mesh()));
}


void plPlan::addIGuideSite(PLuint modelIndex)
{
    if (_modelIndexErrorCheck("addIGuideSite", modelIndex))
        return;

    _iGuideSites.push_back(new plIGuideSite(_models[ modelIndex ]->mesh()));
}


void plPlan::addGraft(const plPlug &harvest, const plPlug &recipient, PLfloat radius, PLfloat length)
{
    _grafts.push_back(new plGraft(harvest, recipient, radius, length));
}


void plPlan::removeDefectSite(PLuint index)
{
    delete _defectSites[index];
    _defectSites.erase(_defectSites.begin()+index);
}


void plPlan::removeDonorSite(PLuint index)
{
    delete _donorSites[index];
    _donorSites.erase(_donorSites.begin()+index);
}


void plPlan::removeIGuideSite(PLuint index)
{
    delete _iGuideSites[index];
    _iGuideSites.erase(_iGuideSites.begin()+index);
    // TODO: add any code to ensure that iGuides remain coherent after removing iguidesites
}


void plPlan::removeGraft(PLuint index)
{
    delete _grafts[index];
    _grafts.erase(_grafts.begin()+index);
}


void plPlan::importFile(const plString &filename)
{
    plCSV csv(filename);

    for (PLuint i = 0; i < csv.data.size(); i++)
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

            PLuint     splineModelID  (std::stoi(csv.data[++i][1]));
            plSpline   spline         (_models[ splineModelID ]->mesh(), csv.data[++i]);

            _defectSites.push_back(new plDefectSite(spline, csv.data[++i]));
            std::cout << "\t\t\t\t\tComplete." << std::endl;

        }
        else if (field.compareCaseInsensitive("donor_site"))
        {
            std::cout << "Loading donor site...";

            PLuint     boundaryModelID(std::stoi(csv.data[++i][1]));
            plBoundary boundary       (PL_PICKING_TYPE_DONOR_BOUNDARY, _models[ boundaryModelID ]->mesh(), csv.data[++i]);

            _donorSites.push_back(new plDonorSite(boundary));
            std::cout << "\t\t\t\t\tComplete." << std::endl;

        }
        else if (field.compareCaseInsensitive("iGuide_site"))
        {
            std::cout << "Loading iGuide site...";

            PLuint     boundaryModelID(std::stoi(csv.data[++i][1]));
            plBoundary boundary       (PL_PICKING_TYPE_IGUIDE_BOUNDARY, _models[ boundaryModelID ]->mesh(), csv.data[++i]);

            _iGuideSites.push_back(new plIGuideSite(boundary));
            std::cout << "\t\t\t\t\tComplete." << std::endl;
        }
        else if (field.compareCaseInsensitive("graft"))
        {
            std::cout << "Loading graft...";

            PLuint      recipientSiteID    (std::stoi(csv.data[++i][1]));
            plTransform recipientTransform (csv.data[++i]);
            plTransform recipientRotation  (csv.data[++i]);
            PLuint      harvestModelID     (std::stoi(csv.data[++i][1]));
            plTransform harvestTransform   (csv.data[++i]);
            plTransform harvestRotation    (csv.data[++i]);
            PLfloat     radius             (std::stof(csv.data[++i][1]));
            PLfloat     length             (std::stof(csv.data[++i][1]));
            plVector3   markDirection      (csv.data[++i][1]);

            plPlug recipientPlug (_defectSites[ recipientSiteID ]->spline.surfaceMesh(), PL_PICKING_INDEX_GRAFT_DEFECT, recipientTransform, recipientRotation);
            plPlug harvestPlug   (_models[ harvestModelID ]->mesh(),                     PL_PICKING_INDEX_GRAFT_DONOR,  harvestTransform,   harvestRotation);

            _grafts.push_back(new plGraft(harvestPlug, recipientPlug, radius, length, markDirection));
            std::cout << "\t\t\t\t\tComplete." << std::endl;
        }
        else if (field.compareCaseInsensitive("iguide"))
        {

            PLuint  siteID    (std::stoi(csv.data[++i][1]));
            PLuint  plugCount (std::stoi(csv.data[++i][1]));

            // get plug info
            std::vector<plPlugInfo> plugs;
            for (PLuint j=0; j<plugCount; j++)
            {
                PLuint  graftID (std::stoi(csv.data[i][2+(j*2)]));
                PLuint  type    (std::stoi(csv.data[i][3+(j*2)]));
                const plPlug  *plug   = &(_grafts[ graftID ]->plug(type));
                const PLfloat *radius = &(_grafts[ graftID ]->radius());
                const PLfloat *length = &(_grafts[ graftID ]->length());
                plugs.push_back(plPlugInfo(plug, radius, length, type, graftID));
            }

            PLuint  kWireCount (std::stoi(csv.data[++i][1]));

            // get kwire info
            std::vector<plKWire*> kWires;
            std::vector<PLuint>   kWireIDs;
            for (PLuint j=0; j<kWireCount; j++)
            {
                // PLuint  kWireID (std::stoi(csv.data[i][2+j]));
                // **write code to add kwires for iguide here**

            }

            PLuint defectSiteCount(std::stoi(csv.data[++i][1]));

            std::vector<const plSpline*> splines;
            std::vector<PLuint>          defectIDs;
            for (PLuint j=0; j<defectSiteCount; j++)
            {
                PLuint  defectID (std::stoi(csv.data[i][2+(j)]));
                defectIDs.push_back(defectID);
                splines.push_back(&(_defectSites[defectID]->spline));
            }

            _iGuides.push_back(new plIGuide(_iGuideSites[ siteID ], siteID, plugs, kWires, kWireIDs, splines, defectIDs));
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
        for (PLuint i=0; i<_models.size(); i++)
        {
            out << "model"              << std::endl
                << "    filename,      " << _models[i]->filename  << std::endl
                << std::endl;
        }

        // defect sites
        for (PLuint i=0; i<_defectSites.size(); i++)
        {
            out << "defect_site"  << std::endl
                << "    spline_model_id,   " << getModelIndex(_defectSites[i]->spline) << std::endl
                << "    spline,            " << _defectSites[i]->spline                   << std::endl
                << "    boundary,          " << _defectSites[i]->boundary                 << std::endl

                << std::endl;
        }

        // donor sites
        for (PLuint i=0; i<_donorSites.size(); i++)
        {
            out << "donor_site"   << std::endl
                << "    boundary_model_id, " << getModelIndex(_donorSites[i]->boundary) << std::endl
                << "    boundary,          " << _donorSites[i]->boundary                   << std::endl
                << std::endl;
        }

        // iGuide sites
        for (PLuint i=0; i<_iGuideSites.size(); i++)
        {
            out << "iguide_site"  << std::endl
                << "    boundary_model_id, " << getModelIndex(_iGuideSites[i]->boundary) << std::endl
                << "    boundary,          " << _iGuideSites[i]->boundary                   << std::endl
                << std::endl;
        }

        // grafts
        for (PLuint i=0; i<_grafts.size(); i++)
        {
            out << "graft"                          << std::endl
                << "    recipient_defect_site_id, " << _getDefectSiteIndex(_grafts[i]->recipient())   << std::endl
                << "    recipient_transform,      " << _grafts[i]->recipient().surfaceTransform()       << std::endl
                << "    recipient_rotation,       " << _grafts[i]->recipient().offsetTransform()        << std::endl
                << "    harvest_model_id,         " << getModelIndex(_grafts[i]->harvest())          << std::endl
                << "    harvest_transform,        " << _grafts[i]->harvest().surfaceTransform()         << std::endl
                << "    harvest_rotation,         " << _grafts[i]->harvest().offsetTransform()          << std::endl
                << "    radius,                   " << _grafts[i]->radius()                             << std::endl
                << "    length,                   " << _grafts[i]->length()                             << std::endl
                << "    mark_direction,           " << _grafts[i]->markDirection()                      << std::endl
                << std::endl;
        }

        // iGuides
        for (PLuint i=0; i<_iGuides.size(); i++)
        {
            out << "iguide"      << std::endl
                << "    iguide_site_id, " << _iGuides[i]->siteID  << std::endl
                << "    plugs,          " << _iGuides[i]->plugs.size();

            for (PLuint j=0; j<_iGuides[i]->plugs.size(); j++)
            {
                out << ", " << _iGuides[i]->plugs[j].graftID() << ", " << _iGuides[i]->plugs[j].type();
            }

            out << std::endl;
            out << "    kwires,         " << _iGuides[i]->kWires.size();

            for (PLuint j=0; j<_iGuides[i]->kWires.size(); j++)
            {
                out << ", " << _iGuides[i]->kWireIDs[j];
            }

            out << std::endl;
            out << "    splines,        " << _iGuides[i]->defectIDs.size();
            for (PLuint j=0; j<_iGuides[i]->defectIDs.size(); j++)
            {
                out << ", " << _iGuides[i]->defectIDs[j];
            }

            out << std::endl << std::endl;
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


void plPlan::generateIGuides()
{
    for (plIGuide* iGuide : _iGuides)
    {
        iGuide->generateIGuideModels();
    }
}


void plPlan::exportIGuides() const
{
    for (plIGuide* iGuide : _iGuides)
    {
        iGuide->exportIGuideModels(PL_FILE_PREPATH"output/");
    }
}


void plPlan::clear()
{
    for (PLuint i = 0; i < _defectSites.size(); i++) delete _defectSites[i];
    for (PLuint i = 0; i < _donorSites.size();  i++) delete _donorSites[i];
    for (PLuint i = 0; i < _grafts.size();      i++) delete _grafts[i];
    for (PLuint i = 0; i < _iGuideSites.size(); i++) delete _iGuideSites[i];
    for (PLuint i = 0; i < _iGuides.size();     i++) delete _iGuides[i];
    _defectSites.clear();
    _donorSites.clear();
    _iGuideSites.clear();
    _grafts.clear();
    _iGuides.clear();
}


PLint plPlan::getModelIndex(const plMeshSpecific& mesh) const
{
    for (PLuint i = 0; i < _models.size(); i++)
    {
        if (&_models[i]->mesh() == &mesh.mesh())
            return i;
    }
    return -1;
}


PLint plPlan::_getDefectSiteIndex(const plMeshSpecific& mesh) const
{
    for (PLuint i = 0; i < _defectSites.size(); i++)
    {
        if (&_defectSites[i]->spline.surfaceMesh() == &mesh.mesh())
            return i;
    }
    return -1;
}


PLbool plPlan::_modelIndexErrorCheck(const std::string& callingFunction, PLint modelIndex) const
{
    if (modelIndex == -1)
    {
        std::cerr << "plPlan::" << callingFunction << " error: model has not been selected" << std::endl;
        return true;
    }

    if (_models.size() < static_cast<PLuint>(modelIndex+1))
    {
        std::cerr << "plPlan::" << callingFunction << " error: model ID does not exist" << std::endl;
        return true;
    }
    return false;
}
