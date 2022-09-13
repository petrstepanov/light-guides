/*
 * DetectorMessenger.cpp
 *
 *  Created on: Sep 7, 2022
 *      Author: petrstepanov
 */

#include "DetectorMessenger.hh"
#include "DetectorConstruction.hh"

#include <G4UIdirectory.hh>
#include <G4UIcmdWithAString.hh>

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

const G4String DetectorMessenger::detectorTypeCmdDefaultValue = "S13360-6025CS-6x6";

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorMessenger::DetectorMessenger(DetectorConstruction *d) : fDetector(d) {
  fDetectorDir = new G4UIdirectory("/detector/");
  fDetectorDir->SetGuidance("Configuration of Detector setup.");

  fCrystalSizeACmd = new G4UIcmdWithADoubleAndUnit("/detector/crystalSizeA", this);
  fCrystalSizeACmd->SetGuidance("Define the width/height of the beginning of the crystal.");
  // fCrystalSizeACmd->SetParameterName("choice", true);
  fCrystalSizeACmd->SetDefaultValue(crystalSizeACmdDefaultValue);
  fCrystalSizeACmd->AvailableForStates(G4State_PreInit);

  fCrystalSizeBCmd = new G4UIcmdWithADoubleAndUnit("/detector/crystalSizeB", this);
  fCrystalSizeBCmd->SetGuidance("Define the width/height of the end of the crystal.");
  // fCrystalSizeBCmd->SetParameterName("choice", true);
  fCrystalSizeBCmd->SetDefaultValue(crystalSizeBCmdDefaultValue);
  fCrystalSizeBCmd->AvailableForStates(G4State_PreInit);

  fCrystalLengthCmd = new G4UIcmdWithADoubleAndUnit("/detector/crystalLength", this);
  fCrystalLengthCmd->SetGuidance("Define the length of the crystal.");
  // fCrystalLengthCmd->SetParameterName("choice", true);
  fCrystalLengthCmd->SetDefaultValue(crystalLengthCmdDefaultValue);
  fCrystalLengthCmd->AvailableForStates(G4State_PreInit);

  fLightGuideLengthCmd = new G4UIcmdWithADoubleAndUnit("/detector/lightGuideLength", this);
  fLightGuideLengthCmd->SetGuidance("Define the length of the light guide.");
  // fLightGuideLengthCmd->SetParameterName("choice", true);
  fLightGuideLengthCmd->SetDefaultValue(lightGuideLengthCmdDefaultValue);
  fLightGuideLengthCmd->AvailableForStates(G4State_PreInit);

  fDetectorTypeCmd = new G4UIcmdWithAString("/detector/detectorType", this);
  fDetectorTypeCmd->SetGuidance("Define model of the sensitive detector.");
  // fDetectorTypeCmd->SetParameterName("choice", true);
  fDetectorTypeCmd->SetDefaultValue("");
  fDetectorTypeCmd->AvailableForStates(G4State_PreInit);

//  G4UIcmdWithADouble*          fCrystalSizeBCmd;
//  G4UIcmdWithADouble*          fCrystalLengthBCmd;
//  G4UIcmdWithADouble*          fLightGuideLengthBCmd;
//  G4UIcmdWithAString*          fSiPmTypeCmd;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorMessenger::~DetectorMessenger() {
  delete fDetectorDir;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorMessenger::SetNewValue(G4UIcommand *command, G4String newValue) {
  // Select the type of reflection at ground
  if (command == fCrystalSizeACmd) {
    fDetector->setCrystalSideA(G4UIcmdWithADoubleAndUnit::GetNewDoubleValue(newValue));
  } else if (command == fCrystalSizeBCmd) {
    fDetector->setCrystalSideB(G4UIcmdWithADoubleAndUnit::GetNewDoubleValue(newValue));
  } else if (command == fCrystalLengthCmd) {
    fDetector->setCrystalLength(G4UIcmdWithADoubleAndUnit::GetNewDoubleValue(newValue));
  } else if (command == fLightGuideLengthCmd) {
    fDetector->setLightGuideLength(G4UIcmdWithADoubleAndUnit::GetNewDoubleValue(newValue));
  }

}
