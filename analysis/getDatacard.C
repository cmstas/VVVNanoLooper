#include <iostream>
#include <fstream>

std::vector<float> getProcYields(TString process, TString directory);

void getDatacard(TString dir){

     std::vector<TString> procs = {"NonResWWZ.root","ZHWWZ.root","ZZ.root","TTZ.root","tWZ.root","VVV.root","WZ.root","Other.root"};

     std::vector<float> NonResWWZ = getProcYields("NonResWWZ.root",dir);
     std::vector<float> ZHWWZ = getProcYields("ZHWWZ.root",dir);
     std::vector<float> ZZ = getProcYields("ZZ.root",dir);
     std::vector<float> TTZ = getProcYields("TTZ.root",dir);
     std::vector<float> tWZ = getProcYields("tWZ.root",dir);
     std::vector<float> VVV = getProcYields("VVV.root",dir);
     std::vector<float> WZ = getProcYields("WZ.root", dir);
     std::vector<float> Other = getProcYields("Other.root", dir);

     std::vector<float> Bkgds = {ZZ[0]+TTZ[0]+tWZ[0]+VVV[0]+WZ[0]+Other[0],ZZ[1]+TTZ[1]+tWZ[1]+VVV[1]+WZ[1]+Other[1],ZZ[2]+TTZ[2]+tWZ[2]+VVV[2]+WZ[2]+Other[2],ZZ[3]+TTZ[3]+tWZ[3]+VVV[3]+WZ[3]+Other[3],ZZ[4]+TTZ[4]+tWZ[4]+VVV[4]+WZ[4]+Other[4],ZZ[5]+TTZ[5]+tWZ[5]+VVV[5]+WZ[5]+Other[5],ZZ[6]+TTZ[6]+tWZ[6]+VVV[6]+WZ[6]+Other[6]};

     ofstream datacard;
     datacard.open("datacard_"+dir+".txt");
     datacard << "imax    " << 7 << " number of bins" << "\n";
     datacard << "jmax    " << 2 << " number of processes minus 1" << "\n";
     datacard << "kmax    " << "*" << " number of nuisance parameters" << "\n";
     datacard << "------------------------------------------------------------------------------" << "\n";
     datacard << "------------------------------------------------------------------------------" << "\n";
     datacard << "bin          " << "SR1     SR2     SR3     SR4     SR5     SR6     SR7" << "\n";
     datacard << "observation  " << NonResWWZ[0]+ZHWWZ[0]+Bkgds[0] << " " << NonResWWZ[1]+ZHWWZ[1]+Bkgds[1] << " " << NonResWWZ[2]+ZHWWZ[2]+Bkgds[2] << " " << NonResWWZ[3]+ZHWWZ[3]+Bkgds[3] << " " << NonResWWZ[4]+ZHWWZ[4]+Bkgds[4] << " " << NonResWWZ[5]+ZHWWZ[5]+Bkgds[5] << " " << NonResWWZ[6]+ZHWWZ[6]+Bkgds[6] << "\n";
     datacard << "------------------------------------------------------------------------------" << "\n";
     datacard << "bin          SR1     SR1     SR1     SR2     SR2     SR2     SR3     SR3     SR3     SR4     SR4     SR4     SR5     SR5     SR5     SR6     SR6     SR6     SR7     SR7     SR7" << "\n";
     datacard << "process      ZH      WWZ     Bkg     ZH      WWZ     Bkg     ZH      WWZ     Bkg     ZH      WWZ     Bkg     ZH      WWZ     Bkg     ZH      WWZ     Bkg     ZH      WWZ     Bkg" << "\n";
     datacard << "process      -1      0       1       -1      0       1       -1      0       1       -1      0       1       -1      0       1       -1      0       1       -1      0       1" << "\n";
     datacard << "rate         " << ZHWWZ[0] << " " << NonResWWZ[0] << " " << Bkgds[0] << " " << ZHWWZ[1] << " " << NonResWWZ[1] << " " << Bkgds[1] << " " << ZHWWZ[2] << " " << NonResWWZ[2] << " " << Bkgds[2] << " " << ZHWWZ[3] << " " << NonResWWZ[3] << " " << Bkgds[3] << " " << ZHWWZ[4] << " " << NonResWWZ[4] << " " << Bkgds[4] << " " << ZHWWZ[5] << " " << NonResWWZ[5] << " " << Bkgds[5] << " " << ZHWWZ[6] << " " << NonResWWZ[6] << " " << Bkgds[6] << "\n";
     datacard << "------------------------------------------------------------------------------" << "\n";

     datacard.close();
     std::cout << "Wrote datacard ====> datacard_"+dir+".txt" << std::endl;
          
}

std::vector<float> getProcYields(TString process, TString directory){

     TFile *file = TFile::Open( directory+"/Run2/"+process,"READ");
     TH1D *h_of = (TH1D*)file->Get("CutEMuMT2__SRBin");
     TH1D *h_sf = (TH1D*)file->Get("CutOffZ__SRBin");

     std::vector<float> yields;

     for (int s=1; s<5; s++){
	  yields.push_back(h_of->GetBinContent(s));
     }
     for (int o=5; o<8; o++){
	  yields.push_back(h_sf->GetBinContent(o));
     }

     return yields;

}
