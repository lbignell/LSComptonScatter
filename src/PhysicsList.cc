//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
// PhysicsList.cc
// See more at: http://g4advancedexamples.lngs.infn.it/Examples/hadrontherapy

// This class provides all the physic models that can be activated inside ;
// Each model can be setted via macro commands;
// Inside  the models can be activate with three different complementar methods:
//
// 1. Use of the *Packages*.
//    Packages (that are contained inside the
//    Geant4 distribution at $G4INSTALL/source/physics_lists/lists) provide a full set
//    of models (both electromagnetic and hadronic).
//    The User can use this method simply add the line /physic/addPackage <nameOfPackage>
//    in his/her macro file. No other action is required.
//    For  applications we suggest the use of the QGSP_BIC package
//    for proton beams. The same can be used
//    also for ligth ion beam.
//    Example of use of package can be found in the packageQGSP_BIC.mac file.
//
// 2. Use of the *Physic Lists*.
//    Physic lists are also already ready to use inside the Geant4 distribution
//    ($G4INSTALL/source/physics_lists/builders). To use them the simple
//    /physic/addPhysics <nameOfPhysicList> command must be used in the macro.
//    In  we provide physics list to activate Electromagnetic,
//    Hadronic elastic and Hadronic inelastic models.
//
//    For  we suggest the use of:
//
//    /physic/addPhysic/emstandard_option3 (electromagnetic model)
//    /physic/addPhysic/QElastic (hadronic elastic model)
//    /physic/addPhysic/binary (hadronic inelastic models for proton and neutrons)
//    /physic/addPhysic/binary_ion (hadronic inelastic models for ions)
//
//    Example of the use of physics lists can be found in the macro files included in the
//    'macro' folder .
//
// 3. Use of a *local* physics. In this case the models are implemented in local files
//    contained in the  folder. The use of local physic is recommended
//    to more expert Users.
//    We provide as local, only the LocalStandardICRU73EmPhysic.cc (an Elecromagnetic
//    implementation containing the new ICRU73 data table for ions stopping powers)
//    and the LocalIonIonInelasticPhysic.cc (physic list to use for the ion-ion interaction
//    case)
//    The *local* physics can be activated with the same /physic/addPhysic <nameOfPhysic> command;
//
//    While Packages approch must be used exclusively, Physics List and Local physics can
//    be activated, if necessary, contemporaneously in the same simulation run.
//
//    AT MOMENT, IF ACCURATE RESULTS ARE NEDED, WE STRONGLY RECOMMEND THE USE OF THE MACROS:
//    proton_therapy.mac: use of the built-in Geant4 physics list for proton beams)
//    ion_therapy.mac   : use of mixed combination of native Geant4 physic lists
//                        and local physic for ion-ion enelastic processes)

#include "PhysicsList.hh"
#include "PhysicsListMessenger.hh"
//#include "StepMax.hh"
#include "G4PhysListFactory.hh"
#include "G4VPhysicsConstructor.hh"
#include "G4SystemOfUnits.hh"

// Physic lists (contained inside the Geant4 distribution)
#include "G4EmStandardPhysics_option3.hh"
#include "G4EmLivermorePhysics.hh"
#include "G4EmPenelopePhysics.hh"
#include "G4DecayPhysics.hh"
#include "G4Decay.hh"
#include "G4EmExtraPhysics.hh"
#include "G4StoppingPhysics.hh"

#include "G4LossTableManager.hh"
#include "G4UnitsTable.hh"
#include "G4ProcessManager.hh"

#include "G4IonFluctuations.hh"
#include "G4IonParametrisedLossModel.hh"
#include "G4EmProcessOptions.hh"

#include "G4RadioactiveDecayPhysics.hh"
#include "G4GenericIon.hh"

/////////////////////////////////////////////////////////////////////////////
PhysicsList::PhysicsList() : G4VModularPhysicsList()
{
  //G4LossTableManager::Instance();
  defaultCutValue = 1.*mm;
  cutForGamma     = defaultCutValue;
  cutForElectron  = defaultCutValue;
  cutForPositron  = defaultCutValue;

  helIsRegisted  = false;
  bicIsRegisted  = false;
  biciIsRegisted = false;
  locIonIonInelasticIsRegistered = false;
  radioactiveDecayIsRegisted = false;

  SetVerboseLevel(1);

}

/////////////////////////////////////////////////////////////////////////////
PhysicsList::~PhysicsList()
{
  delete emPhysicsList;
  delete decPhysicsList;
}
/////////////////////////////////////////////////////////////////////////////
void PhysicsList::ConstructParticle()
{
  decPhysicsList = new G4DecayPhysics();
  decPhysicsList->ConstructParticle();

  //Required for multithreading...
  G4GenericIon::GenericIonDefinition();
}
/////////////////////////////////////////////////////////////////////////////
void PhysicsList::ConstructProcess()
{
  // transportation
  //
  AddTransportation();

  //emPhysicsList = new G4EmLivermorePhysics();
  emPhysicsList = new G4EmPenelopePhysics();

  // Deacy physics and all particles
  decPhysicsList = new G4DecayPhysics();

  // electromagnetic physics list
  //
  emPhysicsList->ConstructProcess();
  em_config.AddModels();

  // decay physics list
  //
  decPhysicsList->ConstructProcess();
}

/////////////////////////////////////////////////////////////////////////////
void PhysicsList::SetCuts()
{

  if (verboseLevel >0){
    G4cout << "PhysicsList::SetCuts:";
    G4cout << "CutLength : " << G4BestUnit(defaultCutValue,"Length") << G4endl;
  }

  // set cut values for gamma at first and for e- second and next for e+,
  // because some processes for e+/e- need cut values for gamma
  SetCutValue(0.1*mm, "gamma");
  SetCutValue(0.1*mm, "e-");
  SetCutValue(0.1*mm, "e+");
  SetCutValue(0.1*mm, "proton");

  if (verboseLevel>0) DumpCutValuesTable();
}
/////////////////////////////////////////////////////////////////////////////
void PhysicsList::SetCutForGamma(G4double cut)
{
  cutForGamma = cut;
  SetParticleCuts(cutForGamma, G4Gamma::Gamma());
}
/////////////////////////////////////////////////////////////////////////////
void PhysicsList::SetCutForElectron(G4double cut)
{
  cutForElectron = cut;
  SetParticleCuts(cutForElectron, G4Electron::Electron());
}
/////////////////////////////////////////////////////////////////////////////
void PhysicsList::SetCutForPositron(G4double cut)
{
  cutForPositron = cut;
  SetParticleCuts(cutForPositron, G4Positron::Positron());
}
/////////////////////////////////////////////////////////////////////////////
