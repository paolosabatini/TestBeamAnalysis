#include<vector>
#include<map>

double eff(TString filename, TString mode){

  TFile* f = new TFile(filename,"R");
  TDirectory* dir_dut = (TDirectory*)f->Get("dut1_ibl");
  TTree* matched = (TTree*)dir_dut->Get("tracks_clusters_matched");
  TTree* unmatched = (TTree*)dir_dut->Get("clusters_unmatched");

  
  
  Float_t nMatch = matched->GetEntries();
  Float_t nUnmatch = unmatched->Draw("evt_frame","(evt_ntracks==1)","goff");
  Float_t nTotal = nMatch+nUnmatch;

  
  Float_t eff = nMatch/nTotal;
  Float_t err_eff = sqrt(eff*(1-eff)/nTotal);
  
  double output;
  if (mode.EqualTo("mean")) output = eff*100.;
  else output = err_eff*100.;
  return output;
  
}
