#include "vvvtree.h"
#include "rooutil.h"
#include "cxxopts.h"

float BLIND() { return vvv.Common_isData() ? 1.: 1.; }

class AnalysisConfig {

public:

    // TString that holds the input file list (comma separated)
    TString input_file_list_tstring;

    // TString that holds the name of the TTree to open for each input files
    TString input_tree_name;

    // Output TFile
    TFile* output_tfile;

    // Number of events to loop over
    int n_events;

    // Jobs to split (if this number is positive, then we will skip certain number of events)
    // If there are N events, and was asked to split 2 ways, then depending on job_index, it will run over first half or latter half
    int nsplit_jobs;

    // Job index (assuming nsplit_jobs is set, the job_index determine where to loop over)
    int job_index;

    // EFT reweighting index
    int eft_reweighting_idx;

    // Debug boolean
    bool debug;

    // Lepton ID boolean
    //bool new_lepton_ID = true;

    // TChain that holds the input TTree's
    TChain* events_tchain;

    // Custom Looper object to facilitate looping over many files
    RooUtil::Looper<vvvtree> looper;

    // Custom Cutflow framework
    RooUtil::Cutflow cutflow;

    // Custom Histograms object compatible with RooUtil::Cutflow framework
    RooUtil::Histograms histograms;

    RooUtil::Histograms histograms_3LepTau;


};

AnalysisConfig ana;

// ./process INPUTFILEPATH OUTPUTFILE [NEVENTS]
int main(int argc, char** argv)
{

    bool new_lepton_ID = true;
//********************************************************************************
//
// 1. Parsing options
//
//********************************************************************************

    // cxxopts is just a tool to parse argc, and argv easily

    // Grand option setting
    cxxopts::Options options("\n  $ doAnalysis",  "\n         **********************\n         *                    *\n         *       Looper       *\n         *                    *\n         **********************\n");

    // Read the options
    options.add_options()
        ("i,input"       , "Comma separated input file list OR if just a directory is provided it will glob all in the directory BUT must end with '/' for the path", cxxopts::value<std::string>())
        ("t,tree"        , "Name of the tree in the root file to open and loop over"                                             , cxxopts::value<std::string>())
        ("o,output"      , "Output file name"                                                                                    , cxxopts::value<std::string>())
        ("n,nevents"     , "N events to loop over"                                                                               , cxxopts::value<int>()->default_value("-1"))
        ("j,nsplit_jobs" , "Enable splitting jobs by N blocks (--job_index must be set)"                                         , cxxopts::value<int>())
        ("I,job_index"   , "job_index of split jobs (--nsplit_jobs must be set. index starts from 0. i.e. 0, 1, 2, 3, etc...)"   , cxxopts::value<int>())
        ("d,debug"       , "Run debug job. i.e. overrides output option to 'debug.root' and 'recreate's the file.")
        ("e,eftidx"      , "EFT reweighting index"                                                                               , cxxopts::value<int>())
        ("h,help"        , "Print help")
        ;

    auto result = options.parse(argc, argv);

    // NOTE: When an option was provided (e.g. -i or --input), then the result.count("<option name>") is more than 0
    // Therefore, the option can be parsed easily by asking the condition if (result.count("<option name>");
    // That's how the several options are parsed below

    //_______________________________________________________________________________
    // --help
    if (result.count("help"))
    {
        std::cout << options.help() << std::endl;
        exit(1);
    }

    //_______________________________________________________________________________
    // --input
    if (result.count("input"))
    {
        ana.input_file_list_tstring = result["input"].as<std::string>();
    }
    else
    {
        std::cout << options.help() << std::endl;
        std::cout << "ERROR: Input list is not provided! Check your arguments" << std::endl;
        exit(1);
    }

    //_______________________________________________________________________________
    // --tree
    if (result.count("tree"))
    {
        ana.input_tree_name = result["tree"].as<std::string>();
    }
    else
    {
        std::cout << options.help() << std::endl;
        std::cout << "ERROR: Input tree name is not provided! Check your arguments" << std::endl;
        exit(1);
    }

    //_______________________________________________________________________________
    // --debug
    if (result.count("debug"))
    {
        ana.output_tfile = new TFile("debug.root", "recreate");
    }
    else
    {
        //_______________________________________________________________________________
        // --output
        if (result.count("output"))
        {
            ana.output_tfile = new TFile(result["output"].as<std::string>().c_str(), "create");
            if (not ana.output_tfile->IsOpen())
            {
                std::cout << options.help() << std::endl;
                std::cout << "ERROR: output already exists! provide new output name or delete old file. OUTPUTFILE=" << result["output"].as<std::string>() << std::endl;
                exit(1);
            }
        }
        else
        {
            std::cout << options.help() << std::endl;
            std::cout << "ERROR: Output file name is not provided! Check your arguments" << std::endl;
            exit(1);
        }
    }

    //_______________________________________________________________________________
    // --nevents
    ana.n_events = result["nevents"].as<int>();

    //_______________________________________________________________________________
    // --nsplit_jobs
    if (result.count("nsplit_jobs"))
    {
        ana.nsplit_jobs = result["nsplit_jobs"].as<int>();
        if (ana.nsplit_jobs <= 0)
        {
            std::cout << options.help() << std::endl;
            std::cout << "ERROR: option string --nsplit_jobs" << ana.nsplit_jobs << " has zero or negative value!" << std::endl;
            std::cout << "I am not sure what this means..." << std::endl;
            exit(1);
        }
    }
    else
    {
        ana.nsplit_jobs = -1;
    }

    //_______________________________________________________________________________
    // --nsplit_jobs
    if (result.count("job_index"))
    {
        ana.job_index = result["job_index"].as<int>();
        if (ana.job_index < 0)
        {
            std::cout << options.help() << std::endl;
            std::cout << "ERROR: option string --job_index" << ana.job_index << " has negative value!" << std::endl;
            std::cout << "I am not sure what this means..." << std::endl;
            exit(1);
        }
    }
    else
    {
        ana.job_index = -1;
    }

    //_______________________________________________________________________________
    // --eftidx
    if (result.count("eftidx"))
    {
        ana.eft_reweighting_idx = result["eftidx"].as<int>();
    }
    else
    {
        ana.eft_reweighting_idx = 0;
    }


    // Sanity check for split jobs (if one is set the other must be set too)
    if (result.count("job_index") or result.count("nsplit_jobs"))
    {
        // If one is not provided then throw error
        if ( not (result.count("job_index") and result.count("nsplit_jobs")))
        {
            std::cout << options.help() << std::endl;
            std::cout << "ERROR: option string --job_index and --nsplit_jobs must be set at the same time!" << std::endl;
            exit(1);
        }
        // If it is set then check for sanity
        else
        {
            if (ana.job_index >= ana.nsplit_jobs)
            {
                std::cout << options.help() << std::endl;
                std::cout << "ERROR: --job_index >= --nsplit_jobs ! This does not make sense..." << std::endl;
                exit(1);
            }
        }
    }

    //
    // Printing out the option settings overview
    //
    std::cout <<  "=========================================================" << std::endl;
    std::cout <<  " Setting of the analysis job based on provided arguments " << std::endl;
    std::cout <<  "---------------------------------------------------------" << std::endl;
    std::cout <<  " ana.input_file_list_tstring: " << ana.input_file_list_tstring <<  std::endl;
    std::cout <<  " ana.output_tfile: " << ana.output_tfile->GetName() <<  std::endl;
    std::cout <<  " ana.n_events: " << ana.n_events <<  std::endl;
    std::cout <<  " ana.nsplit_jobs: " << ana.nsplit_jobs <<  std::endl;
    std::cout <<  " ana.job_index: " << ana.job_index <<  std::endl;
    std::cout <<  "=========================================================" << std::endl;

//********************************************************************************
//
// 2. Opening input baby files
//
//********************************************************************************

    // Create the TChain that holds the TTree's of the baby ntuples
    ana.events_tchain = RooUtil::FileUtil::createTChain(ana.input_tree_name, ana.input_file_list_tstring);

    // Create a Looper object to loop over input files
    // the "www" object is defined in the wwwtree.h/cc
    // This is an instance which helps read variables in the WWW baby TTree
    // It is a giant wrapper that facilitates reading TBranch values.
    // e.g. if there is a TBranch named "lep_pt" which is a std::vector<float> then, one can access the branch via
    //
    //    std::vector<float> lep_pt = www.lep_pt();
    //
    // and no need for "SetBranchAddress" and declaring variable shenanigans necessary
    // This is a standard thing SNT does pretty much every looper we use
    ana.looper.init(ana.events_tchain, &vvv, ana.n_events);

//********************************************************************************
//
// Interlude... notes on RooUtil framework
//
//********************************************************************************

    // ~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=
    // Quick tutorial on RooUtil::Cutflow object cut tree formation
    // ~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=
    //
    // The RooUtil::Cutflow object facilitates creating a tree structure of cuts
    //
    // To add cuts to each node of the tree with cuts defined, use "addCut" or "addCutToLastActiveCut"
    // The "addCut" or "addCutToLastActiveCut" accepts three argument, <name>, and two lambda's that define the cut selection, and the weight to apply to that cut stage
    //
    // e.g. To create following cut-tree structure one does
    //
    //             (Root) <--- Always exists as soon as RooUtil::Cutflow object is created. But this is basically hidden underneath and users do not have to care
    //                |
    //            CutWeight
    //            |       |
    //     CutPreSel1    CutPreSel2
    //       |                  |
    //     CutSel1           CutSel2
    //
    //
    //   code:
    //
    //      // Create the object (Root node is created as soon as the instance is created)
    //      RooUtil::Cutflow cutflow;
    //
    //      cutflow.addCut("CutWeight"                 , <lambda> , <lambda>); // CutWeight is added below "Root"-node
    //      cutflow.addCutToLastActiveCut("CutPresel1" , <lambda> , <lambda>); // The last "active" cut is "CutWeight" since I just added that. So "CutPresel1" is added below "CutWeight"
    //      cutflow.addCutToLastActiveCut("CutSel1"    , <lambda> , <lambda>); // The last "active" cut is "CutPresel1" since I just added that. So "CutSel1" is added below "CutPresel1"
    //
    //      cutflow.getCut("CutWeight"); // By "getting" the cut object, this makes the "CutWeight" the last "active" cut.
    //      cutflow.addCutToLastActiveCut("CutPresel2" , <lambda> , <lambda>); // The last "active" cut is "CutWeight" since I "getCut" on it. So "CutPresel2" is added below "CutWeight"
    //      cutflow.addCutToLastActiveCut("CutSel2"    , <lambda> , <lambda>); // The last "active" cut is "CutPresel2" since I just added that. So "CutSel2" is added below "CutPresel1"
    //
    // (Side note: "UNITY" lambda is defined in the framework to just return 1. This so that use don't have to type [&]() {return 1;} so many times.)
    //
    // Once the cutflow is formed, create cutflow histograms can be created by calling RooUtil::Cutflow::bookCutflows())
    // This function looks through the terminating nodes of the tree structured cut tree. and creates a histogram that will fill the yields
    // For the example above, there are two terminationg nodes, "CutSel1", and "CutSel2"
    // So in this case Root::Cutflow::bookCutflows() will create two histograms. (Actually four histograms.)
    //
    //  - TH1F* type object :  CutSel1_cutflow (4 bins, with first bin labeled "Root", second bin labeled "CutWeight", third bin labeled "CutPreSel1", fourth bin labeled "CutSel1")
    //  - TH1F* type object :  CutSel2_cutflow (...)
    //  - TH1F* type object :  CutSel1_rawcutflow (...)
    //  - TH1F* type object :  CutSel2_rawcutflow (...)
    //                                ^
    //                                |
    // NOTE: There is only one underscore "_" between <CutName>_cutflow or <CutName>_rawcutflow
    //
    // And later in the loop when RooUtil::Cutflow::fill() function is called, the tree structure will be traversed through and the appropriate yields will be filled into the histograms
    //
    // After running the loop check for the histograms in the output root file

    // ~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=
    // Quick tutorial on RooUtil::Histograms object
    // ~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=
    //
    // The RooUtil::Histograms object facilitates book keeping histogram definitions
    // And in conjunction with RooUtil::Cutflow object, one can book same histograms across different cut stages easily without copy pasting codes many times by hand.
    //
    // The histogram addition happens in two steps.
    // 1. Defining histograms
    // 2. Booking histograms to cuts
    //
    // Histograms are defined via following functions
    //
    //      RooUtil::Histograms::addHistogram       : Typical 1D histogram (TH1F*) "Fill()" called once per event
    //      RooUtil::Histograms::addVecHistogram    : Typical 1D histogram (TH1F*) "Fill()" called multiple times per event
    //      RooUtil::Histograms::add2DHistogram     : Typical 2D histogram (TH2F*) "Fill()" called once per event
    //      RooUtil::Histograms::add2DVecHistogram  : Typical 2D histogram (TH2F*) "Fill()" called multiple times per event
    // e.g.
    //
    //    RooUtil::Histograms histograms;
    //    histograms.addHistogram   ("MllSS"    , 180 , 0. , 300. , [&]() { return www.MllSS()  ; }); // The lambda returns float
    //    histograms.addVecHistogram("AllLepPt" , 180 , 0. , 300. , [&]() { return www.lep_pt() ; }); // The lambda returns vector<float>
    //
    // The addVecHistogram will have lambda to return vector<float> and it will loop over the values and call TH1F::Fill() for each item
    //
    // To book histograms to cuts one uses
    //
    //      RooUtil::Cutflow::bookHistogramsForCut()
    //      RooUtil::Cutflow::bookHistogramsForCutAndBelow()
    //      RooUtil::Cutflow::bookHistogramsForCutAndAbove()
    //      RooUtil::Cutflow::bookHistogramsForEndCuts()
    //
    // e.g. Given a tree like the following, we can book histograms to various cuts as we want
    //
    //              Root
    //                |
    //            CutWeight
    //            |       |
    //     CutPreSel1    CutPreSel2
    //       |                  |
    //     CutSel1           CutSel2
    //
    // For example,
    //
    //    1. book a set of histograms to one cut:
    //
    //       cutflow.bookHistogramsForCut(histograms, "CutPreSel2")
    //
    //    2. book a set of histograms to a cut and below
    //
    //       cutflow.bookHistogramsForCutAndBelow(histograms, "CutWeight") // will book a set of histograms to CutWeight, CutPreSel1, CutPreSel2, CutSel1, and CutSel2
    //
    //    3. book a set of histograms to a cut and above (... useless...?)
    //
    //       cutflow.bookHistogramsForCutAndAbove(histograms, "CutPreSel2") // will book a set of histograms to CutPreSel2, CutWeight (nothing happens to Root node)
    //
    //    4. book a set of histograms to a terminating nodes
    //
    //       cutflow.bookHistogramsForEndCuts(histograms) // will book a set of histograms to CutSel1 and CutSel2
    //
    // The naming convention of the booked histograms are as follows
    //
    //   cutflow.bookHistogramsForCut(histograms, "CutSel1");
    //
    //  - TH1F* type object : CutSel1__MllSS;
    //  - TH1F* type object : CutSel1__AllLepPt;
    //                               ^^
    //                               ||
    // NOTE: There are two underscores "__" between <CutName>__<HistogramName>
    //
    // And later in the loop when RooUtil::CutName::fill() function is called, the tree structure will be traversed through and the appropriate histograms will be filled with appropriate variables
    // After running the loop check for the histograms in the output root file

    // Set the cutflow object output file
    ana.cutflow.setTFile(ana.output_tfile);

    ana.cutflow.addCut("CutWeight_3LepTau", UNITY, [&]()
                       {
                           bool isWWZEFT = ana.looper.getCurrentFileName().Contains("WWZ_RunIISummer20UL18NanoAODv9_FourleptonFilter_FilterFix_merged");
                           bool isWZZEFT = ana.looper.getCurrentFileName().Contains("WZZ_RunIISummer20UL18NanoAODv9_FourleptonFilter_FilterFix_merged");
                           bool isZZZEFT = ana.looper.getCurrentFileName().Contains("ZZZ_RunIISummer20UL18NanoAODv9_FourleptonFilter_FilterFix_merged");

                           float sm_weight = 1;
                           sm_weight = (isWWZEFT ? vvv.Common_LHEWeight_mg_reweighting()[0] * 0.1651 * 0.3272 * 0.3272 * 0.1009 /0.0005972 : 1.)
                                     * (isWZZEFT ? vvv.Common_LHEWeight_mg_reweighting()[0] : 1.)
                                     * (isZZZEFT ? vvv.Common_LHEWeight_mg_reweighting()[0] : 1.)
                                     ;
                                     float eftweight = 1;
                                     eftweight = (isWWZEFT ? vvv.Common_LHEWeight_mg_reweighting()[ana.eft_reweighting_idx] * 0.1651 * 0.3272 * 0.3272 * 0.1009 /0.0005972 : 1.)
                                     * (isWZZEFT ? vvv.Common_LHEWeight_mg_reweighting()[ana.eft_reweighting_idx] : 1.)
                                     * (isZZZEFT ? vvv.Common_LHEWeight_mg_reweighting()[ana.eft_reweighting_idx] : 1.)                                                                                                                                 ; 
  
                                     float weight = ana.eft_reweighting_idx != 0 ? (eftweight - sm_weight) : sm_weight;

        			     if (ana.looper.getCurrentFileName().Contains("WWZJets"))
                                     {
                                         weight = 0.002067 / 0.0005972;
                                     }
                                    
        			     //float weight_0 = 0.;
 
        			     //if ( vvv.Var_3LepTauMET_scaleLumi() < -100. ) return weight_0;
                                     return vvv.Var_3LepTauMET_scaleLumi() * vvv.Common_genWeight() * weight;
                                                                                                                                                                                                                                                                                                                                                                                                                                                              } );
    ana.cutflow.addCutToLastActiveCut("CutAdditionalLeptonID_3LepTau",
        			      [&]()
        			      {
        				  // Z candidate leptons
        				  std::vector<int> Zcand_lep_idxs = {vvv.Var_3LepTauMET_Zcand_lep_idx_0(), vvv.Var_3LepTauMET_Zcand_lep_idx_1()};

        				  for (auto& idx : Zcand_lep_idxs){
        				      // Electron
        				      if (abs(vvv.Common_lep_pdgid()[idx]) == 11){
        					  if (not (vvv.Common_lep_sip3d()[idx] < 4)) return false;
        					  if (not (vvv.Common_lep_relIso03_all()[idx] < 0.2)) return false;
        				      }
        				      // Muon
        				      if (abs(vvv.Common_lep_pdgid()[idx]) == 13){
        					  if (not (vvv.Common_lep_sip3d()[idx] < 4)) return false;
        					  if (not (new_lepton_ID)){
        					      if (not ((vvv.Common_lep_ID()[idx] >> 2) >= 2)) return false;
        					  }
        				      }
        				  }

        				  // W candidate lepton
        				  int W_idx = vvv.Var_3LepTauMET_other_lep_idx_0();
        				  // W candidate is electron
        				  if (abs(vvv.Common_lep_pdgid()[W_idx]) == 11){
        				      if (not (vvv.Common_lep_sip3d()[W_idx] < 4)) return false;
        				      if (not (vvv.Common_lep_relIso03_all()[W_idx] < 0.2)) return false;
        				      if (not (new_lepton_ID)){
        					  if (not (vvv.Common_lep_ID()[W_idx] & (1 << 4))) return false; 
        				      }
        				  }
        				  // W candidate is muon
        				  if (abs(vvv.Common_lep_pdgid()[W_idx]) == 13){
        				      if (not (vvv.Common_lep_sip3d()[W_idx] < 4)) return false;
        				      if (not (new_lepton_ID)){
        					  if (not ((vvv.Common_lep_ID()[W_idx] >> 2) >= 3)) return false;
        				      }
        				  }

        			          double max_pt = std::max({vvv.Var_3LepTauMET_Zcand_lep_p4_0().pt(),vvv.Var_3LepTauMET_Zcand_lep_p4_1().pt(),vvv.Var_3LepTauMET_other_lep_p4_0().pt()});
        				  if (not ( max_pt > 25. )) return false;

        				  double lepton_pts[] = {vvv.Var_3LepTauMET_Zcand_lep_p4_0().pt(),vvv.Var_3LepTauMET_Zcand_lep_p4_1().pt(),vvv.Var_3LepTauMET_other_lep_p4_0().pt()};
        				  int n = sizeof(lepton_pts) / sizeof(lepton_pts[0]);
        				  std::sort(lepton_pts, lepton_pts+n,greater<double>());

        				  if (not ( lepton_pts[1] > 15. )) return false;

        				  return true;
        		
        			      },
        			      [&]()
        			      {
        				  return 1.;
        			      });

    ana.cutflow.addCutToLastActiveCut("CutDuplicate_3LepTau", UNITY,
        			      [&]()
        			      {
        				  if (vvv.Common_isData())
        				  {
        				      duplicate_removal::DorkyEventIdentifier id(vvv.Common_run(), vvv.Common_evt(), vvv.Common_lumi());
        				      if (is_duplicate(id))
        					  return false;
        				      else
        					  return true;
        				  }
        				  else
        				  {
        				      return true;
        				  }
        			      
        			       } );

    // Apply min Mll > 12 GeV selection on any pair of opposite-sign charged leptons
    ana.cutflow.addCutToLastActiveCut("CutVetoLowMassResonance_3LepTau",
            [&]()
            {

        	// Loop over light charged leptons
        	for (unsigned int ilep = 0; ilep < vvv.Common_lep_pdgid().size(); ++ilep){
        	     const LorentzVector& ip4 = vvv.Common_lep_p4()[ilep];
        	     // Loop over other light leptons
        	     for (unsigned int jlep = ilep + 1; jlep < vvv.Common_lep_pdgid().size(); ++jlep){
        		  const LorentzVector& jp4 = vvv.Common_lep_p4()[jlep];
        		  if ( not ( vvv.Common_lep_pdgid()[ilep]*vvv.Common_lep_pdgid()[jlep] < 0 ) ) continue;
        		  if ( not ( (ip4+jp4).mass() > 12.) ) return false;
        	     }
        	     // Do the same with the tau candidate
        	     const LorentzVector& tau_p4 = vvv.Var_3LepTauMET_tau_p4_0();
        	     if ( not ( vvv.Common_lep_pdgid()[ilep]*vvv.Var_3LepTauMET_tau_pdgid_0() < 0 ) ) continue;
        	     if ( not ( (ip4+tau_p4).mass() > 12.) ) return false;
        	}

        	return true;

            }, UNITY);

    ana.cutflow.addCutToLastActiveCut("CutVetoZHZWW_3LepTau", [&]()
        			      {
        				  if (ana.looper.getCurrentFileName().Contains("VHToNonbb"))
        				  {
        				      if (vvv.Common_gen_VH_channel() == 1){
        					  return false;
        				      }
        				  }
        				  return true;
        			      }, UNITY);

    ana.cutflow.addCutToLastActiveCut("CutPresel_3LepTau", UNITY, UNITY);

    ana.cutflow.getCut("CutPresel_3LepTau");
    ana.cutflow.addCutToLastActiveCut("CutBVeto_3LepTau", [&]() { return vvv.Common_nb_loose_CSV() == 0; }, UNITY);

    ana.cutflow.getCut("CutBVeto_3LepTau");
    ana.cutflow.addCutToLastActiveCut("CutETau", [&]() { return vvv.Cut_3LepTauMET_etauChannel(); }, UNITY);

    ana.cutflow.getCut("CutBVeto_3LepTau");
    ana.cutflow.addCutToLastActiveCut("CutMuTau", [&]() { return vvv.Cut_3LepTauMET_mutauChannel(); }, UNITY);


    LorentzVector leading_tau_p4;
    int ntaus;
    // TODO: Need to do this by accessing the Common_tau_ branch and selecting the highest pt one that passes the ID requirements
    
    ana.cutflow.getCut("CutBVeto_3LepTau");
    ana.cutflow.addCutToLastActiveCut("CutHighMET_3LepTau", [&]() {

					if ( vvv.Common_met_p4().pt() < 100. ) return false;

					return true; 

				     }, UNITY);

    ana.cutflow.getCut("CutBVeto_3LepTau");
    ana.cutflow.addCutToLastActiveCut("CutHighMETandMT2_3LepTau", [&]() {

					bool mt2_cut = ( vvv.Var_3LepTauMET_mt2() > 25. );
                                        if ( !mt2_cut || vvv.Common_met_p4().pt() < 100. ) return false;

                                        return true;

                                     }, UNITY);

    ana.cutflow.getCut("CutBVeto_3LepTau");
    ana.cutflow.addCutToLastActiveCut("CutMedMET_3LepTau", [&]() {

                                        if ( vvv.Common_met_p4().pt() > 100. || vvv.Common_met_p4().pt() < 50. ) return false;

                                        return true;

                                     }, UNITY);

    ana.cutflow.getCut("CutBVeto_3LepTau");
    ana.cutflow.addCutToLastActiveCut("CutMedMETandMT2_3LepTau", [&]() {

                                        bool mt2_cut = ( vvv.Var_3LepTauMET_mt2() > 25. );
                                        if ( !mt2_cut || vvv.Common_met_p4().pt() > 100. || vvv.Common_met_p4().pt() < 50. ) return false;

                                        return true;

                                     }, UNITY);

    ana.cutflow.getCut("CutBVeto_3LepTau");
    ana.cutflow.addCutToLastActiveCut("CutLowMET_3LepTau", [&]() {

                                        if ( vvv.Common_met_p4().pt() > 50. ) return false;

                                        return true;

                                     }, UNITY);

    ana.cutflow.getCut("CutBVeto_3LepTau");
    ana.cutflow.addCutToLastActiveCut("CutLowMETandMT2_3LepTau", [&]() {

                                        bool mt2_cut = ( vvv.Var_3LepTauMET_mt2() > 25. );
                                        if ( !mt2_cut || vvv.Common_met_p4().pt() > 50. ) return false;

                                        return true;

                                     }, UNITY);

    ana.cutflow.getCut("CutETau");
    ana.cutflow.addCutToLastActiveCut("CutETau_idVS", [&]() {
        			
        			     std::vector<float> tau_pts;	
        			     for (unsigned int itau = 0; itau < vvv.Common_tau_p4().size(); itau++){
        				  int idVSe = vvv.Common_tau_idVSe()[itau];
        				  int idVSmu = vvv.Common_tau_idVSmu()[itau];
        				  int idVSjet = vvv.Common_tau_idVSjet()[itau];
        				 
        				  if (not ((idVSe >= 1)&&(idVSmu >= 0)&&(idVSjet >= 7)) ) continue;
        				  tau_pts.push_back(vvv.Common_tau_p4()[itau].pt());
        			     }

        		             if ( tau_pts.size() < 1 ) return false;

        			     return true;
        
                                     }, UNITY);



    ana.cutflow.getCut("CutMuTau");
    ana.cutflow.addCutToLastActiveCut("CutMuTau_idVS", [&]() {

        			     std::vector<float> tau_pts;
                                     for (unsigned int itau = 0; itau < vvv.Common_tau_p4().size(); itau++){
                                          int idVSe = vvv.Common_tau_idVSe()[itau];
                                          int idVSmu = vvv.Common_tau_idVSmu()[itau];
                                          int idVSjet = vvv.Common_tau_idVSjet()[itau];

                                          if (not ((idVSe >= 1)&&(idVSmu >= 0)&&(idVSjet >= 7)) ) continue;
                                          tau_pts.push_back(vvv.Common_tau_p4()[itau].pt());
                                     }

                                     if ( tau_pts.size() < 1 ) return false;

                                     return true;
                              

                                     }, UNITY);

    


    // Print cut structure
    ana.cutflow.printCuts();

    // Histogram utility object that is used to define the histograms
        
    // Histograms for 3-Lepton + Tau Final State
    ana.histograms_3LepTau.addHistogram("Zcand_mll", 180, 60, 120, [&]() { return vvv.Var_3LepTauMET_Zcand_mll(); } );
    ana.histograms_3LepTau.addHistogram("Zcand_mll_full", 180, 0, 120, [&]() { return vvv.Var_3LepTauMET_Zcand_mll(); } );
    ana.histograms_3LepTau.addHistogram("Zcand_lep0_pt", 180, 0, 150, [&]() { return vvv.Var_3LepTauMET_Zcand_lep_p4_0().pt(); } );
    ana.histograms_3LepTau.addHistogram("Zcand_lep1_pt", 180, 0, 150, [&]() { return vvv.Var_3LepTauMET_Zcand_lep_p4_1().pt(); } );
    ana.histograms_3LepTau.addHistogram("other_mll", 180, 60, 120, [&]() { return (vvv.Var_3LepTauMET_other_lep_p4_0()+leading_tau_p4).mass(); } );
    ana.histograms_3LepTau.addHistogram("other_mll_full", 180, 0, 300, [&]() { return (vvv.Var_3LepTauMET_other_lep_p4_0()+leading_tau_p4).mass(); } );
    ana.histograms_3LepTau.addHistogram("other_mll_varbin", {0., 40., 60., 100., 200.}, [&]() { return (vvv.Var_3LepTauMET_other_lep_p4_0()+leading_tau_p4).mass(); } );
    ana.histograms_3LepTau.addHistogram("other_lep0_pt", 180, 0, 150, [&]() { return vvv.Var_3LepTauMET_other_lep_p4_0().pt(); } );
    ana.histograms_3LepTau.addHistogram("tau0_pt", 180, 0, 150, [&]() { return leading_tau_p4.pt(); } );
    ana.histograms_3LepTau.addHistogram("other_lep0_sip3d", 180, 0, 10, [&]() { return abs(vvv.Common_lep_sip3d()[vvv.Var_3LepTauMET_other_lep_idx_0()]); } );
    ana.histograms_3LepTau.addHistogram("other_lep0_dxy", 180, 0, 0.01, [&]() { return abs(vvv.Common_lep_dxy()[vvv.Var_3LepTauMET_other_lep_idx_0()]); } );
    ana.histograms_3LepTau.addHistogram("other_lep0_dz", 180, 0, 0.1, [&]() { return abs(vvv.Common_lep_dz()[vvv.Var_3LepTauMET_other_lep_idx_0()]); } );
    ana.histograms_3LepTau.addHistogram("MET", 180, 0, 300., [&]() { return vvv.Common_met_p4().pt(); } );
    ana.histograms_3LepTau.addHistogram("pfMET", 180, 0, 300., [&]() { return vvv.Common_met_p4_MET().pt(); } );
    ana.histograms_3LepTau.addHistogram("PuppiMET", 180, 0, 300., [&]() { return vvv.Common_met_p4_PuppiMET().pt(); } );
    ana.histograms_3LepTau.addHistogram("Pt4l", 180, 0., 300., [&]() { return (vvv.Var_3LepTauMET_Zcand_lep_p4_0()+vvv.Var_3LepTauMET_Zcand_lep_p4_1()+vvv.Var_3LepTauMET_other_lep_p4_0()+leading_tau_p4).pt(); } );
    ana.histograms_3LepTau.addHistogram("nb", 5, 0, 5, [&]() { return vvv.Common_nb_loose_CSV(); } );
    ana.histograms_3LepTau.addHistogram("Yield", 1, 0, 1, [&]() { return 0; } );
    ana.histograms_3LepTau.addHistogram("ntau", 5, 0, 5, [&]() { return ntaus; } );
    ana.histograms_3LepTau.addHistogram("MT2", 180, 0., 200., [&]() { return vvv.Var_3LepTauMET_mt2(); } );
    ana.histograms_3LepTau.addVecHistogram("SRBin_MET", 3, 0, 3,
					[&]()
					{
					    std::vector<float> rtn;
					    if ( vvv.Common_met_p4().pt() < 50. ) rtn.push_back(0.);
					    if ( vvv.Common_met_p4().pt() > 50. && vvv.Common_met_p4().pt() < 100. ) rtn.push_back(1.);
					    if ( vvv.Common_met_p4().pt() > 100. ) rtn.push_back(2.);
					
					    return rtn;

				 } );

    ana.histograms_3LepTau.addVecHistogram("SRBin_METandMT2", 3, 0, 3,
					[&]()
					{
					    std::vector<float> rtn;
					    bool mt2_cut = ( vvv.Var_3LepTauMET_mt2() > 25. );
					    if ( mt2_cut && vvv.Common_met_p4().pt() < 50. ) rtn.push_back(0.);
					    if ( mt2_cut && vvv.Common_met_p4().pt() > 50. && vvv.Common_met_p4().pt() < 100. ) rtn.push_back(1.);
					    if ( mt2_cut && vvv.Common_met_p4().pt() > 100. ) rtn.push_back(2.);

					    return rtn;

				 } );

    // Book cutflows
    ana.cutflow.bookCutflows();

    // Book Histograms
    ana.cutflow.bookHistogramsForCutAndBelow(ana.histograms_3LepTau, "CutDuplicate_3LepTau");

    // Book Event list
    ana.cutflow.bookEventLists();

    // Load event list
    //RooUtil::EventList eventlist_to_check("eventlist_to_check.txt");

    // Looping input file
    while (ana.looper.nextEvent())
    {

        // If splitting jobs are requested then determine whether to process the event or not based on remainder
        if (result.count("job_index") and result.count("nsplit_jobs"))
        {
            if (ana.looper.getNEventsProcessed() % ana.nsplit_jobs != (unsigned int) ana.job_index)
                continue;
        }


        if (ana.cutflow.getCut("CutETau").pass || ana.cutflow.getCut("CutMuTau").pass){
                std::vector<LorentzVector> tau_p4_vec;
                for (unsigned int itau = 0; itau < vvv.Common_tau_p4().size(); itau++){
                        int idVSe = vvv.Common_tau_idVSe()[itau];
                        int idVSmu = vvv.Common_tau_idVSmu()[itau];
                        int idVSjet = vvv.Common_tau_idVSjet()[itau];

                        //if ( not ((idVSe >= 1) && (idVSmu >= 0) && (idVSjet >= 7)) ) continue;
                        if ( not ((idVSe >= 15 /*medium WP*/) && (idVSmu >= 3 /*medium WP*/) && (idVSjet >= 7 /*Loose WP*/)) ) continue;
                        //if ( not ((idVSe >= 31 /*tight WP*/) && (idVSmu >= 7 /*tight WP*/) && (idVSjet >= 7 /*Loose WP*/)) ) continue;
                        tau_p4_vec.push_back(vvv.Common_tau_p4()[itau]);
                }
                ntaus = tau_p4_vec.size();
                if ( ntaus < 1 ) continue;
                leading_tau_p4 = tau_p4_vec[0];
        }

        ana.cutflow.setEventID(vvv.Common_run(), vvv.Common_lumi(), vvv.Common_evt());

        //Do what you need to do in for each event here
        //To save use the following function
        ana.cutflow.fill();

	

    }


    // Writing output file
    ana.cutflow.saveOutput();

    // The below can be sometimes crucial
    delete ana.output_tfile;
}

