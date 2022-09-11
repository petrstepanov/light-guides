/*
 * S13360_6025CS.hh
 *
 *  Created on: Feb 22, 2021
 *      Author: petrstepanov
 */

#ifndef INCLUDE_S13360_6025CS_HH_
#define INCLUDE_S13360_6025CS_HH_

#include "AbsMPPC.hh"

class S13360_6025CS : public AbsMPPC {
public:
	S13360_6025CS();
	virtual ~S13360_6025CS();

	G4String getName();
	G4double getSize();
	G4double getLength();
	G4double getWindowThickness();
	G4double getCathodeThickness();
	G4Material* getWindowMaterial();
	G4Material* getCathodeMaterial();
};

#endif /* INCLUDE_S13360_6025CS_HH_ */
