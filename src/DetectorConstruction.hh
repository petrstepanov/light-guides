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
/// \file DetectorConstruction.hh
/// \brief Definition of the B1::DetectorConstruction class

#ifndef B1DetectorConstruction_h
#define B1DetectorConstruction_h 1

#include "G4VUserDetectorConstruction.hh"
#include "DetectorMessenger.hh"
#include "detectors/AbsDetector.hh"
#include "globals.hh"

class G4VPhysicalVolume;
class G4LogicalVolume;

/// Detector construction class to define materials and geometry.

class DetectorConstruction: public G4VUserDetectorConstruction {
  public:
    DetectorConstruction();
    ~DetectorConstruction() override;

    G4VPhysicalVolume* Construct() override;

    G4LogicalVolume* GetScoringVolume() const {
      return fScoringVolume;
    }

  protected:
    G4LogicalVolume *fScoringVolume = nullptr;
    AbsDetector *detector;
    G4double greaseThickness;
    G4double wrapThickness;
    void addReflectiveSkin(G4LogicalVolume* volume);
    void addReflectiveBorder(G4VPhysicalVolume* volume1, G4VPhysicalVolume* volume2);

    // Define the messenger of the detector component
    DetectorMessenger* fDetectorMessenger = nullptr;

  public:
    // User parameters and Detector Messenger setters
    G4double crystalSideA;
    void setCrystalSideA(G4double value);
    G4double crystalSideB;
    void setCrystalSideB(G4double value);
    G4double crystalLength;
    void setCrystalLength(G4double value);
    G4double lightGuideLength;
    void setLightGuideLength(G4double value);
    G4bool hasReflector;
    void setHasReflector(G4bool value);
    G4String detectorType;
    void setDetectorType(G4String value);

  public:
    // Getters
    AbsDetector* getDetector();
    G4double getLightGuideLength();
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
