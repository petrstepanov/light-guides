/*
 * DetectorMessenger.h
 *
 *  Created on: Sep 7, 2022
 *      Author: petrstepanov
 */

#ifndef SRC_DETECTORMESSENGER_H_
#define SRC_DETECTORMESSENGER_H_

#include <G4UImessenger.hh>

#include <globals.hh>
#include <G4SystemOfUnits.hh>
#include <G4UIcmdWithADoubleAndUnit.hh>
#include <G4UIcmdWithAString.hh>

class DetectorConstruction;
class G4UIdirectory;


class DetectorMessenger: public G4UImessenger
{
	friend class DetectorConstruction;

public:

  void SetNewValue(G4UIcommand*, G4String);

private:

  DetectorMessenger(DetectorConstruction* );
  ~DetectorMessenger();

  DetectorConstruction*        fDetector;           // pointer to detector
  G4UIdirectory*               fDetectorDir;
  G4UIcmdWithADoubleAndUnit*   fCrystalSizeACmd;
  G4UIcmdWithADoubleAndUnit*   fCrystalSizeBCmd;
  G4UIcmdWithADoubleAndUnit*   fCrystalLengthCmd;
  G4UIcmdWithAString*          fDetectorTypeCmd;

public:
  // Command default values
  static constexpr G4double crystalSizeACmdDefaultValue = 4*cm;
  static constexpr G4double crystalSizeBCmdDefaultValue = 6*cm;
  static constexpr G4double crystalLengthCmdDefaultValue = 40*cm;
  static const G4String detectorTypeCmdDefaultValue;
};
#endif /* SRC_DETECTORMESSENGER_H_ */
