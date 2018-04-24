#include "DataFormats/L1TrackTrigger/interface/TTTypes.h"
#include "Geometry/TrackerGeometryBuilder/interface/TrackerGeometry.h"
#include "DataFormats/TrackerCommon/interface/TrackerTopology.h"
#include "Geometry/TrackerGeometryBuilder/interface/PixelGeomDetUnit.h"
#include "Geometry/CommonTopologies/interface/PixelTopology.h"
#include "TRandom.h"

using namespace std;

// layersToKill - a vector stating the layers we are killing stubs in.  Can be an empty vector.
// Barrel layers are encoded as 1-6. The endcap layers are encoded as 11-15 (-z) and 21-25 (+z)
// min/max Phi/Z/R - stubs within the region specified by these boundaries and layersToKill are flagged for killing
// fractionToKillInLayers - The fraction of stubs to kill in the specified layers/region.
// fractionToKillEverywhere - The fraction of stubs to kill throughout the tracks

bool killStub( const TTStub<Ref_Phase2TrackerDigi_>* stub,
		const TrackerTopology* trackerTopology, 
		const TrackerGeometry* trackerGeometry, 
		const vector<int> layersToKill,
		const int minPhiToKill,
		const int maxPhiToKill,
		const int minZToKill,
		const int maxZToKill,
		const int minRToKill,
		const int maxRToKill,
		const double fractionToKillInLayers = 1,
		const double fractionToKillEverywhere = 0
	) {

	// Only kill stubs in specified layers
	if ( layersToKill.size() > 0 ) {
		// Get the layer the stub is in, and check if it's in the layer you want to kill
		DetId stackDetid = stub->getDetId();
		DetId geoDetId(stackDetid.rawId() + 1);

		bool isInBarrel = geoDetId.subdetId()==StripSubdetector::TOB || geoDetId.subdetId()==StripSubdetector::TIB;

		int layerID = 0;
		if (isInBarrel) {
		  layerID = trackerTopology->layer( geoDetId );
		} else {
		  layerID = 10*trackerTopology->side( geoDetId ) + trackerTopology->tidWheel( geoDetId );
		}

		if ( find(layersToKill.begin(), layersToKill.end(), layerID ) != layersToKill.end() ) {
			// Get the phi and z of stub, and check if it's in the region you want to kill
			const GeomDetUnit* det0 = trackerGeometry->idToDetUnit( geoDetId );
			const PixelGeomDetUnit* theGeomDet = dynamic_cast< const PixelGeomDetUnit* >( det0 );
			const PixelTopology* topol = dynamic_cast< const PixelTopology* >( &(theGeomDet->specificTopology()) );
			MeasurementPoint measurementPoint = stub->getClusterRef(0)->findAverageLocalCoordinatesCentered();
			LocalPoint clustlp   = topol->localPosition(measurementPoint);
			GlobalPoint pos  =  theGeomDet->surface().toGlobal(clustlp);

			if ( pos.phi() > minPhiToKill && pos.phi() < maxPhiToKill &&
				pos.z() > minZToKill && pos.z() < maxZToKill &&
				pos.perp() > minRToKill && pos.perp() < maxRToKill ) {

				// Kill fraction of stubs
				static TRandom randomGenerator;
				double randomNumber = randomGenerator.Rndm();
				if ( randomGenerator.Rndm() < fractionToKillInLayers ) {
					return true;
				}
			}
		}
	}

	// Kill fraction of stubs throughout tracker
	if ( fractionToKillEverywhere > 0 ) {
		static TRandom randomGenerator;
		double randomNumber = randomGenerator.Rndm();
		if ( randomGenerator.Rndm() < fractionToKillEverywhere ) {
			return true;
		}
	}

	return false;
}
