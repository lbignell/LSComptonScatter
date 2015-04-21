//once again, fairly simple - copied off the UOW tute
#include "PrimaryGeneratorAction.hh"
#include "TRandom3.h"
#include "TROOT.h"

//include class definition for the particle gun
#include "G4ParticleGun.hh"
#include "G4SystemOfUnits.hh"

//include header file for particle 
//#include "G4Proton.hh"
//#include "G4Electron.hh"
#include "G4Gamma.hh"
#include "Randomize.hh"
#include "G4Event.hh"
#define _USE_MATH_DEFINES
#include <math.h>

using namespace std;

PrimaryGeneratorAction::PrimaryGeneratorAction(){

  gun = new G4ParticleGun(1);
  gun->SetParticleDefinition(G4Gamma::GammaDefinition());
  G4double BeamEn = 0.662*MeV;
  gun->SetParticleEnergy(BeamEn);
  

  G4double X = -10*cm;
    //beamsigma*sqrt(-2*log(G4UniformRand()))*cos(2*M_PI*G4UniformRand());
  //G4double Y = 0;  //y = vertical direction; x = horizontal direction.
    //beamsigma*sqrt(-2*log(G4UniformRand()))*cos(2*M_PI*G4UniformRand());
  
  gun->SetParticlePosition(G4ThreeVector(X,0,0));

  gun->SetParticleMomentumDirection(G4ThreeVector(1,0,0));
  //momentum directional cosines


}

PrimaryGeneratorAction::~PrimaryGeneratorAction(){
//free the dynamically allocated memory
  delete gun;

}

//this method will be called by the RunManager at the beginning of each particle history
void PrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent){

  gun->SetParticleDefinition(G4Gamma::GammaDefinition());
  G4double BeamEn = 0.662*MeV;
  gun->SetParticleEnergy(BeamEn);
  

  G4double X = -10*cm;
    //beamsigma*sqrt(-2*log(G4UniformRand()))*cos(2*M_PI*G4UniformRand());
  //G4double Y = 0;  //y = vertical direction; x = horizontal direction.
    //beamsigma*sqrt(-2*log(G4UniformRand()))*cos(2*M_PI*G4UniformRand());
  
  gun->SetParticlePosition(G4ThreeVector(X,0,0));

  gun->SetParticleMomentumDirection(G4ThreeVector(1,0,0));
  //momentum directional cosines


  //this gets the particle firing
  gun->GeneratePrimaryVertex(anEvent);

}
