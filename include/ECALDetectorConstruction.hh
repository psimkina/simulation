#ifndef ECALDetectorConstruction_h
#define ECALDetectorConstruction_h 1

#include "G4VUserDetectorConstruction.hh"
#include "globals.hh"
#include "G4FieldManager.hh"

class G4VPhysicalVolume;
class G4VSensitiveDetector;

/// Detector construction class to define materials and geometry.

class ECALDetectorConstruction : public G4VUserDetectorConstruction
{
  public:
    ECALDetectorConstruction();
    virtual ~ECALDetectorConstruction();

    virtual G4VPhysicalVolume* Construct();
    virtual void ConstructSDandField();

  private:
    G4LogicalVolume* emScintiLogical;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
