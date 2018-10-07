#include<vector>
#include<map>

double calib(TString filename){

  TFile* f = new TFile(filename,"R");
  if (!f->IsOpen())
    {std::cout<<"[ERROR] File "<<filename<<" not found: return 0"<<std::endl; return 0;}

  f->ls();

  TTree* trk_tree = (TTree*) f->Get("Tracks");

  TString PlaneDUT("Plane5");
  TDirectory* dut_dir = (TDirectory*)f->Get(PlaneDUT);
  dut_dir->ls();
  TTree* int_tree = (TTree*) dut_dir->Get("Intercepts");
  TTree* clus_tree = (TTree*) dut_dir->Get("Clusters");
  
  const Int_t NMAX = 100;

  Int_t NClusters = 0;
  Double_t Col[NMAX];
  Double_t VarRow[NMAX];
  Double_t VarCol[NMAX];
  Double_t Row[NMAX];

  Int_t Track[NMAX];

  Int_t NTracks = 0;
  Double_t U[NMAX];
  Double_t V[NMAX];
    
  clus_tree->SetBranchAddress("NClusters",&NClusters);
  clus_tree->SetBranchAddress("Col",&Col);
  clus_tree->SetBranchAddress("Row",&Row);
  clus_tree->SetBranchAddress("VarCol",&VarCol);
  clus_tree->SetBranchAddress("VarRow",&VarRow);
  clus_tree->SetBranchAddress("Track",&Track);

  int_tree->SetBranchAddress("NIntercepts",&NTracks);
  int_tree->SetBranchAddress("U",&U);
  int_tree->SetBranchAddress("V",&V);


  TProfile* h_v = new TProfile("h_v",";Cluster U [#mum];Track U [#mum]",40,-4e3,4e3);
  TProfile* h_u = new TProfile("h_u",";Cluster V [#mum];Track V [#mum]",40,-4e3,4e3);
  TF1* f_u = new TF1("f_u","[0]+[1]*x",-10e3,10e3);
  f_u->SetParameters(0,1);
  f_u->SetParNames("Offset U","Slope U");
  TF1* f_v = new TF1("f_v","[0]+[1]*x",-10e3,10e3);
  f_v->SetParameters(0,1);
  f_v->SetParNames("Offset V","Slope V");
  
  ULong64_t ENTRIES = int_tree->GetEntries();
  Double_t PIX_SIZE_X = 50;
  Double_t PIX_SIZE_Y = 200;

  Int_t NMAXRANGE = 5;
  
  for (int i = 0; i< ENTRIES; i++)
    {
      clus_tree->GetEntry(i);
      int_tree->GetEntry(i);
      for (int ic = 0; ic < NClusters; ic++)
	{
	  Double_t clus_u = Col[ic]*PIX_SIZE_Y-80*PIX_SIZE_Y/2.;
	  Double_t clus_u_var = VarCol[ic]*PIX_SIZE_Y;
	  Double_t clus_v = Row[ic]*PIX_SIZE_X-336*PIX_SIZE_X/2.;
	  Double_t clus_v_var = VarRow[ic]*PIX_SIZE_X;
	  for (int it = 0; it < NTracks; it++)
	    {
	      h_u->Fill(clus_u, U[it]);
	      h_v->Fill(clus_v, V[it]);
	    }
	  
	}
      
    }

  TCanvas* c_u = new TCanvas("c_u","c_u",600,600);
  h_u->Draw("COLZ");
  h_u->Fit(f_u);
  c_u->SaveAs("plot/u_corr.png");


  
  TCanvas* c_v = new TCanvas("c_v","c_v",600,600);
  h_v->Draw("COLZ");
  h_v->Fit(f_v);
  c_v->SaveAs("plot/v_corr.png");


  
}
