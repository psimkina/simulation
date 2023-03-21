#include "emCalorimeterSD.hh"
#include "emCalorimeterHit.hh"
#include "ECALConstants.hh"

#include "G4HCofThisEvent.hh"
#include "G4TouchableHistory.hh"
#include "G4Track.hh"
#include "G4Step.hh"
#include "G4SDManager.hh"
#include "G4ios.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

emCalorimeterSD::emCalorimeterSD(G4String name)
: G4VSensitiveDetector(name),
  fHitsCollection(nullptr), fHCID(-1)
{
  collectionName.insert("emCalorimeterColl");
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

emCalorimeterSD::~emCalorimeterSD()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void emCalorimeterSD::Initialize(G4HCofThisEvent* hce)
{
  fHitsCollection
    = new emCalorimeterHitsCollection(SensitiveDetectorName,collectionName[0]);
  if (fHCID<0) {
    fHCID = G4SDManager::GetSDMpointer()->GetCollectionID(fHitsCollection);
  }
  hce->AddHitsCollection(fHCID,fHitsCollection);

  // fill calorimeter hits with zero energy deposition
  for (auto column=0;column<kNofCrystals;column++) {
    for (auto row=0;row<kNofCrystals;row++) {
      for (auto plane=0;plane<kNofPlanes;plane++) {
      fHitsCollection->insert(new emCalorimeterHit());
    }
    }
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4bool emCalorimeterSD::ProcessHits(G4Step* step, G4TouchableHistory*)
{
  auto edep = step->GetTotalEnergyDeposit();
  if (edep==0.) return true;

  // track information
  G4Track* track = step->GetTrack();
  G4int pid = track->GetParentID();
  //G4int pid = 0;

  G4String name = track->GetDefinition()->GetParticleName();

  //  if (pid < 2){
  //  std:: cout << "in the if statement" << std::endl;
  // std::cout <<"pid: " << pid << std::endl;
  // std::cout <<"particle: " << name<< std::endl;
  // };
  
  auto touchable = step->GetPreStepPoint()->GetTouchable();
  auto rowNo = touchable->GetCopyNumber(2);
  auto columnNo = touchable->GetCopyNumber(3);
  auto planeNo = touchable->GetCopyNumber(1);

  auto hitID = kNofCells*planeNo+kNofCrystals*columnNo+rowNo;
  auto hit = (*fHitsCollection)[hitID];

  // check if it is first touch
  if (hit->GetColumnID()<0) {
    hit->SetColumnID(columnNo);
    hit->SetRowID(rowNo);
    hit->SetPlaneID(planeNo);
    auto depth = touchable->GetHistory()->GetDepth();
    auto transform = touchable->GetHistory()->GetTransform(depth-2);
    transform.Invert();
    hit->SetRot(transform.NetRotation());
    hit->SetPos(transform.NetTranslation());
  }
  // add energy deposition
  hit->AddEdep(edep);

  return true;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
