//--------------------------------------------------------------------------------------------------
// $Id: FatJetExtenderMod.h,v 1.9 2011/03/01 17:27:22 mzanetti Exp $
//
// FatJetExtender
//
// This module processes a collection of input fatjets, compute the substrucure
// and fill a output collections of fXlFatJets
//
// Authors: L.DiMatteo, S.Narayanan
//--------------------------------------------------------------------------------------------------

#ifndef MITPHYSICS_MODS_FATJETEXTENDER_H
#define MITPHYSICS_MODS_FATJETEXTENDER_H

#include <TVector2.h>

#include "fastjet/PseudoJet.hh"
#include "fastjet/JetDefinition.hh"
#include "fastjet/GhostedAreaSpec.hh"
#include "fastjet/AreaDefinition.hh"
#include "fastjet/ClusterSequenceArea.hh"
#include "fastjet/tools/Pruner.hh"
#include "fastjet/tools/Filter.hh"

#include "MitAna/DataTree/interface/XlFatJetFwd.h"
#include "MitAna/DataTree/interface/XlFatJet.h"
#include "MitAna/DataTree/interface/XlSubJetFwd.h"

#include "MitAna/TreeMod/interface/BaseMod.h"
#include "MitAna/DataTree/interface/JetCol.h"
#include "MitAna/DataTree/interface/FatJetCol.h"
#include "MitAna/DataTree/interface/PFCandidateCol.h"
#include "MitAna/DataTree/interface/PFJet.h"
#include "MitAna/DataTree/interface/PileupEnergyDensityCol.h"
#include "MitAna/DataTree/interface/VertexCol.h"
#include "MitAna/PhysicsUtils/interface/QGTagger.h"

#include "MitPhysics/SDAlgorithm/interface/AnalysisParameters.h"
#include "MitPhysics/SDAlgorithm/interface/Exception.h"
#include "MitPhysics/SDAlgorithm/interface/Message.h"
#include "MitPhysics/SDAlgorithm/interface/TopGluonModel.h"
#include "MitPhysics/SDAlgorithm/interface/BackgroundModel.h"
#include "MitPhysics/SDAlgorithm/interface/ISRModel.h"
#include "MitPhysics/SDAlgorithm/interface/Deconstruct.h"
#include "MitPhysics/SDAlgorithm/interface/ParseUtils.h"

namespace mithep
{
  class FatJetExtenderMod : public BaseMod
  {
    public:
      typedef XlSubJet::ESubJetType ESubJetType;
      FatJetExtenderMod(const char *name = "FatJetExtenderMod",
                   const char *title = "XlFatJets Filler module");
      ~FatJetExtenderMod();

      void IsData(Bool_t b)                { fIsData = b;           }
      void SetQGTaggingOn(Bool_t b)        { fQGTaggingActive = b;  }
      void SetSubJetTypeOn(ESubJetType t) { fSubJetFlags |= (1<<t); }
      void SetSubJetTypeOff(ESubJetType t) { fSubJetFlags &= ~(1<<t); }
      void SetQGTaggerCHS(bool b)          { fQGTaggerCHS = b;      }
      void PublishOutput(Bool_t b)         { fPublishOutput = b;    }

      void SetProcessNJets(UInt_t n)       { fProcessNJets = n;     }

      void SetInputName(const char *n)      { fFatJetsName = n;         }
      void SetInputFromBranch(Bool_t b)     { fFatJetsFromBranch = b;   }

      void SetOutputName(const char *n)   { fXlFatJetsName = n;    }
      const char * GetOutputName()        { return fXlFatJetsName;  }

      void SetSoftDropZCut(double d)       { fSoftDropZCut = d;     }
      void SetSoftDropR0(double d)      { fSoftDropR0 = d;    }
      void SetPruneZCut(double d)          { fPruneZCut = d;        }
      void SetPruneDistCut(double d)       { fPruneDistCut = d;     }
      void SetFilterN(int n)               { fFilterN = n;          }
      void SetFilterRad(double d)          { fFilterRad = d;        }
      void SetTrimRad(double d)            { fTrimRad = d;          }
      void SetTrimPtFrac(double d)         { fTrimPtFrac = d;       }
      void SetConeSize(double d)           { fConeSize = d;         }
      void SetPFCandsName(const char *n)   { fPFCandidatesName = n; }
      void SetPileUpDenName(const char *n) { fPileUpDenName = n;    }
      void SetVertexesName(const char *n)  { fVertexesName = n;     }

    protected:
      void Process();
      void SlaveBegin();
      void SlaveTerminate();

      void FillXlFatJet (const FatJet *fatJet);
      void FillXlSubJets(std::vector<fastjet::PseudoJet> &fjSubJets, XlFatJet *pFatJet, XlSubJet::ESubJetType subJetType);

      // Jet collection helpers
      std::vector <fastjet::PseudoJet>
            Sorted_by_pt_min_pt(std::vector <fastjet::PseudoJet> &jets,
                                float jetPtMin);
      // Subjet QGTagging helpers
      void   FillSubjetQGTagging(fastjet::PseudoJet &jet, float constitsPtMin,
                                 XlSubJet *pSubJet, XlFatJet *pFatJet);
      // Color pull helpers
      TVector2 GetPull(fastjet::PseudoJet &jet, float constitsPtMin);
      double GetPullAngle(std::vector<fastjet::PseudoJet> &fjSubJets, float constitsPtMin);
      double fMicrojetR0 = -1.0;

      double GetQjetVolatility (std::vector<fastjet::PseudoJet> &constits, int QJetsN = 25, int seed = 12345);
      void GetJetConstituents(fastjet::PseudoJet&, std::vector<fastjet::PseudoJet>&, float);
      double FindRMS(std::vector<float>);
      double FindMean(std::vector<float>);

      Vect4M GetCorrectedMomentum(fastjet::PseudoJet fj_tmp, double thisJEC);

    private:
      Bool_t fIsData;                      //is this data or MC?
      Bool_t fQGTaggingActive;             //=true if QGTagging info is filled
      Bool_t fQGTaggerCHS;                 //=true if QGTagging weights are taken from CHS
      Bool_t fPublishOutput;               //=true if output collection are published

      TString fFatJetsName;                   //(i) name of input jets
      Bool_t fFatJetsFromBranch;              //are input jets from Branch?
      const JetCol *fFatJets;                 //input jets

      TString fPFCandidatesName;           //(i) name of PF candidates coll
      Bool_t fPFCandidatesFromBranch;
      const PFCandidateCol *fPFCandidates; //particle flow candidates coll handle

      TString fPileUpDenName;              //(i) name of PU energy density coll
      Bool_t fPileUpDenFromBranch;         //is PU energy density from Branch?
      const PileupEnergyDensityCol *fPileUpDen; //PU energy density coll handle

      TString fVertexesName;               //(i) name of vertex coll
      Bool_t fVertexesFromBranch;          //are vertexex from Branch?
      const VertexCol *fVertexes;          //vertex coll handle

      TString fXlFatJetsName;              //name of output fXlFatJets collection
      XlFatJetArr *fXlFatJets;             //array of fXlFatJets

      // Objects from fastjet we want to use
      fastjet::Pruner *fPruner;
      fastjet::Filter *fFilterer;
      fastjet::Filter *fTrimmer;           //no this is not a typo trimmers belong to fastjet Filter class
      double fSoftDropZCut;                //soft-drop Z cut
      double fSoftDropR0;               //soft-drop angular distance normalisation
      double fPruneZCut;                   //pruning Z cut
      double fPruneDistCut;                //pruning distance cut
      int fFilterN;                        //number of subjets after filtering
      double fFilterRad;                   //filtered subjets radius
      double fTrimRad;                     //trimmed subjet radius
      double fTrimPtFrac;                  //trimmed subjet pt fraction
      double fConeSize;                    //fastjet clustering radius
      fastjet::JetDefinition *fCAJetDef;   //fastjet clustering definition
      fastjet::GhostedAreaSpec *fActiveArea;
      fastjet::AreaDefinition *fAreaDefinition;

      unsigned short fSubJetFlags = 1;    // flags turning on subjet types

      TString fXlSubJetsName[XlSubJet::nSubJetTypes];              //name of output fXlSubJets collection
      XlSubJetArr *fXlSubJets[XlSubJet::nSubJetTypes];             //array of fXlSubJets
      
      Deconstruction::Deconstruct *fDeconstruct;
      AnalysisParameters *fParam;
      Deconstruction::TopGluonModel *fSignal;
      Deconstruction::BackgroundModel *fBackground;
      Deconstruction::ISRModel *fISR; 
      
      UInt_t fProcessNJets;

      Bool_t fDoShowerDeconstruction;

      // QG tagger
      QGTagger *fQGTagger;                 //QGTagger calculator

      // Counters : used to initialize seed for QJets volatility
      Long64_t fCounter;

      ClassDef(FatJetExtenderMod, 0)         //XlJets, Fat and Sub, filler
  };
}
#endif
