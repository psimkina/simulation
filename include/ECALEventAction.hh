#ifndef ECALEventAction_h
#define ECALEventAction_h 1


#include "G4UserEventAction.hh"
#include "globals.hh"

#include <vector>
#include <array>

// named constants
const G4int kHad = 0;
const G4int kDim = 1;

/// Event action

class ECALEventAction : public G4UserEventAction
{
public:
    ECALEventAction();
    virtual ~ECALEventAction();

    virtual void BeginOfEventAction(const G4Event*);
    virtual void EndOfEventAction(const G4Event*);

    std::vector<G4double>& GetHadCalEdep() { return fCalEdep[kHad]; }
    std::vector<G4double>& GetHadCalRow() { return fCalRow[kHad]; }
    std::vector<G4double>& GetHadCalColumn() { return fCalColumn[kHad]; }

private:
    // hit collections Ids
    std::array<G4int, kDim> fCalHCID;
    // energy deposit in calorimeters cells
    std::array<std::vector<G4double>, kDim> fCalEdep;
    std::array<std::vector<G4double>, kDim> fCalRow;
    std::array<std::vector<G4double>, kDim> fCalColumn;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
