#ifndef ECALRunAction_h
#define ECALRunAction_h 1

#include "G4UserRunAction.hh"
#include "globals.hh"

class ECALEventAction;

class G4Run;

/// Run action class

class ECALRunAction : public G4UserRunAction
{
  public:
    ECALRunAction(ECALEventAction* eventAction);
    virtual ~ECALRunAction();

    virtual void BeginOfRunAction(const G4Run*);
    virtual void   EndOfRunAction(const G4Run*);

  private:
    ECALEventAction* fEventAction;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
