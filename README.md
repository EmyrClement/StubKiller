# StubKiller

## Checkout
git clone git@github.com:EmyrClement/StubKiller.git

## Description
A class which can be used to decide whether a stub should be killed.  The functions in the class return a bool, which you can then use to remove stubs i.e. the class does not actually kill the stubs, it just flags those that should be killed.

### Example usage
The constructor for the class doesn't take any arguments:
```
  StubKiller();
```

You must then call the initialise method once you have access to the TrackerTopology and TrackerGeometry objects in your CMSSW code.  This will also set the "kill scenario" you are considering:

```
  void initialise( unsigned int killScenario, const TrackerTopology* trackerTopology, const TrackerGeometry* trackerGeometry );

```

The "killScenario" options are :

* 0 : kill nothing
* 1 : kill one quadrant (-pi to 0) of layer 5, and 5% of stubs throughout tracker
* 2 : kill one quadrant (-pi to 0) of layer 1, and 5% of stubs throughout tracker
* 3 : kill one quadrant (-pi to 0) of layers 1 & 2, and 5% of stubs throughout tracker
* 4 : kill one quadrant (-pi to 0) of layer 1 & disk 1, and 5% of stubs throughout tracker


To determine whether a stub should be killed, you can call the killStub method:

```
bool killStub( const TTStub<Ref_Phase2TrackerDigi_>* stub );
```


### Expert usage

There is also a more complicated killStub function, which you can use to test scenarios not possible with those listed above:
```
bool killStub(
        const TTStub<Ref_Phase2TrackerDigi_>* stub,
        const vector<int> layersToKill,
        const int minPhiToKill,
        const int maxPhiToKill,
        const int minZToKill,
        const int maxZToKill,
        const int minRToKill,
        const int maxRToKill,
        const double fractionToKillInLayers,
        const double fractionToKillEverywhere
    );
```
