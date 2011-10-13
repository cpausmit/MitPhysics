// $Id: MVATools.cc,v 1.1 2011/10/13 17:28:29 mingyang Exp $

#include "MitPhysics/Utils/interface/PhotonTools.h"
#include "MitPhysics/Utils/interface/MVATools.h"
#include "MitPhysics/Utils/interface/ElectronTools.h"
#include "MitPhysics/Utils/interface/IsolationTools.h"
#include "MitAna/DataTree/interface/StableData.h"
#include <TFile.h>
#include <TRandom3.h>
#include "TMVA/Tools.h"//MVA
#include "TMVA/Reader.h"//MVA

ClassImp(mithep::MVATools)

using namespace mithep;


//--------------------------------------------------------------------------------------------------
MVATools::MVATools():
  fReaderEndcap(0),
  fReaderBarrel(0),
  
  //MVA variables 0
  HoE(0),
  covIEtaIEta(0),
  tIso1(0),
  tIso3(0),
  tIso2(0),
  R9(0),
  
  //MVA variables 1
  RelIsoEcal(0),
  RelIsoHcal(0),
  
  RelEMax(0),
  RelETop(0),
  RelEBottom(0),
  RelELeft(0),
  RelERight(0),
  RelE2x5Max(0),
  RelE2x5Top(0),
  RelE2x5Bottom(0),
  RelE2x5Left(0),
  RelE2x5Right(0),
  RelE5x5(0),
  
  //MVA variables 2
  EtaWidth(0),
  PhiWidth(0),
  CoviEtaiPhi(0), 
  CoviPhiiPhi(0),
  RelPreshowerEnergy(0)
  
{
  // Constructor.
}

//--------------------------------------------------------------------------------------------------
void MVATools::InitializeMVA(int VariableType, TString EndcapWeights,TString BarrelWeights) {
  
  if (fReaderEndcap) delete fReaderEndcap;  
  if (fReaderBarrel) delete fReaderBarrel; 
  
  fReaderEndcap = new TMVA::Reader( "!Color:!Silent:Error" );       
  fReaderBarrel = new TMVA::Reader( "!Color:!Silent:Error" );   
  
  TMVA::Reader *readers[2];
  readers[0]  = fReaderEndcap;
  readers[1]  = fReaderBarrel;  
  
  for (UInt_t i=0; i<2; ++i) {
    
    if(VariableType==0||VariableType==1||VariableType==2){
      readers[i]->AddVariable( "HoE", &HoE );
      readers[i]->AddVariable( "covIEtaIEta", &covIEtaIEta );
      readers[i]->AddVariable( "tIso1", &tIso1 );
      readers[i]->AddVariable( "tIso3", &tIso3 );
      readers[i]->AddVariable( "tIso2", &tIso2 );
      readers[i]->AddVariable( "R9", &R9 );
    }
    
    if(VariableType==1||VariableType==2){
      readers[i]->AddVariable( "RelIsoEcal", &RelIsoEcal);
      readers[i]->AddVariable( "RelIsoHcal", &RelIsoHcal);
      readers[i]->AddVariable( "RelEMax", &RelEMax);
      readers[i]->AddVariable( "RelETop",&RelETop);
      readers[i]->AddVariable( "RelEBottom", &RelEBottom);
      readers[i]->AddVariable( "RelELeft", &RelELeft );
      readers[i]->AddVariable( "RelERight", &RelERight);
      readers[i]->AddVariable( "RelE2x5Max", &RelE2x5Max );
      readers[i]->AddVariable( "RelE2x5Top", &RelE2x5Top );
      readers[i]->AddVariable( "RelE2x5Bottom", &RelE2x5Bottom );
      readers[i]->AddVariable( "RelE2x5Left", &RelE2x5Left );
      readers[i]->AddVariable( "RelE2x5Right", &RelE2x5Right );
      readers[i]->AddVariable( "RelE5x5", &RelE5x5 );
    }
    
    if(VariableType==2){
      readers[i]->AddVariable( "EtaWidth", &EtaWidth );
      readers[i]->AddVariable( "PhiWidth", &PhiWidth );
      readers[i]->AddVariable( "CoviEtaiPhi", &CoviEtaiPhi );
      readers[i]->AddVariable( "CoviPhiiPhi", &CoviPhiiPhi );
      if(i==0){
	readers[i]->AddVariable( "RelPreshowerEnergy", &RelPreshowerEnergy );
      }
    }
  }
  
  fReaderEndcap->BookMVA("BDT method",EndcapWeights);
  fReaderBarrel->BookMVA("BDT method",BarrelWeights);
 
  assert(fReaderEndcap);
  assert(fReaderBarrel);
  
}

//--------------------------------------------------------------------------------------------------

Bool_t MVATools::PassMVASelection(const Photon* p,const Vertex* vtx,const TrackCol* trackCol,const VertexCol* vtxCol,Double_t _tRho,const ElectronCol* els,double MVAPtMin, Float_t bdtCutBarrel, Float_t bdtCutEndcap) {
  
  // these values are taken from the H2GGlobe code... (actually from Marco/s mail)
  float cic4_allcuts_temp_sublead[] = { 
    3.8,         2.2,         1.77,        1.29,
    11.7,        3.4,         3.9,         1.84,
    3.5,         2.2,         2.3,         1.45,
    0.0106,      0.0097,      0.028,       0.027,
    0.082,       0.062,       0.065,       0.048,
    0.94,        0.36,        0.94,        0.32,
    1.,          0.062,       0.97,        0.97,
    1.5,         1.5,         1.5,         1.5 };  // the last line is PixelmatchVeto and un-used
  
  
  //initilize the bool value
  PassMVA=kFALSE;
  PassElecVeto=kFALSE;
  //Bool_t PassElecVeto=PhotonTools::PassElectronVeto(p,els);
  
  
  //get the variables used to compute MVA variables
  ecalIso3 = p->EcalRecHitIsoDr03();
  ecalIso4 = p->EcalRecHitIsoDr04();
  hcalIso4 = p->HcalTowerSumEtDr04();
  
  wVtxInd = 0;
  
  trackIso1 = IsolationTools::CiCTrackIsolation(p,vtx, 0.3, 0.02, 0.0, 0.0, 0.1, 1.0, trackCol);//Question Ming:whyfPV->At(0) instead of selected vertex using ranking method?
  
  // track iso only
  trackIso3 = IsolationTools::CiCTrackIsolation(p,vtx, 0.3, 0.02, 0.0, 0.0, 0.1, 1.0, trackCol);
  
  // track iso worst vtx
  trackIso2 = IsolationTools::CiCTrackIsolation(p,vtx, 0.4, 0.02, 0.0, 0.0, 0.1, 1.0, trackCol, &wVtxInd,vtxCol);
  
  combIso1 = ecalIso3+hcalIso4+trackIso1 - 0.17*_tRho;
  combIso2 = ecalIso4+hcalIso4+trackIso2 - 0.52*_tRho;
  
  RawEnergy = p->SCluster()->RawEnergy();
  
  dRTrack = PhotonTools::ElectronVetoCiC(p, els);
  
  //compute MVA variables 0
  HoE = p->HadOverEm();
  covIEtaIEta = p->CoviEtaiEta();
  tIso1 = (combIso1) *50./p->Et();
  tIso3 = (trackIso3)*50./p->Et();
  tIso2 = (combIso2) *50./(p->MomVtx(vtxCol->At(wVtxInd)->Position()).Pt());
  R9 = p->R9();
  
  //newly added MVA variables 1
  RelIsoEcal=(ecalIso3-0.17*_tRho)/p->Et();
  RelIsoHcal=(hcalIso4-0.17*_tRho)/p->Et();
  
  RelEMax=p->SCluster()->Seed()->EMax()/RawEnergy;
  RelETop=p->SCluster()->Seed()->ETop()/RawEnergy;
  RelEBottom=p->SCluster()->Seed()->EBottom()/RawEnergy;
  RelELeft=p->SCluster()->Seed()->ELeft()/RawEnergy;
  RelERight=p->SCluster()->Seed()->ERight()/RawEnergy;
  RelE2x5Max=p->SCluster()->Seed()->E2x5Max()/RawEnergy;
  RelE2x5Top=p->SCluster()->Seed()->E2x5Top()/RawEnergy;
  RelE2x5Bottom=p->SCluster()->Seed()->E2x5Bottom()/RawEnergy;
  RelE2x5Left=p->SCluster()->Seed()->E2x5Left()/RawEnergy;
  RelE2x5Right=p->SCluster()->Seed()->E2x5Right()/RawEnergy;
  RelE5x5=p->SCluster()->Seed()->E5x5()/RawEnergy;
  
  //newly added MVA variables 2
  EtaWidth=p->SCluster()->EtaWidth();
  PhiWidth=p->SCluster()->PhiWidth();
  CoviEtaiPhi=p->SCluster()->Seed()->CoviEtaiPhi(); 
  CoviPhiiPhi=p->SCluster()->Seed()->CoviPhiiPhi();
  RelPreshowerEnergy=p->SCluster()->PreshowerEnergy()/RawEnergy;
  
  //spectator variables
  Pt_MVA=p->Pt();
  ScEta_MVA=p->SCluster()->Eta();
  
  isbarrel = (fabs(ScEta_MVA)<1.4442);
  
  // check which category it is ...
  _tCat = 1;
  if ( !isbarrel ) _tCat = 3;
  if ( R9 < 0.94 ) _tCat++;
  
  //Electron Veto
  if(dRTrack > cic4_allcuts_temp_sublead[_tCat-1+6*4]){
    PassElecVeto=kTRUE;
  }
  
  if(PassElecVeto && Pt_MVA>MVAPtMin && ((fabs(ScEta_MVA)<1.4442)||(fabs(ScEta_MVA)>1.566 && fabs(ScEta_MVA)<2.5)) && tIso1<250 && tIso2<250 && tIso3<250){
    
    if (isbarrel) {
      reader = fReaderBarrel;
    }
    else {
      reader = fReaderEndcap;
    }
    
    bdt = reader->EvaluateMVA( "BDT method" );
    
    if (isbarrel) {
      if(bdt>bdtCutBarrel){
	PassMVA=kTRUE;	
      }
    }
    else {
      if(bdt>bdtCutEndcap){
	PassMVA=kTRUE;	
      }
    } 

  }
 
  return PassMVA;
}

//---------------------------------------------------------------------------------
Int_t MVATools::PassElectronVetoInt(const Photon* p, const ElectronCol* els) {
  
  // these values are taken from the H2GGlobe code... (actually from Marco/s mail)
  float cic4_allcuts_temp_sublead[] = { 
    3.8,         2.2,         1.77,        1.29,
    11.7,        3.4,         3.9,         1.84,
    3.5,         2.2,         2.3,         1.45,
    0.0106,      0.0097,      0.028,       0.027,
    0.082,       0.062,       0.065,       0.048,
    0.94,        0.36,        0.94,        0.32,
    1.,          0.062,       0.97,        0.97,
    1.5,         1.5,         1.5,         1.5 };  // the last line is PixelmatchVeto and un-used
  
  //initilize the bool value
  PassElecVetoInt=0;
  
  dRTrack = PhotonTools::ElectronVetoCiC(p, els);
  
  ScEta_MVA=p->SCluster()->Eta();
  
  isbarrel = (fabs(ScEta_MVA)<1.4442);

  R9 = p->R9();
  
  // check which category it is ...
  _tCat = 1;
  if ( !isbarrel ) _tCat = 3;
  if ( R9 < 0.94 ) _tCat++;
  
  //Electron Veto
  if(dRTrack > cic4_allcuts_temp_sublead[_tCat-1+6*4]){
    PassElecVetoInt=1;
  }
  
  return  PassElecVetoInt;
  
}

//--------------------------------------------------------------------------------------------------

Float_t MVATools::GetMVAbdtValue(const Photon* p,const Vertex* vtx,const TrackCol* trackCol,const VertexCol* vtxCol,Double_t _tRho,const ElectronCol* els) {
  
  //get the variables used to compute MVA variables
  ecalIso3 = p->EcalRecHitIsoDr03();
  ecalIso4 = p->EcalRecHitIsoDr04();
  hcalIso4 = p->HcalTowerSumEtDr04();
  
  wVtxInd = 0;
  
  trackIso1 = IsolationTools::CiCTrackIsolation(p,vtx, 0.3, 0.02, 0.0, 0.0, 0.1, 1.0, trackCol);//Question Ming:whyfPV->At(0) instead of selected vertex using ranking method?
  
  // track iso only
  trackIso3 = IsolationTools::CiCTrackIsolation(p,vtx, 0.3, 0.02, 0.0, 0.0, 0.1, 1.0, trackCol);
  
  // track iso worst vtx
  trackIso2 = IsolationTools::CiCTrackIsolation(p,vtx, 0.4, 0.02, 0.0, 0.0, 0.1, 1.0, trackCol, &wVtxInd,vtxCol);
  
  combIso1 = ecalIso3+hcalIso4+trackIso1 - 0.17*_tRho;
  combIso2 = ecalIso4+hcalIso4+trackIso2 - 0.52*_tRho;
  
  RawEnergy = p->SCluster()->RawEnergy();
  
  dRTrack = PhotonTools::ElectronVetoCiC(p, els);
  
  //compute MVA variables 0
  HoE = p->HadOverEm();
  covIEtaIEta = p->CoviEtaiEta();
  tIso1 = (combIso1) *50./p->Et();
  tIso3 = (trackIso3)*50./p->Et();
  tIso2 = (combIso2) *50./(p->MomVtx(vtxCol->At(wVtxInd)->Position()).Pt());
  R9 = p->R9();
  
  //newly added MVA variables 1
  RelIsoEcal=(ecalIso3-0.17*_tRho)/p->Et();
  RelIsoHcal=(hcalIso4-0.17*_tRho)/p->Et();
  
  RelEMax=p->SCluster()->Seed()->EMax()/RawEnergy;
  RelETop=p->SCluster()->Seed()->ETop()/RawEnergy;
  RelEBottom=p->SCluster()->Seed()->EBottom()/RawEnergy;
  RelELeft=p->SCluster()->Seed()->ELeft()/RawEnergy;
  RelERight=p->SCluster()->Seed()->ERight()/RawEnergy;
  RelE2x5Max=p->SCluster()->Seed()->E2x5Max()/RawEnergy;
  RelE2x5Top=p->SCluster()->Seed()->E2x5Top()/RawEnergy;
  RelE2x5Bottom=p->SCluster()->Seed()->E2x5Bottom()/RawEnergy;
  RelE2x5Left=p->SCluster()->Seed()->E2x5Left()/RawEnergy;
  RelE2x5Right=p->SCluster()->Seed()->E2x5Right()/RawEnergy;
  RelE5x5=p->SCluster()->Seed()->E5x5()/RawEnergy;
  
  //newly added MVA variables 2
  EtaWidth=p->SCluster()->EtaWidth();
  PhiWidth=p->SCluster()->PhiWidth();
  CoviEtaiPhi=p->SCluster()->Seed()->CoviEtaiPhi(); 
  CoviPhiiPhi=p->SCluster()->Seed()->CoviPhiiPhi();
  RelPreshowerEnergy=p->SCluster()->PreshowerEnergy()/RawEnergy;
  
  //spectator variables
  Pt_MVA=p->Pt();
  ScEta_MVA=p->SCluster()->Eta();
  
  isbarrel = (fabs(ScEta_MVA)<1.4442);
  
  if (isbarrel) {
    reader = fReaderBarrel;
  }
  else {
    reader = fReaderEndcap;
  }
  
  assert(reader); 

  bdt = reader->EvaluateMVA("BDT method");

  /*  printf("HoE: %f\n",HoE);
  printf("covIEtaIEta: %f\n",covIEtaIEta);
  printf("tIso1: %f\n",tIso1);
  printf("tIso3: %f\n",tIso3);
  printf("tIso2: %f\n",tIso2);
  printf("tIso1: %f\n",tIso1);
  printf("R9: %f\n",R9);
  printf("RelIsoEcal: %f\n",RelIsoEcal);
  printf("RelIsoHcal: %f\n",RelIsoHcal);
  
  printf("RelEMax: %f\n",RelEMax);
  printf("RelETop: %f\n",RelETop);
  printf("RelEBottom: %f\n",RelEBottom);
  printf("RelELeft: %f\n",RelELeft);
  printf("RelERight: %f\n",RelERight);
  printf("RelE2x5Max: %f\n",RelE2x5Max);
  printf("RelE2x5Top: %f\n",RelE2x5Top);
  printf("RelE2x5Bottom: %f\n",RelE2x5Bottom);
  printf("RelE2x5Left: %f\n",RelE2x5Left);
  printf("RelE2x5Right;: %f\n",RelE2x5Right);
  printf("RelE5x5: %f\n",RelE5x5);
  
  printf("EtaWidth: %f\n",EtaWidth);
  printf("PhiWidth: %f\n",PhiWidth);
  printf("CoviEtaiPhi: %f\n",CoviEtaiPhi);
  printf("CoviPhiiPhi: %f\n",CoviPhiiPhi);
  
  if (isbarrel) {
    printf("RelPreshowerEnergy: %f\n",RelPreshowerEnergy);
    }*/
  
  return bdt;
}