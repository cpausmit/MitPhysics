//-------------------
//
// Jet Tools
//
// S Markson
//
//-------------------

#ifndef MITPHYSICS_UTILS_JETTOOLS_H
#define MITPHYSICS_UTILS_JETTOOLS_H

#include "MitCommon/MathTools/interface/MathUtils.h"
#include "MitAna/DataUtil/interface/Debug.h"
#include "MitAna/DataTree/interface/Names.h"
#include "MitAna/DataTree/interface/ParticleCol.h"
#include "MitAna/DataTree/interface/JetCol.h"
#include "MitAna/DataTree/interface/Jet.h"
#include "MitPhysics/Init/interface/ModNames.h"
#include "MitAna/DataCont/interface/ObjArray.h"
#include "MitAna/DataTree/interface/TrackCol.h"
#include "MitAna/DataTree/interface/MetCol.h"
#include <TVector3.h>
#include <TLorentzVector.h>

namespace mithep {
  class JetTools {
    public:
      JetTools();
      virtual ~JetTools();

      static Double_t NJettiness(const ParticleOArr *particles, const JetOArr *jets, double Y = 0.0);
      static Double_t NJettiness(const TrackOArr *tracks, const JetOArr *jets, double Y = 0.0);
      static Double_t NJettiness(const JetOArr *jetsS, const JetOArr *jets, double Y = 0.0);
      static Double_t M_r(const ParticleOArr *particles);
      static Double_t Beta_r(const ParticleOArr *particles);
      static Double_t M_r_t(const ParticleOArr *particles, const Met *met);
      static Double_t Razor(const ParticleOArr *particles, const Met *met);
      static Double_t CosineOmega(const ParticleOArr *particles);
      static Double_t MtHiggs(const CompositeParticle *dilepton, const Met *met, int nsel);

      ClassDef(JetTools, 0)
  };
}

#endif