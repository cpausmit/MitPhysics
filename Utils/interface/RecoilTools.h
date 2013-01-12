//--------------------------------------------------------------------------------------------------
// $Id $
//
// Recoil Tools
//
// Helper Class for Recoil Tools
//
// Authors: P. Harris
//--------------------------------------------------------------------------------------------------

#ifndef MITPHYSICS_UTILS_RecoilTools_H
#define MITPHYSICS_UTILS_RecoilTools_H

#include "MitAna/DataTree/interface/PFJetFwd.h"
#include "MitAna/DataTree/interface/VertexFwd.h"
#include "MitAna/DataTree/interface/TrackFwd.h"
#include "MitAna/DataTree/interface/PFMet.h"
#include "MitAna/DataTree/interface/PFMetCol.h"
#include "MitAna/DataTree/interface/PFJet.h"
#include "MitAna/DataTree/interface/PFJetCol.h"
#include "MitAna/DataTree/interface/PFCandidateCol.h"
#include "MitAna/DataTree/interface/PileupEnergyDensityCol.h"
#include "MitCommon/MathTools/interface/MathUtils.h"

#include "MitPhysics/Utils/interface/JetIDMVA.h"

class TRandom3;

namespace mithep {
  class RecoilTools {
    public:
    RecoilTools(TString iJetLowPtMVAFile ="$CMSSW_BASE/src/MitPhysics/data/mva_JetID_lowpt.weights.xml",
		TString iJetHighPtMVAFile="$CMSSW_BASE/src/MitPhysics/data/mva_JetID_highpt.weights.xml",
		TString iCutFile         ="$CMSSW_BASE/src/MitPhysics/Utils/python/JetIdParams_cfi.py",
		bool i42=false,JetIDMVA::MVAType iType=JetIDMVA::kBaseline);
    ~RecoilTools();
    JetIDMVA *fJetIDMVA;

    Met pfRecoil(Double_t iVisPt,Double_t iVisPhi,Double_t iVisSumEt,const PFCandidateCol *iCands);
    void addType1(FourVectorM &iVec,Double_t &iSumEt,
                  const PFJetCol *iJets,FactorizedJetCorrector *iJetCorrector,const PileupEnergyDensityCol *iPUEnergyDensity,
                  double iPhi1,double iEta1,double iPhi2,double iEta2);

    Met pfRecoilType1(Double_t iVisPt,Double_t iVisPhi,Double_t iVisSumEt,
                      const PFCandidateCol *iCands,const PFJetCol *iJets,
                      FactorizedJetCorrector *iJetCorrector,const PileupEnergyDensityCol *iPUEnergyDensity,
                      double iPhi1,double iEta1,double iPhi2,double iEta2);

    //Candidate filtered
    Met pfRecoil(double iPhi1,double iEta1,double iPhi2,double iEta2,const PFCandidateCol *iCands);
    Met pfCone  (double iPhi1,double iEta1,const PFCandidateCol *iCands,const Vertex *iVertex,bool iCharge=false,Double_t iDZCut=0.3);

    Met trackMet(const PFCandidateCol *iCands,const Vertex *iVertex,Double_t iDZCut=0.1);

    //Candidate filtered
    Met trackRecoil(double iPhi1,double iEta1,double iPhi2,double iEta2,
		    const PFCandidateCol *iCands,const Vertex *iVertex,Double_t iDZCut=0.1);

    Met trackRecoil(Double_t iVisPt,Double_t iVisPhi,Double_t iVisSumEt,
			     const PFCandidateCol *iCands,const Vertex *iVertex,double iDZCut=0.1);

    bool filter (const PFJet *iJet,Double_t iPhi1,Double_t iEta1,Double_t iPhi2,Double_t iEta2);

    bool filter(const PFCandidate *iCand,Double_t iPhi1,Double_t iEta1,Double_t iPhi2,Double_t iEta2);
    //Uncorrected Jets
    void addNeut(const PFJet *iJet,FourVectorM &iVec,Double_t &iSumEt, 
		 FactorizedJetCorrector *iJetCorrector,const PileupEnergyDensityCol *iPUEnergyDensity,
		 int iSign=1);

    //Corrected Jets
    void addNeut(const PFJet *iJet,FourVectorM &iVec,Double_t &iSumEt,Double_t iRho,
		 int iSign=1);
    
    //Uncorrected Jets
    Met NoPUMet( const PFJetCol       *iJets,FactorizedJetCorrector *iJetCorrector,
		 const PileupEnergyDensityCol *iPileupEnergyDensity,
		 const PFCandidateCol *iCands,const Vertex *iVertex,const VertexCol *iVertices,
		 Double_t iPhi1=1000,Double_t iEta1=1000,Double_t iPhi2=1000,Double_t iEta2=1000,
		 Double_t iDZCut=0.1);
    //Corrected Jets
    Met NoPUMet( const PFJetCol       *iJets,
		 const PFCandidateCol *iCands,const Vertex *iVertex,const VertexCol *iVertices,Double_t iRho,
		 Double_t iPhi1=1000,Double_t iEta1=1000,Double_t iPhi2=1000,Double_t iEta2=1000,
		 Double_t iDZCut=0.1);

    //Candidate filtered
    Met NoPURecoil(Double_t iPhi1,Double_t iEta1,Double_t iPhi2,Double_t iEta2,
		   const PFJetCol            *iJets,
		   const PFCandidateCol   *iCands   ,
		   const Vertex *iVertex,const VertexCol *iVertices,
		   FactorizedJetCorrector *iJetCorrector=0,
		   const PileupEnergyDensityCol *iPileupEnergyDensity=0,Double_t iDZCut=0.1);
    //UnCorrrected Jets
    Met NoPURecoil(Double_t iVisPt,Double_t iVisPhi,Double_t iVisSumEt,   
		   const PFJetCol       *iJets,FactorizedJetCorrector *iJetCorrector,
		   const PileupEnergyDensityCol *iPileupEnergyDensity,
		   const PFCandidateCol *iCands,const Vertex *iVertex,const VertexCol *iVertices,
		   Double_t iPhi1=1000,Double_t iEta1=1000,Double_t iPhi2=1000,Double_t iEta2=1000,
		   Double_t iDZCut=0.1);

    //Corrrected Jets
    Met NoPURecoil(Double_t iVisPt,Double_t iVisPhi,Double_t iVisSumEt,   
		   const PFJetCol       *iJets,
		   const PFCandidateCol *iCands,const Vertex *iVertex,const VertexCol *iVertices,Double_t iRho,
		   Double_t iPhi1=1000,Double_t iEta1=1000,Double_t iPhi2=1000,Double_t iEta2=1000,
		   Double_t iDZCut=0.1);

    //Uncorrected Jets
    Met PUCMet( const PFJetCol       *iJets,FactorizedJetCorrector *iJetCorrector,
		const PileupEnergyDensityCol *iPileupEnergyDensity,
		const PFCandidateCol *iCands,const Vertex *iVertex,const VertexCol *iVertices,
		Double_t iPhi1=1000,Double_t iEta1=1000,Double_t iPhi2=1000,Double_t iEta2=1000,
		Double_t iDZCut=0.1);

    //Corrected Jets
    Met PUCMet( const PFJetCol       *iJets,
		const PFCandidateCol *iCands,const Vertex *iVertex,const VertexCol *iVertices,Double_t iRho,
		Double_t iPhi1=1000,Double_t iEta1=1000,Double_t iPhi2=1000,Double_t iEta2=1000,
		Double_t iDZCut=0.1);

    //Candidate Filtered
    Met PUCRecoil( Double_t iPhi1,Double_t iEta1,Double_t iPhi2,Double_t iEta2,
		   const PFJetCol            *iJets,
		   const PFCandidateCol      *iCands,
		   const Vertex *iVertex,const VertexCol *iVertices,
		   FactorizedJetCorrector *iJetCorrector=0,
		   const PileupEnergyDensityCol *iPileupEnergyDensity=0,Double_t iDZCut=0.1);

    //Uncorrected Jets
    Met PUCRecoil(Double_t iVisPt,Double_t iVisPhi,Double_t iVisSumEt,
		  const PFJetCol       *iJets,FactorizedJetCorrector *iJetCorrector,
		  const PileupEnergyDensityCol *iPileupEnergyDensity,
		  const PFCandidateCol *iCands,const Vertex *iVertex,const VertexCol *iVertices,
		  Double_t iPhi1=1000,Double_t iEta1=1000,Double_t iPhi2=1000,Double_t iEta2=1000,
		  Double_t iDZCut=0.1);

    //Corrected Jets
    Met PUCRecoil(Double_t iVisPt,Double_t iVisPhi,Double_t iVisSumEt,
		  const PFJetCol       *iJets,
		  const PFCandidateCol *iCands,const Vertex *iVertex,const VertexCol *iVertices,Double_t iRho,
		  Double_t iPhi1=1000,Double_t iEta1=1000,Double_t iPhi2=1000,Double_t iEta2=1000,
		  Double_t iDZCut=0.1);

    //Uncorrected Jets
    Met PUMet( const PFJetCol       *iJets,FactorizedJetCorrector *iJetCorrector,
	       const PileupEnergyDensityCol *iPileupEnergyDensity,
	       const PFCandidateCol *iCands,const Vertex *iVertex,const VertexCol *iVertices,
	       Double_t iPhi1=1000,Double_t iEta1=1000,Double_t iPhi2=1000,Double_t iEta2=1000,
	       Double_t iDZCut=0.2);

    //Corrected Jets
    Met PUMet( const PFJetCol       *iJets,
	       const PFCandidateCol *iCands,const Vertex *iVertex,const VertexCol *iVertices,Double_t iRho,
	       Double_t iPhi1=1000,Double_t iEta1=1000,Double_t iPhi2=1000,Double_t iEta2=1000,
	       Double_t iDZCut=0.2);

    //Candidate Filtered
    Met PUMet( Double_t iPhi1,Double_t iEta1,Double_t iPhi2,Double_t iEta2,
	       const PFJetCol            *iJets,
	       const PFCandidateCol      *iCands,
	       const Vertex *iVertex,const VertexCol *iVertices,
	       FactorizedJetCorrector *iJetCorrector=0,
	       const PileupEnergyDensityCol *iPileupEnergyDensity=0,Double_t iDZCut=0.2);

    //Uncorrected Jets
    Met PURecoil(Double_t iVisPt,Double_t iVisPhi,Double_t iVisSumEt,
		 const PFJetCol       *iJets,FactorizedJetCorrector *iJetCorrector,
		 const PileupEnergyDensityCol *iPileupEnergyDensity,
		 const PFCandidateCol *iCands,const Vertex *iVertex,const VertexCol *iVertices,
		 Double_t iPhi1=1000,Double_t iEta1=1000,Double_t iPhi2=1000,Double_t iEta2=1000,
		 Double_t iDZCut=0.1);

    //Corrected Jets
    Met PURecoil(Double_t iVisPt,Double_t iVisPhi,Double_t iVisSumEt,
		 const PFJetCol       *iJets,
		 const PFCandidateCol *iCands,const Vertex *iVertex,const VertexCol *iVertices,Double_t iRho,
		 Double_t iPhi1=1000,Double_t iEta1=1000,Double_t iPhi2=1000,Double_t iEta2=1000,
		 Double_t iDZCut=0.1);
    
    bool f42;
    ClassDef(RecoilTools, 0) // Recoil tools
  };
}
#endif
