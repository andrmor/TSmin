#include "DetectorConstruction.hh"
#include "G4SystemOfUnits.hh"

#include "G4Element.hh"
#include "G4Material.hh"
#include "G4NistManager.hh"
#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4RotationMatrix.hh"
#include "G4ThreeVector.hh"
#include "G4Transform3D.hh"
#include "G4PVPlacement.hh"

G4VPhysicalVolume* DetectorConstruction::Construct()
{
  G4NistManager * man = G4NistManager::Instance();

  G4Material * material = nullptr;

  material = man->FindOrBuildMaterial("G4_POLYETHYLENE");

  //G4Element * alEle = new G4Element("TS_Aluminium_Metal", "Al", 13.0, 26.982*g/mole);
  //   //G4Element * alEle = new G4Element("Al", "Al", 13.0, 26.982*g/mole);
  //material = new G4Material("Al", 2.699*g/cm3, 1, kStateSolid);
  //material->AddElement(alEle, 1);

  if (!material)
  {
      G4cout.flush();
      std::cout << "\nMaterial does not exist, exiting...\n";
      exit(-1);
  }

  G4Box             * solidWorld = new G4Box("World", 500.0*mm, 500.0*mm, 500.0*mm);
  G4LogicalVolume   * logicWorld = new G4LogicalVolume(solidWorld, material, "World");
  G4VPhysicalVolume * physWorld  = new G4PVPlacement(0, G4ThreeVector(0.0, 0.0, 0.0), logicWorld, "World", 0, false, 0);

  return physWorld;
}

