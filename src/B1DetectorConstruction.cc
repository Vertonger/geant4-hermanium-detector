//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
//
/// \file B1DetectorConstruction.cc
/// \brief Implementation of the B1DetectorConstruction class

#include "B1DetectorConstruction.hh"

#include "G4RunManager.hh"
#include "G4NistManager.hh"
#include "G4Box.hh"
#include "G4Cons.hh"
#include "G4Orb.hh"
#include "G4Sphere.hh"
#include "G4Tubs.hh"
#include "G4Trd.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

B1DetectorConstruction::B1DetectorConstruction()
: G4VUserDetectorConstruction(),
  fScoringVolume(0)
{ }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

B1DetectorConstruction::~B1DetectorConstruction()
{ }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VPhysicalVolume* B1DetectorConstruction::Construct()
{  
  // Get nist material manager
  G4NistManager* nist = G4NistManager::Instance();

  auto air = nist->FindOrBuildMaterial("G4_AIR");
  auto alum = nist->FindOrBuildMaterial("G4_Al");
  auto silicon = nist->FindOrBuildMaterial("G4_Si");
  auto bor = nist->FindOrBuildMaterial("G4_Br");

  G4Box* world_solid = new G4Box("World", 20 * cm, 20 * cm, 20 * cm);
  G4LogicalVolume* world_logic = new G4LogicalVolume(world_solid, air, "World");
  G4VPhysicalVolume* world_physical = new G4PVPlacement(
    nullptr,
    G4ThreeVector(),
    "World",
    world_logic,
    nullptr, //ссылка на физический объём
    0,
    0,
    false);
  
  //крышка
  G4Tubs* cap_solid = new G4Tubs("Cap", 25.2 * mm, 26.2 * mm, 64.8 * mm, 0, CLHEP::twopi);
  G4LogicalVolume* cap_logical = new G4LogicalVolume(cap_solid, alum, "Cap");

  G4VPhysicalVolume* cap_physical = new G4PVPlacement(
    nullptr,
    G4ThreeVector(0 * mm, 0 * mm, 64.8 * mm),
    "Cap",
    cap_logical,
    world_physical,
    0,
    0,
    false);

  //крепление
  G4Tubs* fix_solid = new G4Tubs("Fixture", 24.2 * mm, 25.2 * mm, 52.8 * mm, 0, CLHEP::twopi);
  G4LogicalVolume* fix_logical = new G4LogicalVolume(fix_solid, alum, "Fixture");
  G4VPhysicalVolume* fix_physical = new G4PVPlacement(
    nullptr,
    G4ThreeVector(0 * mm, 0 * mm, 64.8 * mm),
    "Fixture",
    fix_logical,
    world_physical,
    0,
    0,
    false);


  auto fix_position = G4ThreeVector(0 * mm, 0 * mm, 64.8 * mm);
  auto dead_position = fix_position + G4ThreeVector(0 * mm, 0 * mm, -7.3 * mm);


  // мёртвый слой (толщина - 2 мм)
  G4Tubs* dead_solid = new G4Tubs("Dead", 0 * mm, 23.5 * mm, 45.5 * mm, 0, CLHEP::twopi);
  G4LogicalVolume* dead_logical = new G4LogicalVolume(dead_solid, silicon, "Dead");
  G4VPhysicalVolume* dead_physical = new G4PVPlacement(
    nullptr,
    dead_position,
    "Dead",
    dead_logical,
    world_physical,
    0,
    0,
    false);

  //кристалл
  G4Tubs* crystal_solid = new G4Tubs("Crystal", 0 * mm, 21.5 * mm, 43.5 * mm, 0, CLHEP::twopi);
  G4LogicalVolume* crystal_logical = new G4LogicalVolume(crystal_solid, silicon, "Crystal");
  G4VPhysicalVolume* crystal_physical = new G4PVPlacement(
    nullptr,
    G4ThreeVector(),
    "Crystal",
    crystal_logical,
    dead_physical,
    0,
    0,
    false);

  //борный слой
  G4Tubs* bor_solid = new G4Tubs("Bor", 0 * mm, 5 * mm, 35.5 * mm, 0, CLHEP::twopi);
  G4LogicalVolume* bor_logical = new G4LogicalVolume(bor_solid, bor, "Bor");
  G4VPhysicalVolume* bor_physical = new G4PVPlacement(
    nullptr,
    G4ThreeVector(0 * mm, 0 * mm, 9.4 * mm),
    "Bor",
    bor_logical,
    crystal_physical,
    0,
    0,
    false);

  //дырка
  G4Tubs* hole_solid = new G4Tubs("Hole", 0 * mm, 4.7 * mm, 34.8 * mm, 0, CLHEP::twopi);
  G4LogicalVolume* hole_logical = new G4LogicalVolume(hole_solid, air, "Hole");
  G4VPhysicalVolume* hole_physical = 
  new G4PVPlacement(
    nullptr,
    G4ThreeVector(0 * mm, 0 * mm, 0.7*mm),
    "Hole",
    hole_logical,
    bor_physical,
    0,
    0,
    false);
    
  fScoringVolume = crystal_logical;
  //
  //always return the physical World
  //
  return world_physical;
}