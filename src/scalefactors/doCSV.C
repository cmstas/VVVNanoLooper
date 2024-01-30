//don't store rootfiles in git - here's how I got the CSV file
void doOneCSV(int year=2016, int period = 0, bool iso = true, bool egamma = false){
  string filename;
  string filepath;
  std::ofstream myoutputfile;
  myoutputfile.open("LeptonSF_TopULMVA.csv", std::ios_base::app);
  myoutputfile << "PDGid" << "," << "WP" << "," << "year" << "," << "period" << "," << "ptmin" << "," << "ptmax" << "," << "etamin" << "," << "etamax" << ","<< "SF" << "," << "SFerr" << endl;
  if(egamma){
    filepath += "/home/users/kdownham/public_html/WWZ/Lepton_SFs/Electron/";
    if(year==2016 && period == 0){ 
       filename = "egammaEffi_txt_EGM2D_UL16.root";
       filepath += "2016/";
    }
    if(year==2016 && period != 0){
       filename = "egammaEffi_txt_EGM2D_UL16APV.root";
       filepath += "2016APV/";
    }
    if(year==2017){ 
       filename = "egammaEffi_txt_EGM2D_UL17.root";
       filepath += "2017/";  
    }
    if(year==2018){ 
       filename = "egammaEffi_txt_EGM2D_UL18.root";
       filepath += "2018/";  
    }
    TFile *f = TFile::Open((filepath+filename).c_str());
    TH2F *h = (TH2F*)f->Get("EGamma_SF2D");
    //-2.5 ... 2.5 x //10 ... 500 y
    for(unsigned int y = 1; y<=h->GetNbinsY(); ++y){
      for(unsigned int x = 1; x<=h->GetNbinsX(); ++x){
        myoutputfile << 11 << "," << "Tight" << "," << year << ",X," << h->GetYaxis()->GetBinLowEdge(y) << "," << h->GetYaxis()->GetBinLowEdge(y)+h->GetYaxis()->GetBinWidth(y) << "," << h->GetXaxis()->GetBinLowEdge(x) << "," << h->GetXaxis()->GetBinLowEdge(x)+h->GetXaxis()->GetBinWidth(x) << ","<< h->GetBinContent(x,y) << "," << h->GetBinError(x,y) << endl;
      }
    }
  }
  else {
    string histname;
    filepath += "/home/users/kdownham/public_html/WWZ/Lepton_SFs/Muon/";
    if(!iso){
      if(year==2016 && period == 0) { filename = "NUM_LeptonMvaTight_DEN_TrackerMuons_abseta_pt.root"; filepath += "2016/"; histname = "NUM_LeptonMvaTight_DEN_TrackerMuons_abseta_pt"; }//TH2D 2016 //-2.5 ... 2.5 x //20 ... 120 y
      if(year==2016 && period != 0) { filename = "NUM_LeptonMvaTight_DEN_TrackerMuons_abseta_pt.root"; filepath += "2016APV/"; histname = "NUM_LeptonMvaTight_DEN_TrackerMuons_abseta_pt"; }//TH2D 2016 //-2.5 ... 2.5 x //20 ... 120 y
      if(year==2017) { filename = "NUM_LeptonMvaTight_DEN_TrackerMuons_abseta_pt.root"; filepath += "2017/"; histname = "NUM_LeptonMvaTight_DEN_TrackerMuons_abseta_pt"; }//TH2D 2017 //20 ... 120 x //0 ... 2.4 y
      if(year==2018) { filename = "NUM_LeptonMvaTight_DEN_TrackerMuons_abseta_pt.root"; filepath += "2018/"; histname = "NUM_LeptonMvaTight_DEN_TrackerMuons_abseta_pt"; }//TH2D 2018 //20 ... 120 x //0 ... 2.4 y
    } else {
      if(year==2016 && period == 0) { filename = "2016RunBCDEF_SF_ISO.root"; histname = "NUM_LooseRelIso_DEN_MediumID_eta_pt"; }//TH2D 2016 //-2.5 ... 2.5 x //20 ... 120 y
      if(year==2016 && period != 0) { filename = "2016RunGH_SF_ISO.root";    histname = "NUM_LooseRelIso_DEN_MediumID_eta_pt"; }//TH2D 2016 //-2.5 ... 2.5 x //20 ... 120 y
      if(year==2017) { filename = "2017RunBCDEF_SF_ISO.root"; histname = "NUM_LooseRelIso_DEN_MediumID_pt_abseta"; }//TH2D 2017 //20 ... 120 x //0 ... 2.4 y
      if(year==2018) { filename = "2018RunABCD_SF_ISO.root"; histname = "NUM_LooseRelIso_DEN_MediumID_pt_abseta"; }//TH2D 2018 //20 ... 120 x //0 ... 2.4 y
    }
    string theperiod = "X";
    if(year==2016 && period == 0) theperiod = "BCDEF";
    if(year==2016 && period != 0) theperiod = "GH";
    string WPid = "tightID";
    if(iso) WPid = "looseIso";
    TFile *f = TFile::Open((filepath+filename).c_str());
    TH2D *h = (TH2D*)f->Get(histname.c_str());
    
    if(year==2016){
      for(unsigned int y = 1; y<=h->GetNbinsY(); ++y){
        for(unsigned int x = 1; x<=h->GetNbinsX(); ++x){
          myoutputfile << 13 << "," << WPid << "," << year << "," << theperiod << "," << h->GetYaxis()->GetBinLowEdge(y) << "," << h->GetYaxis()->GetBinLowEdge(y)+h->GetYaxis()->GetBinWidth(y) << "," << h->GetXaxis()->GetBinLowEdge(x) << "," << h->GetXaxis()->GetBinLowEdge(x)+h->GetXaxis()->GetBinWidth(x) << ","<< h->GetBinContent(x,y) << "," << h->GetBinError(x,y) << endl;
        }
      }
    } else {
      for(unsigned int x = 1; x<=h->GetNbinsX(); ++x){
        for(unsigned int y = 1; y<=h->GetNbinsY(); ++y){
          myoutputfile << 13 << "," << WPid << "," << year << "," << theperiod << "," << h->GetXaxis()->GetBinLowEdge(x) << "," << h->GetXaxis()->GetBinLowEdge(x)+h->GetXaxis()->GetBinWidth(x) << "," << h->GetYaxis()->GetBinLowEdge(y) << "," << h->GetYaxis()->GetBinLowEdge(y)+h->GetYaxis()->GetBinWidth(y) << ","<< h->GetBinContent(x,y) << "," << h->GetBinError(x,y) << endl;
        }
      }
    }
  }
  myoutputfile.close();
}
  
void doCSV(){
  //doOneCSV(2016, 0,true , true );
  //doOneCSV(2017, 0,true , true );
  //doOneCSV(2018, 0,true , true );
  //doOneCSV(2016, 0,false, false);
  //doOneCSV(2016, 1,false, false);
  //doOneCSV(2016, 0,true , false);
  //doOneCSV(2016, 1,true , false);
  //doOneCSV(2017, 0,false, false);
  //doOneCSV(2017, 0,true , false);
  //doOneCSV(2018, 0,false, false);
  //doOneCSV(2018, 0,true , false);
  doOneCSV(2016, 0, false, false); // 2016 pre-APV Muons
  doOneCSV(2016, 1, false, false); // 2016 post-APV Muons
  doOneCSV(2017, 0, false, false); // 2017 Muons
  doOneCSV(2018, 0, false, false); // 2018 Muons
  doOneCSV(2016, 0, false, true);  // 2016 pre-APV Electrons
  doOneCSV(2016, 1, false, true);  // 2016 post-APV Electrons
  doOneCSV(2017, 0, false, true);  // 2017 Electrons
  doOneCSV(2018, 0, false, true);  // 2018 Electrons
}
