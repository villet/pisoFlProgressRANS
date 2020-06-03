### Date created
2010-06-02

### Changelog

#### 1.0.0 - 2020-06-02

Added original source code

## RANS flamelet/progress variable solver for OpenFOAM

## Description

This solver is part of my research work on reactive flows and LES in 2005-2010. It is based on the work by [Charles Pierce (2001)](https://web.stanford.edu/group/ctr/pdf/charles_pierce_thesis.pdf).

### Requirements

The solver was developed for [OpenFOAM 1.7.x](https://github.com/OpenCFD/OpenFOAM-1.7.x). Modifications for OpenFOAM 2.x and newer version are required since the structure of thermodynamics library was changed.

Additionally, [FlameMaster package](https://www.itv.rwth-aachen.de/downloads/flamemaster/) or similar flamelet calculation software is needed to create a thermodynamics library.

### Additional Sources

1. The OpenFOAM Foundation https://openfoam.org

2. Progress-Variable Approach for Large-Eddy Simulation of Turbulent Combustion, Charles Pierce, Ph.D. Thesis, Stanford University, 2001 https://web.stanford.edu/group/ctr/pdf/charles_pierce_thesis.pdf

3. FlameMaster website https://www.itv.rwth-aachen.de/downloads/flamemaster/
