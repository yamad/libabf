//**********************************************************************************************
//
//    Copyright (c) 2002 Axon Instruments.
//    All rights reserved.
//
//**********************************************************************************************
// MODULE:  PopulateEpoch.cpp
// PURPOSE: Contains functions that populate each epoch type.
// AUTHOR:  GRB  Mar 2002
//

#ifndef INC_POPULATEEPOCH_HPP
#define INC_POPULATEEPOCH_HPP

void PopulateStep( int nDuration, double dLevel, double * pdValue );
void PopulateRamp( int nDuration, double dStartLevel, double dLevel, double * pdValue );
void PopulateCosine( int nDuration, double dStartLevel, double dLevel, int nPeriod, double * pdValue );
void PopulateRectangle( int nDuration, double dStartLevel, double dLevel, int nPeriod, int nWidth, double * pdValue );
void PopulateBiphasic( int nDuration, double dStartLevel, double dLevel, int nPeriod, int nWidth, double * pdValue );
void PopulateTriangle( int nDuration, double dStartLevel, double dLevel, int nPeriod, int nWidth, double * pdValue );
void PopulateResistance( int nDuration, double dLevel, double dHolding, double * pdValue );

#endif   // INC_POPULATEEPOCH_HPP