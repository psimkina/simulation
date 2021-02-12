#ifndef emCalorimeterSD_h
#define emCalorimeterSD_h 1

#include "G4VSensitiveDetector.hh"

#include "emCalorimeterHit.hh"

class G4Step;
class G4HCofThisEvent;
class G4TouchableHistory;

/// Electromagnetic calorimeter sensitive detector

class emCalorimeterSD : public G4VSensitiveDetector
{
  public:
    emCalorimeterSD(G4String name);
    virtual ~emCalorimeterSD();

    virtual void Initialize(G4HCofThisEvent*HCE);
    virtual G4bool ProcessHits(G4Step*aStep, G4TouchableHistory*ROhist);

  private:
    emCalorimeterHitsCollection* fHitsCollection;
    G4int fHCID;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
