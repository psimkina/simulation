#include "ECALPrimaryGeneratorAction.hh"
#include "ECALConstants.hh"

#include "G4RunManager.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4LogicalVolume.hh"
#include "G4Box.hh"
#include "G4Event.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4SystemOfUnits.hh"
#include "Randomize.hh"

//CLHEP::HepRandom::setTheSeed((unsigned)clock());


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

ECALPrimaryGeneratorAction::ECALPrimaryGeneratorAction()
 : G4VUserPrimaryGeneratorAction(),
   fParticleGun(nullptr)
{
  G4int nofParticles = 1;
  fParticleGun = new G4ParticleGun(nofParticles);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

ECALPrimaryGeneratorAction::~ECALPrimaryGeneratorAction()
{
  delete fParticleGun;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void ECALPrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{
  // This function is called at the begining of event

  // default particle kinematic
  //
  //int seed = 0
  //CLHEP::HepRandom::setTheSeed(seed); 
  //G4Random::setTheSeed(seed);
  G4Random::setTheSeed((unsigned)clock());
  auto particleDefinition
    = G4ParticleTable::GetParticleTable()->FindParticle("e-");
  fParticleGun->SetParticleDefinition(particleDefinition);
  // G4double angle = 3.*CLHEP::pi/180.;
  //std::cout << "angle " << sin(angle) << std:: endl;
  fParticleGun->SetParticleMomentumDirection(G4ThreeVector(0.,0.,1.));

  auto mass = particleDefinition->GetPDGMass();
  //G4double momentum = (1.+G4UniformRand()*99.)*GeV;
  G4double momentum = 60.*GeV;
  G4double pp = momentum;

  G4double X = 0.8*(G4UniformRand()-0.5)*detectSizeX;
  G4double Y = 0.8*(G4UniformRand()-0.5)*detectSizeY;
  //G4double X = 0.;
  //G4double Y = 0.;
  auto ekin = std::sqrt(pp*pp+mass*mass)-mass;
  fParticleGun->SetParticleEnergy(ekin);

  // Set gun position
  //fParticleGun
  //  ->SetParticlePosition(G4ThreeVector(X*cm, Y*cm, -(crystSizeZ/2.)*cm));

  //fParticleGun                                                                                                                                                                              
  //    ->SetParticlePosition(G4ThreeVector(0*cm, 0*cm, -(detectSizeZ/2 + 130.)*cm));  

  fParticleGun->SetParticlePosition(G4ThreeVector(X*cm, Y*cm, -(detectSizeZ/2)*cm));
  fParticleGun->GeneratePrimaryVertex(anEvent);

  std::cout << "Z size: " << detectSizeZ/2 + 130 << std::endl;
   }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
