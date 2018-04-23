#include "DataFormats/L1TrackTrigger/interface/TTTypes.h"
#include "Geometry/TrackerGeometryBuilder/interface/TrackerGeometry.h"
#include "DataFormats/TrackerCommon/interface/TrackerTopology.h"
#include "Geometry/TrackerGeometryBuilder/interface/PixelGeomDetUnit.h"
#include "Geometry/CommonTopologies/interface/PixelTopology.h"
#include "TRandom.h"

using namespace std;

// layerToKill - which layer we are killing stubs in.
// Barrel layers are encoded as 1-6. The endcap layers are encoded as 11-15 and 21-25
// To kill stubs in all layers, set layerToKill == -1, however min/max phi/z will still be checked to decide whether the stub is killed or not
// min/max Phi/Z - stubs within the region specified by these boundaries can be flagged for killing
// fractionToKill - The fraction of stubs to kill.  An indentical stub in different events could be killed in one event but not the other

bool killStub( const TTStub<Ref_Phase2TrackerDigi_>* stub,
		const TrackerTopology* trackerTopology, 
		const TrackerGeometry* trackerGeometry, 
		const int layerToKill,
		const int minPhiToKill,
		const int maxPhiToKill,
		const int minZToKill,
		const int maxZToKill,
		const double fractionToKill = 1
	) {

	DetId stackDetid = stub->getDetId();
	DetId geoDetId(stackDetid.rawId() + 1);

	// If we are killing stubs in all layers or a specific layer
	if ( layerToKill > 0 ) {
		// Get the layer the stub is in, and check if it's in the layer you want to kill
		bool isInBarrel = geoDetId.subdetId()==StripSubdetector::TOB || geoDetId.subdetId()==StripSubdetector::TIB;

		int layerID = 0;
		if (isInBarrel) {
		  layerID = trackerTopology->layer( geoDetId );
		} else {
		  layerID = 10*trackerTopology->side( geoDetId ) + trackerTopology->tidWheel( geoDetId );
		}

		if ( layerID != layerToKill ) return false;		
	}


	// Get the phi and z of stub, and check if it's in the region you want to kill
	const GeomDetUnit* det0 = trackerGeometry->idToDetUnit( geoDetId );
	const PixelGeomDetUnit* theGeomDet = dynamic_cast< const PixelGeomDetUnit* >( det0 );
	const PixelTopology* topol = dynamic_cast< const PixelTopology* >( &(theGeomDet->specificTopology()) );
	MeasurementPoint measurementPoint = stub->getClusterRef(0)->findAverageLocalCoordinatesCentered();
	LocalPoint clustlp   = topol->localPosition(measurementPoint);
	GlobalPoint pos  =  theGeomDet->surface().toGlobal(clustlp);

	if ( pos.phi() > minPhiToKill && pos.phi() < maxPhiToKill &&
		pos.z() > minZToKill && pos.z() < maxZToKill ) {

		// Kill fraction of stubs
		static TRandom randomGenerator;
		double randomNumber = randomGenerator.Rndm();
		if ( randomGenerator.Rndm() < fractionToKill ) {
			return true;
		}
		else {
			return false;
		}
	}
	else{
		return false;	
	}
}
