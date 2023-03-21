#include "ECALActionInitialization.hh"
#include "ECALPrimaryGeneratorAction.hh"
#include "ECALEventAction.hh"
#include "ECALRunAction.hh"
#include "ECALSteppingAction.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

ECALActionInitialization::ECALActionInitialization()
 : G4VUserActionInitialization()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

ECALActionInitialization::~ECALActionInitialization()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void ECALActionInitialization::BuildForMaster() const
{
  ECALEventAction* eventAction = new ECALEventAction;
  SetUserAction(new ECALRunAction(eventAction));
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void ECALActionInitialization::Build() const
{
  SetUserAction(new ECALPrimaryGeneratorAction);
  auto eventAction = new ECALEventAction;
  SetUserAction(eventAction);

  SetUserAction(new ECALRunAction(eventAction));
  auto steppingAction = new ECALSteppingAction;
  SetUserAction(steppingAction);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
