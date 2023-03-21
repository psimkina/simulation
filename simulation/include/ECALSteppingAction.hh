#ifndef ECALSteppingAction_h
#define ECALSteppingAction_h 1

#include "G4UserSteppingAction.hh"
#include "ECALDetectorConstruction.hh"


/// Stepping action class.
///
/// In UserSteppingAction() there are collected the energy deposit and track
/// lengths of charged particles in Absober and Gap layers and
/// updated in RunData object.
// named constants
const G4int kHad1 = 0;
const G4int kDim1 = 1;

class ECALSteppingAction : public G4UserSteppingAction
{
public:
  ECALSteppingAction();
  ~ECALSteppingAction() override = default;

  void UserSteppingAction(const G4Step* step) override;

  std::vector<G4double>& GetFirstPhotonX() { return fFirstX[kHad1]; }
  std::vector<G4double>& GetFirstPhotonY() { return fFirstY[kHad1]; }
  std::vector<G4double>& GetSecondPhotonX() { return fSecondX[kHad1]; }
  std::vector<G4double>& GetSecondPhotonY() { return fSecondY[kHad1]; }

  private:
    // photon coordinates
    std::array<std::vector<G4double>, kDim1> fFirstX;
    std::array<std::vector<G4double>, kDim1> fFirstY;
    std::array<std::vector<G4double>, kDim1> fSecondX;
    std::array<std::vector<G4double>, kDim1> fSecondY;

};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

