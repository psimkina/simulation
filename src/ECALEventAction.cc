#include "ECALEventAction.hh"
#include "emCalorimeterHit.hh"

#include "G4Event.hh"
#include "G4RunManager.hh"
#include "G4EventManager.hh"
#include "G4HCofThisEvent.hh"
#include "G4VHitsCollection.hh"
#include "G4SDManager.hh"
#include "G4SystemOfUnits.hh"
#include "G4ios.hh"
#include "g4analysis.hh"

using std::array;
using std::vector;


namespace {

// Utility function which finds a hit collection with the given Id
// and print warnings if not found
G4VHitsCollection* GetHC(const G4Event* event, G4int collId) {
  auto hce = event->GetHCofThisEvent();
  if (!hce) {
      G4ExceptionDescription msg;
      msg << "No hits collection of this event found." << G4endl;
      G4Exception("ECALEventAction::EndOfEventAction()",
                  "ECALCode001", JustWarning, msg);
      return nullptr;
  }

  auto hc = hce->GetHC(collId);
  if ( ! hc) {
    G4ExceptionDescription msg;
    msg << "Hits collection " << collId << " of this event not found." << G4endl;
    G4Exception("ECALEventAction::EndOfEventAction()",
                "ECALCode001", JustWarning, msg);
  }
  return hc;
}

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

ECALEventAction::ECALEventAction()
: G4UserEventAction(),
  fCalHCID  {{ -1 }},
  fCalEdep{{ vector<G4double>(9.,0.) }},
  fCalRow{{ vector<G4double>(9., 0.) }},
  fCalColumn{{ vector<G4double>(9., 0.) }}
      // std::array<T, N> is an aggregate that contains a C array.
      // To initialize it, we need outer braces for the class itself
      // and inner braces for the C array
{
  // set printing per each event
  G4RunManager::GetRunManager()->SetPrintProgress(1);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

ECALEventAction::~ECALEventAction()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void ECALEventAction::BeginOfEventAction(const G4Event*)
{
  // Find hit collections and histogram Ids by names (just once)
  // and save them in the data members of this class
  if (fCalHCID[0] == -1) {
    auto sdManager = G4SDManager::GetSDMpointer();
    auto analysisManager = G4AnalysisManager::Instance();
    array<G4String, kDim> cHCName
      = {{ "ECALCalorimeter/emCalorimeterColl" }};


    for (G4int iDet = 0; iDet < kDim; ++iDet) {
      // hit collections IDs
      fCalHCID[iDet]   = sdManager->GetCollectionID(cHCName[iDet]);
    }
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void ECALEventAction::EndOfEventAction(const G4Event* event)
{
  //
  // Fill histograms & ntuple
  //

  // Get analysis manager
  auto analysisManager = G4AnalysisManager::Instance();

  // Em/Had Calorimeters hits
  array<G4int, kDim> totalCalHit = {{ 0 }};
  array<G4double, kDim> totalCalEdep = {{ 0. }};

  for (G4int iDet = 0; iDet < kDim; ++iDet) {
    auto hc = GetHC(event, fCalHCID[iDet]);
    if ( ! hc ) return;

    totalCalHit[iDet] = 0;
    totalCalEdep[iDet] = 0.;
    for (unsigned long i = 0; i < hc->GetSize(); ++i) {
      G4double edep = 0.;
      G4int row = -1;
      G4int column = -1;

      auto hit = static_cast<emCalorimeterHit*>(hc->GetHit(i));
      edep = hit->GetEdep();
      row = hit->GetRowID();
      column = hit->GetColumnID();

      if ( edep > 0. ) {
        totalCalHit[iDet]++;
        totalCalEdep[iDet] += edep;
      }
      fCalEdep[iDet][i] = edep;
      fCalRow[iDet][i] = row;
      fCalColumn[iDet][i] = column;
    }
    // columns 0
    analysisManager->FillNtupleDColumn(0, totalCalEdep[iDet]);
  }

  analysisManager->AddNtupleRow();

  //
  // Print diagnostics
  //

  auto printModulo = G4RunManager::GetRunManager()->GetPrintProgress();
  if ( printModulo == 0 || event->GetEventID() % printModulo != 0) return;

  auto primary = event->GetPrimaryVertex(0)->GetPrimary(0);
  G4cout
    << G4endl
    << ">>> Event " << event->GetEventID() << " >>> Simulation truth : "
    << primary->GetG4code()->GetParticleName()
    << " " << primary->GetMomentum() << G4endl;

  // Calorimeters
  array<G4String, kDim> calName = {{ "EM" }};
  for (G4int iDet = 0; iDet < kDim; ++iDet) {
    G4cout << calName[iDet] << " Calorimeter has " << totalCalHit[iDet] << " hits."
           << " Total Edep is " << totalCalEdep[iDet]/MeV << " (MeV)" << G4endl;
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
