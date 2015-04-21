//
#include "RunAction.hh"
#include "G4Run.hh"
#include "G4SDManager.hh"
//
#include "DetectorConstruction.hh"
#include "G4VSensitiveDetector.hh"
#include <math.h>
#include "TH1D.h"
#include "TFile.h"
#include <sstream>
#include "TTree.h"
#include "TROOT.h"

RunAction::RunAction(DetectorConstruction* DC){
  //
  
  //take the DetectorConstruction pointer given when this object is created (in main) and copy to local member
  myDC = DC;
  //
  FileName = "Edep.root";  
}

RunAction::~RunAction(){

}

void RunAction::SetFileName(string fname){
  cout << "Setting FileName to : " << fname << endl;
  FileName = fname;
}

void RunAction::BeginOfRunAction(const G4Run* aRun){

  //Arguments:Name of file,type of rewrite option(UPDATE means append),comments.
  RootOP = new TFile(FileName.c_str(),"RECREATE","Compton Scatter Sim Output");
    
  EdepTree = new TTree("Results", "Tree of results from Compton Scatter sim.");
  EdepTree->Branch("Edep", &Edep);

}

void RunAction::TallyEvtData(double theEdep){
  //Set the branch values and fill the tree.
  Edep = theEdep;
  EdepTree->Fill();
}

//task to be carried out at the end of the run
void RunAction::EndOfRunAction(const G4Run* aRun){
  //get the number of primary particles being simulated for this run
  G4double NumberOfEvents = aRun->GetNumberOfEventToBeProcessed();
 
  RootOP->Write();
  RootOP->Close();
}
