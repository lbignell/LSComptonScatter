#ifndef SensitiveDetector_h
#define SensitiveDetector_h 1

#include "G4VSensitiveDetector.hh"
#include "G4Run.hh"

using namespace std;

class G4Step;
class G4HCofThisEvent;
class G4TouchableHistory;

class SensitiveDetector : public G4VSensitiveDetector
{

public:
  SensitiveDetector(G4String name);
  ~SensitiveDetector();

  void Initialize(G4HCofThisEvent*HCE);
  G4bool ProcessHits(G4Step*aStep,G4TouchableHistory*ROhist);
  void EndOfEvent(G4HCofThisEvent*HCE);

  //This function will accept an energy, apply Birk's correction for Ionisation
  //quenching, and return the quenched energy.
  G4double ApplyBirksQuench(G4double Energy);
  G4double EvaluateBirks(G4double Energy);

private:
  string LVname;
  unsigned long int runnum;

  //Indexed by track ID.
  //1st bin is Vertex X position, 2nd is Vertex Y, 3rd vertex Z, 4th Energy.
  //G4double SecondaryArray[1000][4];

  //Family tree.
  //Logged by track ID
  //0th argument is the track ID of the parent.
  //1st argument is the particle type (0=Gamma, 1=electron, 2=positron).
  //G4int FamilyTree[1000][2];

  //Events Array.
  //The argument is the trackID of the ancestor that started the interaction.
  //The value stored is the energy deposited by that track and any electron
  //secondaries.
  //G4double Events[1000];
  //G4double DirectGammaInts[1000];

  //Different types of Edep Collections
  G4double EdepThisEventUnquenched;
  //G4double EdepQuenchPerEvent;
  //G4double EdepQuenchPerInteraction;

  //Number of Bremm interactions. Use this as a simple counter for the # of
  //Brem interactions occuring in the scintillant. Pass to RunAction at the end
  //of the event.
  //G4int numInts;
  //G4int numCorrs;
  //G4int Counter;

  G4int TrackID;
  // G4int ParentID;

  //Range fit parameters (9th order polynomial coeffs, lowest to highest order)
  G4double Const;
  G4double SpdOfLight;
  G4double ZonA;
  G4double MeanExEn;
  G4double ElecRestEnergy;
  G4double DensityEffectCorr;
  G4double ScintDensity;
  G4double kB;

  unsigned long long int TotOptPhotons;
  vector< vector< double > > InitEnVec;
  vector< vector< int > > ParentIDVec;
  vector< string > ProcVec;
  vector< double > dblvec;
  vector< int > intvec;

  G4int PhotonID;

};

#endif
