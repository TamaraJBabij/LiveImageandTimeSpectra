#pragma once

#include "TTree.h"
#include "TH1D.h"
#include "DataSet.h"
#include "HistogramPair.h"
#include "HistogramTimeSums.h"
#include "HistogramXY.h"
#include "histogramElecLayers.h"
#include "FitSet.h"
#include "Event.h"
#include "PitchPropSet.h"
#include "HistElecDifferences.h"
#include "calibrateLayersHist.h"


TTree* readWriteTree(TTree* tree);
DataSet* loadFromTree(TTree* tree);
void loadFromTreeDataSet(TTree* tree, DataSet* data);
TH1D* plotTimeSpectra(TTree* tree);
HistogramPair plotTimeSpectraDS(DataSet* data, enum imagingDetectors);
void constructEvents(DataSet* data);
HistogramTimeSums calculateTimeSums(DataSet* data, imagingDetectors userDet);
void plotTimeSums(HistogramTimeSums timesums);
void checkTimeSums(DataSet* data, FitSet fits, imagingDetectors userDet);
void identifyAndTime(DataSet* data);
void selectPosIonElec(DataSet* data);
void checkReconstructable(DataSet* data);
PitchPropSet calculatePitchProp(FitSet fits);
DataSet* sortReconData(DataSet* data);
void convertLayerPosition(DataSet* reconData, PitchPropSet Pitches, imagingDetectors userDet);
void convertCartesianPosition(DataSet* reconData, imagingDetectors userDet);
HistogramXY histogramXYPositions(DataSet *reconData);
HistogramElecLayers histogramElectronLayers(DataSet *reconData);
void differenceOfLayers(DataSet* reconData);
HistogramDIFF histogramElecLayerDiff(DataSet *reconData);
DataSet* scanFiles();
calibrateLayersHist histogramMaskLayers(DataSet *reconData);
