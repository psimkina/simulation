#include "ECALSteppingAction.hh"
#include "ECALDetectorConstruction.hh"

#include "G4Step.hh"
#include "G4RunManager.hh"


#include "ECALEventAction.hh"
#include "emCalorimeterHit.hh"
#include "ECALConstants.hh"

#include "G4Event.hh"
#include "G4RunManager.hh"
#include "G4EventManager.hh"
#include "G4HCofThisEvent.hh"
#include "G4VHitsCollection.hh"
#include "G4SDManager.hh"
#include "G4SystemOfUnits.hh"
#include "G4ios.hh"
#include "g4analysis.hh"
#include <math.h>
#define PI 3.14159265

using std::array;
using std::vector;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

ECALSteppingAction::ECALSteppingAction(){}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void ECALSteppingAction::UserSteppingAction(const G4Step* step)
{
// Collect energy and track length step by step
  
  auto analysisManager = G4AnalysisManager::Instance();
  // get volume of the current step
  auto volume = step->GetPreStepPoint()->GetTouchableHandle()->GetVolume();

  // energy deposit
  auto edep = step->GetTotalEnergyDeposit();

    // track information                                                                                                                                                                        
  G4Track* track = step->GetTrack();
  G4int trid = track->GetTrackID();
  //G4int pid = 0;                                                                                                                                                                            
  G4String name = track->GetDefinition()->GetParticleName();

  if (trid==1){
    std::cout << "trid: " << trid << std::endl;
    std::cout << "name: " << name << std::endl;
    std::cout << "position: " << track->GetPosition() / cm<< std::endl;
    std::cout << "direction: " << track->GetMomentumDirection()<< std::endl;

    const G4TrackVector* secondaries = step->GetSecondary();
    for (size_t i = 0; i < secondaries->size(); i++) {

         G4String name = (*secondaries)[i]->GetDefinition()->GetParticleName();
	 G4ThreeVector pos = (*secondaries)[i]->GetPosition() / cm;
	 G4ThreeVector momentum = (*secondaries)[i]->GetMomentumDirection();
	 G4double Etot = (*secondaries)[i]->GetTotalEnergy();
	 G4double Ekin = (*secondaries)[i]->GetKineticEnergy();

	 std::cout << "secondaries name: " << name << std::endl;
	 std::cout << "secondaries position: " << pos << std::endl;
	 std::cout << "secondaries direction: " << momentum << std::endl;

	 G4double theta = momentum[0]/momentum[2];
	 G4double phi = momentum[1]/momentum[0];

	 G4double d = 130.;
	 G4double x = (momentum[0]/momentum[2]) * d;
	 G4double y = (momentum[1]/momentum[2]) * d;


	 std::cout << "d: " << d + pos[2] << std::endl;
	 std::cout << "x: " << x + pos[0] << std::endl;
	 std::cout << "y: " << y + pos[1] << std::endl;

	 std::cout << "Etot: " << Etot << std::endl;
	 std::cout << "Ekin: " << Ekin << std::endl;

	 if (i == 0){
	   analysisManager->FillNtupleDColumn(8, x);
	   analysisManager->FillNtupleDColumn(10, y);
	   analysisManager->FillNtupleDColumn(12, Etot);
	 }

	 if (i == 1){
           analysisManager->FillNtupleDColumn(9, x);
           analysisManager->FillNtupleDColumn(11, y);
           analysisManager->FillNtupleDColumn(13, Etot);
         }

    }
    
  };
  // step length
  G4double stepLength = 0.;
  if ( step->GetTrack()->GetDefinition()->GetPDGCharge() != 0. ) {
    stepLength = step->GetStepLength();
  }


}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
