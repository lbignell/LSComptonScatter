#include "DetectorConstruction.hh"
//include header files for all classes used in the methods
#include "globals.hh"
#include "G4Element.hh"
#include "G4Material.hh"
#include "G4PVPlacement.hh"
#include "G4LogicalVolume.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4MaterialPropertiesTable.hh"
#include "G4Colour.hh"
#include "G4VisAttributes.hh"
#include "G4UnionSolid.hh"
#include "G4LogicalSkinSurface.hh"
#include "G4OpticalSurface.hh"
#include "G4RotationMatrix.hh"
#include "G4VSolid.hh"
#include "G4SubtractionSolid.hh"
#include "G4Isotope.hh"
#include "G4SystemOfUnits.hh"
#include <math.h>
#include <stdlib.h>
#include <stdio.h>

//for sensitive detector definition
#include "SensitiveDetector.hh"
#include "G4SDManager.hh"

#include "G4NistManager.hh"
#include "G4RunManager.hh"

#include "G4GeometryManager.hh"
#include "G4PhysicalVolumeStore.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4SolidStore.hh"

#include "G4LogicalBorderSurface.hh"
#include "G4RegionStore.hh"

using namespace std;

//constructor / destructor do nothing
DetectorConstruction::DetectorConstruction(){

}

DetectorConstruction::~DetectorConstruction(){ 

}

G4VPhysicalVolume* DetectorConstruction::Construct(){
/* materials definition */

/*define the elements that will be used in our materials*/
//define hydrogen 
  G4double A = 1.01 * g/mole;
  G4double Z = 1;
  G4Element* elH = new G4Element ("Hydrogen", "H", Z, A);

  //define oxygen
  A = 16.0 * g/mole;
   Z = 8;
  G4Element* elO = new G4Element ("Oxygen", "O", Z, A);

  //define nitrogen
  A = 14.0 * g/mole;
  Z = 7;
  G4Element* elN = new G4Element("Nitrogen", "N", Z, A);

  //define carbon
  A = 12.0107 * g/mole;
  Z = 6;
  G4Element* elC = new G4Element("Carbon", "C", Z, A);

  //Define Fluorine
  A = 18.9984 * g/mole;
  Z = 9;
  G4Element* elF = new G4Element("Fluorine", "F", Z, A);

  //define Silicon
  A = 28.086 * g/mole;
  Z = 14;
  G4Element* elSi = new G4Element("Silicon", "Si", Z, A);

  //define sodium
  A = 22.990 * g/mole;
  Z = 11;
  G4Element* elNa = new G4Element("Sodium", "Na", Z, A);

  //define Calcium
  A = 40.08 * g/mole;
  Z = 20;
  G4Element* elCa = new G4Element("Calcium", "Ca", Z, A);

  //define Phosphorus
  A = 30.973761*g/mole;
  Z = 15;
  G4Element* elP = new G4Element("Phosphorus", "P", Z, A);

  //define Sulphur
  A = 32.065*g/mole;
  Z = 16;
  G4Element* elS = new G4Element("Sulphur", "S", Z, A);

  //define Aluminium
  A = 26.961538*g/mole;
  Z = 13;
  G4Element* elAl = new G4Element("Aluminium", "Al",Z,A);

  //define Iron
  A = 55.845*g/mole;
  Z = 26;
  G4Element* elFe = new G4Element("Iron", "Fe",Z,A);
 
  //Define Copper
  A = 63.546*g/mole;
  Z = 29;
  G4Element* elCu = new G4Element("Copper", "Cu",Z,A);

  //Define Manganese
  A = 54.938045*g/mole;
  Z = 25;
  G4Element* elMn = new G4Element("Manganese", "Mn", Z, A);

  //Define Magnesium
  A = 24.3050*g/mole;
  Z = 12;
  G4Element* elMg = new G4Element("Magnesium", "Mg", Z, A);

  //Define Titanium
  A = 47.867*g/mole;
  Z = 22;
  G4Element* elTi = new G4Element("Titanium", "Ti", Z, A);

  //Define Chromium
  A = 51.9961*g/mole;
  Z = 24;
  G4Element* elCr = new G4Element("Chromium", "Cr",Z,A);

  //Define Zinc
  A = 65.409*g/mole;
  Z = 30;
  G4Element* elZn = new G4Element("Zinc", "Zn",Z,A);

  //Define Boron
  A = 10.811*g/mole;
  Z = 5;
  G4Element* elB = new G4Element("Boron", "B",Z,A);

  //Define Potassium
  A = 39.0983*g/mole;
  Z = 19;
  G4Element* elK = new G4Element("Potassium", "K",Z,A);

  //constructor of the G4Material class requires arguments: string 
  //conG4OpticalSurfacetaining name of material, density, number of elements
  G4Material* water = new G4Material("water", 1.0 * g/cm3, 2);
  water->AddElement(elH,2);
  water->AddElement(elO,1);

  /*now we define air for the world volume*/
  G4Material* air = new G4Material("dry air", 0.01*mg/cm3, 2, kStateGas, 293*kelvin, 1*atmosphere);
  //we can also specify the percentage (by mass) composition
  air->AddElement(elN, 75*perCent);
  air->AddElement(elO, 25*perCent);

  //High Density Polyethylene
  //G4Material* HDPE = new G4Material("HD Poly-Ethylene", 0.941*g/cm3, 2);
  //G4Material* HDPE = new G4Material("HD Poly-Ethylene", 0.93*g/cm3, 2);
  G4Material* HDPE = new G4Material("HD Poly-Ethylene", 0.97*g/cm3, 2);
  HDPE->AddElement(elC, 33.33333*perCent);
  HDPE->AddElement(elH, 66.66667*perCent);

  // Get nist material manager
  G4NistManager* nist = G4NistManager::Instance();

  G4Material* water_nist = nist->FindOrBuildMaterial("G4_WATER");
  G4Material* air_nist = nist->FindOrBuildMaterial("G4_AIR");
  G4Material* HDPE_nist = nist->FindOrBuildMaterial("G4_POLYETHYLENE");
  G4Material* Polystyrene_nist = nist->FindOrBuildMaterial("G4_POLYSTYRENE");
  G4Material* Acrylic_nist = nist->FindOrBuildMaterial("G4_POLYACRYLONITRILE");
  G4Material* glass_nist = nist->FindOrBuildMaterial("G4_Pyrex_Glass");
  G4Element* C_nist = nist->FindOrBuildElement("C");
  G4Element* H_nist = nist->FindOrBuildElement("H");
  G4Material* PFTE_white_nist = nist->FindOrBuildMaterial("G4_TEFLON");
  G4Material* PFTE_black_nist = nist->FindOrBuildMaterial("G4_TEFLON");

  G4Material* PFTE_black = new G4Material("Teflon", 2.2*g/cm3, 2);
  PFTE_black->AddElement(elC, 2);
  PFTE_black->AddElement(elF, 6);

  G4Material* Scint = new G4Material("LABscintillator", 0.86*g/cm3, 2);
  //Scint->AddElement(C_nist, (18/(18+30)));
  //Scint->AddElement(H_nist, (30/(18+30)));
  Scint->AddElement(C_nist, 18*12);
  Scint->AddElement(H_nist, 30*1);


  //Make WbLS as used in expt.
  //G4double WbLSfraction = 0.004;
  //G4double WbLSdensity = (water_nist->GetDensity())*(1-WbLSfraction) + 
  //(Scint->GetDensity())*WbLSfraction;
  //G4Material* WbLS;
  //if(WbLSfraction==1){
  //WbLS = new G4Material("WbLS", WbLSdensity, 1);
  ////kStateLiquid, 293*kelvin, 1*atmosphere);
  //WbLS->AddMaterial(Scint, WbLSfraction);
  //}
  //else{
  //WbLS = new G4Material("WbLS", WbLSdensity, 2);
  ////kStateLiquid, 293*kelvin, 1*atmosphere);
    //WbLS->AddMaterial(water_nist, (1-WbLSfraction));
    //WbLS->AddMaterial(Scint, WbLSfraction);
  //  }
  //  G4Material* PureLS_NoOpt = new G4Material("PureLS_NoOpt", 

  //  G4Material* WbLS_NoLight = new G4Material("WbLS_noLight", WbLSdensity, 1);
  //WbLS_NoLight->AddMaterial(WbLS, 1);

  //Aluminium Alloy used in the walls Alloy #6063
  G4Material* AlAlloy=new G4Material("Al Alloy", 2680.*kg/m3, 9, kStateSolid);
  //Adding elements in this way seems pointless now but is useful for later
  AlAlloy->AddElement(elAl, 98.5*perCent);
  AlAlloy->AddElement(elSi, 0.40*perCent);
  AlAlloy->AddElement(elFe, 0.175*perCent);
  AlAlloy->AddElement(elCu, 0.05*perCent);
  AlAlloy->AddElement(elMn, 0.05*perCent);
  AlAlloy->AddElement(elMg, 0.675*perCent);
  AlAlloy->AddElement(elCr, 0.05*perCent);
  AlAlloy->AddElement(elZn, 0.05*perCent);
  AlAlloy->AddElement(elTi, 0.05*perCent);

  
  /*the volumes: */

  G4double worldx = 1 * m;
  G4double worldy = 1 * m;
  G4double worldz = 1 * m;

//the whole simulation must be contained within a "world" volume
//defining a volume requires definition of solid, logical, and physical volume
//solid volume is the shape, has dimensions
  G4Box* world = new G4Box("world_box", worldx, worldy, worldz);

//logical volume: has a solid volume as a member, a material, last 3???
  G4LogicalVolume* logical_world = new G4LogicalVolume(world, air_nist, "world_log", 0,0,0);

  //make the world invisible!
  logical_world->SetVisAttributes(G4VisAttributes::Invisible);

  //physical volume: G4PVPlacement class, represents a logical volume that is
  //placed somewhere in the mother volume.
  physical_world = new G4PVPlacement(0, G4ThreeVector(), logical_world,
				     "world_phys", 0, false, 0, false);
  //The final argument is a flag to check the geometry for overlaps.


  //Create the liquid in T1 (it will need to be sensitive).
  G4Tubs* liquid = new G4Tubs("liquid", 0, (25.4*mm/2), (25.4*mm/2),
			      0, 360*deg);
  G4LogicalVolume* liq_log =
      new G4LogicalVolume(liquid, Scint, "liq_log", 0,0,0);

  G4VPhysicalVolume* liq_phys =
    new G4PVPlacement(0, G4ThreeVector(0, 0, 0), liq_log, "liq_phys",
		      logical_world, false, 0, false);

  return physical_world;
}
