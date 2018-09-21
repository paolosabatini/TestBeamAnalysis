#include<vector>
#include<map>

double tot(TString filename, TString mode){

  TFile* f = new TFile(filename,"R");
  TDirectory* dir_dut = (TDirectory*)f->Get("dut1_ibl");
  TTree* matched = (TTree*)dir_dut->Get("tracks_clusters_matched");

  Float_t clus_tot = 0;
  
  matched->SetBranchAddress("clu_value",&clus_tot);
  
  
  ULong64_t ENTRIES = matched->GetEntries();
  
  TH1F* h_tot = new TH1F("h_tot","; Cluster ToT [BC]; AU",100,-0.5,99.5);
  
  for (int i = 0; i<ENTRIES; i++)
    {
      matched->GetEntry(i);
      h_tot->Fill(clus_tot);
    }

  double output;
  if (mode.EqualTo("mean")) output = h_tot->GetMean();
  else output = h_tot->GetMeanError();
  return output;
  
}
