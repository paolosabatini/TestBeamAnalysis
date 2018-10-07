#include<vector>
#include<map>

double GetCoordError(TString Coord, float val){

  TF1* f = new TF1("f","TMath::Sqrt( [0]*[0] + ([1]*[1]*x*x)*([1]*[1]*x*x) + ([2]*[2]*[3]*[3]) )",-10e3,10e3);

  if(Coord.EqualTo("U"))
    {
      float PIX_SIZE = 200;
      float N_PIX = 80;
      float u = val * PIX_SIZE - N_PIX * PIX_SIZE /2.;
      float sigma_u = 1./sqrt(12.) * PIX_SIZE;
      f->SetParameters(1.91230,9.25456e-4,1.23529,sigma_u);
      return f->Eval(u);
    }

  if(Coord.EqualTo("V"))
    {
      float PIX_SIZE = 50;
      float N_PIX = 336;
      float v = val * PIX_SIZE - N_PIX * PIX_SIZE /2.;
      float sigma_v = 1./sqrt(12.) * PIX_SIZE;
      f->SetParameters(2.80746,1.32208e-3,0.988919,sigma_v);
      return f->Eval(v);
    }
  std::cout<<"[ERROR] What kind of variable you have requested ("<<Coord<<")? Results will be wrong";
  return 0;

}

double GetCoord(TString Coord, float val){

  TF1* f = new TF1("f","[0]+[1]*x",-10e3,10e3);

  if(Coord.EqualTo("U"))
    {
      float PIX_SIZE = 200;
      float N_PIX = 80;
      float u = val * PIX_SIZE - N_PIX * PIX_SIZE /2.;
      f->SetParameters(-24.5837,1.23529);
      return f->Eval(u);
    }

  if(Coord.EqualTo("V"))
    {
      float PIX_SIZE = 50;
      float N_PIX = 336;
      float v = val * PIX_SIZE - N_PIX * PIX_SIZE /2.;
      f->SetParameters(-17.9566,0.988919);
      return f->Eval(v);
    }
  std::cout<<"[ERROR] What kind of variable you have requested ("<<Coord<<")? Results will be wrong";
  return 0;

}


double tot(TString filename, TString mode){

  TFile* f = new TFile(filename,"R");
  if (!f->IsOpen())
    {std::cout<<"[ERROR] File "<<filename<<" not found: return 0"<<std::endl; return 0;}

  TTree* trk_tree = (TTree*) f->Get("Tracks");

  TString PlaneDUT("Plane5");
  TDirectory* dut_dir = (TDirectory*)f->Get(PlaneDUT);

  TTree* clus_tree = (TTree*) dut_dir->Get("Clusters");
  TTree* int_tree = (TTree*) dut_dir->Get("Intercepts");

  const Int_t NMAX = 1000;

  Int_t NClusters = 0;
  Double_t Col[NMAX];
  Double_t VarRow[NMAX];
  Double_t VarCol[NMAX];
  Double_t Row[NMAX];
  Double_t Value[NMAX];  
  Int_t Track[NMAX];

  Int_t NTracks = 0;
  Double_t U[NMAX];
  Double_t V[NMAX];
    
  clus_tree->SetBranchAddress("NClusters",&NClusters);
  clus_tree->SetBranchAddress("Col",&Col);
  clus_tree->SetBranchAddress("Row",&Row);
  clus_tree->SetBranchAddress("VarCol",&VarCol);
  clus_tree->SetBranchAddress("VarRow",&VarRow);
  clus_tree->SetBranchAddress("Value",&Value);
  clus_tree->SetBranchAddress("Track",&Track);

  int_tree->SetBranchAddress("NIntercepts",&NTracks);
  int_tree->SetBranchAddress("U",&U);
  int_tree->SetBranchAddress("V",&V);

  TH1F* h_tot = new TH1F("h_tot","; Cluster ToT; AU",50,-0.5,49.5);

  ULong64_t ENTRIES = min(int_tree->GetEntries(),2000);
 
  Double_t PIX_SIZE_X = 50;
  Double_t PIX_SIZE_Y = 200;

  Int_t NMAXRANGE = 5;
  
  for (int i = 0; i< ENTRIES; i++)
    {
      clus_tree->GetEntry(i);
      int_tree->GetEntry(i);
      //if (i%1000==0)std::cout<<"entries "<<i<<std::endl;
      for (int ic = 0; ic < NClusters; ic++)
	{
	  //std::cout<<"cluster "<<ic<<" inTrack "<<Track[ic]<<" (r"<<Row[ic]<<" +- "<<VarRow[ic]<<","<<Col[ic]<<" +- "<<VarCol[ic]<<")"<<std::endl;
	  Double_t clus_u = GetCoord("U",Col[ic]);
	  Double_t clus_u_var =GetCoordError("U",Col[ic]);
	  Double_t clus_v = GetCoord("V",Row[ic]);
	  Double_t clus_v_var = GetCoordError("V",Row[ic]);
	  //std::cout<<">> clus_u "<<clus_u<<" +- "<<clus_u_var*5<<" ,clus_v "<<clus_v<<"+-"<<clus_v_var*5<<std::endl;
	  Bool_t hasTrack = kFALSE;
	  for (int it = 0; it < NTracks; it++)
	    {
	      //std::cout<<">>> track "<<it<<" u "<<U[it]<<" v"<<V[it]<<std::endl;
	      if (fabs(U[it]-clus_u)<5*clus_u_var && fabs(V[it]-clus_v)<5*clus_v_var)
	      	{//std::cout<<">>>>>>>>>>>>> MATCHED!!!!!" <<std::endl;
		  hasTrack = kTRUE;}

	    }

	  if (!hasTrack) continue;
	  h_tot->Fill(Value[ic]);
	  
	}
      
    }

  double output;
  if (mode.EqualTo("mean")) output = h_tot->GetMean();
  else output = h_tot->GetMeanError();
  return output;



}
