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
* 1 : kill layer 5 in one quadrant (-pi to 0) +5% random module loss 
* 2 : kill layer 1 in one quadrant (-pi to 0) +5% random module loss
* 3 : kill layer 1 + layer 2, both in same quadrant
* 4 : kill layer 1 and disk 1, both in same quadrant
* 5 : 5% random module loss.

Note : scenarios 1-4 correspond to those listed on slide 12 of https://indico.cern.ch/event/719985/contributions/2970687/attachments/1634587/2607365/StressTestTF-Acosta-Apr18.pdf

Note : "5% random module loss" means that 5% of modules are chosen at the start of the CMSSW job, and the same modules remain dead in each event

To determine whether a stub should be killed, you can call the killStub method:

```
bool killStub( const TTStub<Ref_Phase2TrackerDigi_>* stub );
```

For scenarios where the same tracker modules are dead in evert event, you can access the DetId of the dead modules with:

```
vector<DetId> getListOfDeadModules()
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
        const double fractionOfStubsToKillInLayers,
        const double fractionOfStubsToKillEverywhere
    );
```
e.g. you can kill X% of stubs throughout the tracker randomly in each event, or kill X% of stubs in one quadrant randomly etc.