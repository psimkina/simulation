#ifndef ECALConstants_h
#define ECALConstants_h 1

#include "globals.hh"

constexpr G4int kNofCrystals = 5;
constexpr G4int kNofPlanes = 1;
constexpr G4int kNofCells = kNofCrystals * kNofCrystals;

constexpr G4double crystSizeX = 2.2; 
constexpr G4double crystSizeY = 2.2;
constexpr G4double crystSizeZ = 23; 

constexpr G4double detectSizeX = crystSizeX*kNofCrystals; 
constexpr G4double detectSizeY = crystSizeY*kNofCrystals;
constexpr G4double detectSizeZ = crystSizeZ*kNofPlanes;

#endif