#include "Analysis/VVAnalysis/interface/ZZBackgroundSelector.h"
#include "TLorentzVector.h"
#include <TStyle.h>

void ZZBackgroundSelector::SlaveBegin(TTree * /*tree*/)
{
    //systematics_ = {
    //    {jetEnergyScaleUp, "CMS_scale_jUp"}, 
    //    {jetEnergyScaleDown, "CMS_scale_jDown"}, 
    //    {jetEnergyResolutionUp, "CMS_res_jUp"},
    //    {jetEnergyResolutionDown, "CMS_res_jDown"},
    //};
    //isNonpromptEstimate_ = true;
    //doaQGC_ = false;

    ZZSelector::SlaveBegin(0);
    fakeRate_allE_ = (ScaleFactor *) GetInputList()->FindObject("fakeRate_allE");
    if (fakeRate_allE_ == nullptr ) Abort("Must pass electron fake rate to input list!");
    fakeRate_allMu_ = (ScaleFactor *) GetInputList()->FindObject("fakeRate_allMu");
    if (fakeRate_allMu_ == nullptr ) Abort("Must pass muon fake rate to input list!");
}
void ZZBackgroundSelector::Init(TTree *tree) {
      isNonPrompt_ = true;
      ZZSelector::Init(tree);
      systematics_ = {};
}

void ZZBackgroundSelector::SetupNewDirectory()
{
    ZZSelector::SetupNewDirectory();
   
    // Insure that hist ranges are exactly the same as ZZSelector, just change name
    for (const auto && obj : *currentHistDir_) {
        std::string name = obj->GetName();
        TNamed* named = dynamic_cast<TNamed*>(obj);
        named->SetName(name.insert(name.length()-4, "Fakes_").c_str());
    } 
      AddObject<TH1D>(Z1MassHistPPPF_, ("Z1Mass_PPPF_"+channelName_).c_str(), "Z1Mass; m_{Z_{1}} [GeV]; Events;", 60, 0, 120);
      AddObject<TH1D>(Z1MassHistPPFF_, ("Z1Mass_PPFF_"+channelName_).c_str(), "Z1Mass; m_{Z_{1}} [GeV]; Events;", 60, 0, 120);
      
      AddObject<TH1D>(Z2MassHistPPPF_, ("Z2Mass_PPPF_"+channelName_).c_str(), "Z2Mass; m_{Z_{2}} [GeV]; Events;", 60, 0, 120);
      AddObject<TH1D>(Z2MassHistPPFF_, ("Z2Mass_PPFF_"+channelName_).c_str(), "Z2Mass; m_{Z_{2}} [GeV]; Events;", 60, 0, 120);

      AddObject<TH1D>(MassHistPPPF_, ("Mass_PPPF_"+channelName_).c_str(), "Mass; m_{4l} [GeV]; Events;", 40, 70, 870);
      AddObject<TH1D>(MassHistPPFF_, ("Mass_PPFF_"+channelName_).c_str(), "Mass; m_{4l} [GeV]; Events;", 40, 70, 870);
      
       AddObject<TH1D>(CosThetaHist1PPPF_, ("CosTheta1_PPPF_"+channelName_).c_str(), "CosTheta1; PolAngle_{4l} [GeV]; Events;", 20, -1.0, 1.0);
       AddObject<TH1D>(CosThetaHist1PPFF_, ("CosTheta1_PPFF_"+channelName_).c_str(), "CosTheta1; PolAngle_{4l} [GeV]; Events;", 20, -1.0, 1.0);
       AddObject<TH1D>(CosThetaHist2PPPF_, ("CosTheta2_PPPF_"+channelName_).c_str(), "CosTheta2; PolAngle_{4l} [GeV]; Events;", 20, -1.0, 1.0);
       AddObject<TH1D>(CosThetaHist2PPFF_, ("CosTheta2_PPFF_"+channelName_).c_str(), "CosTheta2; PolAngle_{4l} [GeV]; Events;", 20, -1.0, 1.0);
      AddObject<TH1D>(RapidityDiffHistPPPF_, ("RapidityDiff_PPPF_"+channelName_).c_str(), "RapidityDiff; RapidityDiff_{4l} [GeV]; Events;", 20, 0, 4);
      AddObject<TH1D>(RapidityDiffHistPPFF_, ("RapidityDiff_PPFF_"+channelName_).c_str(), "RapidityDiff; Rapidityfiff_{4l} [GeV]; Events;", 20, 0, 4);
       AddObject<TH1D>(DeltaPhiHistPPPF_, ("DeltaPhi_PPPF_"+channelName_).c_str(), "DeltaPhi; DeltaPhi [GeV]; Events;", 16, 0, 4);
       AddObject<TH1D>(DeltaPhiHistPPFF_, ("DeltaPhi_PPFF_"+channelName_).c_str(), "DeltaPhi; DeltaPhi [GeV]; Events;", 16, 0, 4);
       
      AddObject<TH1D>(WeightsHistmmee_, ("Weights_mmee_"+channelName_).c_str(), "Weight; Event Weight; Events;", 10, -5, 5);
      AddObject<TH1D>(WeightsHisteemm_, ("Weights_eemm_"+channelName_).c_str(), "Weight; Event Weight; Events;", 100, -5, 5);
}
/*double computeLeptonEnergy(double pt, double eta, double m) {
    // Calculate energy using E = sqrt(pt^2 * cosh^2(eta) + m^2)
    double cosh_eta = std::cosh(eta);
    double energy = std::sqrt((pt * cosh_eta) * (pt * cosh_eta) + m * m);
    return energy;
    }*/
double costheta_bkg(const TLorentzVector& z1P4_input,
                       const TLorentzVector& z2P4_input,
                       const TLorentzVector& leptonP4_input) {

    TLorentzVector z1P4 = z1P4_input;
    TLorentzVector z2P4 = z2P4_input;
    TLorentzVector leptonP4 = leptonP4_input;

    TLorentzVector zzP4 = z1P4 + z2P4;

    leptonP4.Boost(-z1P4.BoostVector());  // Boost lepton to Z1 rest frame
    z1P4.Boost(-zzP4.BoostVector());      // Boost Z1 to ZZ rest frame

    double cosTheta_bkg = leptonP4.Vect().Dot(z1P4.Vect()) /
                      (leptonP4.Vect().Mag() * z1P4.Vect().Mag());

    return cosTheta_bkg;
}
double rapidity_bkg(TLorentzVector z1P4, TLorentzVector z2P4)
 {
   double  Z1Rapidity = z1P4.Rapidity();
   double  Z2Rapidity = z2P4.Rapidity();
   return (abs((Z1Rapidity)-(Z2Rapidity)));
 };

float ZZBackgroundSelector::getEventWeight(Long64_t entry) {
    float evtwgt = 0;

    if(channel_ == mmee && (e1e2IsZ1(entry)))
        WeightsHistmmee_->Fill(1,weight);

    if(channel_ == eemm && !(e1e2IsZ1(entry)))
        WeightsHisteemm_->Fill(1,weight);

    //if ((channel_ == eeee) || (channel_ == mmmm)){
    //}
    //in eemm the e1e2IsZ1 function already sets the correct Z1,Z2 leptons and even the IDs from LoadBranches
    //So only need to take care which is l3 and l4
   // if ((channel_ == eemm) || (channel_ == mmee)){
   //   if(Z2FP()){
   //     float templ3Pt = l3Pt;
   //     l3Pt = l4Pt;
   //     l4Pt = templ3Pt;
   //     float templ3Eta = l3Eta;
   //     l3Eta = l4Eta;
   //     l4Eta = templ3Eta;
   //   }
   // }
   // std::cout<<"Weight in Bkg Seletor getEventWeight function: "<<weight<<std::endl;
      if (IsPPPFRegion()) {
        if (true){
	  //std::cout<<"Weight in PPPF: "<<weight<<std::endl;
	  if (ZZSelection()){
	    /* if(channel_==eeee || channel_==eemm){
	    if(l1Mass<0) {
	     l1Mass=0.000511;
	    l1Energy= computeLeptonEnergy (l1Pt,l1Eta,l1Mass);}
	    if(l2Mass<0) {
	    l2Mass=0.000511;
	     l2Energy= computeLeptonEnergy (l2Pt,l2Eta,l2Mass);
	     }}*/
	    
		Z1MassHistPPPF_->Fill(Z1mass, weight);
	    Z2MassHistPPPF_->Fill(Z2mass, weight);
	    MassHistPPPF_->Fill(Mass,weight);
	    if (l1Charge>0 && l2Charge<0){
	      CosThetaHist1PPPF_->Fill(cosTheta_1_bkg,weight);}
	    else if (l2Charge>0 && l1Charge<0){
	      CosThetaHist1PPPF_->Fill(cosTheta_2_bkg,weight);}
	     if (l3Charge>0 && l4Charge<0){
              CosThetaHist2PPPF_->Fill(cosTheta_3_bkg,weight);}
            else if (l4Charge>0 && l3Charge<0){
              CosThetaHist2PPPF_->Fill(cosTheta_4_bkg,weight);}
                 
                 
	    if (l1Charge>0 && l2Charge<0){                                                                                                   
              CosThetaHist1PPFF_->Fill(cosTheta_1_bkg,weight);}                                                                                
            else if (l2Charge>0 && l1Charge<0){                                                                                               
              CosThetaHist1PPFF_->Fill(cosTheta_2_bkg,weight);}   
	     if (l3Charge>0 && l4Charge<0){
              CosThetaHist2PPFF_->Fill(cosTheta_3_bkg,weight);}
            else if (l4Charge>0 && l3Charge<0){
              CosThetaHist2PPFF_->Fill(cosTheta_4_bkg,weight);}
	    RapidityDiffHistPPFF_->Fill(rapidityDiff_bkg,weight);
	    DeltaPhiHistPPFF_->Fill(dPhill,weight);
	   }
	}
        evtwgt = ((-1*getl3FakeRate(entry)*getl4FakeRate(entry))*weight);
        //evtwgt = ((getl3FakeRate(entry)*getl4FakeRate(entry))*weight);
        //WeightsHistPPFF_->Fill(1,evtwgt);
      }
      //std::cout<<"eventWeight after Fake Rate: "<<evtwgt<<std::endl;
    return evtwgt;
}

void ZZBackgroundSelector::LoadBranchesUWVV(Long64_t entry, std::pair<Systematic, std::string> variation) {
    ZZSelector::LoadBranchesUWVV(entry, variation);
    SetZ1Z2Masses(); 
    //turn off ZZSelection otherwise it will cause problem in full mass range m4l plots
    //if (!ZZSelection()){
    //  return;}

    //std::cout<<"isNonPrompt_ from ZZSelector: "<<isNonPrompt_<<std::endl;
    //isNonPrompt_ = true;
    //std::cout<<"isNonPrompt_ from ZZBackgroundSelector: "<<isNonPrompt_<<std::endl;
    //We don't want systematics to run for data-driven nonPrompt Estimate so save time and space
    //systematics_ = {};
    //doSystematics_ = false;
    weight = getEventWeight(entry);
    //std::cout<<"eventWeight in loadBranchesUWVV: "<<weight<<std::endl;
}
float ZZBackgroundSelector::getl3FakeRate(Long64_t entry) {
    float pt_fillval = l3Pt < FR_MAX_PT_ ? l3Pt : FR_MAX_PT_ - 0.01;
    float fr = 1;
    if ((channel_ == eeee) || (channel_ == mmee && !(e1e2IsZ1(entry)))){
        fr = fakeRate_allE_->Evaluate2D(pt_fillval, std::abs(l3Eta));
        //fr = 0.03;//avg e fake rate
	// std::cout<<"channel: "<<channel_<<std::endl;
        //std::cout<<"l3 E Fake Rate: "<<fr<<std::endl;
    }
    else if ((channel_ == mmmm) || (channel_ == eemm && (e1e2IsZ1(entry)))){
        fr = fakeRate_allMu_->Evaluate2D(pt_fillval, std::abs(l3Eta));
        //fr = 0.1;//avg mu fake rate
        //std::cout<<"channel: "<<channel_<<std::endl;
        //std::cout<<"l3 Mu Fake Rate: "<<fr<<std::endl;
    }
    else{
      fr=0.;
    }
    return fr/(1-fr); 
}

float ZZBackgroundSelector::getl4FakeRate(Long64_t entry) {
    float pt_fillval = l4Pt < FR_MAX_PT_ ? l4Pt : FR_MAX_PT_ - 0.01;
    float fr = 1;
    if ((channel_ == eeee) || (channel_ == mmee && !(e1e2IsZ1(entry)))){
        fr = fakeRate_allE_->Evaluate2D(pt_fillval, std::abs(l4Eta));
        //fr = 0.03;//avg e fake rate
        //std::cout<<"channel: "<<channel_<<std::endl;
        //std::cout<<"l4 E Fake Rate: "<<fr<<std::endl;
    }
    else if ((channel_ == mmmm) || (channel_ == eemm && (e1e2IsZ1(entry)))){
        fr = fakeRate_allMu_->Evaluate2D(pt_fillval, std::abs(l4Eta));
        //fr = 0.1;//avg mu fake rate
        //std::cout<<"channel: "<<channel_<<std::endl;
        //std::cout<<"l4 Mu Fake Rate: "<<fr<<std::endl;
    }
    else{
      fr=0.;
    }
    return fr/(1-fr); 
}
//Remember that we only build Z1 (Real Z) out of OS-SF tight leptons
bool ZZBackgroundSelector::IsPPPFRegion() {
    return ((tightZ1Leptons() && (Z2PF() || Z2FP())) || (tightZ2Leptons() && (Z1PF() || Z1FP())));
}
//Remember that we only build Z1 (Real Z) out of OS-SF tight leptons
bool ZZBackgroundSelector::IsPPFFRegion() {
    if((channel_ == eeee) || (channel_ == mmmm))
      return ((tightZ1Leptons() && Z2FF()) || (tightZ2Leptons() && Z1FF()) || (Z1FP() && Z2PF()) || (Z1PF() && Z2FP()) || (Z1PF() && Z2PF())  || (Z1FP() && Z2FP()));
    else
      return ((tightZ1Leptons() && Z2FF()) || (tightZ2Leptons() && Z1FF()));
}
//Remember that we only build Z1 (Real Z) out of OS-SF tight leptons
//This is only for 4e,4mu channels as eemm/mmee channels is already taken care of by SetVariables function which gets
//called in LoadBranches() from ZZSelector. The SetVariables function should work in CRs, SR equally. We only need
//SetZ1Z2Masses function for 4e/4mu in CRs.
void ZZBackgroundSelector::SetZ1Z2Masses() {
    TLorentzVector lepton1;
    //lepton1.SetPtEtaPhiM(l1Pt, l1Eta, l1Phi, l1Mass);
    lepton1.SetPtEtaPhiE(l1Pt, l1Eta, l1Phi, l1Energy); 
    TLorentzVector lepton2;
    //lepton2.SetPtEtaPhiM(l2Pt, l2Eta, l2Phi, l2Mass);
    lepton2.SetPtEtaPhiE(l2Pt, l2Eta, l2Phi, l2Energy);
    TLorentzVector lepton3;
    //lepton3.SetPtEtaPhiM(l3Pt, l3Eta, l3Phi, l3Mass);
    lepton3.SetPtEtaPhiE(l3Pt, l3Eta, l3Phi, l3Energy); 
    TLorentzVector lepton4;
    //lepton4.SetPtEtaPhiM(l4Pt, l4Eta, l4Phi, l4Mass);
    lepton4.SetPtEtaPhiE(l4Pt, l4Eta, l4Phi, l4Energy);
    TLorentzVector Z14;
    Z14.SetPtEtaPhiM(Z1pt, Z1Eta, Z1Phi, Z1mass);
  //z1etPtEtaPhiM(Z1pt_d, Z1Eta_d, Z1Phi_d, Z1mass_d);                                                                                   
    TLorentzVector Z24;
    Z24.SetPtEtaPhiM(Z2pt, Z2Eta, Z2Phi, Z2mass);
    /*std::cout<<"Post the four-vector declaration"<<std::endl;
    std::cout<<"Z1Pt "<<Z1pt<<std::endl;
    std::cout<<"Z1Eta "<<Z1Eta<<std::endl;
    std::cout<<"Z1Phi "<<Z1Phi<<std::endl;
    std::cout<<"Z1Mass "<<Z1mass<<std::endl;
    std::cout<<"Z2Pt "<<Z2pt<<std::endl;
    std::cout<<"Z2Eta "<<Z2Eta<<std::endl;
    std::cout<<"Z2Phi "<<Z2Phi<<std::endl;
    std::cout<<"Z2Mass "<<Z2mass<<std::endl;
    std::cout<<"-------------------"<<std::endl;*/
    if (l1Charge>0 && l2Charge<0){
      cosTheta_1_bkg = costheta_bkg(Z14,Z24,lepton1);}
    if (l2Charge>0 && l1Charge<0){
      cosTheta_2_bkg = costheta_bkg(Z14,Z24,lepton2);}
    if (l3Charge>0 && l4Charge<0){
      cosTheta_3_bkg = costheta_bkg(Z24,Z14,lepton3);}
    if (l4Charge>0 && l3Charge<0){
      cosTheta_4_bkg = costheta_bkg(Z24,Z14,lepton4);}
    rapidityDiff_bkg = rapidity_bkg(Z14,Z24);
    if(tightZ1Leptons() && !tightZ2Leptons()){
      //std::cout<<"Entering tightZ1Leptons() && !tightZ2Leptons() loop: "<<std::endl;  
      Z1mass = (lepton1+lepton2).M();
      Z2mass = (lepton3+lepton4).M();
      Z1pt = (lepton1+lepton2).Pt();
      Z2pt = (lepton3+lepton4).Pt();
      Z1Phi = (lepton1+lepton2).Phi();
      Z2Phi = (lepton3+lepton4).Phi();
      Z1Eta = (lepton1+lepton2).Eta();

      Z2Eta = (lepton3+lepton4).Eta();
		 //In Z2 what is l3 and l4 can change the fake rate a little bit.
      if(Z2FP()){
        float templ3Pt = l3Pt;
        l3Pt = l4Pt;
        l4Pt = templ3Pt;
        float templ3Eta = l3Eta;
        l3Eta = l4Eta;
        l4Eta = templ3Eta;
	 float templ3Phi = l3Phi;
        l3Phi = l4Phi;
        l4Phi = templ3Phi;
      }
    }
    else if(tightZ2Leptons() && !tightZ1Leptons()){  
      //std::cout<<"Entering tightZ2Leptons() && !tightZ1Leptons()) loop "<<std::endl;
      Z1mass = (lepton3+lepton4).M();
      Z2mass = (lepton1+lepton2).M();
      Z1pt = (lepton3+lepton4).Pt();
      Z2pt = (lepton1+lepton2).Pt();
      Z1Phi = (lepton3+lepton4).Phi();
      Z2Phi = (lepton1+lepton2).Phi();
       Z1Eta = (lepton3+lepton4).Eta();
      Z2Eta = (lepton1+lepton2).Eta();
      //Fakes are l1,l2 from skims, reverse them
      float templ1Pt = l1Pt;
      l1Pt = l3Pt;
      l3Pt = templ1Pt;
      float templ2Pt = l2Pt;
      l2Pt = l4Pt;
      l4Pt = templ2Pt;
      float templ1Eta = l1Eta;
      l1Eta = l3Eta;
      l3Eta = templ1Eta;
      float templ2Eta = l2Eta;
      l2Eta = l4Eta;
      l4Eta = templ2Eta;
      float templ1Phi = l1Phi;
      l1Phi = l3Phi;
      l3Phi = templ1Phi;
      float templ2Phi = l2Phi;
      l2Phi = l4Phi;
      l4Phi = templ2Phi;
      float templ1SIP3D = l1SIP3D;
      l1SIP3D = l3SIP3D;
      l3SIP3D = templ1SIP3D;
      float templ2SIP3D = l2SIP3D;
      l2SIP3D = l4SIP3D;
      l4SIP3D = templ2SIP3D;
      int templ1PdgId = l1PdgId;
      l1PdgId = l3PdgId;
      l3PdgId = templ1PdgId;
      int templ2PdgId = l2PdgId;
      l2PdgId = l4PdgId;
      l4PdgId = templ2PdgId;
      int templ1Charge = l1Charge;
      l1Charge = l3Charge;
      l3Charge = templ1Charge;
      int templ2Charge = l2Charge;
      l2Charge = l4Charge;
      l4Charge = templ2Charge;
      //Now we have two fakes identified by l3Pt, l4Pt and l3Eta, l4Eta
      //Further special condition between l3,l4 which one to use for l4fake rate in PPPF region, their IDs still are labeled l1IsTight,l2IsTight
      if(Z1FP()){
        float templ3Pt = l3Pt;
        l3Pt = l4Pt;
        l4Pt = templ3Pt;
        float templ3Eta = l3Eta;
        l3Eta = l4Eta;
        l4Eta = templ3Eta;
	float templ3Phi = l3Phi;
        l3Phi = l4Phi;
        l4Phi = templ3Phi;
      }
        }
    //The last two conditions only matter for TTJets fakes (very small amount)
    else if(Z1FP() && Z2PF()){
      //Make sure I am not making a Z in eemm with an e and mu!
      if ((channel_ == eeee) || (channel_ == mmmm)){
	//std::cout<<"Z1FP() && Z2PF()) loop enter "<<std::endl;
        Z1mass = (lepton2+lepton3).M();
        Z2mass = (lepton1+lepton4).M();
        Z1pt = (lepton2+lepton3).Pt();
        Z2pt = (lepton1+lepton4).Pt();
	Z1Phi = (lepton2+lepton3).Phi();
	Z2Phi = (lepton1+lepton4).Phi();
	Z1Eta = (lepton2+lepton3).Eta();
	Z2Eta = (lepton1+lepton4).Eta();
        //Here the two fakes are l1,l4 and we only need to relabel l1 -> l3
        float templ1Pt = l1Pt;
        l1Pt = l3Pt;
        l3Pt = templ1Pt;
        float templ1Eta = l1Eta;
        l1Eta = l3Eta;
        l3Eta = templ1Eta;
      float templ1Phi = l1Phi;
        l1Phi = l3Phi;
        l3Phi = templ1Phi;}
      else{
      }
    }
    else if(Z1PF() && Z2FP()){
      //Make sure I am not making a Z in eemm with an e and mu!
      if ((channel_ == eeee) || (channel_ == mmmm)){
	//std::cout<<"Z1PF() && Z2FP()) loop enter "<<std::endl;
        Z1mass = (lepton1+lepton4).M();
        Z2mass = (lepton2+lepton3).M();
        Z1pt = (lepton1+lepton4).Pt();
        Z2pt = (lepton2+lepton3).Pt();
	Z1Phi = (lepton1+lepton4).Phi();
	Z2Phi = (lepton2+lepton3).Phi();
	Z1Eta = (lepton1+lepton4).Eta();
	Z2Eta = (lepton2+lepton3).Eta();
        //Here the two fakes are l2,l3 and we only need to relabel l2 -> l4 since this only matters in PPFF region so l3,l4 are interchangeable
        float templ2Pt = l2Pt;
        l2Pt = l4Pt;
        l4Pt = templ2Pt;
        float templ2Eta = l2Eta;
        l2Eta = l4Eta;
        l4Eta = templ2Eta;
	float templ2Phi = l2Phi;
        l2Eta = l4Phi;
        l4Eta = templ2Phi;
      }
      else{
      }
      
    }
    else if(Z1PF() && Z2PF()){
       if ((channel_ == eeee) || (channel_ == mmmm)){
        //std::cout<<"Z1PF() && Z2FP()) loop enter "<<std::endl;                                                                              
        Z1mass = (lepton1+lepton3).M();
        Z2mass = (lepton2+lepton4).M();
        Z1pt = (lepton1+lepton3).Pt();
        Z2pt = (lepton2+lepton4).Pt();
        Z1Phi = (lepton1+lepton3).Phi();
        Z2Phi = (lepton2+lepton4).Phi();
        Z1Eta = (lepton1+lepton3).Eta();
        Z2Eta = (lepton2+lepton4).Eta();
        //Here the two fakes are l2,l3 and we only need to relabel l2 -> l4 since this only matters in PPFF region so l3,l4 are interchangeable                                                                                                                                             
        float templ2Pt = l2Pt;
        l2Pt = l3Pt;
        l3Pt = templ2Pt;
        float templ2Eta = l2Eta;
        l2Eta = l3Eta;
        l3Eta = templ2Eta;
        float templ2Phi = l2Phi;
        l2Eta = l3Phi;
        l3Eta = templ2Phi;
      }
      else{
      }
	
    }
    else if (Z1FP() && Z2FP()){
      if ((channel_ == eeee) || (channel_ == mmmm)){
        //std::cout<<"Z1PF() && Z2FP()) loop enter "<<std::endl;                                                                            
        Z1mass = (lepton2+lepton4).M();
        Z2mass = (lepton1+lepton3).M();
        Z1pt = (lepton2+lepton4).Pt();
        Z2pt = (lepton1+lepton3).Pt();
        Z1Phi = (lepton2+lepton4).Phi();
        Z2Phi = (lepton1+lepton3).Phi();
        Z1Eta = (lepton2+lepton4).Eta();
        Z2Eta = (lepton1+lepton3).Eta();
        //Here the two fakes are l2,l3 and we only need to relabel l2 -> l4 since this only matters in PPFF region so l3,l4 are interchangeable
	float templ1Pt = l1Pt;
        l1Pt = l4Pt;
        l4Pt = templ1Pt;
        float templ1Eta = l1Eta;
        l1Eta = l4Eta;
        l4Eta = templ1Eta;
        float templ1Phi = l1Phi;
        l1Eta = l4Phi;
        l4Eta = templ1Phi;
      }
      else{
      }

    }

}

