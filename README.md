# StubKiller

## Checkout
git clone git@github.com:EmyrClement/StubKiller.git

## Description
A function to flag (i.e. returns a bool) whether a stub should be killed.

```
bool killStub( const TTStub<Ref_Phase2TrackerDigi_>* stub,
		const TrackerTopology* trackerTopology, 
		const TrackerGeometry* trackerGeometry, 
		const int layerToKill,
		const int minPhiToKill,
		const int maxPhiToKill,
		const int minZToKill,
		const int maxZToKill,
		const double fractionToKill = 1
	)
```

In addition to passing the CMSSW objects (the TTStub you are considering, and the TrackerTopology and TrackerGeometry), you also need to specify the following arguments:
* layerToKill
Which layer to kill stubs in.  If set to -1, then all stubs are considered regardless of which layer they are in.  Otherwise the layer numbering was chosen to match that used in TMTT:
 * 1-6 for the barrel layers
 * 11-15 for -z endcap
 * 21-25 for +z endcap
* minPhiToKill etc.
Specify the region in phi and z, either in layer specified by layerToKill or all layers, to kill stubs.
* fractionToKill
Specify fraction of stubs to kill in the region you've specified i.e. 1 = kill all stubs, 0.05 = kill 5% of stubs.  The default is to kill all stubs in the specified region.

## Examples
* Kill all stubs in one quadrant of layer 5
```
      bool kill = killStub( p_ttstub, 
        trackerTopology, 
        trackerGeometry, 
        11,
        0,
        TMath::PiOver2(),
        -1000,
        0,
        1
         );
```

* Kill 5% of stubs in all layers
```
      bool kill = killStub( p_ttstub, 
        trackerTopology, 
        trackerGeometry, 
        -1,
        -1.0 * TMath::Pi(),
        TMath::Pi(),
        -1000,
        1000,
        0.05
         );
```
Note : you could also set minPhiToKill and maxPhiToKill etc. to large dummy values like -100 and 100 just to be "safe"
