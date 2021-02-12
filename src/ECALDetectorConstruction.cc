#include "ECALDetectorConstruction.hh"
#include "emCalorimeterSD.hh"
#include "ECALConstants.hh"

#include "G4NistManager.hh"
#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"

#include "G4PVReplica.hh"
#include "G4SDManager.hh"
#include "G4VSensitiveDetector.hh"

ECALDetectorConstruction::ECALDetectorConstruction()
: G4VUserDetectorConstruction(),
  emScintiLogical(nullptr)
{ }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

ECALDetectorConstruction::~ECALDetectorConstruction()
{ }

G4VPhysicalVolume* ECALDetectorConstruction::Construct()
{
  G4bool fCheckOverlaps = true;

  //
  // Materials
  //
  G4NistManager* nist = G4NistManager::Instance();
  G4Material* default_mat = nist->FindOrBuildMaterial("G4_AIR");
  G4Material* cryst_mat   = nist->FindOrBuildMaterial("G4_PbWO4");

  // Detector Parameters
  G4double cryst_dX = 2.2*cm, cryst_dY = 2.2*cm, cryst_dZ = 23*cm;
  G4int nb_cryst = kNofCrystals;
  G4int nb_plns = kNofPlanes;

  G4double plns_dX = cryst_dX*nb_cryst, plns_dY = cryst_dY*nb_cryst, plns_dZ = cryst_dZ*nb_plns;

  //
  // World
  //
  G4double world_sizeXY = 1.2*plns_dX;
  G4double world_sizeZ  = 1.2*plns_dZ;

  G4Box* solidWorld =
    new G4Box("World",                       //its name
       0.5*world_sizeXY, 0.5*world_sizeXY, 0.5*world_sizeZ); //its size

  G4LogicalVolume* logicWorld =
    new G4LogicalVolume(solidWorld,          //its solid
                        default_mat,         //its material
                        "World");            //its name

  G4VPhysicalVolume* physWorld =
    new G4PVPlacement(0,                     //no rotation
                      G4ThreeVector(),       //at (0,0,0)
                      logicWorld,            //its logical volume
                      "World",               //its name
                      0,                     //its mother  volume
                      false,                 //no boolean operation
                      0,                     //copy number
                      fCheckOverlaps);       // checking overlaps

  //
  // full detector
  //
  G4Box* solidDetector =
    new G4Box("Detector", 0.5*plns_dX, 0.5*plns_dY, 0.5*plns_dZ);

  G4LogicalVolume* logicDetector =
    new G4LogicalVolume(solidDetector,       //its solid
                        default_mat,         //its material
                        "Detector");         //its name

      // calorimeter column
  auto CalColumn
    = new G4Box("CalColumn",cryst_dX/2,plns_dY/2,plns_dZ/2);
  auto CalColumnLogical
    = new G4LogicalVolume(CalColumn,cryst_mat,"CalColumnLogical");
  new G4PVReplica("CalColumnPhysical",CalColumnLogical,
                  logicDetector,kXAxis,nb_cryst,cryst_dX);

      // calorimeter cell
  auto CalCell
    = new G4Box("CalCell",cryst_dX/2,cryst_dY/2,plns_dZ/2);
  auto CalCellLogical
    = new G4LogicalVolume(CalCell,cryst_mat,"CalCellLogical");
  new G4PVReplica("CalCellPhysical",CalCellLogical,
                  CalColumnLogical,kYAxis,nb_cryst,cryst_dY);

      // calorimeter layers
  auto CalLayer
    = new G4Box("CalLayer",cryst_dX/2,cryst_dY/2,cryst_dZ/2);
  auto CalLayerLogical
    = new G4LogicalVolume(CalLayer,cryst_mat,"CalLayerLogical");
  new G4PVReplica("CalLayerPhysical",CalLayerLogical,
                  CalCellLogical,kZAxis,nb_plns,cryst_dZ);

      // scintillator plates
    auto emScintiSolid
      = new G4Box("emScintiBox",cryst_dX/2,cryst_dY/2,cryst_dZ/2);
    emScintiLogical
      = new G4LogicalVolume(emScintiSolid,cryst_mat,
                            "emScintiLogical");
    new G4PVPlacement(0,G4ThreeVector(0.,0.,0.),emScintiLogical,
                      "emScintiPhysical", CalLayerLogical,
                      false,0,fCheckOverlaps);

  //
  // place detector in world
  //
  new G4PVPlacement(0,                       //no rotation
                    G4ThreeVector(),         //at (0,0,0)
                    logicDetector,           //its logical volume
                    "Detector",              //its name
                    logicWorld,              //its mother  volume
                    false,                   //no boolean operation
                    0,                       //copy number
                    fCheckOverlaps);         // checking overlaps

  return physWorld;
}

void ECALDetectorConstruction::ConstructSDandField()
{
    // sensitive detectors -----------------------------------------------------
  auto sdManager = G4SDManager::GetSDMpointer();
  G4String SDname;

  auto emCalorimeter = new emCalorimeterSD(SDname="/ECALCalorimeter");
  sdManager->AddNewDetector(emCalorimeter);
  emScintiLogical->SetSensitiveDetector(emCalorimeter);

}
