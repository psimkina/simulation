#ifndef emCalorimeterHit_h
#define emCalorimeterHit_h 1

#include "G4VHit.hh"
#include "G4THitsCollection.hh"
#include "G4Allocator.hh"
#include "G4ThreeVector.hh"
#include "G4LogicalVolume.hh"
#include "G4Transform3D.hh"
#include "G4RotationMatrix.hh"

class G4AttDef;
class G4AttValue;

/// EM Calorimeter hit
///
/// It records:
/// - the cell column ID and row ID
/// - the energy deposit
/// - the cell position and rotation

class emCalorimeterHit : public G4VHit
{
  public:
    emCalorimeterHit();
    emCalorimeterHit(G4int iCol,G4int iRow, G4int iPlane);
    emCalorimeterHit(const emCalorimeterHit &right);
    virtual ~emCalorimeterHit();

    const emCalorimeterHit& operator=(const emCalorimeterHit &right);
    G4bool operator==(const emCalorimeterHit &right) const;

    inline void *operator new(size_t);
    inline void operator delete(void *aHit);

    virtual void Draw();
    virtual const std::map<G4String,G4AttDef>* GetAttDefs() const;
    virtual std::vector<G4AttValue>* CreateAttValues() const;
    virtual void Print();

    void SetColumnID(G4int z) { fColumnID = z; }
    G4int GetColumnID() const { return fColumnID; }

    void SetRowID(G4int z) { fRowID = z; }
    G4int GetRowID() const { return fRowID; }

    void SetPlaneID(G4int z) { fPlaneID = z; }
    G4int GetPlaneID() const { return fPlaneID; }

    void SetEdep(G4double de) { fEdep = de; }
    void AddEdep(G4double de) { fEdep += de; }
    G4double GetEdep() const { return fEdep; }

    void SetPos(G4ThreeVector xyz) { fPos = xyz; }
    G4ThreeVector GetPos() const { return fPos; }

    void SetRot(G4RotationMatrix rmat) { fRot = rmat; }
    G4RotationMatrix GetRot() const { return fRot; }

  private:
    G4int fColumnID;
    G4int fRowID;
    G4int fPlaneID;
    G4double fEdep;
    G4ThreeVector fPos;
    G4RotationMatrix fRot;
};

using emCalorimeterHitsCollection = G4THitsCollection<emCalorimeterHit>;

extern G4ThreadLocal G4Allocator<emCalorimeterHit>* emCalorimeterHitAllocator;

inline void* emCalorimeterHit::operator new(size_t)
{
  if (!emCalorimeterHitAllocator) {
       emCalorimeterHitAllocator = new G4Allocator<emCalorimeterHit>;
  }
  return (void*)emCalorimeterHitAllocator->MallocSingle();
}

inline void emCalorimeterHit::operator delete(void* aHit)
{
  emCalorimeterHitAllocator->FreeSingle((emCalorimeterHit*) aHit);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
#endif
