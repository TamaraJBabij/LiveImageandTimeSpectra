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
#include "HistUVWPositions.h"
#include "histlayerSums.h"

TTree* readWriteTree(TTree* tree);
DataSet* loadFromTree(TTree* tree);
void loadFromTreeDataSet(TTree* tree, DataSet* data);
TH1D* plotTimeSpectra(TTree* tree);
void plotTimeSpectraDS(DataSet* data, imagingDetectors userDet, HistogramPair *histograms);
void constructEvents(DataSet* data);
HistogramTimeSums calculateTimeSums(DataSet* data, imagingDetectors userDet);
void plotTimeSums(HistogramTimeSums timesums);
void checkTimeSums(DataSet* data, FitSet fits, imagingDetectors userDet);
void identifyAndTime(DataSet* data);
void selectPosIonElec(DataSet* data);
void checkReconstructable(DataSet* data);
PitchPropSet calculatePitchProp();
DataSet* sortReconData(DataSet* data);
void convertLayerPosition(DataSet* reconData, PitchPropSet Pitches, imagingDetectors userDet, UVWpositionsHist *UVWPositions = NULL);
void convertCartesianPosition(DataSet* reconData, imagingDetectors userDet, HistogramXY* XYpositions = NULL, HistogramElecLayers * UVWlayers = NULL, calibrateLayersHist* UVWMasklayers = NULL);
void histogramXYPositions(DataSet *reconData, HistogramXY XYpositions);
void histogramElectronLayers(DataSet *reconData, HistogramElecLayers *hel, imagingDetectors userDet);
void differenceOfLayers(DataSet* reconData);
HistogramDIFF histogramElecLayerDiff(DataSet *reconData);
DataSet* scanFiles();
void histogramMaskLayers(DataSet *reconData, calibrateLayersHist *hml);
void histogramUVWPositions(DataSet *reconData, UVWpositionsHist *UVWPositions);
void positionsTreeToDataSet(TTree* tree, DataSet* data, imagingDetectors userDet);
PitchPropData getCalibrationParameters(DataSet* reconData, PitchPropSet initial, imagingDetectors userDet);
void createMaskDataTree(imagingDetectors userDet, HistogramXY* XYpositions, CalibrateLoadType sessionOption);
void checkDensity(DataSet* data, imagingDetectors userDet);
void histogramDensity(DataSet* data, imagingDetectors userDet, TH1I hist);
void calibrateFromSlope(DataSet* reconData, imagingDetectors userDet, layerDiffHist* diffHist);
