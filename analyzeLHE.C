void analyzeLHE(TString filepath, TString mode, bool isNew, bool makePlots){

     gROOT->ProcessLine("#include <vector>");

     TString mod = "";
     if (isNew) mod += "Run3";
     if (!isNew) mod += "Run2";

     // Declare tree variables
     int numParticles;
     float eventWeight;
     float eventScale;
     float alphaEM;
     float alphaS;
     std::vector<int> *pdgID = 0;
     std::vector<int> *pdgStatus = 0;
     std::vector<int> *mother1 = 0;
     std::vector<int> *mother2 = 0;
     std::vector<int> *color1 = 0;
     std::vector<int> *color2 = 0;
     std::vector<float> *px = 0;
     std::vector<float> *py = 0;
     std::vector<float> *pz = 0;     
     std::vector<float> *E = 0;
     std::vector<float> *Mass = 0;
     std::vector<float> *Tau = 0;
     std::vector<float> *Spin = 0;

     TFile *input = TFile::Open(filepath); 
     TTree *tree = (TTree*)input->Get("mytree");

     tree->SetBranchAddress("numParticles", &numParticles);          
     tree->SetBranchAddress("eventWeight" , &eventWeight);
     tree->SetBranchAddress("eventScale"  , &eventScale);
     tree->SetBranchAddress("alphaEM"     , &alphaEM);
     tree->SetBranchAddress("alphaS"      , &alphaS);
     tree->SetBranchAddress("pdgID"       , &pdgID);
     tree->SetBranchAddress("pdgStatus"   , &pdgStatus);
     tree->SetBranchAddress("mother1"     , &mother1);
     tree->SetBranchAddress("mother2"     , &mother2);
     tree->SetBranchAddress("color1"      , &color1);
     tree->SetBranchAddress("color2"      , &color2);
     tree->SetBranchAddress("px"          , &px);
     tree->SetBranchAddress("py"          , &py);
     tree->SetBranchAddress("pz"          , &pz);
     tree->SetBranchAddress("E"           , &E);
     tree->SetBranchAddress("Mass"        , &Mass);
     tree->SetBranchAddress("Tau"         , &Tau);
     tree->SetBranchAddress("Spin"        , &Spin);

     // Define WWZ histograms here
     TH1F *h_wp_pt = new TH1F("h_wp_pt","h_wp_pt",100,0.,300.);
     TH1F *h_wp_m  = new TH1F("h_wp_m" ,"h_wp_m" ,100,0.,300.);
     TH1F *h_wm_pt = new TH1F("h_wm_pt","h_wm_pt",100,0.,300.);
     TH1F *h_wm_m  = new TH1F("h_wm_m" ,"h_wm_m" ,100,0.,300.);
     TH1F *h_z_pt = new TH1F("h_z_pt","h_z_pt",100,0.,300.);
     TH1F *h_z_m  = new TH1F("h_z_m" ,"h_z_m" ,100,0.,300.);
     TH1F *h_e_wp_pt = new TH1F("h_e_wp_pt","h_e_wp_pt",100,0.,300.);
     TH1F *h_m_wp_pt = new TH1F("h_m_wp_pt","h_m_wp_pt",100,0.,300.);
     TH1F *h_t_wp_pt = new TH1F("h_t_wp_pt","h_t_wp_pt",100,0.,300.);
     TH1F *h_e_wm_pt = new TH1F("h_e_wm_pt","h_e_wm_pt",100,0.,300.);
     TH1F *h_m_wm_pt = new TH1F("h_m_wm_pt","h_m_wm_pt",100,0.,300.);
     TH1F *h_t_wm_pt = new TH1F("h_t_wm_pt","h_t_wm_pt",100,0.,300.);
     TH1F *h_e_z_pt = new TH1F("h_e_z_pt","h_e_z_pt",100,0.,300.);
     TH1F *h_m_z_pt = new TH1F("h_m_z_pt","h_m_z_pt",100,0.,300.);
     TH1F *h_t_z_pt = new TH1F("h_t_z_pt","h_t_z_pt",100,0.,300.);

     // Define ttZ histograms here
     TH1F *h_t_pt = new TH1F("h_t_pt","h_t_pt",100,0.,300.);
     TH1F *h_t_m  = new TH1F("h_t_m" ,"h_t_m" ,100,0.,300.);
     TH1F *h_at_pt = new TH1F("h_at_pt","h_at_pt",100,0.,300.);
     TH1F *h_at_m  = new TH1F("h_at_m" ,"h_at_m" ,100,0.,300.);
     TH1F *h_b_pt = new TH1F("h_b_pt","h_b_pt",100,0.,300.);
     TH1F *h_b_m  = new TH1F("h_b_m" ,"h_b_m" ,100,0.,10.);

     TH1F *h_e_mll = new TH1F("h_e_mll","h_e_mll",100,0.,300.);
     TH1F *h_m_mll = new TH1F("h_m_mll","h_m_mll",100,0.,300.);
     TH1F *h_t_mll = new TH1F("h_t_mll","h_t_mll",100,0.,300.); 

     // Consistency checks (WWZ)
     // ----------------------------------------------------------
     // How many events have a W+?
     // How many times does W+ go to e/mu/tau?
     // How many events have a W-?
     // How many times does W- go to e/mu/tau?
     // How many events have a Z?
     // How many times does Z go to e/mu/tau?

     // WWZ variables
     int Wp = 0;
     int Wm = 0;
     //int Z = 0;
     int WW = 0;
     int WW_4l = 0;
     int WWZ = 0;
     int WWZ_4l = 0;
     int fourlep = 0;
     //int Wm_e = 0;
     //int Wm_m = 0;
     //int Wm_t = 0;
     //int Wp_e = 0;
     //int Wp_m = 0;
     //int Wp_t = 0;
     //int Z_ee = 0;
     //int Z_mm = 0;
     //int Z_tt = 0;

     // ttZ variables
     int two_tops = 0;
     int two_b_W = 0;
     int ttZ = 0;
     int ttZ_4l = 0;
     int n_ee = 0;
     int n_mm = 0;
     int n_tt = 0;
 
     // Shared variables
     int Z = 0;
     int Z_ee = 0;
     int Z_mm = 0;
     int Z_tt = 0;
     int Wm_e = 0;
     int Wm_m = 0;
     int Wm_t = 0;
     int Wp_e = 0;
     int Wp_m = 0;
     int Wp_t = 0;
     

     for (int i=0; i<tree->GetEntries(); i++){
	  tree->GetEntry(i);

	  if ( mode == "WWZ" ){

	       bool has_Wp = false;
               bool has_Wm = false;
               bool has_Z = false;
               bool has_4l = false;
               int numLeps = 0;
               for ( int p=0; p<numParticles; p++ ){
                     if ( pdgID->at(p) == 23 ){
			  has_Z = true;
			  Z++;
			  h_z_pt->Fill(sqrt(std::pow(px->at(p),2.)+std::pow(py->at(p),2.)),eventWeight);
			  h_z_m->Fill(Mass->at(p),eventWeight);
		     }
		     if ( pdgID->at(p) == 24 ){
			  has_Wp = true;
			  Wp++;
			  h_wp_pt->Fill(sqrt(std::pow(px->at(p),2.)+std::pow(py->at(p),2.)),eventWeight);
			  h_wp_m->Fill(Mass->at(p),eventWeight);
		     }
		     if ( pdgID->at(p) == -24 ){
			  has_Wm = true;
			  Wm++;
			  h_wm_pt->Fill(sqrt(std::pow(px->at(p),2.)+std::pow(py->at(p),2.)),eventWeight);
			  h_wm_m->Fill(Mass->at(p),eventWeight);
		     }
		     int id = pdgID->at(p);
		     if ( std::abs(id) == 11 || std::abs(id) == 13 || std::abs(id) == 15 ) numLeps++;

     		     // Show the branching fraction of W's and Z to e/mu/tau from simulation. Also show pT distributions
     		     if ( std::abs(id) == 11 ){
			  if (pdgID->at(mother1->at(p)-1) == 23){ 
			      Z_ee++;
			      h_e_z_pt->Fill(sqrt(std::pow(px->at(p),2.)+std::pow(py->at(p),2.)),eventWeight);
			  }
			  if (pdgID->at(mother1->at(p)-1) == 24){
                              Wp_e++;
                              h_e_wp_pt->Fill(sqrt(std::pow(px->at(p),2.)+std::pow(py->at(p),2.)),eventWeight);
                          }
			  if (pdgID->at(mother1->at(p)-1) == -24){
                              Wm_e++;
                              h_e_wm_pt->Fill(sqrt(std::pow(px->at(p),2.)+std::pow(py->at(p),2.)),eventWeight);
                          }
		     }
		     
                     if ( std::abs(id) == 13 ){
                          if (pdgID->at(mother1->at(p)-1) == 23){
                              Z_mm++;
                              h_m_z_pt->Fill(sqrt(std::pow(px->at(p),2.)+std::pow(py->at(p),2.)),eventWeight);
                          }
                          if (pdgID->at(mother1->at(p)-1) == 24){
                              Wp_m++;
                              h_m_wp_pt->Fill(sqrt(std::pow(px->at(p),2.)+std::pow(py->at(p),2.)),eventWeight);
                          }
                          if (pdgID->at(mother1->at(p)-1) == -24){
                              Wm_m++;
                              h_m_wm_pt->Fill(sqrt(std::pow(px->at(p),2.)+std::pow(py->at(p),2.)),eventWeight);
                          }
                     }

                     if ( std::abs(id) == 15 ){
                          if (pdgID->at(mother1->at(p)-1) == 23){
                              Z_tt++;
                              h_t_z_pt->Fill(sqrt(std::pow(px->at(p),2.)+std::pow(py->at(p),2.)),eventWeight);
                          }
                          if (pdgID->at(mother1->at(p)-1) == 24){
                              Wp_t++;
                              h_t_wp_pt->Fill(sqrt(std::pow(px->at(p),2.)+std::pow(py->at(p),2.)),eventWeight);
                          }
                          if (pdgID->at(mother1->at(p)-1) == -24){
                              Wm_t++;
                              h_t_wm_pt->Fill(sqrt(std::pow(px->at(p),2.)+std::pow(py->at(p),2.)),eventWeight);
                          }
                     }

               }               

               if ( numLeps == 4 ){ 
		    fourlep++;
		    has_4l = true;
               }

               if ( has_Wp && has_Wm ) WW++;
               if ( has_Wp && has_Wm && has_4l ) WW_4l++;
               if ( has_Wp && has_Wm && has_Z ) WWZ++;
               if ( has_Wp && has_Wm && has_Z && has_4l ) WWZ_4l++;

	  }

          if ( mode == "TTZ_M10" ){
	       bool top = false;
               bool antitop = false;
               bool t_b = false;
               bool at_b = false;
	       bool t_W = false;
	       bool at_W = false;
	       bool has_Z = false;
	       bool Wplus_e = false;
	       bool Wplus_m = false;
               bool Wplus_t = false;
               bool Wminus_e = false;
               bool Wminus_m = false;
               bool Wminus_t = false;
	       bool Z_e = false;
	       bool Z_m = false;
               bool Z_t = false;

	       // Loop over particles in the event
               for ( int k=0; k<numParticles; k++ ){
		     int id = pdgID->at(k);
		     //std::cout << "Debug 1" << std::endl;
		     // Find the Z boson
		     if ( id == 23 ){
                          has_Z = true;
			  h_z_pt->Fill(sqrt(std::pow(px->at(k),2.)+std::pow(py->at(k),2.)),eventWeight);
			  h_z_m->Fill(Mass->at(k),eventWeight);
		     }
		     // Find the top/antitop quarks
		     if ( id == 6 ){
			  top = true;
			  // Fill top histograms?
			  h_t_pt->Fill(sqrt(std::pow(px->at(k),2.)+std::pow(py->at(k),2.)),eventWeight);
			  h_t_m->Fill(Mass->at(k),eventWeight);
		     }
		     if ( id == -6 ){
			  antitop = true;
			  // Fill antitop histograms?
			  h_at_pt->Fill(sqrt(std::pow(px->at(k),2.)+std::pow(py->at(k),2.)),eventWeight);
                          h_at_m->Fill(Mass->at(k),eventWeight);
		     }
		     // Find the bW from the top
		     if ( id == 5 && k > 1 ){
			  if ( std::abs(pdgID->at(mother1->at(k)-1)) == 6 ){
			       t_b = true;
			       h_b_pt->Fill(sqrt(std::pow(px->at(k),2.)+std::pow(py->at(k),2.)),eventWeight);
                               h_b_m->Fill(Mass->at(k),eventWeight);
			  }
                     }
	             if ( id == 24 ){
			  if ( std::abs(pdgID->at(mother1->at(k)-1)) == 6 ){
			       t_W = true;
			       h_wp_pt->Fill(sqrt(std::pow(px->at(k),2.)+std::pow(py->at(k),2.)),eventWeight);
                               h_wp_m->Fill(Mass->at(k),eventWeight);
			  }
		     }
		     // Find the bW from the antitop
		     if ( id == -5 && k > 1 ){
                          if ( std::abs(pdgID->at(mother1->at(k)-1)) == 6 ){
                               at_b = true;
			       h_b_pt->Fill(sqrt(std::pow(px->at(k),2.)+std::pow(py->at(k),2.)),eventWeight);
                               h_b_m->Fill(Mass->at(k),eventWeight);
                          }
                     }
                     if ( id == -24 ){
			  if ( std::abs(pdgID->at(mother1->at(k)-1)) == 6 ){
                               at_W = true;
			       h_wm_pt->Fill(sqrt(std::pow(px->at(k),2.)+std::pow(py->at(k),2.)),eventWeight);
                               h_wm_m->Fill(Mass->at(k),eventWeight);
                          }
                     }

                     // Now find the leptons------------------------------------------
                     // Electrons
                     if ( std::abs(id) == 11 ){
			  // Parent is W+
			  if ( pdgID->at(mother1->at(k)-1) == 24 ){ 
			       Wplus_e = true;
			       h_e_wp_pt->Fill(sqrt(std::pow(px->at(k),2.)+std::pow(py->at(k),2.)),eventWeight);

			  }
			  // Parent is W-
			  if ( pdgID->at(mother1->at(k)-1) == -24 ){ 
			       Wminus_e = true;
			       h_e_wm_pt->Fill(sqrt(std::pow(px->at(k),2.)+std::pow(py->at(k),2.)),eventWeight);
			  }
			  // Parent is Z
			  if ( pdgID->at(mother1->at(k)-1) == 23 ){ 
			       Z_e = true;
			       h_e_z_pt->Fill(sqrt(std::pow(px->at(k),2.)+std::pow(py->at(k),2.)),eventWeight);
			  }
			  // Loop over particles to find electrons to compute m_ll
			  for (int j=k+1; j < numParticles; j++){
			       if ( pdgID->at(j) + id == 0 ){
				    bool parent_bW = (std::abs(pdgID->at(mother1->at(j)-1)) == 24 || std::abs(pdgID->at(mother1->at(k)-1)) == 24 || std::abs(pdgID->at(mother1->at(j)-1)) == 5 || std::abs(pdgID->at(mother1->at(k)-1)) == 5);
				    if (parent_bW) continue;
				    float E_2 = std::pow(E->at(j)+E->at(k),2.);
				    float p_2 = (std::pow(px->at(j)+px->at(k),2.) + std::pow(py->at(j)+py->at(k),2.) + std::pow(pz->at(j)+pz->at(k),2.));
				    float m_ll = sqrt(E_2 - p_2);
				    if ( m_ll < 10. ){ 
				         n_ee++;
					 std::cout << "Mass of electron pair = " << m_ll << std::endl;
					 std::cout << "PdgID of e1 parent = " << pdgID->at(mother1->at(k)-1) << std::endl;
					 std::cout << "PdgID of e2 parent = " << pdgID->at(mother1->at(j)-1) << std::endl;
				    }
				    h_e_mll->Fill(m_ll,eventWeight);
			       }
			  }
			  
		     }
	             // Muons
	             //std::cout << "Debug 7" << std::endl;
	             if ( std::abs(id) == 13 ){
			  // Parent is W+
			  if ( pdgID->at(mother1->at(k)-1) == 24 ){ 
			       Wplus_m = true;
			       h_m_wp_pt->Fill(sqrt(std::pow(px->at(k),2.)+std::pow(py->at(k),2.)),eventWeight);
			  }
                          // Parent is W-
			  if ( pdgID->at(mother1->at(k)-1) == -24 ){ 
			       Wminus_m = true;
			       h_m_wm_pt->Fill(sqrt(std::pow(px->at(k),2.)+std::pow(py->at(k),2.)),eventWeight);
			  }
			  // Parent is Z
			  if ( pdgID->at(mother1->at(k)-1) == 23 ){ 
			       Z_m = true;
			       h_m_z_pt->Fill(sqrt(std::pow(px->at(k),2.)+std::pow(py->at(k),2.)),eventWeight);
			  }
			  for (int j=k+1; j < numParticles; j++){
                               if ( pdgID->at(j) + id == 0 ){
				    bool parent_bW = (std::abs(pdgID->at(mother1->at(j)-1)) == 24 || std::abs(pdgID->at(mother1->at(k)-1)) == 24 || std::abs(pdgID->at(mother1->at(j)-1)) == 5 || std::abs(pdgID->at(mother1->at(k)-1)) == 5);
                                    if (parent_bW) continue;
				    float E_2 = std::pow(E->at(j)+E->at(k),2.);
                                    float p_2 = (std::pow(px->at(j)+px->at(k),2.) + std::pow(py->at(j)+py->at(k),2.) + std::pow(pz->at(j)+pz->at(k),2.));
                                    float m_ll = sqrt(E_2 - p_2);
				    if ( m_ll < 10. ) n_mm++;
                                    h_m_mll->Fill(m_ll,eventWeight);
                               }
                          }
		     }
		     // Taus
		     //std::cout << "Debug 8" << std::endl;
		     if ( std::abs(id) == 15 ){
			  // Parent is W+
			  if ( pdgID->at(mother1->at(k)-1) == 24 ){ 
			       Wplus_t = true;
			       h_t_wp_pt->Fill(sqrt(std::pow(px->at(k),2.)+std::pow(py->at(k),2.)),eventWeight);
			  }
			  // Parent is W-
			  if ( pdgID->at(mother1->at(k)-1) == -24 ){ 
			       Wminus_t = true;
			       h_t_wm_pt->Fill(sqrt(std::pow(px->at(k),2.)+std::pow(py->at(k),2.)),eventWeight);
			  }
			  // Parent is Z
			  if ( pdgID->at(mother1->at(k)-1) == 23 ){ 
			       Z_t = true;
			       h_t_z_pt->Fill(sqrt(std::pow(px->at(k),2.)+std::pow(py->at(k),2.)),eventWeight);
			  }
                     
		          for (int j=k+1; j < numParticles; j++){
                               if ( pdgID->at(j) + id == 0 ){
				    bool parent_bW = (std::abs(pdgID->at(mother1->at(j)-1)) == 24 || std::abs(pdgID->at(mother1->at(k)-1)) == 24 || std::abs(pdgID->at(mother1->at(j)-1)) == 5 || std::abs(pdgID->at(mother1->at(k)-1)) == 5);
                                    if (parent_bW) continue;
				    float E_2 = std::pow(E->at(j)+E->at(k),2.);
                                    float p_2 = (std::pow(px->at(j)+px->at(k),2.) + std::pow(py->at(j)+py->at(k),2.) + std::pow(pz->at(j)+pz->at(k),2.));
                                    float m_ll = sqrt(E_2 - p_2);
				    if ( m_ll < 10. ) n_tt++;
                                    h_t_mll->Fill(m_ll,eventWeight);
                               }
                          }

		     }

	       }

	       if ( Wplus_e ) Wp_e++;
               if ( Wplus_m ) Wp_m++;
               if ( Wplus_t ) Wp_t++;
	       if ( Wminus_e ) Wm_e++;
               if ( Wminus_m ) Wm_m++;
               if ( Wminus_t ) Wm_t++;
	       if ( Z_e ) Z_ee++;
               if ( Z_m ) Z_mm++;
               if ( Z_t ) Z_tt++;

               bool four_leps = ( (Wplus_e || Wplus_m || Wplus_t) && (Wminus_e || Wminus_m || Wminus_t) && (Z_e || Z_m || Z_t) );

               if ( has_Z ) Z++;
	       if ( top && antitop ) two_tops++;
	       if ( top && antitop && t_b && t_W && at_b && at_W ) two_b_W++;
	       if ( has_Z && top && antitop /*&& t_b && t_W && at_b && at_W*/ ) ttZ++;
	       if ( has_Z && top && antitop && /*t_b && t_W && at_b && at_W &&*/ four_leps ) ttZ_4l++;

	  } 

     }

     if ( mode == "TTZ_M10" && makePlots ){
	  std::cout << "==============================" << std::endl;
	  std::cout << "Total number of events = " << tree->GetEntries() << std::endl;
	  std::cout << "------------------------------" << std::endl;
	  std::cout << "Number of events with a Z boson = " << Z << std::endl;
	  std::cout << "Number of events with two top quarks = " << two_tops << std::endl;
          std::cout << "Number of events with two top quarks to bW = " << two_b_W << std::endl;
          std::cout << "Number of events with ttZ = " << ttZ << std::endl;
	  std::cout << "Number of events with ttZ -> 4 leptons = " << ttZ_4l << std::endl;
	  std::cout << "-----------------------------------------" << std::endl;
	  std::cout << "Branching Fractions" << std::endl;
	  std::cout << "-----------------------------------------" << std::endl;
	  std::cout << "W- to electron = " << Wm_e << std::endl;
          std::cout << "W- to muon = " << Wm_m << std::endl;
          std::cout << "W- to tau = " << Wm_t << std::endl;
          std::cout << "SUM = " << Wm_e + Wm_m + Wm_t << std::endl;
	  std::cout << "-----------------------------------------" << std::endl;
          std::cout << "W+ to electron = " << Wp_e << std::endl;
          std::cout << "W+ to muon = " << Wp_m << std::endl;
          std::cout << "W+ to tau = " << Wp_t << std::endl;
          std::cout << "SUM = " << Wp_e + Wp_m + Wp_t << std::endl;
	  std::cout << "-----------------------------------------" << std::endl;
          std::cout << "Z to electrons = " << Z_ee << std::endl;
          std::cout << "Z to muons = " << Z_mm << std::endl;
          std::cout << "Z to taus = " << Z_tt << std::endl;
          std::cout << "SUM = " << Z_ee + Z_mm + Z_tt << std::endl;
	  std::cout << "====================================================" << std::endl;
	  std::cout << "Sanity check" << std::endl;
	  std::cout << "Number of OF ee pairs with m_ll < 10 GeV = " << n_ee << std::endl;
	  std::cout << "Number of OF mm pairs with m_ll < 10 GeV = " << n_mm << std::endl;
          std::cout << "Number of OF tt pairs with m_ll < 10 GeV = " << n_tt << std::endl;

          //TCanvas *cs = new TCanvas("cs","cs",10,10,1400,900);
          //cs->Divide(3,2);

          //cs->cd(1);
          //h_wp_pt->Draw("hist");
          //cs->cd(2);
          //h_wm_pt->Draw("hist");
	  //cs->cd(3);
	  //h_z_pt->Draw("hist");
	  //cs->cd(4);
	  //h_wp_m->Draw("hist");
	  //cs->cd(5);
          //h_wm_m->Draw("hist");
          //cs->cd(6);
          //h_z_m->Draw("hist");

          ////cs->Print(mode+"_LHEPlots_"+mod+".png");

          //TCanvas *cs2 = new TCanvas("cs2","cs2",10,10,1400,900);
          //cs2->Divide(3,2);

          //cs2->cd(1);
	  //h_t_pt->Draw("hist");
          //cs2->cd(2);
          //h_at_pt->Draw("hist");
          //cs2->cd(3);
          //h_b_pt->Draw("hist");
	  //cs2->cd(4);
          //h_t_m->Draw("hist");
	  //cs2->cd(5);
          //h_at_m->Draw("hist");
          //cs2->cd(6);
          //h_b_m->Draw("hist");

          ////cs2->Print(mode+"_LHEPlots_quarks_"+mod+".png");

          //TCanvas *cs1 = new TCanvas("cs1","cs1",10,10,1400,900);
          //cs1->Divide(3,3);
          //cs1->cd(1);
          //h_e_wp_pt->Draw("hist");
          //cs1->cd(2);
          //h_m_wp_pt->Draw("hist");
          //cs1->cd(3);
          //h_t_wp_pt->Draw("hist");
          //cs1->cd(4);
          //h_e_wm_pt->Draw("hist");
          //cs1->cd(5);
          //h_m_wm_pt->Draw("hist");
          //cs1->cd(6);
          //h_t_wm_pt->Draw("hist");
          //cs1->cd(7);
          //h_e_z_pt->Draw("hist");
          //cs1->cd(8);
          //h_m_z_pt->Draw("hist");
          //cs1->cd(9);
          //h_t_z_pt->Draw("hist");

          ////cs1->Print(mode+"_LHEPlots_leptons_"+mod+".png");

          TCanvas *cs3 = new TCanvas("cs3","cs3",10,10,1400,450);
          cs3->Divide(3,1);
	  cs3->cd(1);
          h_e_mll->Draw("hist");
	  cs3->cd(2);
          h_m_mll->Draw("hist");
          cs3->cd(3);
          h_t_mll->Draw("hist");

          cs3->Print(mode+"_LHEPlots_mll_"+mod+".png");
          
     } 

     if ( mode == "WWZ" ){
     std::cout << "Total number of events: " << tree->GetEntries() << std::endl;
     std::cout << "-----------------------------" << std::endl;
     std::cout << "Number of events with a W- = " << Wm << std::endl;
     std::cout << "W- to electron = " << Wm_e << std::endl;
     std::cout << "W- to muon = " << Wm_m << std::endl;
     std::cout << "W- to tau = " << Wm_t << std::endl;
     std::cout << "SUM = " << Wm_e + Wm_m + Wm_t << std::endl;
     std::cout << "-----------------------------" << std::endl;
     std::cout << "Number of events with a W+ = " << Wp << std::endl;
     std::cout << "W+ to electron = " << Wp_e << std::endl;
     std::cout << "W+ to muon = " << Wp_m << std::endl;
     std::cout << "W+ to tau = " << Wp_t << std::endl;
     std::cout << "SUM = " << Wp_e + Wp_m + Wp_t << std::endl;
     std::cout << "-----------------------------" << std::endl;
     std::cout << "Number of events with a Z  = " << Z << std::endl;
     std::cout << "Z to electrons = " << Z_ee << std::endl;
     std::cout << "Z to muons = " << Z_mm << std::endl;
     std::cout << "Z to taus = " << Z_tt << std::endl;
     std::cout << "SUM = " << Z_ee + Z_mm + Z_tt << std::endl;
     std::cout << "-----------------------------" << std::endl;
     std::cout << "Number of events with 4leps = " << fourlep << std::endl;
     std::cout << "Number of events with WW = " << WW << std::endl;
     std::cout << "Number of events with WW and 4 leps = " << WW_4l << std::endl;
     std::cout << "Number of events with WWZ = " << WWZ << std::endl;
     std::cout << "Number of events with WWZ -> 4l = " << WWZ_4l << std::endl;

     TCanvas *cs = new TCanvas("cs","cs",10,10,1400,900);
     cs->Divide(3,2);

     cs->cd(1);
     h_wp_pt->Draw("hist");
     cs->cd(2);
     h_wm_pt->Draw("hist");
     cs->cd(3);
     h_z_pt->Draw("hist");
     cs->cd(4);
     h_wp_m->Draw("hist");
     cs->cd(5);
     h_wm_m->Draw("hist");
     cs->cd(6);
     h_z_m->Draw("hist");
     
     cs->Print(mode+"_LHEPlots_"+mod+".png");

     TCanvas *cs1 = new TCanvas("cs1","cs1",10,10,1400,900);
     cs1->Divide(3,3);
     cs1->cd(1);
     h_e_wp_pt->Draw("hist");
     cs1->cd(2);
     h_m_wp_pt->Draw("hist");
     cs1->cd(3);
     h_t_wp_pt->Draw("hist");
     cs1->cd(4);
     h_e_wm_pt->Draw("hist");
     cs1->cd(5);
     h_m_wm_pt->Draw("hist");
     cs1->cd(6);
     h_t_wm_pt->Draw("hist");
     cs1->cd(7);
     h_e_z_pt->Draw("hist");
     cs1->cd(8);
     h_m_z_pt->Draw("hist");
     cs1->cd(9);
     h_t_z_pt->Draw("hist");

     cs1->Print(mode+"_LHEPlots_leptons_"+mod+".png");

     }

}
