#include<vector>
#include<map>

double npix(TString filename, TString mode){

  TFile* f = new TFile(filename,"R");
  TDirectory* dir_dut = (TDirectory*)f->Get("dut1_ibl");
  TTree* matched = (TTree*)dir_dut->Get("tracks_clusters_matched");

  Short_t NHitsInCluster = 0;
  
  matched->SetBranchAddress("clu_size",&NHitsInCluster);
  
  
  ULong64_t ENTRIES = matched->GetEntries();
  
  TH1F* h_npix = new TH1F("h_npix","; Number of pixels in cluster; AU",20,-0.5,19.5);
  
  for (int i = 0; i<ENTRIES; i++)
    {
      matched->GetEntry(i);
      h_npix->Fill(NHitsInCluster);
    }

  double output;
  if (mode.EqualTo("mean")) output = h_npix->GetMean();
  else output = h_npix->GetMeanError();
  return output;
  
}
