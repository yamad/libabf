//***************************************************************************************
//
//    Copyright (c) 1996-1999 Axon Instruments.
//    All rights reserved.
//
//***************************************************************************************
//
// MODULE:  ConstantsAndUnits.h
// PURPOSE: Header file containing physical constants and numerical units.
// AUTHOR:  BLB Jun 1996
//
// REFERENCES: Handbook of Chemistry and Physics. 64th Edition. CRC Press, Inc.
//
//***************************************************************************************

#if !defined __CONSTANTSANDUNITS_H__
#define      __CONSTANTSANDUNITS_H__

#include <math.h>

//=======================================================================================
// SI Numerical Prefixes.
const double cVendeka = 1.0E+33;
// const double ? = 1.0E+30;
const double cXenna   = 1.0E+27;
const double cYotta   = 1.0E+24;
const double cZetta   = 1.0E+21;
const double cEtta    = 1.0E+18;
const double cPetta   = 1.0E+15;
const double cTera    = 1.0E+12;
const double cGiga    = 1.0E+09;
const double cMega    = 1.0E+06;
const double cKilo    = 1.0E+03;

const double cMilli   = 1.0E-03;
const double cMicro   = 1.0E-06;
const double cNano    = 1.0E-09;
const double cPico    = 1.0E-12;
const double cFemto   = 1.0E-15;
const double cAtto    = 1.0E-18;
const double cZepto   = 1.0E-21;
const double cYocto   = 1.0E-24;
const double cZenno   = 1.0E-27;
//const double ? = 1.0E-30;
const double cVendeko = 1.0E-33;

//=======================================================================================
// Physical Constants                                         Units 

const double cFaradayConstant        = 9.648456E+04;       // C/mol
const double cGasConstantJoule       = 8.31441;            // J/mol-deg K
const double cGasConstantCalorie     = 1.9872;             // C/mol-deg K
const double cAbsoluteTemperature    = -273.15;            // degrees C

const double cElementaryCharge       = 1.6021892E-19;      // C
const double cPlanckConstant         = 6.626176E-34;       // J/Hz
const double cAvagadroNumber         = 6.022045E+23;       // mol^-1
const double cAtomicMassUint         = 1.6605655E-27;      // kg
const double cAtomicMassUnit         = 1.6605655E-27;      // kg
const double cElectronMass           = 0.9109534E-30;      // kg
const double cBoltzmanConstant       = 1.380662E-23;       // J/deg K
const double cVacuumPermittivity     = 8.85418782E-12;     // C/V-m
const double cVacuumPermeability     = 12.5663706144E-07;  // H/m
const double cRydbergConstant        = 1.097373177E+07;    // m^-1
const double cBohrRadius             = 0.52917706E-10;     // m
const double cComptonWavelength      = 2.4263089E-12;      // m
const double cStefanBoltzmanConstant = 5.67032E-08;        // W/m^2-K^4
const double cGravitationalConstant  = 6.6720E-11;         // N-m^2/kg^2
const double cSpeedOfLight           = 2.99792458E+08;     // m/s 

// Conversion Factors
const double cJoulesPerCalorie       = 4.19002;            // J
const double cCaloriesPerJoule       = 0.23866;            // C

// Miscellaneous
const double cRoomTemperature        = 20.0;               // deg C
const double cPi                     = 4.0 * atan(1.0);    // trigonometric Pi
const double ce                      = exp(1.0);           // Natural log base e.


// Other important constants (some no longer implemented).
// 
// ASF: Alan S. Finkel
// AJD: Andrew J. Dalgleish
// BHI: Bruce H. Ikin
// BLB: Borys L. Brezden
// JDH: Jack D. Herrington
// JL:  Jeff Liu
// JPK: Joe P. Kleinschmidt
// JT:  John Tyson
// PRC: Philip R. Churchward
// RHW: Robert H. Wicke
// RYS: Richard Y. Su
// GR:  Greg Richardson

#endif      //__CONSTANTSANDUNITS_H__
