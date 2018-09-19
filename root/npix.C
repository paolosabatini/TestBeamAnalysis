#include<vector>
#include<map>

double npix(TString filename, TString mode){

  TFile* f = new TFile(filename,"R");
  //  f->ls();
  TDirectory* dir_dut = (TDirectory*)f->Get("Plane3");
  /*
  std::cout<<"plane 3 entries cluster "<<((TTree*)dir_dut->Get("Clusters"))->GetEntries()<<std::endl;
  std::cout<<"event entries "<<((TTree*)f->Get("Event"))->GetEntries()<<std::endl;
  */

  Int_t NHits = 0;
  Int_t inCluster[10000];
  
  
  TTree* hits = (TTree*)dir_dut->Get("Hits");
  //  hits->Print();

  hits->SetBranchAddress("NHits",&NHits);
  hits->SetBranchAddress("HitInCluster",inCluster);
  
  ULong64_t ENTRIES = hits->GetEntries();

  TH1F* h_npix = new TH1F("h_npix","; Number of pixels in cluster; AU",10,-0.5,9.5);
  
  for (int i = 0; i<ENTRIES; i++)
    {
      hits->GetEntry(i);
      Int_t pixInCluster = 0;
      for (int ipix =0; ipix <NHits; ipix++)
	{

	  if (inCluster[ipix]!=0) pixInCluster++;
	}
      if (pixInCluster!=0) h_npix->Fill(pixInCluster);
    }

  double output;
  if (mode.EqualTo("mean")) output = h_npix->GetMean();
  else output = h_npix->GetMeanError();
  return output;
  
}
