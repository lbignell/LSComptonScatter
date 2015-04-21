#include "SensitiveDetector.hh"
#include "G4Step.hh"

#include "G4HCofThisEvent.hh"
#include "G4TouchableHistory.hh"
#include "G4SystemOfUnits.hh"

#include "RunAction.hh"
#include "G4RunManager.hh"
#include "G4Run.hh"
#include "G4SteppingManager.hh"
#include <iterator>
#include "G4TrackVector.hh"
#include "G4SDManager.hh"

//Called at the beginning of each new session.
SensitiveDetector::SensitiveDetector(G4String name)
  : G4VSensitiveDetector(name){
  //Get name of LV corresponding to this SD.
  LVname = name;
  runnum = 0;
}

SensitiveDetector::~SensitiveDetector(){;}

//Called at the beginning of each event
void SensitiveDetector::Initialize(G4HCofThisEvent* HCE){

  TrackID = 0;
  EdepThisEventUnquenched = 0;

  //Set constants for Stopping Power calculation
  Const = (0.15353745)*((MeV*cm2)/g);
  SpdOfLight = (2.99792458*pow(10.,10.))*(cm/s);
  ElecRestEnergy = 0.511003*MeV;
  DensityEffectCorr = 0.;

  dblvec.clear();
  intvec.clear();
  InitEnVec.clear();
  ParentIDVec.clear();
  ProcVec.clear();

  for(int i = 0; i<3; i++){
    //G4cout << "Initialising vector dimensions" << G4endl;
    InitEnVec.push_back(dblvec);
    ParentIDVec.push_back(intvec);
  }

}

//This method is invoked by G4SteppingManager when a step is composed in the
//G4LogicalVolume which has the pointer to this sensitive detector. The first
//argument of this method is a G4Step  object of the current step.
G4bool SensitiveDetector::ProcessHits(G4Step* theStep, G4TouchableHistory*){
  
  //Need to add in alternate Edep collection, simple Edep per step.
  EdepThisEventUnquenched += theStep->GetTotalEnergyDeposit()*MeV;

  return true;  
}


/*This method is invoked at the end of each event. The argument of this method is the same object as the previous method. Hits collections occasionally created in your sensitive detector can be associated to the G4HCofThisEvent object.*/
void SensitiveDetector::EndOfEvent(G4HCofThisEvent*){
  //get run action pointer
  RunAction* myRunAction = (RunAction*)(G4RunManager::GetRunManager()->GetUserRunAction());
  if(myRunAction){
    myRunAction->TallyEvtData(EdepThisEventUnquenched);
  }
}

G4double SensitiveDetector::ApplyBirksQuench(G4double En){
  //method to get the ionisation quench correction.

  //I can get the Mean Ionisation Potential in the following ways:
  //1.If I'm happy with the way G4 calculates this (see web) then I can get it
  //  via theStep()->GetTrack()->GetMaterial()->GetIonisation()
  //  ->GetMeanExcitationEnergy() method.
  //2.Define the Mean Excitation Energy as an entry in the Materials Property
  //  Table which can be accessed via theStep->GetTrack()->GetMaterial()
  //  ->GetMaterialPropertiesTable()->GetConstProperty(ref) method.
  //  This would also be a convenient way of getting kB.
  //3.Calculate it myself here.
  //4.Define it here as a number which will need to be changed depending on the
  //  scintillant used.

  //Going to go with Option 2. on advice from Li.
  //Code is implemented in ProcessHits.

  //Need to evaluate integral by either Simpson's rule or Trapezoidal rule
  //Take approximation over 10000 points.

  if(En!=0){
    G4int numPoints = 1000;
    G4double SumPart = 0;
    for(int i = 1; i<(numPoints); i++){
      SumPart += EvaluateBirks((En*i)/numPoints);
    }

    G4double BraketedBit = (((EvaluateBirks(En)+EvaluateBirks(0.))/2)+SumPart);
    G4double QuenchIntegral = BraketedBit/numPoints;
    G4double QuenchedEnergy = En*QuenchIntegral;

    //Then return the quenched energy.
    return QuenchedEnergy;
  }
  else{return 0;}
}

G4double SensitiveDetector::EvaluateBirks(G4double En){
  if(En!=0){
    G4bool Interp = false;
    G4double EnOrig;
    if(En<0.1*keV){
      EnOrig = En;
      En = 0.1*keV;
      Interp = true;
    }
    
    //Get necessary constants sorted out.
    G4double BetaSquared = 1 - pow((ElecRestEnergy/(ElecRestEnergy+En)),2);
    G4double Tau = (En/ElecRestEnergy);
    G4double Fminus = (1-BetaSquared)*(1+((Tau*Tau)/8)-((2*Tau+1)*log(2)));
    
    //Calculate the stopping power
    G4double dEdX = ScintDensity*Const*(1/BetaSquared)*ZonA*(log(pow((En/MeanExEn),2))+log(1+Tau/2)+Fminus-DensityEffectCorr);
    
    if(Interp==true){
      //Interpolate to zero for < 100eV particles.
      dEdX = dEdX*(EnOrig/(0.1*keV));
    }

    //Apply the basic Birk's formula
    G4double BirksCorr = (1/(1+kB*dEdX));

    return BirksCorr;  
  }
  else{return 0;}
}
