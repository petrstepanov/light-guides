/*
 * DetectorProvider.hh
 *
 *  Created on: Sep 10, 2022
 *      Author: petrstepanov
 */

#ifndef SRC_DETECTORS_DETECTORPROVIDER_HH_
#define SRC_DETECTORS_DETECTORPROVIDER_HH_

#include "AbsDetector.hh"
#include <G4String.hh>

class DetectorProvider {
  public:
    DetectorProvider();
    virtual ~DetectorProvider();

    static AbsDetector* getDetector(G4String);
};

#endif /* SRC_DETECTORS_DETECTORPROVIDER_HH_ */
