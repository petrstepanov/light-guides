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
/// \file DetectorConstruction.cc
/// \brief Implementation of the B1::DetectorConstruction class

#include "DetectorConstruction.hh"
#include "DetectorMessenger.hh"
#include "Materials.hh"
#include "detectors/DetectorProvider.hh"
#include "detectors/AbsMPPC.hh"

#include <G4RunManager.hh>
//#include <G4NistManager.hh>
#include <G4Box.hh>
#include <G4Cons.hh>
#include <G4Orb.hh>
#include <G4Sphere.hh>
#include <G4Trd.hh>
#include <G4SubtractionSolid.hh>
#include <G4LogicalVolume.hh>
#include <G4PVPlacement.hh>
#include <G4VPhysicalVolume.hh>
#include <G4Trd.hh>
#include <G4Box.hh>
#include <G4PVPlacement.hh>
#include <G4SystemOfUnits.hh>
#include <G4OpticalSurface.hh>
#include <G4LogicalSkinSurface.hh>
#include <G4RotationMatrix.hh>
#include <G4LogicalBorderSurface.hh>
#include <G4VisAttributes.hh>

#include <algorithm>
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorConstruction::DetectorConstruction() {
  // Initialize default user parameter values
  crystalSideA = DetectorMessenger::crystalSizeACmdDefaultValue;
  crystalSideB = DetectorMessenger::crystalSizeBCmdDefaultValue;
  crystalLength = DetectorMessenger::crystalLengthCmdDefaultValue;
  detectorType = DetectorMessenger::detectorTypeCmdDefaultValue;
  lightGuideLength = DetectorMessenger::lightGuideLengthCmdDefaultValue;
  hasReflector = DetectorMessenger::hasReflectorCmdDefaultValue;

  // Initialize default values
  wrapThickness = 65 * micrometer;
  greaseThickness = 0.3 * mm;

  // Initialize private objects
  detector = DetectorProvider::getDetector(detectorType);

  // Initialize the detector messenger
  fDetectorMessenger = new DetectorMessenger(this);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorConstruction::~DetectorConstruction() {
}

// Setters for geometry parameters

void DetectorConstruction::setCrystalSideA(G4double value) {
  crystalSideA = value;
  G4RunManager::GetRunManager()->GeometryHasBeenModified(true);
}

void DetectorConstruction::setCrystalSideB(G4double value) {
  crystalSideB = value;
  G4RunManager::GetRunManager()->GeometryHasBeenModified(true);
}

void DetectorConstruction::setCrystalLength(G4double value) {
  crystalLength = value;
  G4RunManager::GetRunManager()->GeometryHasBeenModified(true);
}

void DetectorConstruction::setLightGuideLength(G4double value) {
  lightGuideLength = value;
  G4RunManager::GetRunManager()->GeometryHasBeenModified(true);
}

void DetectorConstruction::setDetectorType(G4String value) {
  detectorType = value;
  detector = DetectorProvider::getDetector(detectorType);
  G4RunManager::GetRunManager()->GeometryHasBeenModified(true);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VPhysicalVolume* DetectorConstruction::Construct() {
  // Option to switch on/off checking of volumes overlaps
  G4bool checkOverlaps = true;

  // █░█░█ █▀█ █▀█ █░░ █▀▄
  // ▀▄▀▄▀ █▄█ █▀▄ █▄▄ █▄▀
  // https://fsymbols.com/generators/carty/

  G4double assemblyLength = wrapThickness + crystalLength + greaseThickness
                            + (lightGuideLength > 0 ? lightGuideLength + greaseThickness : 0) + detector->getZSize();
  G4double maxXY = std::max(crystalSideB, detector->getXYSize());
  G4double worldSizeXY = maxXY;
  G4double worldSizeZ = crystalLength + lightGuideLength + detector->getZSize();
  worldSizeXY *= 4.;
  worldSizeZ *= 2.;
  G4Material *worldMaterial = Materials::getInstance()->getMaterial("G4_AIR");

  G4Box *worldSolid = new G4Box("World" /* name */, 0.5 * worldSizeXY /* size x */, 0.5 * worldSizeXY /* size y */,
                                0.5 * worldSizeZ /* size z */);
  G4LogicalVolume *worldLogical = new G4LogicalVolume(worldSolid /* its solid */, worldMaterial /* its material */,
                                                      "World" /* its name */);
  G4VPhysicalVolume *worldPhysical = new G4PVPlacement(0 /* rotation */, G4ThreeVector() /* position */,
                                                       worldLogical /* logical volume */, "World" /* name */,
                                                       0 /* parent logical */, false /* boolean operation */,
                                                       0 /* copy number */, checkOverlaps);

  // Calculate position of the front assembly edge along Z axis
  G4double z0 = -assemblyLength / 2;

  // █▀▀ █▀█ █▄█ █▀ ▀█▀ ▄▀█ █░░
  // █▄▄ █▀▄ ░█░ ▄█ ░█░ █▀█ █▄▄

  G4Trd *crystalSolid = new G4Trd("crystalSolid", crystalSideA / 2., crystalSideB / 2., crystalSideA / 2.,
                                  crystalSideB / 2., crystalLength / 2.);
  G4Material *crystalMaterial = Materials::getInstance()->getMaterial("SciGlass-4-1-L");
  G4LogicalVolume *crystalLogical = new G4LogicalVolume(crystalSolid, crystalMaterial, "crystalLogical");

  G4ThreeVector crystalPosition = G4ThreeVector(0, 0, z0 + wrapThickness + crystalLength / 2.);
  G4VPhysicalVolume *crystalPhysical = new G4PVPlacement(0 /* rotation */, crystalPosition /* position */,
                                                         crystalLogical /* logical volume */, "crystal" /* name */,
                                                         worldLogical /* parent logical */,
                                                         false /* boolean operation */, 0 /* copy number */,
                                                         checkOverlaps);

  // █▀▀ █▀█ █▄█ █▀ ▀█▀ ▄▀█ █░░   █░█░█ █▀█ ▄▀█ █▀█
  // █▄▄ █▀▄ ░█░ ▄█ ░█░ █▀█ █▄▄   ▀▄▀▄▀ █▀▄ █▀█ █▀▀

  G4Trd *crystalWrapASolid = new G4Trd("crystalWrapASolid", (crystalSideA + 2. * wrapThickness) / 2.,
                                       (crystalSideB + 2. * wrapThickness) / 2.,
                                       (crystalSideA + 2. * wrapThickness) / 2.,
                                       (crystalSideB + 2. * wrapThickness) / 2., (crystalLength + wrapThickness) / 2.);
  G4Trd *crystalWrapBSolid = new G4Trd("crystalWrapBSolid", crystalSideA / 2., crystalSideB / 2., crystalSideA / 2.,
                                       crystalSideB / 2., crystalLength / 2.);
  G4RotationMatrix *noRotation = new G4RotationMatrix();
  G4ThreeVector wrapTranslation(0, 0, wrapThickness / 2);
  G4SubtractionSolid *crystalWrapSolid = new G4SubtractionSolid("crystalWrapSolid", crystalWrapASolid,
                                                                crystalWrapBSolid, noRotation, wrapTranslation);

  G4Material *wrapMaterial = Materials::getInstance()->getMaterial("C10H8O4");
  G4LogicalVolume *crystalWrapLogical = new G4LogicalVolume(crystalWrapSolid, wrapMaterial, "crystalWrapLogical");
  G4ThreeVector crystalWrapPosition = G4ThreeVector(0, 0, z0 + (wrapThickness + crystalLength) / 2.);
  G4VPhysicalVolume *crystalWrapPhysical = new G4PVPlacement(0 /* rotation */, crystalWrapPosition /* position */,
                                                             crystalWrapLogical /* logical volume */,
                                                             "crystalWrap" /* name */,
                                                             worldLogical /* parent logical */,
                                                             false /* boolean operation */, 0 /* copy number */,
                                                             checkOverlaps);
  // Force wrap to render as wireframe
  G4VisAttributes *wrapLogicalVisAttr = new G4VisAttributes();
  // crystalWrapLogicalVisAttr->SetForceCloud();
  // crystalWrapLogicalVisAttr->SetForceNumberOfCloudPoints(1e4);
  wrapLogicalVisAttr->SetForceWireframe();
  // wrapLogicalVisAttr->SetForceAuxEdgeVisible();
  // crystalWrapLogicalVisAttr->SetLineStyle(G4VisAttributes::LineStyle::dotted);
  // crystalWrapLogicalVisAttr->SetForceWireframe();
  // crystalWrapLogicalVisAttr->SetForceAuxEdgeVisible();
  crystalWrapLogical->SetVisAttributes(wrapLogicalVisAttr);

  addReflectiveBorder(crystalPhysical, crystalWrapPhysical);

  // █░░ █ █▀▀ █░█ ▀█▀   █▀▀ █░█ █ █▀▄ █▀▀
  // █▄▄ █ █▄█ █▀█ ░█░   █▄█ █▄█ █ █▄▀ ██▄

  G4double z1 = z0 + wrapThickness + crystalLength;

  if (lightGuideLength > 0) {
    G4Box *greaseGuideSolid = new G4Box("greaseGuideSolid", (crystalSideB + 2 * wrapThickness) / 2.,
                                        (crystalSideB + 2 * wrapThickness) / 2., greaseThickness / 2.);
    G4Material *greaseGuideMaterial = Materials::getInstance()->getMaterial("BC630");
    G4LogicalVolume *greaseGuideLogical = new G4LogicalVolume(greaseGuideSolid, greaseGuideMaterial,
                                                              "greaseGuideLogical");

    if (greaseThickness > 0) {
      G4ThreeVector greaseGuidePosition = G4ThreeVector(0, 0, z1 + greaseThickness / 2.);
      /* G4VPhysicalVolume *greasePhysical = */new G4PVPlacement(0 /* rotation */, greaseGuidePosition /* position */,
                                                                 greaseGuideLogical /* logical volume */,
                                                                 "greaseGuide" /* name */,
                                                                 worldLogical /* parent logical */,
                                                                 false /* boolean operation */, 0 /* copy number */,
                                                                 checkOverlaps);
    }

    // LIGHT GUIDE
    G4double detectorXY = detector->getXYSize();
    G4Trd *lightGuideSolid = new G4Trd("lightGuideSolid", crystalSideB / 2., detectorXY / 2., crystalSideB / 2.,
                                       detectorXY / 2., lightGuideLength / 2.);
    G4Material *lightGuideMaterial = Materials::getInstance()->getMaterial("PMMA");

    G4LogicalVolume *lightGuideLogical = new G4LogicalVolume(lightGuideSolid, lightGuideMaterial, "lightGuideLogical");

    G4ThreeVector lightGuidePosition = G4ThreeVector(0, 0, z1 + greaseThickness + lightGuideLength / 2.);
    G4VPhysicalVolume *lightGuidePhysical = new G4PVPlacement(0 /* rotation */, lightGuidePosition /* position */,
                                                              lightGuideLogical /* logical volume */,
                                                              "lightGuide" /* name */,
                                                              worldLogical /* parent logical */,
                                                              false /* boolean operation */, 0 /* copy number */,
                                                              checkOverlaps);
    // WRAP
    G4Trd *lightGuideWrapASolid = new G4Trd("lightGuideWrapASolid", (crystalSideB + 2. * wrapThickness) / 2.,
                                            (detectorXY + 2. * wrapThickness) / 2.,
                                            (crystalSideB + 2. * wrapThickness) / 2.,
                                            (detectorXY + 2. * wrapThickness) / 2., lightGuideLength / 2.);
    // Tip: added 1 nanometer because "Stuck Track: potential geometry or navigation problem." between "lightGuideWrap" and "grease"
    G4Trd *lightGuideWrapBSolid = new G4Trd("lightGuideWrapBSolid", crystalSideB / 2., detectorXY / 2. /*+ 1 * nanometer*/,
                                            crystalSideB / 2., detectorXY / 2. /*+ 1 * nanometer*/, lightGuideLength / 2.);

    G4SubtractionSolid *lightGuideWrapSolid = new G4SubtractionSolid("lightGuideWrapSolid", lightGuideWrapASolid,
                                                                     lightGuideWrapBSolid, noRotation,
                                                                     G4ThreeVector(0, 0, 0));

    G4LogicalVolume *lightGuideWrapLogical = new G4LogicalVolume(lightGuideWrapSolid, wrapMaterial,
                                                                 "lightGuideWrapLogical");
    G4VPhysicalVolume *lightGuideWrapPhysical = new G4PVPlacement(0 /* rotation */, lightGuidePosition /* position */,
                                                                  lightGuideWrapLogical /* logical volume */,
                                                                  "lightGuideWrap" /* name */,
                                                                  worldLogical /* parent logical */,
                                                                  false /* boolean operation */, 0 /* copy number */,
                                                                  checkOverlaps);
    lightGuideWrapLogical->SetVisAttributes(wrapLogicalVisAttr);
    addReflectiveBorder(lightGuidePhysical, lightGuideWrapPhysical);
  }

  // █▀▄ █▀▀ ▀█▀ █▀▀ █▀▀ ▀█▀ █▀█ █▀█
  // █▄▀ ██▄ ░█░ ██▄ █▄▄ ░█░ █▄█ █▀▄

  // coordinate of front edge of the detector
  G4double zDet = z0 + wrapThickness + crystalLength + (lightGuideLength > 0 ? greaseThickness + lightGuideLength : 0);

  if (AbsMPPC *mppc = dynamic_cast<AbsMPPC*>(detector)) {

    // GREASE
    if (greaseThickness > 0) {
      G4double detectorXY = detector->getXYSize();
      G4double greaseWidthHeight = hasReflector ? detectorXY : maxXY + wrapThickness * 2; // either fits into the reflector window : or covers up the shielding
      G4Box *greaseSolid = new G4Box("greaseSolid", greaseWidthHeight / 2., greaseWidthHeight / 2., greaseThickness / 2.);
      G4Material *greaseMaterial = Materials::getInstance()->getMaterial("BC630");
      G4LogicalVolume *greaseLogical = new G4LogicalVolume(greaseSolid, greaseMaterial, "greaseLogical");
      G4ThreeVector greasePosition = G4ThreeVector(0, 0, zDet + greaseThickness / 2.);
      /* G4VPhysicalVolume *greasePhysical = */new G4PVPlacement(0 /* rotation */, greasePosition /* position */,
                                                                 greaseLogical /* logical volume */,
                                                                 "grease" /* name */, worldLogical /* parent logical */,
                                                                 false /* boolean operation */, 0 /* copy number */,
                                                                 checkOverlaps);
    }

    // MPPC REFLECTOR
    // Here the reflector thickness is same as grease thickness
    G4Box *mppcReflectorOuter = new G4Box("mppcReflectorOuterSolid", 0.5 * (maxXY + wrapThickness * 2),
                                          0.5 * (maxXY + wrapThickness * 2), 0.5 * greaseThickness);
    G4Box *mppcReflectorInner = new G4Box("mppcReflectorInnerSolid", 0.5 * mppc->getXYSize(), 0.5 * mppc->getXYSize(),
                                          0.5 * greaseThickness);
    G4SubtractionSolid *mppcReflectorSolid = new G4SubtractionSolid("mppcReflectorSolid", mppcReflectorOuter,
                                                                    mppcReflectorInner, noRotation, G4ThreeVector());

    G4LogicalVolume *mppcReflectorLogical = new G4LogicalVolume(mppcReflectorSolid, wrapMaterial,
                                                                "mppcReflectorLogical");
    G4ThreeVector mppcReflectorPosition = G4ThreeVector(0, 0, zDet + greaseThickness / 2.);
    /*G4VPhysicalVolume* mppcReflectorPhysical =*/new G4PVPlacement(0, mppcReflectorPosition, mppcReflectorLogical,
                                                                    "mppcReflector", worldLogical, false, 0,
                                                                    checkOverlaps);
    // Make reflector surface reflective
    addReflectiveSkin(mppcReflectorLogical);

    // MPPC WINDOW
    G4Box *mppcWindowSolid = new G4Box("mppcWindowSolid", 0.5 * mppc->getXYSize(), 0.5 * mppc->getXYSize(),
                                       0.5 * mppc->getWindowThickness());
    G4LogicalVolume *mppcWindowLogical = new G4LogicalVolume(mppcWindowSolid, mppc->getWindowMaterial(),
                                                             "mppcWindowLogical");
    G4ThreeVector mppcWindowPosition = G4ThreeVector(0, 0,
                                                     zDet + greaseThickness + detector->getWindowThickness() / 2.);
    /*G4VPhysicalVolume* mppcWindowPhysical =*/new G4PVPlacement(0, mppcWindowPosition, mppcWindowLogical, "mppcWindow",
                                                                 worldLogical, false, 0, checkOverlaps);

    // MPPC CATHODE
    G4Box *mppcCathodeSolid = new G4Box("mppcCathodeSolid", 0.5 * mppc->getXYSize(), 0.5 * mppc->getXYSize(),
                                        0.5 * mppc->getCathodeThickness());
    G4LogicalVolume *mppcCathodeLogical = new G4LogicalVolume(mppcCathodeSolid, mppc->getCathodeMaterial(),
                                                              "mppcCathodeLogical");
    G4ThreeVector mppcCathodePosition = G4ThreeVector(
        0, 0, zDet + greaseThickness + detector->getWindowThickness() + mppc->getCathodeThickness() / 2.);
    /*G4VPhysicalVolume* mppcCathodePhysical =*/new G4PVPlacement(0, mppcCathodePosition, mppcCathodeLogical,
                                                                  "mppcCathode", worldLogical, false, 0, checkOverlaps);
    // Set scoring volume (for Stepping Action)
    fScoringVolume = mppcCathodeLogical;

    // MPPC SHIELD
    G4double shieldLength = mppc->getWindowThickness() + mppc->getCathodeThickness();
    G4Box *mppcShieldASolid = new G4Box("mppcShieldASolid", 0.5 * (maxXY + wrapThickness * 2),
                                        0.5 * (maxXY + wrapThickness * 2), 0.5 * shieldLength);
    G4Box *mppcShieldBSolid = new G4Box("mppcShieldBSolid", 0.5 * mppc->getXYSize(), 0.5 * mppc->getXYSize(),
                                        0.5 * shieldLength);
    G4SubtractionSolid *mppcShieldSolid = new G4SubtractionSolid("mppcShieldSolid", mppcShieldASolid, mppcShieldBSolid,
                                                                 noRotation, G4ThreeVector());
    G4Material *mppcShieldMaterial = Materials::getInstance()->getMaterial("MuMetal");
    G4LogicalVolume *mppcShieldLogical = new G4LogicalVolume(mppcShieldSolid, mppcShieldMaterial, "mppcShieldLogical");
    G4ThreeVector mppcShieldPosition = G4ThreeVector(0, 0, zDet + greaseThickness + shieldLength / 2.);
    /*G4VPhysicalVolume* mppcShieldPhysical =*/new G4PVPlacement(0, mppcShieldPosition, mppcShieldLogical, "mppcShield",
                                                                 worldLogical, false, 0, checkOverlaps);

  }
  // always return the physical World
  return worldPhysical;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorConstruction::addReflectiveSkin(G4LogicalVolume *volumeLogical) {
  G4String surfaceName = volumeLogical->GetName() + "Surface";
  G4OpticalSurface *surface = new G4OpticalSurface(surfaceName);
  surface->SetType(dielectric_LUT);
  surface->SetModel(LUT);
  surface->SetFinish(polishedvm2000air);

  G4String skinSurfaceName = volumeLogical->GetName() + "SkinSurface";
  /*G4LogicalSkinSurface* skinSurface = */new G4LogicalSkinSurface(skinSurfaceName, volumeLogical, surface);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorConstruction::addReflectiveBorder(G4VPhysicalVolume *volumePhysical1, G4VPhysicalVolume *volumePhysical2) {
  G4String surfaceName = volumePhysical1->GetName() + volumePhysical2->GetName() + "Surface";
  G4OpticalSurface *surface = new G4OpticalSurface(surfaceName);
  surface->SetType(dielectric_LUT);
  surface->SetModel(LUT);
  surface->SetFinish(polishedvm2000air);

  G4String borderSurfaceName = volumePhysical1->GetName() + volumePhysical2->GetName() + "BorderSurface";
  /*G4LogicalBorderSurface* borderSurface = */new G4LogicalBorderSurface(borderSurfaceName, volumePhysical1,
                                                                         volumePhysical2, surface);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

AbsDetector* DetectorConstruction::getDetector() {
  return detector;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4double DetectorConstruction::getLightGuideLength() {
  return lightGuideLength;
}
