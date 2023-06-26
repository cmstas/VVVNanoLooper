#include "Process_Common.h"
#include "MuonIDHelper.h"
#include "ElectronIDHelper.h"

void Process_Common()
{
    Process_Common_NanoAOD();
}

void Process_Common_NanoAOD()
{
    //==============================================
    // Process_Common:
    // This function gets called during the event looping.
    // This is where one sets the variables used for the category Common.
    //==============================================

    // Set variables used in this category.
    // If histograms are booked with these variables the histograms will be filled automatically.
    // Please follow the convention of <category>_<varname> structure.

    //---------------------------------------------------------------------------------------------
    // Event information
    //---------------------------------------------------------------------------------------------
    // Flag for lepton ID
    bool new_lepton_ID = true;  // true for new ID, false for old ID
    const ULong64_t event_check = 1800002;

    if (nt.event() == event_check) std::cout << "Debug 0.1" << endl;

    // Event level information
    ana.tx.setBranch<int>                  ("Common_isData", nt.isData());
    ana.tx.setBranch<int>                  ("Common_run", nt.run());
    ana.tx.setBranch<int>                  ("Common_lumi", nt.luminosityBlock());
    ana.tx.setBranch<unsigned long long>   ("Common_evt", nt.event());

    //std::cout << "Run = " << nt.run() << endl;
    //std::cout << "Lumi Block = " << nt.luminosityBlock() << endl;
    //std::cout << "Event = " << nt.event() << endl;

    try { ana.tx.setBranch<float>("Common_event_puWeight"                                       , nt.puWeight());                                         } catch (std::runtime_error) { ana.tx.setBranch<float>("Common_event_puWeight"                                     , 1.); }
    try { ana.tx.setBranch<float>("Common_event_puWeightup"                                     , nt.puWeightUp());                                       } catch (std::runtime_error) { ana.tx.setBranch<float>("Common_event_puWeightup"                                   , 1.); }
    try { ana.tx.setBranch<float>("Common_event_puWeightdn"                                     , nt.puWeightDown());                                     } catch (std::runtime_error) { ana.tx.setBranch<float>("Common_event_puWeightdn"                                   , 1.); }

    try { ana.tx.setBranch<float>("Common_event_prefireWeight"                                  , nt.PrefireWeight());                                    } catch (std::runtime_error) { ana.tx.setBranch<float>("Common_event_prefireWeight"                                , 1.); }
    try { ana.tx.setBranch<float>("Common_event_prefireWeightup"                                , nt.PrefireWeight_Up());                                 } catch (std::runtime_error) { ana.tx.setBranch<float>("Common_event_prefireWeightup"                              , 1.); }
    try { ana.tx.setBranch<float>("Common_event_prefireWeightdn"                                , nt.PrefireWeight_Down());                               } catch (std::runtime_error) { ana.tx.setBranch<float>("Common_event_prefireWeightdn"                         , 1.); }
    if (not nt.isData())
    {
        ana.tx.setBranch<float>            ("Common_genWeight", nt.genWeight());
        if (nt.year() < 2022)
        {
            if (nt.year() == 2016)
                ana.tx.setBranch<float>        ("Common_btagWeight_DeepCSVB", 1); // TODO
            else
                ana.tx.setBranch<float>        ("Common_btagWeight_DeepCSVB", nt.btagWeight_DeepCSVB());
        }        
        if (ana.is_EFT_sample)
            ana.tx.setBranch<vector<float>>("Common_LHEWeight_mg_reweighting", nt.LHEReweightingWeight());
        ana.tx.setBranch<float>            ("Common_wgt", ana.wgt * ((nt.genWeight() > 0) - (nt.genWeight() < 0)));
    }
    else
    {
        ana.tx.setBranch<float>            ("Common_genWeight", 1);
        if (nt.year() < 2022)
            ana.tx.setBranch<float>            ("Common_btagWeight_DeepCSVB", 1);
        ana.tx.setBranch<float>            ("Common_wgt", 1);
    }

    //---------------------------------------------------------------------------------------------
    // Trigger information
    //---------------------------------------------------------------------------------------------

    if (nt.event() == event_check) std::cout << "Debug 0.2" << endl;

    try { ana.tx.setBranch<bool>("Common_HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ"                , nt.HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ());                } catch (std::runtime_error) { ana.tx.setBranch<bool>("Common_HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ"                , 0); } // Lowest unprescaled
    try { ana.tx.setBranch<bool>("Common_HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL"                   , nt.HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL());                   } catch (std::runtime_error) { ana.tx.setBranch<bool>("Common_HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL"                   , 0); } // Lowest unprescaled
    try { ana.tx.setBranch<bool>("Common_HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_Mass3p8"        , nt.HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_Mass3p8());        } catch (std::runtime_error) { ana.tx.setBranch<bool>("Common_HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_Mass3p8"        , 0); } // Lowest unprescaled for >= 2017C
    try { ana.tx.setBranch<bool>("Common_HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL_DZ"          , nt.HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL_DZ());          } catch (std::runtime_error) { ana.tx.setBranch<bool>("Common_HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL_DZ"          , 0); } 
    try { ana.tx.setBranch<bool>("Common_HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL"             , nt.HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL());             } catch (std::runtime_error) { ana.tx.setBranch<bool>("Common_HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL"             , 0); } // Lowest unprescaled
    try { ana.tx.setBranch<bool>("Common_HLT_Mu23_TrkIsoVVL_Ele12_CaloIdL_TrackIdL_IsoVL_DZ" , nt.HLT_Mu23_TrkIsoVVL_Ele12_CaloIdL_TrackIdL_IsoVL_DZ()); } catch (std::runtime_error) { ana.tx.setBranch<bool>("Common_HLT_Mu23_TrkIsoVVL_Ele12_CaloIdL_TrackIdL_IsoVL_DZ" , 0); } // Lowest unprescaled
    try { ana.tx.setBranch<bool>("Common_HLT_Mu23_TrkIsoVVL_Ele12_CaloIdL_TrackIdL_IsoVL"    , nt.HLT_Mu23_TrkIsoVVL_Ele12_CaloIdL_TrackIdL_IsoVL());    } catch (std::runtime_error) { ana.tx.setBranch<bool>("Common_HLT_Mu23_TrkIsoVVL_Ele12_CaloIdL_TrackIdL_IsoVL"    , 0); } 
    try { ana.tx.setBranch<bool>("Common_HLT_Mu8_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL_DZ"  , nt.HLT_Mu8_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL_DZ());  } catch (std::runtime_error) { ana.tx.setBranch<bool>("Common_HLT_Mu8_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL_DZ"  , 0); } // Lowest unprescaled
    try { ana.tx.setBranch<bool>("Common_HLT_Mu8_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL"     , nt.HLT_Mu8_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL());     } catch (std::runtime_error) { ana.tx.setBranch<bool>("Common_HLT_Mu8_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL"     , 0); } 
    try { ana.tx.setBranch<bool>("Common_HLT_PFHT1050"                                       , nt.HLT_PFHT1050());                                       } catch (std::runtime_error) { ana.tx.setBranch<bool>("Common_HLT_PFHT1050"     , 0); } 
    try { ana.tx.setBranch<bool>("Common_HLT_AK8PFJet500"                                    , nt.HLT_AK8PFJet500());                                    } catch (std::runtime_error) { ana.tx.setBranch<bool>("Common_HLT_AK8PFJet500"  , 0); } 
    try { ana.tx.setBranch<bool>("Common_HLT_AK8PFJet380_TrimMass30"     , nt.HLT_AK8PFJet380_TrimMass30());     } catch (std::runtime_error) { ana.tx.setBranch<bool>("Common_HLT_AK8PFJet380_TrimMass30"     , 0); } 
    try { ana.tx.setBranch<bool>("Common_HLT_AK8PFJet360_TrimMass30"     , nt.HLT_AK8PFJet360_TrimMass30());     } catch (std::runtime_error) { ana.tx.setBranch<bool>("Common_HLT_AK8PFJet360_TrimMass30"     , 0); } 
    try { ana.tx.setBranch<bool>("Common_HLT_AK8PFJet400_TrimMass30"     , nt.HLT_AK8PFJet400_TrimMass30());     } catch (std::runtime_error) { ana.tx.setBranch<bool>("Common_HLT_AK8PFJet400_TrimMass30"     , 0); } 
    try { ana.tx.setBranch<bool>("Common_HLT_AK8PFJet420_TrimMass30"     , nt.HLT_AK8PFJet420_TrimMass30());     } catch (std::runtime_error) { ana.tx.setBranch<bool>("Common_HLT_AK8PFJet420_TrimMass30"     , 0); } 
    try { ana.tx.setBranch<bool>("Common_HLT_AK8PFHT750_TrimMass50"     , nt.HLT_AK8PFHT750_TrimMass50());     } catch (std::runtime_error) { ana.tx.setBranch<bool>("Common_HLT_AK8PFHT750_TrimMass50"     , 0); } 
    try { ana.tx.setBranch<bool>("Common_HLT_AK8PFHT800_TrimMass50"     , nt.HLT_AK8PFHT800_TrimMass50());     } catch (std::runtime_error) { ana.tx.setBranch<bool>("Common_HLT_AK8PFHT800_TrimMass50"     , 0); } 
    try { ana.tx.setBranch<bool>("Common_HLT_AK8PFHT850_TrimMass50"     , nt.HLT_AK8PFHT850_TrimMass50());     } catch (std::runtime_error) { ana.tx.setBranch<bool>("Common_HLT_AK8PFHT850_TrimMass50"     , 0); } 
    try { ana.tx.setBranch<bool>("Common_HLT_AK8PFHT900_TrimMass50"     , nt.HLT_AK8PFHT900_TrimMass50());     } catch (std::runtime_error) { ana.tx.setBranch<bool>("Common_HLT_AK8PFHT900_TrimMass50"     , 0); } 


    if (nt.event() == event_check) std::cout << "Debug 0.3" << endl;

    bool is_pd_ee = ana.looper.getCurrentFileName().Contains("DoubleEG") or ana.looper.getCurrentFileName().Contains("EGamma");
    bool is_pd_em = ana.looper.getCurrentFileName().Contains("MuonEG");
    bool is_pd_mm = ana.looper.getCurrentFileName().Contains("DoubleMuon");
    bool pass_duplicate_ee_em_mm = false;
    bool pass_duplicate_mm_em_ee = false;

    bool trig_ee = false;
    bool trig_em = false;
    bool trig_mm = false;

    switch (nt.year())
    {
        case 2016:
            trig_ee = ana.tx.getBranch<bool>("Common_HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL_DZ");
            trig_em = ana.tx.getBranch<bool>("Common_HLT_Mu23_TrkIsoVVL_Ele12_CaloIdL_TrackIdL_IsoVL") or ana.tx.getBranch<bool>("Common_HLT_Mu8_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL") or
                      ana.tx.getBranch<bool>("Common_HLT_Mu23_TrkIsoVVL_Ele12_CaloIdL_TrackIdL_IsoVL_DZ") or ana.tx.getBranch<bool>("Common_HLT_Mu8_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL_DZ");
            trig_mm = ana.tx.getBranch<bool>("Common_HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ") or
                      ana.tx.getBranch<bool>("Common_HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL");
            break;
        case 2017:
            trig_ee = ana.tx.getBranch<bool>("Common_HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL");
            trig_em = ana.tx.getBranch<bool>("Common_HLT_Mu23_TrkIsoVVL_Ele12_CaloIdL_TrackIdL_IsoVL_DZ") or ana.tx.getBranch<bool>("Common_HLT_Mu8_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL_DZ");
            trig_mm = ana.tx.getBranch<bool>("Common_HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_Mass3p8");
            break;
        case 2018:
            trig_ee = ana.tx.getBranch<bool>("Common_HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL");
            trig_em = ana.tx.getBranch<bool>("Common_HLT_Mu23_TrkIsoVVL_Ele12_CaloIdL_TrackIdL_IsoVL_DZ") or ana.tx.getBranch<bool>("Common_HLT_Mu8_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL_DZ");
            trig_mm = ana.tx.getBranch<bool>("Common_HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_Mass3p8");
            break;
    }

    if (nt.event() == event_check) std::cout << "Debug 0.4" << endl;

    ana.tx.setBranch<bool>("Common_HLT_DoubleEl", trig_ee);
    ana.tx.setBranch<bool>("Common_HLT_MuEG", trig_em);
    ana.tx.setBranch<bool>("Common_HLT_DoubleMu", trig_mm);

    if (is_pd_ee)
    {
        if (trig_ee)
            pass_duplicate_ee_em_mm = true;
        if (not trig_mm and not trig_em and trig_ee)
            pass_duplicate_mm_em_ee = true;
    }
    else if (is_pd_em)
    {
        if (not trig_ee and trig_em)
            pass_duplicate_ee_em_mm = true;
        if (not trig_mm and trig_em)
            pass_duplicate_mm_em_ee = true;
    }
    else if (is_pd_mm)
    {
        if (not trig_ee and not trig_em and trig_mm)
            pass_duplicate_ee_em_mm = true;
        if (trig_mm)
            pass_duplicate_mm_em_ee = true;
    }

    if (nt.event() == event_check) std::cout << "Debug 0.5" << endl;

    ana.tx.setBranch<bool>("Common_pass_duplicate_removal_ee_em_mm", pass_duplicate_ee_em_mm); // Flag to identify whether the event passes duplicate removal
    ana.tx.setBranch<bool>("Common_pass_duplicate_removal_mm_em_ee", pass_duplicate_mm_em_ee); // Flag to identify whether the event passes duplicate removal
    bool filterflag   = false;
    bool filterflagMC = false;
    switch (nt.year())
    {
        case 2016:
            filterflag   = nt.Flag_goodVertices() and nt.Flag_globalSuperTightHalo2016Filter() and nt.Flag_HBHENoiseFilter() and nt.Flag_HBHENoiseIsoFilter() and nt.Flag_EcalDeadCellTriggerPrimitiveFilter() and nt.Flag_BadPFMuonFilter();
            filterflagMC = nt.Flag_goodVertices() and                                              nt.Flag_HBHENoiseFilter() and nt.Flag_HBHENoiseIsoFilter() and nt.Flag_EcalDeadCellTriggerPrimitiveFilter() and nt.Flag_BadPFMuonFilter();
            break;
        case 2017:
            filterflag   = nt.Flag_goodVertices() and nt.Flag_globalSuperTightHalo2016Filter() and nt.Flag_HBHENoiseFilter() and nt.Flag_HBHENoiseIsoFilter() and nt.Flag_EcalDeadCellTriggerPrimitiveFilter() and nt.Flag_BadPFMuonFilter() and nt.Flag_ecalBadCalibFilter();
            filterflagMC = nt.Flag_goodVertices() and                                              nt.Flag_HBHENoiseFilter() and nt.Flag_HBHENoiseIsoFilter() and nt.Flag_EcalDeadCellTriggerPrimitiveFilter() and nt.Flag_BadPFMuonFilter() and nt.Flag_ecalBadCalibFilter();
            break;
        case 2018:
            filterflag   = nt.Flag_goodVertices() and nt.Flag_globalSuperTightHalo2016Filter() and nt.Flag_HBHENoiseFilter() and nt.Flag_HBHENoiseIsoFilter() and nt.Flag_EcalDeadCellTriggerPrimitiveFilter() and nt.Flag_BadPFMuonFilter() and nt.Flag_ecalBadCalibFilter();
            filterflagMC = nt.Flag_goodVertices() and                                              nt.Flag_HBHENoiseFilter() and nt.Flag_HBHENoiseIsoFilter() and nt.Flag_EcalDeadCellTriggerPrimitiveFilter() and nt.Flag_BadPFMuonFilter() and nt.Flag_ecalBadCalibFilter();
            break;
        case 2022:
            filterflag   = nt.Flag_goodVertices() and nt.Flag_globalSuperTightHalo2016Filter() and nt.Flag_HBHENoiseFilter() and nt.Flag_HBHENoiseIsoFilter() and nt.Flag_EcalDeadCellTriggerPrimitiveFilter() and nt.Flag_BadPFMuonFilter() and nt.Flag_ecalBadCalibFilter();
            filterflagMC = nt.Flag_goodVertices() and                                              nt.Flag_HBHENoiseFilter() and nt.Flag_HBHENoiseIsoFilter() and nt.Flag_EcalDeadCellTriggerPrimitiveFilter() and nt.Flag_BadPFMuonFilter() and nt.Flag_ecalBadCalibFilter();
            break;
    }

    if (nt.event() == event_check) std::cout << "Debug 0.6" << endl;

    ana.tx.setBranch<bool>("Common_noiseFlag"  , filterflag  ); // Flag to identify whether the event passes noise filter
    ana.tx.setBranch<bool>("Common_noiseFlagMC", filterflagMC); // Flag to identify whether the event passes noise filter

    //check good runs list
    bool goodRun = true; //set true for MC
    if(nt.isData()) goodRun = goodrun(nt.run(), nt.luminosityBlock());

    ana.tx.setBranch<bool>("Common_passGoodRun", goodRun);
     
    if (nt.event() == event_check) std::cout << "Debug 0.7" << endl;

    // Example of reading from Nano
    // std::vector<LorentzVector> electron_p4s = nt.Electron_p4(); // nt is a global variable that accesses NanoAOD
    // std::vector<float> electron_mvaTTH = nt.Electron_mvaTTH(); // electron ttH MVA scores from NanoAOD
    // Semi-complete list of NanoAOD for 102X can be found here: https://cms-nanoaod-integration.web.cern.ch/integration/master-102X/mc102X_doc.html
    // Also consult here: https://github.com/cmstas/NanoTools/blob/d641a6d6c1aa9ecc8094a1af73d5e1bd7d6502ab/NanoCORE/Nano.h#L4875 (if new variables are added they may show up in master)
    float lepSFc(1.), lepSFue(1.), lepSFde(1.), lepSFum(1.), lepSFdm(1.);
    float lepSFcTight(1.),lepSFueTight(1.),lepSFdeTight(1.),lepSFumTight(1.),lepSFdmTight(1.);
    //---------------------------------------------------------------------------------------------
    // Electron selection
    //---------------------------------------------------------------------------------------------
    // Loop over electrons and select Fall17V2Iso_WP90 electrons
    
    if (nt.event() == event_check) std::cout << "N electrons = " << nt.Electron_p4().size() << endl;    

    for (unsigned int iel = 0; iel < nt.Electron_p4().size(); ++iel)
    {
        if (nt.year() >= 2022)
        {
            // Selections
            if (not (nt.Electron_pfRelIso03_all()[iel] < 0.4)) continue;
            if (not (nt.Electron_p4()[iel].pt()       > 10.)) continue;
            if (not (abs(nt.Electron_p4()[iel].eta()) < 2.4)) continue;
            if (abs(nt.Electron_p4()[iel].eta()) < 1.566 && abs(nt.Electron_p4()[iel].eta()) > 1.444) continue; 
            if (not (abs(nt.Electron_dxy()[iel]) < 0.05      )) continue;
            if (not (abs(nt.Electron_dz()[iel]) < 0.1        )) continue;
            if (ana.input_file_list_tstring.Contains("NanoAODv11"))
            {
                if (not (nt.Electron_mvaNoIso_Fall17V2_WPL()[iel])) continue;
            }
            else
            {
                if (not (nt.Electron_mvaNoIso_WPL()[iel]         )) continue;
            }

            // If passed up to here add it to the index list
            ana.tx.pushbackToBranch<int>("Common_lep_idxs", iel);
            ana.tx.pushbackToBranch<int>("Common_lep_pdgid", nt.Electron_pdgId()[iel]);
            ana.tx.pushbackToBranch<LorentzVector>("Common_lep_p4", nt.Electron_p4()[iel]);
            //ana.tx.pushbackToBranch<int>("Common_lep_tight", nt.Electron_mvaFall17V2Iso_WP80()[iel]);
            ana.tx.pushbackToBranch<float>("Common_lep_dxy", nt.Electron_dxy()[iel]);
            ana.tx.pushbackToBranch<float>("Common_lep_dz", nt.Electron_dz()[iel]);
            ana.tx.pushbackToBranch<float>("Common_lep_ip3d", nt.Electron_ip3d()[iel]);
            ana.tx.pushbackToBranch<float>("Common_lep_sip3d", nt.Electron_sip3d()[iel]);
            ana.tx.pushbackToBranch<int>  ("Common_lep_IsoID", -1);
            ana.tx.pushbackToBranch<float>("Common_lep_relIso03_all", nt.Electron_pfRelIso03_all()[iel]); // Pt sorted selected lepton isolation reliso03_all (electrons and muons together)
            int ID = 0;
            //ID |= nt.Electron_mvaFall17V2noIso_WPL()[iel] << 0;
            //ID |= nt.Electron_mvaFall17V2noIso_WP90()[iel] << 1;
            //ID |= nt.Electron_mvaFall17V2noIso_WP80()[iel] << 2;
            //ID |= nt.Electron_mvaFall17V2Iso_WPL()[iel] << 3;
            //ID |= nt.Electron_mvaFall17V2Iso_WP90()[iel] << 4;
            //ID |= nt.Electron_mvaFall17V2Iso_WP80()[iel] << 5;
            ana.tx.pushbackToBranch<int>("Common_lep_ID", ID);
	
	    /*if (nt.run() == 1 and nt.luminosityBlock() == 3000 and nt.event() == 2999700){
		    std::cout << "//////////////////////////////////////////////////" << std::endl;
		    std::cout << "Electron idx: " << iel << std::endl;
		    std::cout << "Electron pt: " << nt.Electron_p4()[iel].pt() << std::endl;
		    std::cout << "Electron pfRelIso: " << nt.Electron_pfRelIso03_all()[iel] << std::endl;
		    std::cout << "Electron ID: " << ID << std::endl;
		    std::cout << "//////////////////////////////////////////////////" << std::endl;
	    }*/

            //---------
            // bool istight = nt.Electron_mvaFall17V2Iso_WP80()[iel];
            float pt = std::min(std::max(nt.Electron_p4()[iel].pt(), 10.01f), 499.9f);
            float eta = std::min(std::max(nt.Electron_p4()[iel].eta(), -2.499f), 2.499f);
        }

        else
        {

            // Gen matching the muons
            //bool elGenMatched = false;
            //
            //for (int igen = 0; igen < nt.GenPart_pdgId().size(); ++igen){
            //     if ( std::abs(nt.GenPart_pdgId()[igen]) != 11 ) continue;
            //     if ( !(nt.GenPart_status().at(igen) == 1) || !((nt.GenPart_statusFlags().at(igen) >> 0) & 1) ) continue;
            //     if ( ROOT::Math::VectorUtil::DeltaR(nt.GenPart_p4().at(igen),nt.Electron_p4().at(iel)) < 0.05 ){
            //          elGenMatched = true;
            //	  break;
            //     }
            //}
            //// Discard muon if it is not matched to gen Muon
            //if (!elGenMatched) continue;


	    // New lepton ID
	    if (new_lepton_ID){
	        if (!ElectronIDHelper::electronIDscore(nt.year(),iel,"tight",gconf.isAPV)) continue;
	    else{
	      //std::cout << "Electron at index " << iel << " has passed tight ID requirements!" << std::endl;
	        }
	    }

            // Selections
            if (!new_lepton_ID){
                if (not (nt.Electron_mvaFall17V2noIso_WPL()[iel])) continue;
                if (not (nt.Electron_pfRelIso03_all()[iel] < 0.4)) continue;
                if (not (nt.Electron_p4()[iel].pt()       > 10.)) continue;
                if (not (abs(nt.Electron_p4()[iel].eta()) < 2.5)) continue;
                if (abs(nt.Electron_p4()[iel].eta()) < 1.566 && abs(nt.Electron_p4()[iel].eta()) > 1.444) continue; 
	    }
            // If passed up to here add it to the index list
            ana.tx.pushbackToBranch<int>("Common_lep_idxs", iel);
            ana.tx.pushbackToBranch<int>("Common_lep_pdgid", nt.Electron_pdgId()[iel]);
            ana.tx.pushbackToBranch<LorentzVector>("Common_lep_p4", nt.Electron_p4()[iel]);
	    //ana.tx.pushbackToBranch<float>("Common_lep_eta", nt.Electron_eta()[iel]);
            ana.tx.pushbackToBranch<int>("Common_lep_tight", nt.Electron_mvaFall17V2Iso_WP80()[iel]);
            ana.tx.pushbackToBranch<float>("Common_lep_dxy", nt.Electron_dxy()[iel]);
            ana.tx.pushbackToBranch<float>("Common_lep_dz", nt.Electron_dz()[iel]);
            ana.tx.pushbackToBranch<float>("Common_lep_ip3d", nt.Electron_ip3d()[iel]);
            ana.tx.pushbackToBranch<float>("Common_lep_sip3d", nt.Electron_sip3d()[iel]);
            ana.tx.pushbackToBranch<int>  ("Common_lep_IsoID", -1);
            ana.tx.pushbackToBranch<float>("Common_lep_relIso03_all", nt.Electron_pfRelIso03_all()[iel]); // Pt sorted selected lepton isolation reliso03_all (electrons and muons together)
            int ID = 0;
            ID |= nt.Electron_mvaFall17V2noIso_WPL()[iel] << 0;
            ID |= nt.Electron_mvaFall17V2noIso_WP90()[iel] << 1;
            ID |= nt.Electron_mvaFall17V2noIso_WP80()[iel] << 2;
            ID |= nt.Electron_mvaFall17V2Iso_WPL()[iel] << 3;
            ID |= nt.Electron_mvaFall17V2Iso_WP90()[iel] << 4;
            ID |= nt.Electron_mvaFall17V2Iso_WP80()[iel] << 5;
            ana.tx.pushbackToBranch<int>("Common_lep_ID", ID);
	
	    /*if (nt.run() == 1 and nt.luminosityBlock() == 3000 and nt.event() == 2999700){
		    std::cout << "//////////////////////////////////////////////////" << std::endl;
		    std::cout << "Electron idx: " << iel << std::endl;
		    std::cout << "Electron pt: " << nt.Electron_p4()[iel].pt() << std::endl;
		    std::cout << "Electron pfRelIso: " << nt.Electron_pfRelIso03_all()[iel] << std::endl;
		    std::cout << "Electron ID: " << ID << std::endl;
		    std::cout << "//////////////////////////////////////////////////" << std::endl;
	    }*/

            //---------
            // bool istight = nt.Electron_mvaFall17V2Iso_WP80()[iel];
            float pt = std::min(std::max(nt.Electron_p4()[iel].pt(), 10.01f), 499.9f);
            float eta = std::min(std::max(nt.Electron_p4()[iel].eta(), -2.499f), 2.499f);
            float sf = (pt > 20 ? ana.electronRECOSFgt20->eval(eta, pt) : ana.electronRECOSFlt20->eval(eta, pt)) * ana.electronMVAID90SF->eval(eta, pt);
            lepSFc  *= sf;
            lepSFum *= sf;
            lepSFdm *= sf;
            ana.tx.pushbackToBranch<float>("Common_lep_SF",        sf);
            sf       = (pt > 20 ? ana.electronRECOSFgt20->eval_up(eta, pt) : ana.electronRECOSFlt20->eval_up(eta, pt)) * ana.electronMVAID90SF->eval_up(eta, pt);
            lepSFue *= sf;
            ana.tx.pushbackToBranch<float>("Common_lep_SFup",      sf);
            sf       = (pt > 20 ? ana.electronRECOSFgt20->eval_down(eta, pt) : ana.electronRECOSFlt20->eval_down(eta, pt)) * ana.electronMVAID90SF->eval_down(eta, pt);
            lepSFde *= sf;
            ana.tx.pushbackToBranch<float>("Common_lep_SFdn",      sf);
            sf       = (pt > 20 ? ana.electronRECOSFgt20->eval(eta, pt) : ana.electronRECOSFlt20->eval(eta, pt)) * ana.electronMVAID80SF->eval(eta, pt);
            lepSFcTight  *= sf;
            lepSFumTight *= sf;
            lepSFdmTight *= sf;
            ana.tx.pushbackToBranch<float>("Common_lep_SFTight",   sf);
            sf       = (pt > 20 ? ana.electronRECOSFgt20->eval_up(eta, pt) : ana.electronRECOSFlt20->eval_up(eta, pt)) * ana.electronMVAID80SF->eval_up(eta, pt);
            lepSFueTight *= sf;
            ana.tx.pushbackToBranch<float>("Common_lep_SFupTight", sf);
            sf       = (pt > 20 ? ana.electronRECOSFgt20->eval_down(eta, pt) : ana.electronRECOSFlt20->eval_down(eta, pt)) * ana.electronMVAID80SF->eval_down(eta, pt);
            lepSFdeTight *= sf;
            ana.tx.pushbackToBranch<float>("Common_lep_SFdnTight", sf);

            //---------
            // float sf = ana.leptonscalefactors.leptonSF(nt.isData(),nt.year(),11,nt.Electron_p4()[iel].eta(),nt.Electron_p4()[iel].pt(),nt.event(), 0);
            // lepSFc  *= sf;
            // lepSFum *= sf;
            // lepSFdm *= sf;
            // ana.tx.pushbackToBranch<float>("Common_lep_SF",        sf);
            // ana.tx.pushbackToBranch<float>("Common_lep_SFTight",   sf);
            // sf       = ana.leptonscalefactors.leptonSF(nt.isData(),nt.year(),11,nt.Electron_p4()[iel].eta(),nt.Electron_p4()[iel].pt(),nt.event(),+1);
            // lepSFue *= sf;
            // ana.tx.pushbackToBranch<float>("Common_lep_SFup",      sf);
            // ana.tx.pushbackToBranch<float>("Common_lep_SFupTight", sf);
            // sf       = ana.leptonscalefactors.leptonSF(nt.isData(),nt.year(),11,nt.Electron_p4()[iel].eta(),nt.Electron_p4()[iel].pt(),nt.event(),-1);
            // lepSFde *= sf;
            // ana.tx.pushbackToBranch<float>("Common_lep_SFdn",      sf);
            // ana.tx.pushbackToBranch<float>("Common_lep_SFdnTight", sf);
        }
    }
    //---------------------------------------------------------------------------------------------
    // Muon selection
    //---------------------------------------------------------------------------------------------
    // Loop over muons and select POG medium muons
    for (unsigned int imu = 0; imu < nt.Muon_p4().size(); ++imu)
    {
        if (nt.year() >= 2022)
        {
            // Selections
            if (not (nt.Muon_looseId()[imu]              )) continue; // TODO: What is Muon_mediumPromptId in NanoAOD?
            if (not (nt.Muon_p4()[imu].pt()        > 10. )) continue;
            if (not (nt.Muon_pfRelIso03_all()[imu]      < 0.4 )) continue; 
            if (not (abs(nt.Muon_p4()[imu].eta())  < 2.4 )) continue;
            if (not (abs(nt.Muon_dxy()[imu]) < 0.05      )) continue;
            if (not (abs(nt.Muon_dz()[imu]) < 0.1        )) continue;

            // If passed up to here add it to the index list
            ana.tx.pushbackToBranch<int>("Common_lep_idxs", imu);
            ana.tx.pushbackToBranch<int>("Common_lep_pdgid", nt.Muon_pdgId()[imu]);
            ana.tx.pushbackToBranch<LorentzVector>("Common_lep_p4", nt.Muon_p4()[imu]);
            ana.tx.pushbackToBranch<int>("Common_lep_tight", nt.Muon_pfRelIso04_all()[imu] < 0.15); // i.e. Tight from https://twiki.cern.ch/twiki/bin/view/CMS/SWGuideMuonSelection#Particle_Flow_isolation
            ana.tx.pushbackToBranch<float>("Common_lep_dxy", nt.Muon_dxy()[imu]);
            ana.tx.pushbackToBranch<float>("Common_lep_dz", nt.Muon_dz()[imu]);
            ana.tx.pushbackToBranch<float>("Common_lep_ip3d", nt.Muon_ip3d()[imu]);
            ana.tx.pushbackToBranch<float>("Common_lep_sip3d", nt.Muon_sip3d()[imu]);
            ana.tx.pushbackToBranch<float>("Common_lep_relIso03_all", nt.Muon_pfRelIso03_all()[imu]);
            int ID = 0;
            ID |= nt.Muon_mediumId()[imu] << 0;
            ID |= nt.Muon_tightId()[imu] << 1;
            ID |= nt.Muon_pfIsoId()[imu] << 2;
            ana.tx.pushbackToBranch<int>("Common_lep_ID", ID);

	    /*if (nt.run() == 1 and nt.luminosityBlock() == 3000 and nt.event() == 2999700){
	        std::cout << "/////////////////////////////////////////////////////" << std::endl;
	        std::cout << "Muon idx: " << imu << std::endl;
	        std::cout << "Muon pt: " << nt.Muon_p4()[imu].pt() << std::endl;
	        std::cout << "Muon pfRelIso: " << nt.Muon_pfRelIso03_all()[imu] << std::endl;
	        std::cout << "Muon ID: " << ID << std::endl;
	        std::cout << "/////////////////////////////////////////////////////" << std::endl;
	    }*/

            //---------
            // bool istight = nt.Muon_pfRelIso04_all()[imu] < 0.15;
            // float ptreco = std::min(std::max(nt.Muon_p4()[imu].pt(), 2.01f), 39.9f);	//scale factor for reco pt of muon ranged in [2,40]
            // scale factor for muon reco is measured in 40 to 60GeV, but availabe for 10 to 200GeV,
            // see https://twiki.cern.ch/twiki/bin/viewauth/CMS/MuonUL2018#RECO_efficiency
            float pt = std::min(std::max(nt.Muon_p4()[imu].pt(), 15.01f), 119.9f);
            float abseta = std::min(std::max(fabs(nt.Muon_p4()[imu].eta()), 0.01f), 2.399f);
            }
        else
        {
	    // Gen matching the muons
	    //bool muGenMatched = false;
	    //
            //for (int igen = 0; igen < nt.GenPart_pdgId().size(); ++igen){
	    //     if ( std::abs(nt.GenPart_pdgId()[igen]) != 13 ) continue;
	    //     if ( !(nt.GenPart_status().at(igen) == 1) || !((nt.GenPart_statusFlags().at(igen) >> 0) & 1) ) continue;
	    //     if ( ROOT::Math::VectorUtil::DeltaR(nt.GenPart_p4().at(igen),nt.Muon_p4().at(imu)) < 0.05 ){
	    //	  muGenMatched = true;
	    //	  break;
	    //     }
	    //}
	    //// Discard muon if it is not matched to gen Muon
            //if (!muGenMatched) continue;
	
	    if (nt.event() == event_check){
	        std::cout << "Muon index = " << imu << std::endl;
	        std::cout << "Muon pt = " << nt.Muon_pt()[imu] << std::endl;
	        std::cout << "Muon eta = " << nt.Muon_eta()[imu] << std::endl;
	        std::cout << "Muon pfRelIso04_all = " << nt.Muon_pfRelIso04_all()[imu] << std::endl;	
	    }

	    if (nt.event() == event_check) std::cout << "Debug 1" << endl;

	    //Test to make sure the MuonIDHelper functions are implemented correctly
	    if (new_lepton_ID){
	        if (nt.event() == event_check) std::cout << "Debug 1.1" << endl;
	        if (!MuonIDHelper::muonIDscore(nt.year(),imu,"tight",gconf.isAPV)){
		    //std::cout << "Muon at index " << imu << " has failed tight requirements" << std::endl;
		    continue;
	        }
	        else{
	 	    //std::cout << "Muon at index " << imu << " has passed tight requirements" << std::endl;
	        }
            }
	    if (nt.event() == event_check) std::cout << "Debug 2" << endl;
            // Selections
            if (!new_lepton_ID){
                if (not (nt.Muon_looseId()[imu]              )) continue; // TODO: What is Muon_mediumPromptId in NanoAOD?
                if (not (nt.Muon_p4()[imu].pt()        > 10. )) continue;
                if (not (nt.Muon_pfIsoId()[imu]          >= 1)) continue; // i.e. Loose from https://twiki.cern.ch/twiki/bin/view/CMS/SWGuideMuonSelection#Particle_Flow_isolation
                if (not (abs(nt.Muon_p4()[imu].eta())  < 2.4 )) continue;
	    }
            // If passed up to here add it to the index list
            ana.tx.pushbackToBranch<int>("Common_lep_idxs", imu);
            ana.tx.pushbackToBranch<int>("Common_lep_pdgid", nt.Muon_pdgId()[imu]);
            ana.tx.pushbackToBranch<LorentzVector>("Common_lep_p4", nt.Muon_p4()[imu]);
	    //ana.tx.pushbackToBranch<float>("Common_lep_eta", nt.Muon_eta()[imu]);
            ana.tx.pushbackToBranch<int>("Common_lep_tight", nt.Muon_pfRelIso04_all()[imu] < 0.15); // i.e. Tight from https://twiki.cern.ch/twiki/bin/view/CMS/SWGuideMuonSelection#Particle_Flow_isolation
            ana.tx.pushbackToBranch<float>("Common_lep_dxy", nt.Muon_dxy()[imu]);
            ana.tx.pushbackToBranch<float>("Common_lep_dz", nt.Muon_dz()[imu]);
            ana.tx.pushbackToBranch<float>("Common_lep_ip3d", nt.Muon_ip3d()[imu]);
            ana.tx.pushbackToBranch<float>("Common_lep_sip3d", nt.Muon_sip3d()[imu]);
            ana.tx.pushbackToBranch<float>("Common_lep_relIso03_all", nt.Muon_pfRelIso03_all()[imu]);
            if (nt.event() == event_check) std::cout << "Debug 3" << endl;
            int ID = 0;
            ID |= nt.Muon_mediumId()[imu] << 0;
            ID |= nt.Muon_tightId()[imu] << 1;
            ID |= nt.Muon_pfIsoId()[imu] << 2;
            ana.tx.pushbackToBranch<int>("Common_lep_ID", ID);

	    /*if (nt.run() == 1 and nt.luminosityBlock() == 3000 and nt.event() == 2999700){
	        std::cout << "/////////////////////////////////////////////////////" << std::endl;
	        std::cout << "Muon idx: " << imu << std::endl;
	        std::cout << "Muon pt: " << nt.Muon_p4()[imu].pt() << std::endl;
	        std::cout << "Muon pfRelIso: " << nt.Muon_pfRelIso03_all()[imu] << std::endl;
	        std::cout << "Muon ID: " << ID << std::endl;
	        std::cout << "/////////////////////////////////////////////////////" << std::endl;
	    }*/

            //---------
            // bool istight = nt.Muon_pfRelIso04_all()[imu] < 0.15;
            // float ptreco = std::min(std::max(nt.Muon_p4()[imu].pt(), 2.01f), 39.9f);	//scale factor for reco pt of muon ranged in [2,40]
            // scale factor for muon reco is measured in 40 to 60GeV, but availabe for 10 to 200GeV,
            // see https://twiki.cern.ch/twiki/bin/viewauth/CMS/MuonUL2018#RECO_efficiency
            float pt = std::min(std::max(nt.Muon_p4()[imu].pt(), 15.01f), 119.9f);
            float abseta = std::min(std::max(fabs(nt.Muon_p4()[imu].eta()), 0.01f), 2.399f);
            float sf = ana.muonRECOSF->eval(abseta, 50.0f) * ana.muonIDSFMedium->eval(abseta, pt) * ana.muonISOSFLoose->eval(abseta, pt);
            lepSFc  *= sf;
            lepSFue *= sf;
            lepSFde *= sf;
            ana.tx.pushbackToBranch<float>("Common_lep_SF",        sf);
            sf       = ana.muonRECOSF->eval_up(abseta, 50.0f) * ana.muonIDSFMedium->eval_up(abseta, pt) * ana.muonISOSFLoose->eval_up(abseta, pt);
            lepSFum *= sf;
            ana.tx.pushbackToBranch<float>("Common_lep_SFup",      sf);
            sf       = ana.muonRECOSF->eval_down(abseta, 50.0f) * ana.muonIDSFMedium->eval_down(abseta, pt) * ana.muonISOSFLoose->eval_down(abseta, pt);
            lepSFdm *= sf;
            ana.tx.pushbackToBranch<float>("Common_lep_SFdn",      sf);
            sf = ana.muonRECOSF->eval(abseta, 50.0f) * ana.muonIDSFMedium->eval(abseta, pt) * ana.muonISOSFTight->eval(abseta, pt);
            lepSFcTight  *= sf;
            lepSFueTight *= sf;
            lepSFdeTight *= sf;
            ana.tx.pushbackToBranch<float>("Common_lep_SFTight",   sf);
            sf       = ana.muonRECOSF->eval_up(abseta, 50.0f) * ana.muonIDSFMedium->eval_up(abseta, pt) * ana.muonISOSFTight->eval_up(abseta, pt);
            lepSFumTight *= sf;
            ana.tx.pushbackToBranch<float>("Common_lep_SFupTight", sf);
            sf       = ana.muonRECOSF->eval_down(abseta, 50.0f) * ana.muonIDSFMedium->eval_down(abseta, pt) * ana.muonISOSFTight->eval_down(abseta, pt);
            lepSFdmTight *= sf;
            ana.tx.pushbackToBranch<float>("Common_lep_SFdnTight", sf);
            //---------
            // string period = "X";
            // if (nt.year() == 2016 and nt.run() <= 278808) period = "BCDEF";
            // else if (nt.year() == 2016) period = "GH";
            // float sf = ana.leptonscalefactors.leptonSF(nt.isData(),nt.year(),13,nt.Muon_p4()[imu].eta(),nt.Muon_p4()[imu].pt(),nt.event(), 0);
            // lepSFc  *= sf;
            // lepSFue *= sf;
            // lepSFde *= sf;
            // ana.tx.pushbackToBranch<float>("Common_lep_SF",        sf);
            // ana.tx.pushbackToBranch<float>("Common_lep_SFTight",   sf);
            // sf       = ana.leptonscalefactors.leptonSF(nt.isData(),nt.year(),13,nt.Muon_p4()[imu].eta(),nt.Muon_p4()[imu].pt(),nt.event(),+1);
            // lepSFum *= sf;
            // ana.tx.pushbackToBranch<float>("Common_lep_SFup",      sf);
            // ana.tx.pushbackToBranch<float>("Common_lep_SFupTight", sf);
            // sf       = ana.leptonscalefactors.leptonSF(nt.isData(),nt.year(),13,nt.Muon_p4()[imu].eta(),nt.Muon_p4()[imu].pt(),nt.event(),-1);
            // lepSFdm *= sf;
            // ana.tx.pushbackToBranch<float>("Common_lep_SFdn",      sf);
            // ana.tx.pushbackToBranch<float>("Common_lep_SFdnTight", sf);
	    if (nt.event() == event_check) std::cout << "Debug 4" << endl;
        }
    }
    //---------------------------------------------------------------------------------------------
    // Tau selection
    // --------------------------------------------------------------------------------------------
    if (nt.year() >= 2022)
    {

    }
    else
    {
        // Loop over taus and select according to HH -> gamma gamma tau tau analysis requirements
        // TODO add tau branches to pT sorting function at the bottom of this file
        for (unsigned int itau = 0; itau < nt.Tau_p4().size(); ++itau){
	    if ( not (nt.Tau_pt()[itau] > 20.)) continue;
	    if ( not (std::abs(nt.Tau_eta()[itau]) < 2.3)) continue;
	    if ( not (std::abs(nt.Tau_dz()[itau]) < 0.2)) continue;
	    if ( not (nt.Tau_idDeepTau2017v2p1VSe()[itau] >= 0)) continue;      // VVVLoose WP (Kept intentionally loose for studying which WPs give best sensitivity)
	    if ( not (nt.Tau_idDeepTau2017v2p1VSmu()[itau] >= 0)) continue;     // VLoose WP
	    if ( not (nt.Tau_idDeepTau2017v2p1VSjet()[itau] >= 0)) continue;    // VVVLoose WP

	    // tau overlap removal: if a tau is within dR < 0.5 of a selected lepton, discard the tau
	    bool isOverlap = false;
	    for (unsigned int lep = 0; lep < ana.tx.getBranchLazy<vector<int>>("Common_lep_idxs").size(); lep++){
	        int ilep_idx = ana.tx.getBranchLazy<vector<int>>("Common_lep_idxs")[lep];
	        if ( abs(ana.tx.getBranchLazy<vector<int>>("Common_lep_pdgid")[lep]) == 11 ){     
	            if (RooUtil::Calc::DeltaR(nt.Tau_p4()[itau], nt.Electron_p4()[ilep_idx]) < 0.5) isOverlap = true;	   
	        }
	        if ( abs(ana.tx.getBranchLazy<vector<int>>("Common_lep_pdgid")[lep]) == 13 ){
		    if (RooUtil::Calc::DeltaR(nt.Tau_p4()[itau], nt.Muon_p4()[ilep_idx]) < 0.5) isOverlap = true;	  
	        }   
	    }

	    // Discard any taus that overlap with the selected leptons
	    if ( isOverlap ) continue;	 

	    // Save branches
	    ana.tx.pushbackToBranch<LorentzVector>("Common_tau_p4"       ,	 nt.Tau_p4()[itau]);
	    ana.tx.pushbackToBranch<int>(		"Common_tau_idxs"     ,	 itau);	  	 
	    ana.tx.pushbackToBranch<int>(		"Common_tau_decayMode",	 nt.Tau_decayMode()[itau]);          
	    ana.tx.pushbackToBranch<float>(	"Common_tau_dxy"      ,	 nt.Tau_dxy()[itau]);
	    ana.tx.pushbackToBranch<float>(	"Common_tau_dz"       ,	 nt.Tau_dz()[itau]);
	    ana.tx.pushbackToBranch<float>(	"Common_tau_eta"      ,  nt.Tau_eta()[itau]);
	    ana.tx.pushbackToBranch<float>(	"Common_tau_chargedIso", nt.Tau_chargedIso()[itau]);
	    ana.tx.pushbackToBranch<int>(		"Common_tau_jetIdx"   ,  nt.Tau_jetIdx()[itau]);
	    ana.tx.pushbackToBranch<float>(	"Common_tau_neutralIso", nt.Tau_neutralIso()[itau]);
	    ana.tx.pushbackToBranch<int>(		"Common_tau_charge"   ,  nt.Tau_charge()[itau]);
	    ana.tx.pushbackToBranch<int>(		"Common_tau_idVSe"    ,  nt.Tau_idDeepTau2017v2p1VSe()[itau]);
	    ana.tx.pushbackToBranch<int>(		"Common_tau_idVSmu"   ,  nt.Tau_idDeepTau2017v2p1VSmu()[itau]);
 	    ana.tx.pushbackToBranch<int>(		"Common_tau_idVSjet"  ,  nt.Tau_idDeepTau2017v2p1VSjet()[itau]);

        }
    
        for (unsigned int iGenTau = 0; iGenTau < nt.nGenVisTau(); iGenTau++){
	    ana.tx.pushbackToBranch<LorentzVector>("Common_GenVisTau_p4" , nt.GenVisTau_p4()[iGenTau]);
	    ana.tx.pushbackToBranch<int>(          "Common_GenVisTau_idxs",iGenTau);
            ana.tx.pushbackToBranch<int>(		"Common_GenVisTau_status", nt.GenVisTau_status()[iGenTau]);
        }

        ana.tx.setBranch<float>("Common_event_lepSF"      , lepSFc );
        ana.tx.setBranch<float>("Common_event_lepSFelup"  , lepSFue);
        ana.tx.setBranch<float>("Common_event_lepSFeldn"  , lepSFde);
        ana.tx.setBranch<float>("Common_event_lepSFmuup"  , lepSFum);
        ana.tx.setBranch<float>("Common_event_lepSFmudn"  , lepSFdm);

        if (nt.event() == event_check) std::cout << "Debug 5" << endl;

        ana.tx.setBranch<float>("Common_event_lepSFTight"      , lepSFcTight );
        ana.tx.setBranch<float>("Common_event_lepSFelupTight"  , lepSFueTight);
        ana.tx.setBranch<float>("Common_event_lepSFeldnTight"  , lepSFdeTight);
        ana.tx.setBranch<float>("Common_event_lepSFmuupTight"  , lepSFumTight);
        ana.tx.setBranch<float>("Common_event_lepSFmudnTight"  , lepSFdmTight);
    }
    //---------------------------------------------------------------------------------------------

    // b-tagging counter (DeepFlavB)
    float bWPloose  = gconf.WP_DeepFlav_loose;
    float bWPmedium = gconf.WP_DeepFlav_medium;
    float bWPtight  = gconf.WP_DeepFlav_tight;
    float bWPloose_CSV  = gconf.WP_DeepCSV_loose;
    float bWPmedium_CSV = gconf.WP_DeepCSV_medium;
    float bWPtight_CSV  = gconf.WP_DeepCSV_tight;
    int nb_loose = 0;
    int nb_medium = 0;
    int nb_tight = 0;
    int nb_loose_CSV = 0;
    int nb_medium_CSV = 0;
    int nb_tight_CSV = 0;
    float btagTight_prob_MC = 1;
    float btagTight_prob_DATA = 1;
    float btagTight_up_prob_DATA = 1;
    float btagTight_dn_prob_DATA = 1;
    float btagTight_HFup_prob_DATA = 1;
    float btagTight_HFdn_prob_DATA = 1;
    float btagTight_LFup_prob_DATA = 1;
    float btagTight_LFdn_prob_DATA = 1;
    float btagMedium_prob_MC = 1;
    float btagMedium_prob_DATA = 1;
    float btagMedium_up_prob_DATA = 1;
    float btagMedium_dn_prob_DATA = 1;
    float btagMedium_HFup_prob_DATA = 1;
    float btagMedium_HFdn_prob_DATA = 1;
    float btagMedium_LFup_prob_DATA = 1;
    float btagMedium_LFdn_prob_DATA = 1;
    float btagLoose_prob_MC = 1;
    float btagLoose_prob_DATA = 1;
    float btagLoose_up_prob_DATA = 1;
    float btagLoose_dn_prob_DATA = 1;
    float btagLoose_HFup_prob_DATA = 1;
    float btagLoose_HFdn_prob_DATA = 1;
    float btagLoose_LFup_prob_DATA = 1;
    float btagLoose_LFdn_prob_DATA = 1;

    // Loop over jets and do a simple overlap removal against leptons
    for (unsigned int ijet = 0; ijet < nt.Jet_p4().size(); ++ijet)
    {
        if (nt.year() >= 2022)
        {

        }

        else
        {	    
            LorentzVector jet_p4;
            if (ana.is_postprocessed)
            {
                //if postprocessed NanoAOD, set pt to pt_nom, mass to mass_nom
                jet_p4=LorentzVector(nt.Jet_pt_nom()[ijet],nt.Jet_eta()[ijet],nt.Jet_phi()[ijet],nt.Jet_mass_nom()[ijet]);
            }
	    else 
            {
                jet_p4=nt.Jet_p4()[ijet];
            }
            // TODO: What is POG recommendation? do we use nt.Jet_jetId()? nt.Jet_puId()??
            // Figure this out
            // For now if less than 20 GeV, don't ever consider
            if (not (jet_p4.pt() > 20.))
                continue;
            // Because every muon and electron shows up in PF Jet collections
            // Need to check against leptons
            bool is_overlapping_with_a_lepton = false;

            // Overlap check against leptons (electrons)
            for (unsigned int ilep = 0; ilep < ana.tx.getBranchLazy<vector<int>>("Common_lep_idxs").size(); ++ilep)
            {
                int ilep_idx = ana.tx.getBranchLazy<vector<int>>("Common_lep_idxs")[ilep];
                // If electron
                if (abs(ana.tx.getBranchLazy<vector<int>>("Common_lep_pdgid")[ilep]) == 11)
                {
                    if (RooUtil::Calc::DeltaR(jet_p4, nt.Electron_p4()[ilep_idx]) < 0.4)
                    {
                        is_overlapping_with_a_lepton = true;
                        break;
                    }
                }
                // else muon
                else
                {
                    if (RooUtil::Calc::DeltaR(jet_p4, nt.Muon_p4()[ilep_idx]) < 0.4)
                    {
                        is_overlapping_with_a_lepton = true;
                        break;
                    }
                }
            }

            if (is_overlapping_with_a_lepton)
                continue;

            // For the analysis level jets, consider jets only 30 and above
            if (jet_p4.pt() > 20. and abs(jet_p4.eta()) < 4.7)//don't trust jets in HF
            {
                // For now, accept anything that reaches this point
                ana.tx.pushbackToBranch<int>("Common_jet_idxs", ijet);
                ana.tx.pushbackToBranch<int>("Common_jet_id", nt.Jet_jetId()[ijet]);
                ana.tx.pushbackToBranch<int>("Common_jet_puid", nt.Jet_puId()[ijet]);
                ana.tx.pushbackToBranch<LorentzVector>("Common_jet_p4", jet_p4);
                ana.tx.pushbackToBranch<bool>("Common_jet_passBloose" , nt.Jet_btagDeepFlavB()[ijet] > bWPloose );
                ana.tx.pushbackToBranch<bool>("Common_jet_passBmedium", nt.Jet_btagDeepFlavB()[ijet] > bWPmedium);
                ana.tx.pushbackToBranch<bool>("Common_jet_passBtight" , nt.Jet_btagDeepFlavB()[ijet] > bWPtight );
                ana.tx.pushbackToBranch<bool>("Common_jet_passBloose_CSV" , nt.Jet_btagDeepFlavB()[ijet] > bWPloose_CSV );
                ana.tx.pushbackToBranch<bool>("Common_jet_passBmedium_CSV", nt.Jet_btagDeepFlavB()[ijet] > bWPmedium_CSV);
                ana.tx.pushbackToBranch<bool>("Common_jet_passBtight_CSV" , nt.Jet_btagDeepFlavB()[ijet] > bWPtight_CSV );
	        if (!nt.isData()){
	    	    ana.tx.pushbackToBranch<int>("Common_jet_genJetIdx", nt.Jet_genJetIdx()[ijet]);
	    	    ana.tx.pushbackToBranch<int>("Common_jet_hadronFlavour", nt.Jet_hadronFlavour()[ijet]);
	    	    ana.tx.pushbackToBranch<int>("Common_jet_partonFlavour", nt.Jet_partonFlavour()[ijet]);
	        }
                if (ana.is_postprocessed && !nt.isData())
                {
                    ana.tx.pushbackToBranch<float>("Common_jet_pt_jesup", nt.Jet_pt_jesTotalUp()[ijet]);
                    ana.tx.pushbackToBranch<float>("Common_jet_pt_jesdn", nt.Jet_pt_jesTotalDown()[ijet]);
                    ana.tx.pushbackToBranch<float>("Common_jet_pt_jerup", nt.Jet_pt_jerUp()[ijet]);
                    ana.tx.pushbackToBranch<float>("Common_jet_pt_jerdn", nt.Jet_pt_jerDown()[ijet]);
                    ana.tx.pushbackToBranch<float>("Common_jet_mass_jesup", nt.Jet_mass_jesTotalUp()[ijet]);
                    ana.tx.pushbackToBranch<float>("Common_jet_mass_jesdn", nt.Jet_mass_jesTotalDown()[ijet]);
                    ana.tx.pushbackToBranch<float>("Common_jet_mass_jerup", nt.Jet_mass_jerUp()[ijet]);
                    ana.tx.pushbackToBranch<float>("Common_jet_mass_jerdn", nt.Jet_mass_jerDown()[ijet]);
                }
            }

            // b-tagged jet counter
            // For b-tagged jets, consider jets only 20 and above and is central within tracker acceptance

            if (jet_p4.pt() > 20. and abs(jet_p4.eta()) < 2.4)
            {

                if (nt.Jet_btagDeepFlavB()[ijet] > bWPloose)
                {
                    nb_loose++;
                }
                if (nt.Jet_btagDeepFlavB()[ijet] > bWPmedium)
                {
                    nb_medium++;
                }
                if (nt.Jet_btagDeepFlavB()[ijet] > bWPtight)
                {
                    nb_tight++;
                }

                if (nt.Jet_btagDeepB()[ijet] > bWPloose_CSV)
                {
                    nb_loose_CSV++;
                }
                if (nt.Jet_btagDeepB()[ijet] > bWPmedium_CSV)
                {
                    nb_medium_CSV++;
                }
                if (nt.Jet_btagDeepB()[ijet] > bWPtight_CSV)
                {
                    nb_tight_CSV++;
                }

                if (not nt.isData())
                {
                    float pt = min(jet_p4.pt(), 599.99f);
                    float eta = min(abs(jet_p4.eta()), 2.399f);
                    float score = nt.Jet_btagDeepFlavB()[ijet];
                    int flavor = nt.Jet_hadronFlavour()[ijet];
                    bool is_loose_btagged = score > gconf.WP_DeepFlav_loose;
                    bool is_medium_btagged = score > gconf.WP_DeepFlav_medium;
                    bool is_tight_btagged = score > gconf.WP_DeepFlav_tight;

                    float eff_tight = 1;
                    float eff_medium = 1;
                    float eff_loose = 1;

                    eff_tight = flavor == 5 ? ana.btagEffTight_b->eval(pt, eta) : (flavor == 0 ? ana.btagEffTight_l->eval(pt, eta) : ana.btagEffTight_c->eval(pt, eta));
                    eff_medium = flavor == 5 ? ana.btagEffMedium_b->eval(pt, eta) : (flavor == 0 ? ana.btagEffMedium_l->eval(pt, eta) : ana.btagEffMedium_c->eval(pt, eta));
                    eff_loose = flavor == 5 ? ana.btagEffLoose_b->eval(pt, eta) : (flavor == 0 ? ana.btagEffLoose_l->eval(pt, eta) : ana.btagEffLoose_c->eval(pt, eta));

                    float sf_tight;
                    float sf_medium;
                    float sf_loose;
                    float sf_up_tight;
                    float sf_up_medium;
                    float sf_up_loose;
                    float sf_dn_tight;
                    float sf_dn_medium;
                    float sf_dn_loose;
                    float sf_HFup_tight;
                    float sf_HFup_medium;
                    float sf_HFup_loose;
                    float sf_HFdn_tight;
                    float sf_HFdn_medium;
                    float sf_HFdn_loose;
                    float sf_LFup_tight;
                    float sf_LFup_medium;
                    float sf_LFup_loose;
                    float sf_LFdn_tight;
                    float sf_LFdn_medium;
                    float sf_LFdn_loose;

                    if (nt.year() == 2016)
                    {
                        sf_tight =
                            flavor == 5 ? ana.btagReaderTight_v2->eval_auto_bounds("central", BTagEntry_v2::FLAV_B, eta, pt) : (
                                flavor == 0 ? ana.btagReaderTight_v2->eval_auto_bounds("central", BTagEntry_v2::FLAV_UDSG, eta, pt) :
                                ana.btagReaderTight_v2->eval_auto_bounds("central", BTagEntry_v2::FLAV_C, eta, pt));
                        sf_medium =
                            flavor == 5 ? ana.btagReaderMedium_v2->eval_auto_bounds("central", BTagEntry_v2::FLAV_B, eta, pt) : (
                                flavor == 0 ? ana.btagReaderMedium_v2->eval_auto_bounds("central", BTagEntry_v2::FLAV_UDSG, eta, pt) :
                                ana.btagReaderMedium_v2->eval_auto_bounds("central", BTagEntry_v2::FLAV_C, eta, pt));
                        sf_loose =
                            flavor == 5 ? ana.btagReaderLoose_v2->eval_auto_bounds("central", BTagEntry_v2::FLAV_B, eta, pt) : (
                                flavor == 0 ? ana.btagReaderLoose_v2->eval_auto_bounds("central", BTagEntry_v2::FLAV_UDSG, eta, pt) :
                                ana.btagReaderLoose_v2->eval_auto_bounds("central", BTagEntry_v2::FLAV_C, eta, pt));
                        sf_up_tight =
                            flavor == 5 ? ana.btagReaderTight_v2->eval_auto_bounds("up", BTagEntry_v2::FLAV_B, eta, pt) : (
                                flavor == 0 ? ana.btagReaderTight_v2->eval_auto_bounds("up", BTagEntry_v2::FLAV_UDSG, eta, pt) :
                                ana.btagReaderTight_v2->eval_auto_bounds("up", BTagEntry_v2::FLAV_C, eta, pt));
                        sf_up_medium =
                            flavor == 5 ? ana.btagReaderMedium_v2->eval_auto_bounds("up", BTagEntry_v2::FLAV_B, eta, pt) : (
                                flavor == 0 ? ana.btagReaderMedium_v2->eval_auto_bounds("up", BTagEntry_v2::FLAV_UDSG, eta, pt) :
                                ana.btagReaderMedium_v2->eval_auto_bounds("up", BTagEntry_v2::FLAV_C, eta, pt));
                        sf_up_loose =
                            flavor == 5 ? ana.btagReaderLoose_v2->eval_auto_bounds("up", BTagEntry_v2::FLAV_B, eta, pt) : (
                                flavor == 0 ? ana.btagReaderLoose_v2->eval_auto_bounds("up", BTagEntry_v2::FLAV_UDSG, eta, pt) :
                                ana.btagReaderLoose_v2->eval_auto_bounds("up", BTagEntry_v2::FLAV_C, eta, pt));
                        sf_HFup_tight =
                            flavor == 5 ? ana.btagReaderTight_v2->eval_auto_bounds("up", BTagEntry_v2::FLAV_B, eta, pt) : (
                                flavor == 0 ? ana.btagReaderTight_v2->eval_auto_bounds("central", BTagEntry_v2::FLAV_UDSG, eta, pt) :
                                ana.btagReaderTight_v2->eval_auto_bounds("up", BTagEntry_v2::FLAV_C, eta, pt));
                        sf_HFup_medium =
                            flavor == 5 ? ana.btagReaderMedium_v2->eval_auto_bounds("up", BTagEntry_v2::FLAV_B, eta, pt) : (
                                flavor == 0 ? ana.btagReaderMedium_v2->eval_auto_bounds("central", BTagEntry_v2::FLAV_UDSG, eta, pt) :
                                ana.btagReaderMedium_v2->eval_auto_bounds("up", BTagEntry_v2::FLAV_C, eta, pt));
                        sf_HFup_loose =
                            flavor == 5 ? ana.btagReaderLoose_v2->eval_auto_bounds("up", BTagEntry_v2::FLAV_B, eta, pt) : (
                                flavor == 0 ? ana.btagReaderLoose_v2->eval_auto_bounds("central", BTagEntry_v2::FLAV_UDSG, eta, pt) :
                                ana.btagReaderLoose_v2->eval_auto_bounds("up", BTagEntry_v2::FLAV_C, eta, pt));
                        sf_LFup_tight =
                            flavor == 5 ? ana.btagReaderTight_v2->eval_auto_bounds("central", BTagEntry_v2::FLAV_B, eta, pt) : (
                                flavor == 0 ? ana.btagReaderTight_v2->eval_auto_bounds("up", BTagEntry_v2::FLAV_UDSG, eta, pt) :
                                ana.btagReaderTight_v2->eval_auto_bounds("central", BTagEntry_v2::FLAV_C, eta, pt));
                        sf_LFup_medium =
                            flavor == 5 ? ana.btagReaderMedium_v2->eval_auto_bounds("central", BTagEntry_v2::FLAV_B, eta, pt) : (
                                flavor == 0 ? ana.btagReaderMedium_v2->eval_auto_bounds("up", BTagEntry_v2::FLAV_UDSG, eta, pt) :
                                ana.btagReaderMedium_v2->eval_auto_bounds("central", BTagEntry_v2::FLAV_C, eta, pt));
                        sf_LFup_loose =
                            flavor == 5 ? ana.btagReaderLoose_v2->eval_auto_bounds("central", BTagEntry_v2::FLAV_B, eta, pt) : (
                                flavor == 0 ? ana.btagReaderLoose_v2->eval_auto_bounds("up", BTagEntry_v2::FLAV_UDSG, eta, pt) :
                                ana.btagReaderLoose_v2->eval_auto_bounds("central", BTagEntry_v2::FLAV_C, eta, pt));
                        sf_dn_tight =
                            flavor == 5 ? ana.btagReaderTight_v2->eval_auto_bounds("down", BTagEntry_v2::FLAV_B, eta, pt) : (
                                flavor == 0 ? ana.btagReaderTight_v2->eval_auto_bounds("down", BTagEntry_v2::FLAV_UDSG, eta, pt) :
                                ana.btagReaderTight_v2->eval_auto_bounds("down", BTagEntry_v2::FLAV_C, eta, pt));
                        sf_dn_medium =
                            flavor == 5 ? ana.btagReaderMedium_v2->eval_auto_bounds("down", BTagEntry_v2::FLAV_B, eta, pt) : (
                                flavor == 0 ? ana.btagReaderMedium_v2->eval_auto_bounds("down", BTagEntry_v2::FLAV_UDSG, eta, pt) :
                                ana.btagReaderMedium_v2->eval_auto_bounds("down", BTagEntry_v2::FLAV_C, eta, pt));
                        sf_dn_loose =
                            flavor == 5 ? ana.btagReaderLoose_v2->eval_auto_bounds("down", BTagEntry_v2::FLAV_B, eta, pt) : (
                                flavor == 0 ? ana.btagReaderLoose_v2->eval_auto_bounds("down", BTagEntry_v2::FLAV_UDSG, eta, pt) :
                                ana.btagReaderLoose_v2->eval_auto_bounds("down", BTagEntry_v2::FLAV_C, eta, pt));
                        sf_HFdn_tight =
                            flavor == 5 ? ana.btagReaderTight_v2->eval_auto_bounds("down", BTagEntry_v2::FLAV_B, eta, pt) : (
                                flavor == 0 ? ana.btagReaderTight_v2->eval_auto_bounds("central", BTagEntry_v2::FLAV_UDSG, eta, pt) :
                                ana.btagReaderTight_v2->eval_auto_bounds("down", BTagEntry_v2::FLAV_C, eta, pt));
                        sf_HFdn_medium =
                            flavor == 5 ? ana.btagReaderMedium_v2->eval_auto_bounds("down", BTagEntry_v2::FLAV_B, eta, pt) : (
                                flavor == 0 ? ana.btagReaderMedium_v2->eval_auto_bounds("central", BTagEntry_v2::FLAV_UDSG, eta, pt) :
                                ana.btagReaderMedium_v2->eval_auto_bounds("down", BTagEntry_v2::FLAV_C, eta, pt));
                        sf_HFdn_loose =
                            flavor == 5 ? ana.btagReaderLoose_v2->eval_auto_bounds("down", BTagEntry_v2::FLAV_B, eta, pt) : (
                                flavor == 0 ? ana.btagReaderLoose_v2->eval_auto_bounds("central", BTagEntry_v2::FLAV_UDSG, eta, pt) :
                                ana.btagReaderLoose_v2->eval_auto_bounds("down", BTagEntry_v2::FLAV_C, eta, pt));
                        sf_LFdn_tight =
                            flavor == 5 ? ana.btagReaderTight_v2->eval_auto_bounds("central", BTagEntry_v2::FLAV_B, eta, pt) : (
                                flavor == 0 ? ana.btagReaderTight_v2->eval_auto_bounds("down", BTagEntry_v2::FLAV_UDSG, eta, pt) :
                                ana.btagReaderTight_v2->eval_auto_bounds("central", BTagEntry_v2::FLAV_C, eta, pt));
                        sf_LFdn_medium =
                            flavor == 5 ? ana.btagReaderMedium_v2->eval_auto_bounds("central", BTagEntry_v2::FLAV_B, eta, pt) : (
                                flavor == 0 ? ana.btagReaderMedium_v2->eval_auto_bounds("down", BTagEntry_v2::FLAV_UDSG, eta, pt) :
                                ana.btagReaderMedium_v2->eval_auto_bounds("central", BTagEntry_v2::FLAV_C, eta, pt));
                        sf_LFdn_loose =
                            flavor == 5 ? ana.btagReaderLoose_v2->eval_auto_bounds("central", BTagEntry_v2::FLAV_B, eta, pt) : (
                                flavor == 0 ? ana.btagReaderLoose_v2->eval_auto_bounds("down", BTagEntry_v2::FLAV_UDSG, eta, pt) :
                                ana.btagReaderLoose_v2->eval_auto_bounds("central", BTagEntry_v2::FLAV_C, eta, pt));
                    }
                    else
                    {
                        sf_tight =
                            flavor == 5 ? ana.btagReaderTight->eval_auto_bounds("central", BTagEntry::FLAV_B, eta, pt) : (
                                flavor == 0 ? ana.btagReaderTight->eval_auto_bounds("central", BTagEntry::FLAV_UDSG, eta, pt) :
                                ana.btagReaderTight->eval_auto_bounds("central", BTagEntry::FLAV_C, eta, pt));
                        sf_medium =
                            flavor == 5 ? ana.btagReaderMedium->eval_auto_bounds("central", BTagEntry::FLAV_B, eta, pt) : (
                                flavor == 0 ? ana.btagReaderMedium->eval_auto_bounds("central", BTagEntry::FLAV_UDSG, eta, pt) :
                                ana.btagReaderMedium->eval_auto_bounds("central", BTagEntry::FLAV_C, eta, pt));
                        sf_loose =
                            flavor == 5 ? ana.btagReaderLoose->eval_auto_bounds("central", BTagEntry::FLAV_B, eta, pt) : (
                                flavor == 0 ? ana.btagReaderLoose->eval_auto_bounds("central", BTagEntry::FLAV_UDSG, eta, pt) :
                                ana.btagReaderLoose->eval_auto_bounds("central", BTagEntry::FLAV_C, eta, pt));
                        sf_up_tight =
                            flavor == 5 ? ana.btagReaderTight->eval_auto_bounds("up", BTagEntry::FLAV_B, eta, pt) : (
                                flavor == 0 ? ana.btagReaderTight->eval_auto_bounds("up", BTagEntry::FLAV_UDSG, eta, pt) :
                                ana.btagReaderTight->eval_auto_bounds("up", BTagEntry::FLAV_C, eta, pt));
                        sf_up_medium =
                            flavor == 5 ? ana.btagReaderMedium->eval_auto_bounds("up", BTagEntry::FLAV_B, eta, pt) : (
                                flavor == 0 ? ana.btagReaderMedium->eval_auto_bounds("up", BTagEntry::FLAV_UDSG, eta, pt) :
                                ana.btagReaderMedium->eval_auto_bounds("up", BTagEntry::FLAV_C, eta, pt));
                        sf_up_loose =
                            flavor == 5 ? ana.btagReaderLoose->eval_auto_bounds("up", BTagEntry::FLAV_B, eta, pt) : (
                                flavor == 0 ? ana.btagReaderLoose->eval_auto_bounds("up", BTagEntry::FLAV_UDSG, eta, pt) :
                                ana.btagReaderLoose->eval_auto_bounds("up", BTagEntry::FLAV_C, eta, pt));
                        sf_HFup_tight =
                            flavor == 5 ? ana.btagReaderTight->eval_auto_bounds("up", BTagEntry::FLAV_B, eta, pt) : (
                                flavor == 0 ? ana.btagReaderTight->eval_auto_bounds("central", BTagEntry::FLAV_UDSG, eta, pt) :
                                ana.btagReaderTight->eval_auto_bounds("up", BTagEntry::FLAV_C, eta, pt));
                        sf_HFup_medium =
                            flavor == 5 ? ana.btagReaderMedium->eval_auto_bounds("up", BTagEntry::FLAV_B, eta, pt) : (
                                flavor == 0 ? ana.btagReaderMedium->eval_auto_bounds("central", BTagEntry::FLAV_UDSG, eta, pt) :
                                ana.btagReaderMedium->eval_auto_bounds("up", BTagEntry::FLAV_C, eta, pt));
                        sf_HFup_loose =
                            flavor == 5 ? ana.btagReaderLoose->eval_auto_bounds("up", BTagEntry::FLAV_B, eta, pt) : (
                                flavor == 0 ? ana.btagReaderLoose->eval_auto_bounds("central", BTagEntry::FLAV_UDSG, eta, pt) :
                                ana.btagReaderLoose->eval_auto_bounds("up", BTagEntry::FLAV_C, eta, pt));
                        sf_LFup_tight =
                            flavor == 5 ? ana.btagReaderTight->eval_auto_bounds("central", BTagEntry::FLAV_B, eta, pt) : (
                                flavor == 0 ? ana.btagReaderTight->eval_auto_bounds("up", BTagEntry::FLAV_UDSG, eta, pt) :
                                ana.btagReaderTight->eval_auto_bounds("central", BTagEntry::FLAV_C, eta, pt));
                        sf_LFup_medium =
                            flavor == 5 ? ana.btagReaderMedium->eval_auto_bounds("central", BTagEntry::FLAV_B, eta, pt) : (
                                flavor == 0 ? ana.btagReaderMedium->eval_auto_bounds("up", BTagEntry::FLAV_UDSG, eta, pt) :
                                ana.btagReaderMedium->eval_auto_bounds("central", BTagEntry::FLAV_C, eta, pt));
                        sf_LFup_loose =
                            flavor == 5 ? ana.btagReaderLoose->eval_auto_bounds("central", BTagEntry::FLAV_B, eta, pt) : (
                                flavor == 0 ? ana.btagReaderLoose->eval_auto_bounds("up", BTagEntry::FLAV_UDSG, eta, pt) :
                                ana.btagReaderLoose->eval_auto_bounds("central", BTagEntry::FLAV_C, eta, pt));
                        sf_dn_tight =
                            flavor == 5 ? ana.btagReaderTight->eval_auto_bounds("down", BTagEntry::FLAV_B, eta, pt) : (
                                flavor == 0 ? ana.btagReaderTight->eval_auto_bounds("down", BTagEntry::FLAV_UDSG, eta, pt) :
                                ana.btagReaderTight->eval_auto_bounds("down", BTagEntry::FLAV_C, eta, pt));
                        sf_dn_medium =
                            flavor == 5 ? ana.btagReaderMedium->eval_auto_bounds("down", BTagEntry::FLAV_B, eta, pt) : (
                                flavor == 0 ? ana.btagReaderMedium->eval_auto_bounds("down", BTagEntry::FLAV_UDSG, eta, pt) :
                                ana.btagReaderMedium->eval_auto_bounds("down", BTagEntry::FLAV_C, eta, pt));
                        sf_dn_loose =
                            flavor == 5 ? ana.btagReaderLoose->eval_auto_bounds("down", BTagEntry::FLAV_B, eta, pt) : (
                                flavor == 0 ? ana.btagReaderLoose->eval_auto_bounds("down", BTagEntry::FLAV_UDSG, eta, pt) :
                                ana.btagReaderLoose->eval_auto_bounds("down", BTagEntry::FLAV_C, eta, pt));
                        sf_HFdn_tight =
                            flavor == 5 ? ana.btagReaderTight->eval_auto_bounds("down", BTagEntry::FLAV_B, eta, pt) : (
                                flavor == 0 ? ana.btagReaderTight->eval_auto_bounds("central", BTagEntry::FLAV_UDSG, eta, pt) :
                                ana.btagReaderTight->eval_auto_bounds("down", BTagEntry::FLAV_C, eta, pt));
                        sf_HFdn_medium =
                            flavor == 5 ? ana.btagReaderMedium->eval_auto_bounds("down", BTagEntry::FLAV_B, eta, pt) : (
                                flavor == 0 ? ana.btagReaderMedium->eval_auto_bounds("central", BTagEntry::FLAV_UDSG, eta, pt) :
                                ana.btagReaderMedium->eval_auto_bounds("down", BTagEntry::FLAV_C, eta, pt));
                        sf_HFdn_loose =
                            flavor == 5 ? ana.btagReaderLoose->eval_auto_bounds("down", BTagEntry::FLAV_B, eta, pt) : (
                                flavor == 0 ? ana.btagReaderLoose->eval_auto_bounds("central", BTagEntry::FLAV_UDSG, eta, pt) :
                                ana.btagReaderLoose->eval_auto_bounds("down", BTagEntry::FLAV_C, eta, pt));
                        sf_LFdn_tight =
                            flavor == 5 ? ana.btagReaderTight->eval_auto_bounds("central", BTagEntry::FLAV_B, eta, pt) : (
                                flavor == 0 ? ana.btagReaderTight->eval_auto_bounds("down", BTagEntry::FLAV_UDSG, eta, pt) :
                                ana.btagReaderTight->eval_auto_bounds("central", BTagEntry::FLAV_C, eta, pt));
                        sf_LFdn_medium =
                            flavor == 5 ? ana.btagReaderMedium->eval_auto_bounds("central", BTagEntry::FLAV_B, eta, pt) : (
                                flavor == 0 ? ana.btagReaderMedium->eval_auto_bounds("down", BTagEntry::FLAV_UDSG, eta, pt) :
                                ana.btagReaderMedium->eval_auto_bounds("central", BTagEntry::FLAV_C, eta, pt));
                        sf_LFdn_loose =
                            flavor == 5 ? ana.btagReaderLoose->eval_auto_bounds("central", BTagEntry::FLAV_B, eta, pt) : (
                                flavor == 0 ? ana.btagReaderLoose->eval_auto_bounds("down", BTagEntry::FLAV_UDSG, eta, pt) :
                                ana.btagReaderLoose->eval_auto_bounds("central", BTagEntry::FLAV_C, eta, pt));
                    }

                    if (is_tight_btagged)
                    {
                        btagTight_prob_MC *= eff_tight;
                        btagTight_prob_DATA *= sf_tight * eff_tight;
                        btagTight_up_prob_DATA *= sf_up_tight * eff_tight;
                        btagTight_dn_prob_DATA *= sf_dn_tight * eff_tight;
                        btagTight_HFup_prob_DATA *= sf_HFup_tight * eff_tight;
                        btagTight_HFdn_prob_DATA *= sf_HFdn_tight * eff_tight;
                        btagTight_LFup_prob_DATA *= sf_LFup_tight * eff_tight;
                        btagTight_LFdn_prob_DATA *= sf_LFdn_tight * eff_tight;
                    }
                    else
                    {
                        btagTight_prob_MC *= (1 - eff_tight);
                        btagTight_prob_DATA *= (1 - sf_tight * eff_tight);
                        btagTight_up_prob_DATA *= (1 - sf_up_tight * eff_tight);
                        btagTight_dn_prob_DATA *= (1 - sf_dn_tight * eff_tight);
                        btagTight_HFup_prob_DATA *= (1 - sf_HFup_tight * eff_tight);
                        btagTight_HFdn_prob_DATA *= (1 - sf_HFdn_tight * eff_tight);
                        btagTight_LFup_prob_DATA *= (1 - sf_LFup_tight * eff_tight);
                        btagTight_LFdn_prob_DATA *= (1 - sf_LFdn_tight * eff_tight);
                    }

                    if (is_medium_btagged)
                    {
                        btagMedium_prob_MC *= eff_medium;
                        btagMedium_prob_DATA *= sf_medium * eff_medium;
                        btagMedium_up_prob_DATA *= sf_up_medium * eff_medium;
                        btagMedium_dn_prob_DATA *= sf_dn_medium * eff_medium;
                        btagMedium_HFup_prob_DATA *= sf_HFup_medium * eff_medium;
                        btagMedium_HFdn_prob_DATA *= sf_HFdn_medium * eff_medium;
                        btagMedium_LFup_prob_DATA *= sf_LFup_medium * eff_medium;
                        btagMedium_LFdn_prob_DATA *= sf_LFdn_medium * eff_medium;
                    }
                    else
                    {
                        btagMedium_prob_MC *= (1 - eff_medium);
                        btagMedium_prob_DATA *= (1 - sf_medium * eff_medium);
                        btagMedium_up_prob_DATA *= (1 - sf_up_medium * eff_medium);
                        btagMedium_dn_prob_DATA *= (1 - sf_dn_medium * eff_medium);
                        btagMedium_HFup_prob_DATA *= (1 - sf_HFup_medium * eff_medium);
                        btagMedium_HFdn_prob_DATA *= (1 - sf_HFdn_medium * eff_medium);
                        btagMedium_LFup_prob_DATA *= (1 - sf_LFup_medium * eff_medium);
                        btagMedium_LFdn_prob_DATA *= (1 - sf_LFdn_medium * eff_medium);
                    }

                    if (is_loose_btagged)
                    {
                        btagLoose_prob_MC *= eff_loose;
                        btagLoose_prob_DATA *= sf_loose * eff_loose;
                        btagLoose_up_prob_DATA *= sf_up_loose * eff_loose;
                        btagLoose_dn_prob_DATA *= sf_dn_loose * eff_loose;
                        btagLoose_HFup_prob_DATA *= sf_HFup_loose * eff_loose;
                        btagLoose_HFdn_prob_DATA *= sf_HFdn_loose * eff_loose;
                        btagLoose_LFup_prob_DATA *= sf_LFup_loose * eff_loose;
                        btagLoose_LFdn_prob_DATA *= sf_LFdn_loose * eff_loose;
                    }
                    else
                    {
                        btagLoose_prob_MC *= (1 - eff_loose);
                        btagLoose_prob_DATA *= (1 - sf_loose * eff_loose);
                        btagLoose_up_prob_DATA *= (1 - sf_up_loose * eff_loose);
                        btagLoose_dn_prob_DATA *= (1 - sf_dn_loose * eff_loose);
                        btagLoose_HFup_prob_DATA *= (1 - sf_HFup_loose * eff_loose);
                        btagLoose_HFdn_prob_DATA *= (1 - sf_HFdn_loose * eff_loose);
                        btagLoose_LFup_prob_DATA *= (1 - sf_LFup_loose * eff_loose);
                        btagLoose_LFdn_prob_DATA *= (1 - sf_LFdn_loose * eff_loose);
                    }
                }
            }
        }
    }

    if (nt.event() == event_check) std::cout << "Debug 6" << endl;

    ana.tx.setBranch<float>("Common_event_tightBtagSF"      , btagTight_prob_DATA       / btagTight_prob_MC);
    ana.tx.setBranch<float>("Common_event_tightBtagSFup"    , btagTight_up_prob_DATA    / btagTight_prob_MC);
    ana.tx.setBranch<float>("Common_event_tightBtagSFdn"    , btagTight_dn_prob_DATA    / btagTight_prob_MC);
    ana.tx.setBranch<float>("Common_event_tightBtagSFHFup"  , btagTight_HFup_prob_DATA  / btagTight_prob_MC);
    ana.tx.setBranch<float>("Common_event_tightBtagSFHFdn"  , btagTight_HFdn_prob_DATA  / btagTight_prob_MC);
    ana.tx.setBranch<float>("Common_event_tightBtagSFLFup"  , btagTight_LFup_prob_DATA  / btagTight_prob_MC);
    ana.tx.setBranch<float>("Common_event_tightBtagSFLFdn"  , btagTight_LFdn_prob_DATA  / btagTight_prob_MC);
    ana.tx.setBranch<float>("Common_event_mediumBtagSF"     , btagMedium_prob_DATA      / btagMedium_prob_MC);
    ana.tx.setBranch<float>("Common_event_mediumBtagSFup"   , btagMedium_up_prob_DATA   / btagMedium_prob_MC);
    ana.tx.setBranch<float>("Common_event_mediumBtagSFdn"   , btagMedium_dn_prob_DATA   / btagMedium_prob_MC);
    ana.tx.setBranch<float>("Common_event_mediumBtagSFHFup" , btagMedium_HFup_prob_DATA / btagMedium_prob_MC);
    ana.tx.setBranch<float>("Common_event_mediumBtagSFHFdn" , btagMedium_HFdn_prob_DATA / btagMedium_prob_MC);
    ana.tx.setBranch<float>("Common_event_mediumBtagSFLFup" , btagMedium_LFup_prob_DATA / btagMedium_prob_MC);
    ana.tx.setBranch<float>("Common_event_mediumBtagSFLFdn" , btagMedium_LFdn_prob_DATA / btagMedium_prob_MC);
    ana.tx.setBranch<float>("Common_event_looseBtagSF"      , btagLoose_prob_DATA       / btagLoose_prob_MC);
    ana.tx.setBranch<float>("Common_event_looseBtagSFup"    , btagLoose_up_prob_DATA    / btagLoose_prob_MC);
    ana.tx.setBranch<float>("Common_event_looseBtagSFdn"    , btagLoose_dn_prob_DATA    / btagLoose_prob_MC);
    ana.tx.setBranch<float>("Common_event_looseBtagSFHFup"  , btagLoose_HFup_prob_DATA  / btagLoose_prob_MC);
    ana.tx.setBranch<float>("Common_event_looseBtagSFHFdn"  , btagLoose_HFdn_prob_DATA  / btagLoose_prob_MC);
    ana.tx.setBranch<float>("Common_event_looseBtagSFLFup"  , btagLoose_LFup_prob_DATA  / btagLoose_prob_MC);
    ana.tx.setBranch<float>("Common_event_looseBtagSFLFdn"  , btagLoose_LFdn_prob_DATA  / btagLoose_prob_MC);

    ana.tx.setBranch<int>("Common_nb_loose", nb_loose);
    ana.tx.setBranch<int>("Common_nb_medium", nb_medium);
    ana.tx.setBranch<int>("Common_nb_tight", nb_tight);
    ana.tx.setBranch<int>("Common_nb_loose_CSV", nb_loose_CSV);
    ana.tx.setBranch<int>("Common_nb_medium_CSV", nb_medium_CSV);
    ana.tx.setBranch<int>("Common_nb_tight_CSV", nb_tight_CSV);

    if (nt.event() == event_check) std::cout << "Debug 7" << endl;

    //---------------------------------------------------------------------------------------------
    // Fat Jet selection
    //---------------------------------------------------------------------------------------------
    // Loop over jets and do a simple overlap removal against leptons
    float fjSFvlc(1.), fjSFvlu(1.), fjSFvld(1.), fjSFmc(1.), fjSFmu(1.), fjSFmd(1.), fjSFtc(1.), fjSFtu(1.), fjSFtd(1.);
    float fjMDSFlc(1.), fjMDSFlu(1.), fjMDSFld(1.), fjMDSFmc(1.), fjMDSFmu(1.), fjMDSFmd(1.), fjMDSFtc(1.), fjMDSFtu(1.), fjMDSFtd(1.);
    for (unsigned int ifatjet = 0; ifatjet < nt.FatJet_p4().size(); ++ifatjet)
    {
    
        if (nt.year() >= 2022)
        {

        }
        else
        {
            //update with final WPs and for other years
            //currently from https://indico.cern.ch/event/1103765/contributions/4647556/attachments/2364610/4037250/ParticleNet_2018_ULNanoV9_JMAR_14Dec2021_PK.pdf
            //Lesya updated March 22 2022
            //From https://indico.cern.ch/event/1152827/contributions/4840404/attachments/2428856/4162159/ParticleNet_SFs_ULNanoV9_JMAR_25April2022_PK.pdf
            //Yulun updated August 5 2022
            float fjWPVloose  = 0.68;
            float fjWPmedium = 0.94;
            float fjWPtight  = 0.97;        
            float fjWPloose_MD  = 0.64;
            float fjWPmedium_MD = 0.85;
            float fjWPtight_MD  = 0.91;
            if (nt.year() == 2016 && !gconf.isAPV)
            {
                fjWPVloose  = 0.67;
                fjWPmedium = 0.93;
                fjWPtight  = 0.97;
                fjWPloose_MD  = 0.64;
                fjWPmedium_MD = 0.84;
                fjWPtight_MD  = 0.91;
            }
            if (nt.year() == 2017)
            {
                fjWPVloose  = 0.71;
                fjWPmedium = 0.94;
                fjWPtight  = 0.98;
                fjWPloose_MD  = 0.58;
                fjWPmedium_MD = 0.81;
                fjWPtight_MD  = 0.89;
            }
            if (nt.year() == 2018)
            {
                fjWPVloose  = 0.70;
                fjWPmedium = 0.94;
                fjWPtight  = 0.98;
                fjWPloose_MD  = 0.59;
                fjWPmedium_MD = 0.82;
                fjWPtight_MD  = 0.90;
            }

            LorentzVector fatjet_p4;
            float fatjet_msoftdrop;
            if (ana.is_postprocessed)
            {
                //if postprocessed NanoAOD, set pt to pt_nom, mass to mass_nom msoftdrop to msoftdrop_nom
                fatjet_p4=LorentzVector(nt.FatJet_pt_nom()[ifatjet],nt.FatJet_eta()[ifatjet],nt.FatJet_phi()[ifatjet],nt.FatJet_mass_nom()[ifatjet]);
                fatjet_msoftdrop = nt.FatJet_msoftdrop_nom()[ifatjet] / nt.FatJet_msoftdrop_corr_PUPPI()[ifatjet]; //undo the W-JMS correction
            }
            else
            {
                fatjet_p4 = nt.FatJet_p4()[ifatjet];
                fatjet_msoftdrop = nt.FatJet_msoftdrop()[ifatjet];

            }

            // TODO: What is POG recommendation? do we use nt.FatJet_jetId()?
            // Figure this out
            if (not (fatjet_p4.pt() > 180.))
                continue;
            if (not (abs(fatjet_p4.eta()) < 2.4))
                continue;

            // Because every muon and electron shows up in PF FatJet collections
            // Need to check against leptons
            bool is_overlapping_with_a_lepton = false;

            // Overlap check against leptons (electrons)
            for (unsigned int ilep = 0; ilep < ana.tx.getBranchLazy<vector<int>>("Common_lep_idxs").size(); ++ilep)
            {
                int ilep_idx = ana.tx.getBranchLazy<vector<int>>("Common_lep_idxs")[ilep];
                // If electron
                if (abs(ana.tx.getBranchLazy<vector<int>>("Common_lep_pdgid")[ilep]) == 11)
                {
                    if (RooUtil::Calc::DeltaR(fatjet_p4, nt.Electron_p4()[ilep_idx]) < 0.8)
                    {
                        is_overlapping_with_a_lepton = true;
                        break;
                    }
                }
                // else muon
                else
                {
                    if (RooUtil::Calc::DeltaR(fatjet_p4, nt.Muon_p4()[ilep_idx]) < 0.8)
                    {
                        is_overlapping_with_a_lepton = true;
                        break;
                    }
                }
            }

            if (is_overlapping_with_a_lepton)
                continue;
        
            //computed with instructions from NanoAOD reference "For W vs QCD tagging, use (Xcc+Xqq)/(Xcc+Xqq+QCD)"
            float W_MD = ( nt.FatJet_particleNetMD_Xcc()[ifatjet] + nt.FatJet_particleNetMD_Xqq()[ifatjet]) / (nt.FatJet_particleNetMD_Xcc()[ifatjet] + nt.FatJet_particleNetMD_Xqq()[ifatjet] + nt.FatJet_particleNetMD_QCD()[ifatjet]);

            // For now, accept anything that reaches this point
            ana.tx.pushbackToBranch<int>("Common_fatjet_idxs", ifatjet);
            ana.tx.pushbackToBranch<int>("Common_fatjet_id", nt.FatJet_jetId()[ifatjet]);
            ana.tx.pushbackToBranch<LorentzVector>("Common_fatjet_p4", fatjet_p4);
            ana.tx.pushbackToBranch<float>("Common_fatjet_msoftdrop", fatjet_msoftdrop);
            ana.tx.pushbackToBranch<float>("Common_fatjet_deepMD_W", nt.FatJet_deepTagMD_WvsQCD()[ifatjet]);
            ana.tx.pushbackToBranch<float>("Common_fatjet_deep_W", nt.FatJet_deepTag_WvsQCD()[ifatjet]);
            ana.tx.pushbackToBranch<float>("Common_fatjet_deepMD_Z", nt.FatJet_deepTagMD_ZvsQCD()[ifatjet]);
            ana.tx.pushbackToBranch<float>("Common_fatjet_deep_Z", nt.FatJet_deepTag_ZvsQCD()[ifatjet]);
            ana.tx.pushbackToBranch<float>("Common_fatjet_deepMD_T", nt.FatJet_deepTagMD_TvsQCD()[ifatjet]);
            ana.tx.pushbackToBranch<float>("Common_fatjet_deep_T", nt.FatJet_deepTag_TvsQCD()[ifatjet]);
            ana.tx.pushbackToBranch<float>("Common_fatjet_deepMD_bb", nt.FatJet_deepTagMD_bbvsLight()[ifatjet]);
            ana.tx.pushbackToBranch<float>("Common_fatjet_particleNet_W", nt.FatJet_particleNet_WvsQCD()[ifatjet]);
            ana.tx.pushbackToBranch<float>("Common_fatjet_particleNet_Z", nt.FatJet_particleNet_ZvsQCD()[ifatjet]);
            ana.tx.pushbackToBranch<float>("Common_fatjet_particleNet_T", nt.FatJet_particleNet_TvsQCD()[ifatjet]);
            ana.tx.pushbackToBranch<float>("Common_fatjet_particleNet_QCD", nt.FatJet_particleNet_QCD()[ifatjet]);
            ana.tx.pushbackToBranch<float>("Common_fatjet_particleNetMD_QCD", nt.FatJet_particleNetMD_QCD()[ifatjet]);
            ana.tx.pushbackToBranch<float>("Common_fatjet_particleNetMD_Xqq", nt.FatJet_particleNetMD_Xqq()[ifatjet]);
            ana.tx.pushbackToBranch<float>("Common_fatjet_particleNetMD_Xbb", nt.FatJet_particleNetMD_Xbb()[ifatjet]);
            ana.tx.pushbackToBranch<float>("Common_fatjet_particleNetMD_Xcc", nt.FatJet_particleNetMD_Xcc()[ifatjet]);
            ana.tx.pushbackToBranch<float>("Common_fatjet_particleNetMD_W", W_MD);
            ana.tx.pushbackToBranch<float>("Common_fatjet_tau3",  nt.FatJet_tau3()[ifatjet]);
            ana.tx.pushbackToBranch<float>("Common_fatjet_tau2",  nt.FatJet_tau2()[ifatjet]);
            ana.tx.pushbackToBranch<float>("Common_fatjet_tau1",  nt.FatJet_tau1()[ifatjet]);
            ana.tx.pushbackToBranch<float>("Common_fatjet_tau32", nt.FatJet_tau2()[ifatjet]!=0 ? nt.FatJet_tau3()[ifatjet] / nt.FatJet_tau2()[ifatjet] : -999.f);
            ana.tx.pushbackToBranch<float>("Common_fatjet_tau21", nt.FatJet_tau1()[ifatjet]!=0 ? nt.FatJet_tau2()[ifatjet] / nt.FatJet_tau1()[ifatjet] : -999.f);
            ana.tx.pushbackToBranch<float>("Common_fatjet_subjet0_pt",   nt.FatJet_subJetIdx1()[ifatjet] >= 0 ? nt.SubJet_pt()[  nt.FatJet_subJetIdx1()[ifatjet]] : -999.f);
            ana.tx.pushbackToBranch<float>("Common_fatjet_subjet0_eta",  nt.FatJet_subJetIdx1()[ifatjet] >= 0 ? nt.SubJet_eta()[ nt.FatJet_subJetIdx1()[ifatjet]] : -999.f);
            ana.tx.pushbackToBranch<float>("Common_fatjet_subjet0_phi",  nt.FatJet_subJetIdx1()[ifatjet] >= 0 ? nt.SubJet_phi()[ nt.FatJet_subJetIdx1()[ifatjet]] : -999.f);
            ana.tx.pushbackToBranch<float>("Common_fatjet_subjet0_mass", nt.FatJet_subJetIdx1()[ifatjet] >= 0 ? nt.SubJet_mass()[nt.FatJet_subJetIdx1()[ifatjet]] : -999.f);
            ana.tx.pushbackToBranch<float>("Common_fatjet_subjet1_pt",   nt.FatJet_subJetIdx2()[ifatjet] >= 0 ? nt.SubJet_pt()[  nt.FatJet_subJetIdx2()[ifatjet]] : -999.f);
            ana.tx.pushbackToBranch<float>("Common_fatjet_subjet1_eta",  nt.FatJet_subJetIdx2()[ifatjet] >= 0 ? nt.SubJet_eta()[ nt.FatJet_subJetIdx2()[ifatjet]] : -999.f);
            ana.tx.pushbackToBranch<float>("Common_fatjet_subjet1_phi",  nt.FatJet_subJetIdx2()[ifatjet] >= 0 ? nt.SubJet_phi()[ nt.FatJet_subJetIdx2()[ifatjet]] : -999.f);
            ana.tx.pushbackToBranch<float>("Common_fatjet_subjet1_mass", nt.FatJet_subJetIdx2()[ifatjet] >= 0 ? nt.SubJet_mass()[nt.FatJet_subJetIdx2()[ifatjet]] : -999.f);
            ana.tx.pushbackToBranch<LorentzVector>("Common_fatjet_subjet0_p4",  nt.FatJet_subJetIdx1()[ifatjet] >= 0 ? (RooUtil::Calc::getLV(nt.SubJet_pt()[nt.FatJet_subJetIdx1()[ifatjet]], nt.SubJet_eta()[nt.FatJet_subJetIdx1()[ifatjet]], nt.SubJet_phi()[nt.FatJet_subJetIdx1()[ifatjet]], nt.SubJet_mass()[nt.FatJet_subJetIdx1()[ifatjet]])) : (RooUtil::Calc::getLV(0., 0., 0., 0.)));
            ana.tx.pushbackToBranch<LorentzVector>("Common_fatjet_subjet1_p4",  nt.FatJet_subJetIdx2()[ifatjet] >= 0 ? (RooUtil::Calc::getLV(nt.SubJet_pt()[nt.FatJet_subJetIdx2()[ifatjet]], nt.SubJet_eta()[nt.FatJet_subJetIdx2()[ifatjet]], nt.SubJet_phi()[nt.FatJet_subJetIdx2()[ifatjet]], nt.SubJet_mass()[nt.FatJet_subJetIdx2()[ifatjet]])) : (RooUtil::Calc::getLV(0., 0., 0., 0.)));
            if (ana.is_postprocessed && !nt.isData())
            {
                ana.tx.pushbackToBranch<float>("Common_fatjet_pt_jesup", nt.FatJet_pt_jesTotalUp()[ifatjet]);
                ana.tx.pushbackToBranch<float>("Common_fatjet_pt_jesdn", nt.FatJet_pt_jesTotalDown()[ifatjet]);
                ana.tx.pushbackToBranch<float>("Common_fatjet_pt_jerup", nt.FatJet_pt_jerUp()[ifatjet]);
                ana.tx.pushbackToBranch<float>("Common_fatjet_pt_jerdn", nt.FatJet_pt_jerDown()[ifatjet]);
                //undo JMS correction
                ana.tx.pushbackToBranch<float>("Common_fatjet_msoftdrop_jesup", nt.FatJet_msoftdrop_jesTotalUp()[ifatjet] / nt.FatJet_msoftdrop_corr_PUPPI()[ifatjet]);
                ana.tx.pushbackToBranch<float>("Common_fatjet_msoftdrop_jesdn", nt.FatJet_msoftdrop_jesTotalDown()[ifatjet] / nt.FatJet_msoftdrop_corr_PUPPI()[ifatjet]);
                ana.tx.pushbackToBranch<float>("Common_fatjet_msoftdrop_jerup", nt.FatJet_msoftdrop_jerUp()[ifatjet] / nt.FatJet_msoftdrop_corr_PUPPI()[ifatjet]);
                ana.tx.pushbackToBranch<float>("Common_fatjet_msoftdrop_jerdn", nt.FatJet_msoftdrop_jerDown()[ifatjet] / nt.FatJet_msoftdrop_corr_PUPPI()[ifatjet]);
                ana.tx.pushbackToBranch<float>("Common_fatjet_msoftdrop_jmsup", nt.FatJet_msoftdrop_jmsUp()[ifatjet] / nt.FatJet_msoftdrop_corr_PUPPI()[ifatjet]);
                ana.tx.pushbackToBranch<float>("Common_fatjet_msoftdrop_jmsdn", nt.FatJet_msoftdrop_jmsDown()[ifatjet] / nt.FatJet_msoftdrop_corr_PUPPI()[ifatjet]);
                ana.tx.pushbackToBranch<float>("Common_fatjet_msoftdrop_jmrup", nt.FatJet_msoftdrop_jmrUp()[ifatjet] / nt.FatJet_msoftdrop_corr_PUPPI()[ifatjet]);
                ana.tx.pushbackToBranch<float>("Common_fatjet_msoftdrop_jmrdn", nt.FatJet_msoftdrop_jmrDown()[ifatjet] / nt.FatJet_msoftdrop_corr_PUPPI()[ifatjet]);
                //undo JMS correction
                ana.tx.pushbackToBranch<float>("Common_fatjet_mass_jesup", nt.FatJet_mass_jesTotalUp()[ifatjet]);
                ana.tx.pushbackToBranch<float>("Common_fatjet_mass_jesdn", nt.FatJet_mass_jesTotalDown()[ifatjet]);
                ana.tx.pushbackToBranch<float>("Common_fatjet_mass_jerup", nt.FatJet_mass_jerUp()[ifatjet]);
                ana.tx.pushbackToBranch<float>("Common_fatjet_mass_jerdn", nt.FatJet_mass_jerDown()[ifatjet]);
                ana.tx.pushbackToBranch<float>("Common_fatjet_mass_jmsup", nt.FatJet_mass_jmsUp()[ifatjet]);
                ana.tx.pushbackToBranch<float>("Common_fatjet_mass_jmsdn", nt.FatJet_mass_jmsDown()[ifatjet]);
                ana.tx.pushbackToBranch<float>("Common_fatjet_mass_jmrup", nt.FatJet_mass_jmrUp()[ifatjet]);
                ana.tx.pushbackToBranch<float>("Common_fatjet_mass_jmrdn", nt.FatJet_mass_jmrDown()[ifatjet]);

            }

            float WPtemp = 0;
            int WPid = -999;
            int WPid_MD = -999;


            if (nt.FatJet_particleNet_WvsQCD()[ifatjet] > fjWPVloose) WPid = 0;
            if (nt.FatJet_particleNet_WvsQCD()[ifatjet] > fjWPmedium) WPid = 2;
            if (nt.FatJet_particleNet_WvsQCD()[ifatjet] > fjWPtight) WPid = 3;
            if (W_MD > fjWPloose_MD) WPid_MD = 1; 
            if (W_MD > fjWPmedium_MD) WPid_MD = 2; 
            if (W_MD > fjWPtight_MD) WPid_MD = 3; 
            if (fatjet_msoftdrop >= 65. and fatjet_msoftdrop <= 105. and fatjet_p4.pt() > 200.)
            {
                ana.tx.pushbackToBranch<int>("Common_fatjet_WP", WPid);
                ana.tx.pushbackToBranch<int>("Common_fatjet_WP_MD", WPid_MD);
                ana.tx.pushbackToBranch<int>("Common_fatjet_WP_antimasscut", -999);
                ana.tx.pushbackToBranch<int>("Common_fatjet_WP_MD_antimasscut", -999);
            }
            else
            {
                ana.tx.pushbackToBranch<int>("Common_fatjet_WP", -999);
                ana.tx.pushbackToBranch<int>("Common_fatjet_WP_MD", -999);
                ana.tx.pushbackToBranch<int>("Common_fatjet_WP_antimasscut", WPid); // store W DNN cut even off mass peak
                ana.tx.pushbackToBranch<int>("Common_fatjet_WP_MD_antimasscut", WPid_MD); 
                WPid = -999;                                                       // I reset WPid to not store the fatjet SF for offmass peak
                WPid_MD = -999;
            }
            int year=(gconf.isAPV && nt.year()==2016)?10:nt.year();//use 0 for APV samples
            if (WPid >= 0)
            {
                WPtemp = ana.fatjetscalefactors.ak8SF(nt.isData(), year, 24, false, 0, fatjet_p4.eta(), fatjet_p4.pt(), 0);
                fjSFvlc *= WPtemp;
                ana.tx.pushbackToBranch<float>("Common_fatjet_SFVLoose", WPtemp);
                WPtemp = ana.fatjetscalefactors.ak8SF(nt.isData(), year, 24, false, 0, fatjet_p4.eta(), fatjet_p4.pt(), -1);
                fjSFvld *= WPtemp;
                ana.tx.pushbackToBranch<float>("Common_fatjet_SFdnVLoose", WPtemp);
                WPtemp = ana.fatjetscalefactors.ak8SF(nt.isData(), year, 24, false, 0, fatjet_p4.eta(), fatjet_p4.pt(), +1);
                fjSFvlu *= WPtemp;
                ana.tx.pushbackToBranch<float>("Common_fatjet_SFupVLoose", WPtemp);
            }
            else
            {
                ana.tx.pushbackToBranch<float>("Common_fatjet_SFVLoose", 0);
                ana.tx.pushbackToBranch<float>("Common_fatjet_SFdnVLoose", 0);
                ana.tx.pushbackToBranch<float>("Common_fatjet_SFupVLoose", 0);
            }
            if (WPid >= 2)
            {
                WPtemp = ana.fatjetscalefactors.ak8SF(nt.isData(), year, 24, false, 2, fatjet_p4.eta(), fatjet_p4.pt(), 0);
                fjSFmc *= WPtemp;
                ana.tx.pushbackToBranch<float>("Common_fatjet_SFMedium", WPtemp);
                WPtemp = ana.fatjetscalefactors.ak8SF(nt.isData(), year, 24, false, 2, fatjet_p4.eta(), fatjet_p4.pt(), -1);
                fjSFmd *= WPtemp;
                ana.tx.pushbackToBranch<float>("Common_fatjet_SFdnMedium", WPtemp);
                WPtemp = ana.fatjetscalefactors.ak8SF(nt.isData(), year, 24, false, 2, fatjet_p4.eta(), fatjet_p4.pt(), +1);
                fjSFmu *= WPtemp;
                ana.tx.pushbackToBranch<float>("Common_fatjet_SFupMedium", WPtemp);
            }
            else
            {
                ana.tx.pushbackToBranch<float>("Common_fatjet_SFMedium", 0);
                ana.tx.pushbackToBranch<float>("Common_fatjet_SFdnMedium", 0);
                ana.tx.pushbackToBranch<float>("Common_fatjet_SFupMedium", 0);
            }
            if (WPid >= 3)
            {
                WPtemp = ana.fatjetscalefactors.ak8SF(nt.isData(), year, 24, false, 3, fatjet_p4.eta(), fatjet_p4.pt(), 0);
                fjSFtc *= WPtemp;
                ana.tx.pushbackToBranch<float>("Common_fatjet_SFTight", WPtemp);
                WPtemp = ana.fatjetscalefactors.ak8SF(nt.isData(), year, 24, false, 3, fatjet_p4.eta(), fatjet_p4.pt(), -1);
                fjSFtd *= WPtemp;
                ana.tx.pushbackToBranch<float>("Common_fatjet_SFdnTight", WPtemp);
                WPtemp = ana.fatjetscalefactors.ak8SF(nt.isData(), year, 24, false, 3, fatjet_p4.eta(), fatjet_p4.pt(), +1);
                fjSFtu *= WPtemp;
                ana.tx.pushbackToBranch<float>("Common_fatjet_SFupTight", WPtemp);
            }
            else
            {
                ana.tx.pushbackToBranch<float>("Common_fatjet_SFTight", 0);
                ana.tx.pushbackToBranch<float>("Common_fatjet_SFdnTight", 0);
                ana.tx.pushbackToBranch<float>("Common_fatjet_SFupTight", 0);
            }
            if (WPid_MD >= 0)
            {
                WPtemp = ana.fatjetscalefactors.ak8SF(nt.isData(), year, 24, true, 1, fatjet_p4.eta(), fatjet_p4.pt(), 0);
                fjMDSFlc *= WPtemp;
                ana.tx.pushbackToBranch<float>("Common_fatjet_MD_SFLoose", WPtemp);
                WPtemp = ana.fatjetscalefactors.ak8SF(nt.isData(), year, 24, true, 1, fatjet_p4.eta(), fatjet_p4.pt(), -1);
                fjMDSFld *= WPtemp;
                ana.tx.pushbackToBranch<float>("Common_fatjet_MD_SFdnLoose", WPtemp);
                WPtemp = ana.fatjetscalefactors.ak8SF(nt.isData(), year, 24, true, 1, fatjet_p4.eta(), fatjet_p4.pt(), +1);
                fjMDSFlu *= WPtemp;
                ana.tx.pushbackToBranch<float>("Common_fatjet_MD_SFupLoose", WPtemp);
            }
            else
            {
                ana.tx.pushbackToBranch<float>("Common_fatjet_MD_SFLoose", 0);
                ana.tx.pushbackToBranch<float>("Common_fatjet_MD_SFdnLoose", 0);
                ana.tx.pushbackToBranch<float>("Common_fatjet_MD_SFupLoose", 0);
            }
            if (WPid_MD >= 2)
            {
                WPtemp = ana.fatjetscalefactors.ak8SF(nt.isData(), year, 24, true, 2, fatjet_p4.eta(), fatjet_p4.pt(), 0);
                fjMDSFmc *= WPtemp;
                ana.tx.pushbackToBranch<float>("Common_fatjet_MD_SFMedium", WPtemp);
                WPtemp = ana.fatjetscalefactors.ak8SF(nt.isData(), year, 24, true, 2, fatjet_p4.eta(), fatjet_p4.pt(), -1);
                fjMDSFmd *= WPtemp;
                ana.tx.pushbackToBranch<float>("Common_fatjet_MD_SFdnMedium", WPtemp);
                WPtemp = ana.fatjetscalefactors.ak8SF(nt.isData(), year, 24, true, 2, fatjet_p4.eta(), fatjet_p4.pt(), +1);
                fjMDSFmu *= WPtemp;
                ana.tx.pushbackToBranch<float>("Common_fatjet_MD_SFupMedium", WPtemp);
            }
            else
            {
                ana.tx.pushbackToBranch<float>("Common_fatjet_MD_SFMedium", 0);
                ana.tx.pushbackToBranch<float>("Common_fatjet_MD_SFdnMedium", 0);
                ana.tx.pushbackToBranch<float>("Common_fatjet_MD_SFupMedium", 0);
            }
            if (WPid_MD >= 3)
            {
                WPtemp = ana.fatjetscalefactors.ak8SF(nt.isData(), year, 24, true, 3, fatjet_p4.eta(), fatjet_p4.pt(), 0);
                fjMDSFtc *= WPtemp;
                ana.tx.pushbackToBranch<float>("Common_fatjet_MD_SFTight", WPtemp);
                WPtemp = ana.fatjetscalefactors.ak8SF(nt.isData(), year, 24, true, 3, fatjet_p4.eta(), fatjet_p4.pt(), -1);
                fjMDSFtd *= WPtemp;
                ana.tx.pushbackToBranch<float>("Common_fatjet_MD_SFdnTight", WPtemp);
                WPtemp = ana.fatjetscalefactors.ak8SF(nt.isData(), year, 24, true, 3, fatjet_p4.eta(), fatjet_p4.pt(), +1);
                fjMDSFtu *= WPtemp;
                ana.tx.pushbackToBranch<float>("Common_fatjet_MD_SFupTight", WPtemp);
            }
            else
            {
                ana.tx.pushbackToBranch<float>("Common_fatjet_MD_SFTight", 0);
                ana.tx.pushbackToBranch<float>("Common_fatjet_MD_SFdnTight", 0);
                ana.tx.pushbackToBranch<float>("Common_fatjet_MD_SFupTight", 0);
            }
        }
    }

    if (nt.year() >= 2022)
    {

    }
    else
    {
        if (nt.event() == event_check) std::cout << "Debug 8" << endl;

        ana.tx.setBranch<float>("Common_eventweight_fatjet_SFVLoose",   fjSFvlc);
        ana.tx.setBranch<float>("Common_eventweight_fatjet_SFMedium",    fjSFmc);
        ana.tx.setBranch<float>("Common_eventweight_fatjet_SFTight",     fjSFtc);
        ana.tx.setBranch<float>("Common_eventweight_fatjet_SFupVLoose", fjSFvlu);
        ana.tx.setBranch<float>("Common_eventweight_fatjet_SFupMedium",  fjSFmu);
        ana.tx.setBranch<float>("Common_eventweight_fatjet_SFupTight",   fjSFtu);
        ana.tx.setBranch<float>("Common_eventweight_fatjet_SFdnVLoose", fjSFvld);
        ana.tx.setBranch<float>("Common_eventweight_fatjet_SFdnMedium",  fjSFmd);
        ana.tx.setBranch<float>("Common_eventweight_fatjet_SFdnTight",   fjSFtd);
        ana.tx.setBranch<float>("Common_eventweight_fatjet_MD_SFLoose",     fjMDSFlc);
        ana.tx.setBranch<float>("Common_eventweight_fatjet_MD_SFMedium",    fjMDSFmc);
        ana.tx.setBranch<float>("Common_eventweight_fatjet_MD_SFTight",     fjMDSFtc);
        ana.tx.setBranch<float>("Common_eventweight_fatjet_MD_SFupLoose",   fjMDSFlu);
        ana.tx.setBranch<float>("Common_eventweight_fatjet_MD_SFupMedium",  fjMDSFmu);
        ana.tx.setBranch<float>("Common_eventweight_fatjet_MD_SFupTight",   fjMDSFtu);
        ana.tx.setBranch<float>("Common_eventweight_fatjet_MD_SFdnLoose",   fjMDSFld);
        ana.tx.setBranch<float>("Common_eventweight_fatjet_MD_SFdnMedium",  fjMDSFmd);
        ana.tx.setBranch<float>("Common_eventweight_fatjet_MD_SFdnTight",   fjMDSFtd);


        for (unsigned int ijet = 0; ijet < ana.tx.getBranchLazy<vector<int>>("Common_jet_idxs").size(); ++ijet)
        {
            int overlap_with_fatjet = -1;
            for (unsigned int ifat = 0; ifat < ana.tx.getBranchLazy<vector<int>>("Common_fatjet_idxs").size(); ++ifat)
            {
                if (RooUtil::Calc::DeltaR(ana.tx.getBranchLazy<vector<LorentzVector>>("Common_fatjet_p4")[ifat], ana.tx.getBranchLazy<vector<LorentzVector>>("Common_jet_p4")[ijet]) < 0.8)//should this be 0.4
                {
                    overlap_with_fatjet = ifat;
                    break;
                }
            }
            ana.tx.pushbackToBranch<int>("Common_jet_overlapfatjet", overlap_with_fatjet);
        }
    }

    if (nt.event() == event_check) std::cout << "Debug 9" << endl;

    //---------------------------------------------------------------------------------------------
    // MET selection
    //---------------------------------------------------------------------------------------------
    if (ana.is_postprocessed)
    {
        if (nt.isData())	
            ana.tx.setBranch<LorentzVector>("Common_met_p4", RooUtil::Calc::getLV(nt.MET_T1_pt(), 0., nt.MET_T1_phi(), 0));
        else
        {
            ana.tx.setBranch<LorentzVector>("Common_met_p4", RooUtil::Calc::getLV(nt.MET_T1Smear_pt(), 0., nt.MET_T1Smear_phi(), 0));
            ana.tx.setBranch<LorentzVector>("Common_met_p4_jesup", RooUtil::Calc::getLV(nt.MET_T1Smear_pt_jesTotalUp(), 0., nt.MET_T1Smear_phi_jesTotalUp(), 0));
            ana.tx.setBranch<LorentzVector>("Common_met_p4_jesdn", RooUtil::Calc::getLV(nt.MET_T1Smear_pt_jesTotalDown(), 0., nt.MET_T1Smear_phi_jesTotalDown(), 0));
            ana.tx.setBranch<LorentzVector>("Common_met_p4_jerup", RooUtil::Calc::getLV(nt.MET_T1Smear_pt_jerUp(), 0., nt.MET_T1Smear_phi_jerUp(), 0));
            ana.tx.setBranch<LorentzVector>("Common_met_p4_jerdn", RooUtil::Calc::getLV(nt.MET_T1Smear_pt_jerDown(), 0., nt.MET_T1Smear_phi_jerDown(), 0));
        }
    }
    else
    {
        ana.tx.setBranch<LorentzVector>("Common_met_p4", RooUtil::Calc::getLV(nt.MET_pt(), 0., nt.MET_phi(), 0));
    }

    ana.tx.setBranch<LorentzVector>("Common_met_p4_MET", RooUtil::Calc::getLV(nt.MET_pt(), 0., nt.MET_phi(), 0));
    ana.tx.setBranch<LorentzVector>("Common_met_p4_PuppiMET", RooUtil::Calc::getLV(nt.PuppiMET_pt(), 0., nt.PuppiMET_phi(), 0));

    if (nt.event() == event_check) std::cout << "Debug 10" << endl;

    //---------------------------------------------------------------------------------------------
    // Gen-level particle selection
    //---------------------------------------------------------------------------------------------
    // This is only possible when it is MC and has GenPart Branches
    if (not nt.isData())
    {
        if (nt.year() >= 2022)
        {

        }
        else
        {
            bool isSignal = false;
            float genHT = 0; // variable to be used to stitch HT-sliced samples

            // Loop over generator particles and do stuff
            for (unsigned int igen = 0; igen < nt.GenPart_pdgId().size(); ++igen)
            {

                // Following particles are used to compute genHT
                if ((abs(nt.GenPart_pdgId()[igen]) <  6 or // quarks
                    abs(nt.GenPart_pdgId()[igen]) == 21)  // gluons
                        and (nt.GenPart_status()[igen] == 22 or // something to do with "status 3"
                            nt.GenPart_status()[igen] == 23))
                {
                    genHT += nt.GenPart_pt()[igen];
                }

                // Preliminary filter on the gen particles
                if (not (nt.GenPart_statusFlags()[igen] & (1 << 8))) continue; // fromHardProcess
                if (not (abs(nt.GenPart_pdgId()[igen]) <= 25)) continue;
                if (not (abs(nt.GenPart_pdgId()[igen]) != 21)) continue;

                // If vector boson do a last copy
                if (abs(nt.GenPart_pdgId()[igen]) == 23 or abs(nt.GenPart_pdgId()[igen]) == 24)
                {
                    if (not (nt.GenPart_statusFlags()[igen] & (1 << 13))) continue; // isLastCopy
                }
                else if (abs(nt.GenPart_pdgId()[igen]) >= 11 and abs(nt.GenPart_pdgId()[igen]) <= 16)
                {
                    if (not (nt.GenPart_statusFlags()[igen] & (1 << 12))) continue; // isFirstCopy
                }
                else if (abs(nt.GenPart_pdgId()[igen]) >= 1 and abs(nt.GenPart_pdgId()[igen]) <= 5)
                {
                    if (not (nt.GenPart_statusFlags()[igen] & (1 << 12))) continue; // isFirstCopy
                }
                else // If not defined in previous line we don't consider the object
                {
                    continue;
                }

                isSignal = true;
            
                ana.tx.pushbackToBranch<int>          ("Common_gen_idx", igen);                                                            // Selected gen-particle idx in NanoAOD
                ana.tx.pushbackToBranch<int>          ("Common_gen_mother_idx", nt.GenPart_genPartIdxMother()[igen]);                      // Selected gen-particle mother idx in NanoAOD
                ana.tx.pushbackToBranch<int>          ("Common_gen_mother_id",  nt.GenPart_pdgId()[nt.GenPart_genPartIdxMother()[igen] ]); // Selected gen-particle mother idx in NanoAOD
                ana.tx.pushbackToBranch<int>          ("Common_gen_pdgid", nt.GenPart_pdgId()[igen]);                                      // Selected gen-particle pdgids
                ana.tx.pushbackToBranch<LorentzVector>("Common_gen_p4s", nt.GenPart_p4()[igen]);                                           // Selected gen-particle p4s

            }

            ana.tx.setBranch<float>("Common_genHT", genHT);
            ana.tx.setBranch<bool>("Common_isSignal", isSignal);

            // Selecting 6 fermions from VVV decays
            // int ngen = 0;
            if(isSignal){
                vector<int> vvvdecay_candidates; // list of idxs that points to the vvv decays
                for (unsigned int igen = 0; igen < ana.tx.getBranch<vector<int>>("Common_gen_pdgid").size(); ++igen)
                {
                    const int& pdgid = ana.tx.getBranch<vector<int>>("Common_gen_pdgid")[igen];
                    const int& mother_idx = ana.tx.getBranch<vector<int>>("Common_gen_mother_idx")[igen];
                    float pt = ana.tx.getBranch<vector<LorentzVector>>("Common_gen_p4s")[igen].pt();
                    if (abs(pdgid) <= 20 and mother_idx >= 0 and pt >= 1e-8) // pt requirement is added because sometimes the incoming parton gets included ....
                    {
                        vvvdecay_candidates.push_back(igen);
                    }
                }

                // If we find that there are 6 particles only, then we accept it as good and no need to try to salvage
                if (vvvdecay_candidates.size() == 6)
                {
                }
                else if (vvvdecay_candidates.size() == 7) // For WWW this seems to be the common case
                {
                    vvvdecay_candidates.erase(vvvdecay_candidates.begin());
                }
                else
                {
                    // std::cout << "did not find 6 ngen" << vvvdecay_candidates.size() << " " << ana.looper.getCurrentEventIndex() << std::endl;
                    // for (auto& igen : vvvdecay_candidates)
                    // {
                    //     std::cout <<  " ana.tx.getBranch<vector<int>>('Common_gen_idx')[igen]: " << ana.tx.getBranch<vector<int>>("Common_gen_idx")[igen] <<  std::endl;
                    //     std::cout <<  " ana.tx.getBranch<vector<int>>('Common_gen_mother_idx')[igen]: " << ana.tx.getBranch<vector<int>>("Common_gen_mother_idx")[igen] <<  std::endl;
                    //     std::cout <<  " ana.tx.getBranch<vector<int>>('Common_gen_pdgid')[igen]: " << ana.tx.getBranch<vector<int>>("Common_gen_pdgid")[igen] <<  std::endl;
                    //     std::cout <<  " ana.tx.getBranch<vector<LorentzVector>>('Common_gen_p4s')[igen]: " << ana.tx.getBranch<vector<LorentzVector>>("Common_gen_p4s")[igen] <<  std::endl;
                    // }
                    // std::cout << "=======================" << std::endl;
                    // std::cout << "duplicate mother found!" << std::endl;
                    // std::cout << "=======================" << std::endl;
                    // for (unsigned int igen = 0; igen < nt.GenPart_pdgId().size(); ++igen)
                    // {
                    //     std::cout <<  " igen: " << igen <<  std::endl;
                    //     std::cout <<  " nt.GenPart_pdgId()[igen]: " << nt.GenPart_pdgId()[igen] <<  std::endl;
                    //     std::cout <<  " nt.GenPart_status()[igen]: " << nt.GenPart_status()[igen] <<  std::endl;
                    //     std::cout <<  " nt.GenPart_statusFlags()[igen]: " << nt.GenPart_statusFlags()[igen] <<  std::endl;
                    //     std::cout <<  " nt.GenPart_genPartIdxMother()[igen]: " << nt.GenPart_genPartIdxMother()[igen] <<  std::endl;
                    //     std::cout <<  " nt.GenPart_p4()[igen].pt(): " << nt.GenPart_p4()[igen].pt() <<  std::endl;
                    //     std::cout <<  " nt.GenPart_p4()[igen].eta(): " << nt.GenPart_p4()[igen].eta() <<  std::endl;
                    //     std::cout <<  " nt.GenPart_p4()[igen].phi(): " << nt.GenPart_p4()[igen].phi() <<  std::endl;
                    // }
                    // std::cout << "=======================" << std::endl;
                }

                int n_light_lepton = 0;
                for (auto& igen : vvvdecay_candidates)
                {
                    ana.tx.pushbackToBranch<int>          ("Common_gen_vvvdecay_idx"         , ana.tx.getBranch<vector<int>>          ("Common_gen_idx")[igen]); // Selected gen-particle of vvv decays idx in NanoAOD
                    ana.tx.pushbackToBranch<int>          ("Common_gen_vvvdecay_mother_idx"  , ana.tx.getBranch<vector<int>>          ("Common_gen_mother_idx")[igen]); // Selected gen-particle of vvv decays mother idx in NanoAOD
                    ana.tx.pushbackToBranch<int>          ("Common_gen_vvvdecay_mother_id"   , ana.tx.getBranch<vector<int>>          ("Common_gen_mother_id")[igen]); // Selected gen-particle of vvv decays mother idx in NanoAOD
                    ana.tx.pushbackToBranch<int>          ("Common_gen_vvvdecay_pdgid"       , ana.tx.getBranch<vector<int>>          ("Common_gen_pdgid")[igen]); // Selected gen-particle of vvv decays pdgids
                    ana.tx.pushbackToBranch<LorentzVector>("Common_gen_vvvdecay_p4s"         , ana.tx.getBranch<vector<LorentzVector>>("Common_gen_p4s")[igen]); // Selected gen-particle of vvv decays p4s
                    if (abs(ana.tx.getBranch<vector<int>>("Common_gen_pdgid")[igen]) != 15)
                    {
                        ana.tx.pushbackToBranch<int>("Common_gen_vvvdecay_taudecayid", 0); // no tau
                    }
                    else
                    {

                        bool filled = false;

                        for (unsigned int jgen = 0; jgen < ana.tx.getBranch<vector<int>>("Common_gen_pdgid").size(); ++jgen)
                        {

                            if (ana.tx.getBranch<vector<int>>("Common_gen_idx")[igen] != ana.tx.getBranch<vector<int>>("Common_gen_mother_idx")[jgen])
                                continue;

                            //jgen has as mother the tau
                            if (abs(ana.tx.getBranch<vector<int>>("Common_gen_pdgid")[jgen]) == 11 or abs(ana.tx.getBranch<vector<int>>("Common_gen_pdgid")[jgen]) == 13) 
                            {
                                filled = true;
                                ana.tx.pushbackToBranch<int>("Common_gen_vvvdecay_taudecayid"  ,ana.tx.getBranch<vector<int>>("Common_gen_pdgid")[jgen]);
                                break;
                            }
                        }
                        if (not filled)
                        {
                            if(ana.tx.getBranch<vector<int>>("Common_gen_pdgid")[igen] == 15)
                            {
                                ana.tx.pushbackToBranch<int>("Common_gen_vvvdecay_taudecayid", -211);
                            }
                            else
                            {
                                ana.tx.pushbackToBranch<int>("Common_gen_vvvdecay_taudecayid",  211);
                            }
                        }
                    }

                    if (abs(ana.tx.getBranch<vector<int>>("Common_gen_pdgid")[igen]) == 11 or abs(ana.tx.getBranch<vector<int>>("Common_gen_pdgid")[igen]) == 13)
                    {
                        n_light_lepton++;
                    }
                }

                ana.tx.setBranch<int>("Common_gen_n_light_lep", n_light_lepton);

                int nW = 0;
                int nZ = 0;
                int nlepW = 0;
                int ntaulepW = 0;
                int ntauhadW = 0;
                int nlepZ = 0;
                int ntaulepZ = 0;
                int ntauhadZ = 0;
                int nnuZ = 0;
                int nbZ = 0;
                int W1 = -1;
                int W2 = -1;
                bool isSS = false;

                for (unsigned int igen = 0; igen < ana.tx.getBranchLazy<vector<int>>("Common_gen_vvvdecay_idx").size(); ++igen)
                {

                    int decay = ana.tx.getBranchLazy<vector<int>>("Common_gen_vvvdecay_pdgid")[igen];
                    int mother = ana.tx.getBranchLazy<vector<int>>("Common_gen_vvvdecay_mother_id")[igen];
                    int leptau = ana.tx.getBranchLazy<vector<int>>("Common_gen_vvvdecay_taudecayid")[igen];

                    if (abs(mother) == 24)
                    {

                        ++nW;

                        if (abs(decay) == 11 or abs(decay) == 13)
                        {
                            ++nlepW;
                        }
                        else if (abs(decay) == 15 and (abs(leptau) == 11 or abs(leptau) == 13))
                        {
                            ++ntaulepW;
                        }
                        else if (abs(decay) == 15 and abs(leptau) == 211)
                        {
                            ++ntauhadW;
                        }
                        if (abs(decay) == 11 or abs(decay) == 13 or (abs(decay) == 15 and (abs(leptau) == 11 or abs(leptau) == 13)))
                        {
                            if (W1 < 0)
                            {
                                W1 = igen; // first W
                            }
                            else
                            {
                                if (mother == ana.tx.getBranchLazy<vector<int>>("Common_gen_vvvdecay_mother_id")[W1])
                                {
                                    isSS = true; // first and this W are SS
                                }
                                else if (W2 < 0)
                                {
                                    W2 = igen; // second W
                                }
                                else if (mother == ana.tx.getBranchLazy<vector<int>>("Common_gen_vvvdecay_mother_id")[W2])
                                {
                                    isSS = true; // second and this W are SS
                                }
                            }
                        }
                    }

                    if (abs(mother) == 23)
                    {
                        ++nZ;
                        if (abs(decay) == 11 or abs(decay) == 13)
                        {
                            ++nlepZ;
                        }
                        else if (abs(decay) == 15 and (abs(leptau) == 11 or abs(leptau) == 13))
                        {
                            ++ntaulepZ;
                        }
                        else if (abs(decay) == 15 and abs(leptau) == 211)
                        {
                            ++ntauhadZ;
                        }
                        else if (abs(decay) == 12 or abs(decay) == 14 or abs(decay) == 16)
                        {
                            ++nnuZ;
                        }
                        else if (abs(decay) == 5)
                        {
                            ++nbZ;
                        }
                    }
                }

                ana.tx.setBranch<int> ("Common_n_W",        nW      );
                ana.tx.setBranch<int> ("Common_n_lep_W",    nlepW   );
                ana.tx.setBranch<int> ("Common_n_leptau_W", ntaulepW);
                ana.tx.setBranch<int> ("Common_n_hadtau_W", ntauhadW);
                ana.tx.setBranch<int> ("Common_n_Z",        nZ      );
                ana.tx.setBranch<int> ("Common_n_lep_Z",    nlepZ   );
                ana.tx.setBranch<int> ("Common_n_leptau_Z", ntaulepZ);
                ana.tx.setBranch<int> ("Common_n_hadtau_Z", ntauhadZ);
                ana.tx.setBranch<int> ("Common_n_nu_Z",     nnuZ    );
                ana.tx.setBranch<int> ("Common_n_b_Z",      nbZ     );
                ana.tx.setBranch<bool>("Common_haslepWSS",  isSS    );
            }
            // If VH ntuple than process Common_gen_VH_channel
            if (ana.input_file_list_tstring.Contains("VHToNonbb_M125"))
            {

                // The VHToNonbb_M125 sample's 3rd gen particle is always "V" of the VH
                // HOWEVER IN THE FUTURE MAKE SURE THIS IS TRUE ALWAYS IF ONE CHANGES SAMPLES!!
                int V_abspdgid = abs(nt.GenPart_pdgId()[2]);
                int HXX_abspdgid = 0;

                // Loop over generator particles and do stuff
                for (unsigned int igen = 0; igen < nt.GenPart_pdgId().size(); ++igen)
                {

                    // Get the particles with higgs as mother that is not higgs
                    if (
                            /* I am not Higgs      */ abs(nt.GenPart_pdgId()[igen]) != 25 and
                            /* But my mom is Higgs */ abs(nt.GenPart_pdgId()[nt.GenPart_genPartIdxMother()[igen]] == 25)
                    )
                    {
                        HXX_abspdgid = abs(nt.GenPart_pdgId()[igen]);
                        break;
                    }
                }

                if (HXX_abspdgid == 0)
                {
                    std::cout << "WARNING: Did not find a particle that seems to match H->XX topology!";
                    std::cout  << "event # = " << ana.looper.getCurrentEventIndex() << std::endl;
                }

                if      (V_abspdgid == 24 and HXX_abspdgid == 24) ana.tx.setBranch<int>("Common_gen_VH_channel", 0);
                else if (V_abspdgid == 23 and HXX_abspdgid == 24) ana.tx.setBranch<int>("Common_gen_VH_channel", 1);
                else if (V_abspdgid == 24 and HXX_abspdgid == 23) ana.tx.setBranch<int>("Common_gen_VH_channel", 2);
                else if (V_abspdgid == 23 and HXX_abspdgid == 23) ana.tx.setBranch<int>("Common_gen_VH_channel", 3);
                else                                              ana.tx.setBranch<int>("Common_gen_VH_channel", -HXX_abspdgid);
            }
        }
    }
    else
    {
        ana.tx.setBranch<bool>("Common_isSignal", false);
    }

    if (nt.event() == event_check) std::cout << "Debug 11" << endl;
   
    //---------------------------------------------------------------------------------------------
    // Organizing object indices and sorting by Pt
    //---------------------------------------------------------------------------------------------

    // Sorting lepton branches
    ana.tx.sortVecBranchesByPt(
            /* name of the 4vector branch to use to pt sort by*/               "Common_lep_p4",
            /* names of any associated vector<float> branches to sort along */ {"Common_lep_dxy", "Common_lep_dz", "Common_lep_ip3d", "Common_lep_sip3d", "Common_lep_SF", "Common_lep_SFTight", "Common_lep_SFdn", "Common_lep_SFdnTight", "Common_lep_SFup", "Common_lep_SFupTight", "Common_lep_relIso03_all"},
            /* names of any associated vector<int>   branches to sort along */ {"Common_lep_idxs", "Common_lep_pdgid", "Common_lep_tight","Common_lep_ID","Common_lep_IsoID"},
            /* names of any associated vector<bool>  branches to sort along */ {}
            );

    // Sorting tau branches
    ana.tx.sortVecBranchesByPt(
	    /* name of the 4vector branch to use to pt sort by*/	       "Common_tau_p4",
	    /* names of any associated vector<float> branches to sort along */ {"Common_tau_dxy","Common_tau_dz","Common_tau_eta","Common_tau_chargedIso","Common_tau_neutralIso",},
	    /* names of any associated vector<int>   branches to sort along */ {"Common_tau_idxs","Common_tau_decayMode","Common_tau_jetIdx","Common_tau_charge","Common_tau_idVSe","Common_tau_idVSmu","Common_tau_idVSjet",},
	    /* names of any associated vector<bool>  branches to sort along */ {}
    	    );

    ana.tx.sortVecBranchesByPt(
	    								       "Common_GenVisTau_p4",
										{},
										{"Common_GenVisTau_idxs","Common_GenVisTau_status",},
										{}
	    );

    if (nt.event() == event_check) std::cout << "Debug 12" << endl;

    // Sorting jet branches
    ana.tx.sortVecBranchesByPt(
            /* name of the 4vector branch to use to pt sort by*/               "Common_jet_p4",
            /* names of any associated vector<float> branches to sort along */ {},
            /* names of any associated vector<int>   branches to sort along */ {"Common_jet_idxs", "Common_jet_overlapfatjet", "Common_jet_id", "Common_jet_genJetIdx", "Common_jet_hadronFlavour", "Common_jet_partonFlavour",},
            /* names of any associated vector<bool>  branches to sort along */ {"Common_jet_passBloose", "Common_jet_passBmedium", "Common_jet_passBtight","Common_jet_passBloose_CSV","Common_jet_passBmedium_CSV","Common_jet_passBtight_CSV"}
            );

    if (nt.event() == event_check) std::cout << "Debug 13" << endl;

    // Sorting fatjet branches
    ana.tx.sortVecBranchesByPt(
            /* name of the 4vector branch to use to pt sort by*/               "Common_fatjet_p4",
            /* names of any associated vector<float> branches to sort along */ {"Common_fatjet_msoftdrop", "Common_fatjet_deepMD_W", "Common_fatjet_deep_W", "Common_fatjet_deepMD_Z", "Common_fatjet_deep_Z", "Common_fatjet_deepMD_T", "Common_fatjet_deep_T", "Common_fatjet_deepMD_bb", "Common_fatjet_tau3", "Common_fatjet_tau2", "Common_fatjet_tau1", "Common_fatjet_tau32", "Common_fatjet_tau21", "Common_fatjet_subjet0_pt", "Common_fatjet_subjet0_eta", "Common_fatjet_subjet0_phi", "Common_fatjet_subjet0_mass", "Common_fatjet_subjet1_pt", "Common_fatjet_subjet1_eta", "Common_fatjet_subjet1_phi", "Common_fatjet_subjet1_mass", "Common_fatjet_SFVLoose", "Common_fatjet_SFLoose", "Common_fatjet_SFMedium", "Common_fatjet_SFTight", "Common_fatjet_SFdnVLoose", "Common_fatjet_SFdnLoose", "Common_fatjet_SFdnMedium", "Common_fatjet_SFdnTight", "Common_fatjet_SFupVLoose", "Common_fatjet_SFupLoose", "Common_fatjet_SFupMedium", "Common_fatjet_SFupTight" /*, "Common_fatjet_subjet0_p4", "Common_fatjet_subjet1_p4",*/, "Common_fatjet_pt_jesup", "Common_fatjet_pt_jesdn", "Common_fatjet_pt_jerup", "Common_fatjet_pt_jerdn", "Common_fatjet_msoftdrop_jesup", "Common_fatjet_msoftdrop_jesdn", "Common_fatjet_msoftdrop_jerup", "Common_fatjet_msoftdrop_jerdn", "Common_fatjet_msoftdrop_jmsup", "Common_fatjet_msoftdrop_jmsdn", "Common_fatjet_msoftdrop_jmrup", "Common_fatjet_msoftdrop_jmrdn"
                                                                               },
            /* names of any associated vector<int>   branches to sort along */ {"Common_fatjet_idxs", "Common_fatjet_id", "Common_fatjet_WP", "Common_fatjet_WP_antimasscut"},
            /* names of any associated vector<bool>  branches to sort along */ {}
            );
 
     if (nt.event() == event_check) std::cout << "Debug 14" << endl;

     //-------------------------------------------------------------------------------------------
     // Setting Trigger weights
     //-------------------------------------------------------------------------------------------
    if (nt.year() < 2022)
    { 
        if(ana.tx.getBranchLazy<vector<int>>("Common_lep_pdgid").size() >= 2)
        {
            int lep1pdgid = ana.tx.getBranchLazy<vector<int>>("Common_lep_pdgid")[0];
            int lep2pdgid = ana.tx.getBranchLazy<vector<int>>("Common_lep_pdgid")[1];
            if(abs(lep1pdgid) == 11 && abs(lep2pdgid) == 11)
            {
                float lep1pt = std::min(std::max(ana.tx.getBranchLazy<vector<LorentzVector>>("Common_lep_p4")[0].pt(),20.01f),499.9f);
                float lep2pt = std::min(std::max(ana.tx.getBranchLazy<vector<LorentzVector>>("Common_lep_p4")[1].pt(),15.01f),499.9f);
                ana.tx.setBranch<float>("Common_event_triggerWeight",   ana.triggereeSF->eval(lep1pt, lep2pt));
                ana.tx.setBranch<float>("Common_event_triggerWeightup", ana.triggereeSF->eval_up(lep1pt, lep2pt));
                ana.tx.setBranch<float>("Common_event_triggerWeightdn", ana.triggereeSF->eval_down(lep1pt, lep2pt));
            }
            else if(abs(lep1pdgid) == 11 && abs(lep2pdgid) == 13)
            {
                float lepept = std::min(std::max(ana.tx.getBranchLazy<vector<LorentzVector>>("Common_lep_p4")[0].pt(),15.01f),499.9f);
                float lepmupt = std::min(std::max(ana.tx.getBranchLazy<vector<LorentzVector>>("Common_lep_p4")[1].pt(),15.01f),499.9f);
                ana.tx.setBranch<float>("Common_event_triggerWeight",   ana.triggeremuSF->eval(lepept, lepmupt));
                ana.tx.setBranch<float>("Common_event_triggerWeightup", ana.triggeremuSF->eval_up(lepept, lepmupt));
                ana.tx.setBranch<float>("Common_event_triggerWeightdn", ana.triggeremuSF->eval_down(lepept, lepmupt));
            }
            else if(abs(lep1pdgid) == 13 && abs(lep2pdgid) == 11)
            {
                float lepept = std::min(std::max(ana.tx.getBranchLazy<vector<LorentzVector>>("Common_lep_p4")[1].pt(),15.01f),499.9f);
                float lepmupt = std::min(std::max(ana.tx.getBranchLazy<vector<LorentzVector>>("Common_lep_p4")[0].pt(),15.01f),499.9f);
                ana.tx.setBranch<float>("Common_event_triggerWeight",   ana.triggeremuSF->eval(lepept, lepmupt));
                ana.tx.setBranch<float>("Common_event_triggerWeightup", ana.triggeremuSF->eval_up(lepept, lepmupt));
                ana.tx.setBranch<float>("Common_event_triggerWeightdn", ana.triggeremuSF->eval_down(lepept, lepmupt));
            }
            else if(abs(lep1pdgid) == 13 && abs(lep2pdgid) == 13)
            {
                float lep1pt = std::min(std::max(ana.tx.getBranchLazy<vector<LorentzVector>>("Common_lep_p4")[0].pt(),20.01f),499.9f);
                float lep2pt = std::min(std::max(ana.tx.getBranchLazy<vector<LorentzVector>>("Common_lep_p4")[1].pt(),15.01f),499.9f);
                ana.tx.setBranch<float>("Common_event_triggerWeight",   ana.triggermumuSF->eval(lep1pt, lep2pt));
                ana.tx.setBranch<float>("Common_event_triggerWeightup", ana.triggermumuSF->eval_up(lep1pt, lep2pt));
                ana.tx.setBranch<float>("Common_event_triggerWeightdn", ana.triggermumuSF->eval_down(lep1pt, lep2pt));
            }
  
            else
            {
                ana.tx.setBranch<float>("Common_event_triggerWeight",   1.);
                ana.tx.setBranch<float>("Common_event_triggerWeightup", 1.);
                ana.tx.setBranch<float>("Common_event_triggerWeightdn", 1.);
            }
        }
        else
        {
            ana.tx.setBranch<float>("Common_event_triggerWeight",   1.);
            ana.tx.setBranch<float>("Common_event_triggerWeightup", 1.);
            ana.tx.setBranch<float>("Common_event_triggerWeightdn", 1.);
        }  

        if (nt.event() == event_check) std::cout << "Debug 15" << endl;
    }
}

