#ifndef ECALActionInitialization_h
#define ECALActionInitialization_h 1

#include "G4VUserActionInitialization.hh"

/// Action initialization class.

class ECALActionInitialization : public G4VUserActionInitialization
{
  public:
    ECALActionInitialization();
    virtual ~ECALActionInitialization();

    virtual void BuildForMaster() const;
    virtual void Build() const;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
