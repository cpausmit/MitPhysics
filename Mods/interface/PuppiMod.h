//--------------------------------------------------------------------------------------------------
// PuppiMod
//
// This mod makes Puppi weights
//
// Authors: D.Abercrombie
//--------------------------------------------------------------------------------------------------

#ifndef MITPHYSICS_MODS_PUPPIMOD_H
#define MITPHYSICS_MODS_PUPPIMOD_H

#include "MitAna/TreeMod/interface/BaseMod.h" 
#include "MitAna/DataTree/interface/VertexFwd.h"
#include "MitAna/DataTree/interface/PFCandidateFwd.h"
#include "MitPhysics/Utils/interface/ParticleMapper.h"

namespace mithep 
{
  class PuppiMod : public BaseMod
  {
    public:
      PuppiMod( const char *name="PuppiMod", 
                const char *title="Puppi module" );
     ~PuppiMod();

      const char   *GetVertexesName()              const     { return fVertexesName;           }
      const char   *GetInputName()                 const     { return fPFCandidatesName;       }   
      const char   *GetOutputName()                const     { return fPuppiParticlesName;     }
      void SetEtaConfigName( const char *name )              { fEtaConfigName = name;          }
      void SetVertexesName( const char *name )               { fVertexesName = name;           }
      void SetInputName( const char *name )                  { fPFCandidatesName = name;       }
      void SetOutputName( const char *name )                 { fPuppiParticlesName = name;     }

      void SetRMin( Double_t RMin )                          { fRMin = RMin;                   }
      void SetR0( Double_t R0 )                              { fR0 = R0;                       }
      void SetAlpha( Double_t Alpha )                        { fAlpha = Alpha;                 }
      void SetBeta( Double_t Beta )                          { fBeta = Beta;                   }
      
      void SetD0Cut( Double_t cut )                          { fD0Cut = cut;                   }
      void SetDZCut( Double_t cut )                          { fDZCut = cut;                   }

      void SetMinWeightCut( Double_t cut )                   { fMinWeightCut = cut;            }

      void SetRMSScaleFactor( Double_t fact )                { fRMSScaleFactor = fact;         }
      void SetTrackUncertainty( Double_t sig )               { fTrackUncertainty = sig;        }

      void SetKeepPileup( Bool_t keep )                      { fKeepPileup = keep;             }
      void SetInvert( Bool_t invert )                        { fInvert = invert;               }
      void SetApplyCHS( Bool_t apply )                       { fApplyCHS = apply;              }
      void SetApplyLowPUCorr( Bool_t apply )                 { fApplyLowPUCorr = apply;        }
      void SetUseEtaForAlgo( Bool_t use )                    { fUseEtaForAlgo = use;           }
      void SetEtaForAlgo( Double_t eta )                     { fEtaForAlgo = eta;              }
      void SetDump( Bool_t dump )                            { fDumpingPuppi = dump;           }

    protected:
      void                  SlaveBegin() override;
      void                  SlaveTerminate() override;
      void                  Process() override;

      enum ParticleType {
        kChargedPrimary = 1,
        kChargedPU,
        kNeutralCentral,
        kNeutralForward
      };

      ParticleType GetParticleType( const PFCandidate *cand, Vertex const* ) const;
      Int_t GetEtaBin( const PFCandidate *cand ) const;
      Double_t Chi2fromDZ( Double_t dz ) const;
      
      TString               fEtaConfigName;          // Name of the configuration file with eta tables
      TString               fVertexesName;           // Name of vertices collection used for PV
      TString               fPFCandidatesName;       // Name of PFCandidate collection (input)
      TString               fPuppiParticlesName;     // Name of Puppi Particle collection (output)

      PFCandidateArr       *fPuppiParticles;         // The output collection for publishing

      Double_t fRMin;                                // Minimum dR cut for summing up surrounding particles
      Double_t fR0;                                  // Maximum dR cut for summing up surrounding particles
      Double_t fAlpha;                               // Parameter for weighting pt of surrounding particles
      Double_t fBeta;                                // Parameter for weighting dR of surrounding particles

      Double_t fD0Cut;                               // D0 cut for charged particle vertex matching
      Double_t fDZCut;                               // DZ cut for charged particle vertex matching

      Double_t fMinWeightCut;                        // Minimum weight to drop it to zero

      Double_t fRMSScaleFactor;                      // A scale factor for RMS
      Double_t fTrackUncertainty;                    // The experimental uncertainty in the track fit to vertex distance

      Bool_t   fKeepPileup;                          // Keep pileup with zero weight (for debugging)
      Bool_t   fInvert;                              // Option to invert weights
      Bool_t   fApplyCHS;                            // This will force weights to 0 or 1 for tracked particles
      Bool_t   fApplyLowPUCorr;                      // This will cause a correction when lots of PV particles fall below median
      Bool_t   fUseEtaForAlgo;                       // Determines if you use eta cut or PFType to determine algorithm use
      Double_t fEtaForAlgo;                          // Eta cut to switch algorithms, if you want it
      Bool_t   fDumpingPuppi;                        // If this is true, we dump particle information and weights

      // These are parameters that are functions of Eta hopefully we can be more clever some day
      UInt_t fNumEtaBins;                             // This is the number of eta regions we are dividing into
      std::vector<Double_t> fMaxEtas;                // These are the maximum etas for each region of the table
      std::vector<Double_t> fMinPts;                 // Various Pt cuts
      std::vector<Double_t> fMinNeutralPts;          // Minimum Pt cut on neutral particles (after weighting)
      std::vector<Double_t> fMinNeutralPtSlopes;     // Predicted slope of neutral particles as function of PU
      std::vector<Double_t> fRMSEtaSFs;              // Scale factor for RMS as function of Eta
      std::vector<Double_t> fMedEtaSFs;              // Scale factor for median as a function of Eta
      std::vector<Double_t> fEtaMaxExtraps;          // I think this is the maximum eta to calculate median alphas?

      ParticleMapper* fMapper;                        // For efficient determination of particle proximity

      ClassDef(PuppiMod, 1)                          // Puppi module
  };
}
#endif
