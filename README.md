# Geant4 Simulation for the EIC Light Guides

Program evaluates the best length of a lightguide required to obtain maximum light yield from a scintillation-based detector. These studies are rested by the EIC barrel calorimeter engineering group (Rolf Ent, Elke Aschenauer).

## Key Geometry Features

* Trapezoidal scintillation crystals. SciGlass material, size 4×4 → 6×6 cm. Length is 40 cm.
* Trapezoidal light guides made from PMMA material.
* Light guide material is PMMA, properties [found online](https://eljentechnology.com/images/products/data_sheets/Light_Guides_and_Acrylic_Plastic.pdf). 
* Crystal and light guide are wrapped in 65 micron ESR reflector.
* SciGlass material parameters taken from the [Glass Prototype](https://github.com/JeffersonLab/glass-prototype) simulation.
* Sensitive detector part is 6x6 array of [S13360_6025CS SiPMs](https://www.hamamatsu.com/content/dam/hamamatsu-photonics/sites/documents/99_SALES_LIBRARY/ssd/s13360_series_kapd1052e.pdf) with silicone resin window. 

![Geant4 detector geometry](https://raw.githubusercontent.com/petrstepanov/light-guides/main/resources/light-guides-detector.png "Geant4 detector geometry")
<center>*Detector geometry. Top: 5 cm light guide. Bottom: no light guide.*</center>

## Validating Photon Trajectories

Test simulation visualizing thee Optical Photon (OP) trajectories is carried out to ensure reasonable detector geometry and data output.

* 500 OP emitted from the center of the crystal (1 event).
* OP energy distribution histogram for the General Particle Source (GPS) is generated from the SciGlass emission spectrum.
* Number of Photo-electrons (PE) obtained from detector Quantum Efficiency curve.
* OP emitted from a single point inside the crystal (approximately center).

![Geant4 simulation visualization](https://raw.githubusercontent.com/petrstepanov/light-guides/main/resources/test.png "Geant4 simulation visualization")
*Left: Visual validation of the detector geometry, 500 OP emitted. Right: Obtaining number of PE per run via the efficienct curve.*

## Data Analysis

Series of simulations for a set of different light guide length are carried out. 

* 4 GeV electrons are injected into the scintillation crystal in longitudinal direction (500 events).
* Following light guide length are studies: 0, 5, ...(step 5 mm), … 60, 80, and 100 mm.
* Crystal Ball function used to fit distributions of registered PE/event.
* Optical resolutions are calculated for each simulation run.

![Optical resolutions of wave-guides of dofferent lengths](https://raw.githubusercontent.com/petrstepanov/light-guides/main/resources/optical-resolutions.png "Optical resolutions of wave-guides of dofferent lengths")
*Distributions of registered number of PE/Event • 500 events per run •  8 out of 15 length presented*

Cumulative graphs of registered number of PE along with the optical resolution for each light guide length are presented below:

![Simulation results](https://raw.githubusercontent.com/petrstepanov/light-guides/main/resources/results.png "Simulation results")
*Registered number of PE/Event and corresponding optical resolution • 500 injection events per point.*

## Results

Optical simulations on the Farm for 0, 5, ...(step 5 mm), … 60, 80, and 100 mm light guide length are finished. 4 GeV electrons, trapezoidal SciGlass scintillation crystal, 6x6 SiPM matrix with 36x36 mm² surface area. ROOT plotting scripts are added to the Light Guides repository. Registered PE/Event and corresponding energy resolutions are plotted. ** Minimum reasonable light guide length is estimated to be 30 mm**.

