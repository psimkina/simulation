#include "ECALRunAction.hh"
#include "ECALEventAction.hh"

#include "G4Run.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"
#include "G4GenericAnalysisManager.hh"

using G4AnalysisManager = G4GenericAnalysisManager;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

ECALRunAction::ECALRunAction(ECALEventAction* eventAction)
 : G4UserRunAction(),
   fEventAction(eventAction)
{
  // Create the generic analysis manager
  // The choice of analysis technology is done according to the file extension
  auto analysisManager = G4AnalysisManager::Instance();
  analysisManager->SetVerboseLevel(1);

  // Default settings
  analysisManager->SetNtupleMerging(true);
     // Note: merging ntuples is available only with Root output
  analysisManager->SetFileName("ECAL");
     // If the filename extension is not provided, the default file type (root)
     // will be used for all files specified without extension.
  // analysisManager->SetDefaultFileType("xml");
     // The default file type (root) can be redefined by the user.

  // Book histograms, ntuple

  // Creating ntuple
  if ( fEventAction ) {
    analysisManager->CreateNtuple("CaloH", "Hits");
    analysisManager->CreateNtupleDColumn("TotalEn"); // column Id = 0
    analysisManager->CreateNtupleDColumn("InitialMomentum"); // column Id = 1
    analysisManager->CreateNtupleDColumn("InitialColumn"); // column Id = 2
    analysisManager->CreateNtupleDColumn("InitialRow"); // column Id = 3
    analysisManager->CreateNtupleDColumn("EnergyVector", fEventAction->GetHadCalEdep()); // column Id = 4
    analysisManager->CreateNtupleDColumn("Row", fEventAction->GetHadCalRow()); // column Id = 5
    analysisManager->CreateNtupleDColumn("Column", fEventAction->GetHadCalColumn()); // column Id = 6
    analysisManager->FinishNtuple();
  }

  // Set ntuple output file
  analysisManager->SetNtupleFileName(0, "detector");
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

ECALRunAction::~ECALRunAction()
{
  delete G4AnalysisManager::Instance();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void ECALRunAction::BeginOfRunAction(const G4Run* /*run*/)
{
  //inform the runManager to save random number seed
  //G4RunManager::GetRunManager()->SetRandomNumberStore(true);

  // Get analysis manager
  auto analysisManager = G4AnalysisManager::Instance();

  // Open an output file
  analysisManager->OpenFile();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void ECALRunAction::EndOfRunAction(const G4Run* /*run*/)
{
  // save histograms & ntuple
  //
  auto analysisManager = G4AnalysisManager::Instance();
  analysisManager->Write();
  analysisManager->CloseFile();

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
