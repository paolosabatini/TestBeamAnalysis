#include<vector>
#include<map>

double tot(TString filename, TString mode){

  TFile* f = new TFile(filename,"R");
  TDirectory* dir_dut = (TDirectory*)f->Get("Plane3");

  const int NMAX = 10000; 
  
  Int_t NClusters = 0;
  Int_t inTrack[NMAX];
  Double_t Value[NMAX];
  
  
  TTree* clus = (TTree*)dir_dut->Get("Clusters");
  //  hits->Print();

  clus->SetBranchAddress("NClusters",&NClusters);
  clus->SetBranchAddress("Track",inTrack);
  clus->SetBranchAddress("Value",Value);
  
  ULong64_t ENTRIES = clus->GetEntries();

  TH1F* h_tot = new TH1F("h_tot","; Cluster ToT; AU",10,-0.5,9.5);
  
  for (int i = 0; i<ENTRIES; i++)
    {
      clus->GetEntry(i);
      for (int icl =0; icl <NClusters; icl++)
	{

	  if (inTrack[icl]!=0) h_tot->Fill(Value[icl]);
	}
    }

  double output;
  if (mode.EqualTo("mean")) output = h_tot->GetMean();
  else output = h_tot->GetMeanError();
  return output;
  
}
