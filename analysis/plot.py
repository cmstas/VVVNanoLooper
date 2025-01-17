#!/bin/env python

import plottery_wrapper as p
import os

analysis_output="/home/users/kdownham/Triboson/VVVNanoLooper/analysis/output_010124_BDTVars"
plot_output_dir="/WWZ/010924_BDTVars"

histxaxislabeloptions = {
"DRll"             : {"xaxis_label" : "#DeltaR_{ll}"                           , "xaxis_ndivisions" : 505 , "nbins" : 20} , 
#"MET"              : {"xaxis_label" : "MET [GeV]"                              , "xaxis_ndivisions" : 505 , "nbins" : 30} ,
#"PFMET"            : {"xaxis_label" : "pf MET [GeV]"                           , "xaxis_ndivisions" : 505 , "nbins" : 20} ,
"PuppiMET"         : {"xaxis_label" : "Puppi MET [GeV]"                        , "xaxis_ndivisions" : 505 , "nbins" : 45} , 
"MT2"              : {"xaxis_label" : "M_{T2} [GeV]"                           , "xaxis_ndivisions" : 505 , "nbins" : 30} ,
"Pt4l"		   : {"xaxis_label" : "p_{T,4l} [GeV]"			       , "xaxis_ndivisions" : 505 , "nbins" : 48} , 
"SRBin"            : {"xaxis_label" : "SR bins"                                , "xaxis_ndivisions" : 505 , "nbins" : 20} , 
"STLep"            : {"xaxis_label" : "#Sigmap_{T,lep,MET} [GeV]"              , "xaxis_ndivisions" : 505 , "nbins" : 20} , 
"STLepFit"         : {"xaxis_label" : "#Sigmap_{T,lep,MET} [GeV]"              , "xaxis_ndivisions" : 505 , "nbins" : 20} , 
"STLepHad"         : {"xaxis_label" : "#Sigmap_{T,lep,MET,jet} [GeV]"          , "xaxis_ndivisions" : 505 , "nbins" : 20} , 
"STLepHadFit"      : {"xaxis_label" : "#Sigmap_{T,lep,MET,jet} [GeV]"          , "xaxis_ndivisions" : 505 , "nbins" : 20} , 
"Yield"            : {"xaxis_label" : "Yield"                                  , "xaxis_ndivisions" : 505 , "nbins" : 20} , 
"Zcand_lep0_pt"    : {"xaxis_label" : "Z-candidate lead lepton p_{T} [GeV]"    , "xaxis_ndivisions" : 505 , "nbins" : 20} , 
"Zcand_lep1_pt"    : {"xaxis_label" : "Z-candidate sublead lepton p_{T} [GeV]" , "xaxis_ndivisions" : 505 , "nbins" : 20} , 
"Zcand_mll"        : {"xaxis_label" : "Z-candidate m_{ll} [GeV]"               , "xaxis_ndivisions" : 505 , "nbins" : 20} , 
"Zcand_mll_full"   : {"xaxis_label" : "Z-candidate m_{ll} [GeV]"               , "xaxis_ndivisions" : 505 , "nbins" : 20} , 
"nb"               : {"xaxis_label" : "# of b-tagged jets"                     , "xaxis_ndivisions" : 505 , "nbins" : 20} ,
"njets"            : {"xaxis_label" : "N_{jets}"                               , "xaxis_ndivisions" : 505 , "nbins" : 20} ,
"other_lep0_dxy"   : {"xaxis_label" : "W-candidate lead lepton d_{xy}"         , "xaxis_ndivisions" : 505 , "nbins" : 20} , 
"other_lep0_dz"    : {"xaxis_label" : "W-candidate lead lepton d_{z}"          , "xaxis_ndivisions" : 505 , "nbins" : 20} , 
"other_lep0_pt"    : {"xaxis_label" : "W-candidate lead lepton p_{T} [GeV]"    , "xaxis_ndivisions" : 505 , "nbins" : 20} , 
"other_lep0_sip3d" : {"xaxis_label" : "W-candidate lead lepton sip3d"          , "xaxis_ndivisions" : 505 , "nbins" : 20} , 
"other_lep1_dxy"   : {"xaxis_label" : "W-candidate sublead lepton d_{xy}"      , "xaxis_ndivisions" : 505 , "nbins" : 20} , 
"other_lep1_dz"    : {"xaxis_label" : "W-candidate sublead lepton d_{z}"       , "xaxis_ndivisions" : 505 , "nbins" : 20} , 
"other_lep1_pt"    : {"xaxis_label" : "W-candidate sublead lepton p_{T} [GeV]" , "xaxis_ndivisions" : 505 , "nbins" : 20} ,
"other_lep1_sip3d" : {"xaxis_label" : "W-candidate sublead lepton sip3d"       , "xaxis_ndivisions" : 505 , "nbins" : 20} , 
"other_lep_diffdz" : {"xaxis_label" : "W-candidate #Delta d_{z,ll}"            , "xaxis_ndivisions" : 505 , "nbins" : 20} , 
"other_mll"        : {"xaxis_label" : "W-candidate m_{ll} [GeV]"               , "xaxis_ndivisions" : 505 , "nbins" : 20} , 
"other_mll_full"   : {"xaxis_label" : "W-candidate m_{ll} [GeV]"               , "xaxis_ndivisions" : 505 , "nbins" : 40} , 
"other_mll_varbin" : {"xaxis_label" : "W-candidate m_{ll} [GeV]"               , "xaxis_ndivisions" : 505 , "nbins" : 20} ,
"MT2_PuppiMET"     : {"xaxis_label" : "M_{T2} [GeV]"                           , "xaxis_ndivisions" : 505 , "nbins" : 40} ,
"DR_WW_Z"          : {"xaxis_label" : "#Delta R(Wleps,Zleps)"                  , "xaxis_ndivisions" : 505 , "nbins" : 40} , 
#"DPhi_WW_MET"      : {"xaxis_label" : "#Delta#phi(WW,p_{T}^{miss})"            , "xaxis_ndivisions" : 505 , "nbins" : 40} ,
#"DPhi_Z_MET"       : {"xaxis_label" : "#Delta#phi(Z,p_{T}^{miss})"             , "xaxis_ndivisions" : 505 , "nbins" : 40} ,
"DPhi_WW_PuppiMET" : {"xaxis_label" : "#Delta#phi(WW,p_{T}^{miss})"            , "xaxis_ndivisions" : 505 , "nbins" : 40} ,
"DPhi_Z_PuppiMET"  : {"xaxis_label" : "#Delta#phi(Z,p_{T}^{miss})"             , "xaxis_ndivisions" : 505 , "nbins" : 40} ,
#"DPhi_WWZ_MET"     : {"xaxis_label" : "#Delta#phi(WWZ,p_{T}^{miss})"           , "xaxis_ndivisions" : 505 , "nbins" : 40} ,
"DPhi_WWZ_PuppiMET": {"xaxis_label" : "#Delta#phi(WWZ,p_{T}^{miss})"           , "xaxis_ndivisions" : 505 , "nbins" : 40} , 
"nb"               : {"xaxis_label" : "N_{btag} (loose DeepFlav)"              , "xaxis_ndivisions" : 505 , "nbins" : 40} ,
"leading_jet_btagDeepFlav" : {"xaxis_label" : "btagDeepFlav score (leading jet)" , "xaxis_ndivisions" : 505 , "nbins" : 40} ,
"njets"            : {"xaxis_label" : "N_{jets}"                               , "xaxis_ndivisions" : 505 , "nbins" : 40} ,
"leading_jet_pt"   : {"xaxis_label" : "p_{T}^{jet,leading} [GeV]"              , "xaxis_ndivisions" : 505 , "nbins" : 40} ,
"subleading_jet_pt": {"xaxis_label" : "p_{T}^{jet,subleading} [GeV]"           , "xaxis_ndivisions" : 505 , "nbins" : 40} ,
"HT"               : {"xaxis_label" : "H_{T} [GeV]"                            , "xaxis_ndivisions" : 505 , "nbins" : 40} ,
"m4l"              : {"xaxis_label" : "m_{4l} [GeV]"                           , "xaxis_ndivisions" : 505 , "nbins" : 40} , 
"MT_leading_Wcand" : {"xaxis_label" : "M_{T}^{W1} [GeV]"                       , "xaxis_ndivisions" : 505 , "nbins" : 40} ,
"MT_subleading_Wcand": {"xaxis_label" : "M_{T}^{W2} [GeV]"                     , "xaxis_ndivisions" : 505 , "nbins" : 40} ,
"MT_Wcands"        : {"xaxis_label" : "M_{T}^{Wcands} [GeV]"                   , "xaxis_ndivisions" : 505 , "nbins" : 40} ,
"MT_4lep"          : {"xaxis_label" : "M_{T}^{4lep} [GeV]"                     , "xaxis_ndivisions" : 505 , "nbins" : 40} ,
"DPhi_W1_PuppiMET" : {"xaxis_label" : "#Delta#phi(l^{W1},p_{T}^{miss})"        , "xaxis_ndivisions" : 505 , "nbins" : 40} ,
"DPhi_W2_PuppiMET" : {"xaxis_label" : "#Delta#phi(l^{W2},p_{T}^{miss})"        , "xaxis_ndivisions" : 505 , "nbins" : 40} ,
"min_DR_W1_jet"    : {"xaxis_label" : "min(#Delta R(l^{W1},jet))"              , "xaxis_ndivisions" : 505 , "nbins" : 40} ,
"min_DR_W2_jet"    : {"xaxis_label" : "min(#Delta R(l^{W2},jet))"              , "xaxis_ndivisions" : 505 , "nbins" : 40} ,
"cosHelicityX"     : {"xaxis_label" : "cos(#theta_{X})"                        , "xaxis_ndivisions" : 505 , "nbins" : 40} ,
}

def plot(year, filterpattern):

    if year == "Run2": lumi = 137
    if year == "2018": lumi = 59.8
    if year == "2017": lumi = 41.5
    if year == "2016": lumi = 16.8
    if year == "2016APV": lumi = 19.5
    
    p.dump_plot(
            fnames=[
                "{1}/{0}/ZZ.root".format(year, analysis_output),
                "{1}/{0}/TTZ.root".format(year, analysis_output),
                #"{1}/{0}/Higgs.root".format(year, analysis_output),
		"{1}/{0}/tWZ.root".format(year, analysis_output),
                "{1}/{0}/WZ.root".format(year, analysis_output),
		"{1}/{0}/VVV.root".format(year, analysis_output),
                "{1}/{0}/Other.root".format(year, analysis_output),
                #"{1}/{0}/DYM10.root".format(year, analysis_output),         	        
                #"{1}/{0}/DYM50.root".format(year, analysis_output),     
		#"{1}/{0}/GGZZ2e2m.root".format(year, analysis_output),
		#"{1}/{0}/GGZZ2e2t.root".format(year, analysis_output),
		#"{1}/{0}/GGZZ2m2t.root".format(year, analysis_output),
		#"{1}/{0}/GGZZ4e.root".format(year, analysis_output),
		#"{1}/{0}/GGZZ4m.root".format(year, analysis_output),
		#"{1}/{0}/GGZZ4t.root".format(year, analysis_output),
		#"{1}/{0}/SSWW.root".format(year, analysis_output),
		#"{1}/{0}/ST_antitop_nohad_tW.root".format(year, analysis_output),
		#"{1}/{0}/ST_antitop_tchan.root".format(year, analysis_output),
		#"{1}/{0}/ST_schan_lep.root".format(year, analysis_output),
		#"{1}/{0}/ST_top_nohad_tW.root".format(year, analysis_output),
		#"{1}/{0}/ST_top_tchan.root".format(year, analysis_output),
		#"{1}/{0}/TT1l.root".format(year, analysis_output),
	        #"{1}/{0}/TT2l.root".format(year, analysis_output),
                #"{1}/{0}/TTWlv.root".format(year, analysis_output),
                #"{1}/{0}/TTWqq.root".format(year, analysis_output),
		#"{1}/{0}/TTZll_M1.root".format(year, analysis_output),
		#"{1}/{0}/TTZll_M10.root".format(year, analysis_output),
		#"{1}/{0}/TTZqq.root".format(year, analysis_output),
		#"{1}/{0}/VHToNonbb.root".format(year, analysis_output),
		#"{1}/{0}/WWlvlv.root".format(year, analysis_output),
		#"{1}/{0}/WZ.root".format(year, analysis_output),
		#"{1}/{0}/Wlv.root".format(year, analysis_output),
		#"{1}/{0}/ZZ2l2q.root".format(year, analysis_output),
		#"{1}/{0}/ZZ2l2v.root".format(year, analysis_output),
		#"{1}/{0}/ZZ4l.root".format(year, analysis_output),
		#"{1}/{0}/tZq.root".format(year, analysis_output),
		#"{1}/{0}/ttHNonbb.root".format(year, analysis_output),
                ],
            sig_fnames=[
                #"{1}/{0}/WWZ.root".format(year, analysis_output),
                "{1}/{0}/NonResWWZ.root".format(year, analysis_output),
                "{1}/{0}/ZHWWZ.root".format(year, analysis_output),
		#"{1}/{0}/GGZHWW_WW2l.root".format(year, analysis_output),
		#"{1}/{0}/WWZ_4l.root".format(year, analysis_output),
		#"{1}/{0}/ZHWW_4l.root".format(year, analysis_output),
                ],
            #data_fname="outputs/{0}/data.root".format(year),
            dirname="../../../public_html/{1}/{0}".format(year, plot_output_dir),
            legend_labels=[
                "ZZ",
                "t#bar{t}Z",
		"tWZ",
                #"Higgs",
                "WZ",
		"VVV",
                "Other",
		#"DYM10",
                #"DYM50",   
                #"GGZZ2e2m",
                #"GGZZ2e2t",
                #"GGZZ2m2t",
                #"GGZZ4e",
                #"GGZZ4m",
                #"GGZZ4t",
                #"SSWW",
                #"ST_antitop_nohad_tW",
                #"ST_antitop_tchan",
                #"ST_schan_lep",
                #"ST_top_nohad_tW",
                #"ST_top_tchan",
                #"TT1l",
                #"TT2l",
                #"TTWlv",
                #"TTWqq",
                #"TTZll_M1",
                #"TTZll_M10",
                #"TTZqq",
                #"VHToNonbb",
                #"WWlvlv",
                #"WZ",
                #"Wlv",
                #"ZZ2l2q",
                #"ZZ2l2v",
                #"ZZ4l",
                #"tZq",
                #"ttHNonbb",
                ],
            signal_labels=[
                #"WWZ",
                "NonResWWZ",
                "ZHWWZ",
		#"GGZHWW_WW2l",
		#"WWZ_4l",
		#"ZHWW_4l",
                ],
            usercolors=[
                4020,
                4305,
                4024,
                7013,
                920,
                2005,
                2,
		880,
		#860,
		#820,
		#800,
		#1,
		#632,
                ],
            filter_pattern=filterpattern,
            dogrep=True,
            extraoptions={
                "print_yield": True,
                "lumi_value": lumi,
                "nbins": 30,
		#"nbins": 45,
                #"yaxis_log": True,
		"yaxis_log": False,
                "ratio_range": [0., 2.],
		#"xaxis_range": [0., 300.],
		#"signal_scale":"auto",
                #"signal_scale":50.,
                "legend_scalex": 2.0,
                "legend_ncolumns": 3,
                "yield_prec": 3,
                },
            histxaxislabeloptions=histxaxislabeloptions,
            # _plotter=p.plot_cut_scan,
            )

if __name__ == "__main__":

    filterpatterns = [
        #"Yield",
	#"CutOffZ_trgMatch__SRBin", # this is the SF signal region
	#"CutEMuMT2_trgMatch__SRBin", # this is the OF signal region
	#"CutOffZ_trgMatch__SRBin",
	#"CutOffZ__SRBin",
	#"CutEMuMT2_trgMatch__SRBin",
	#"CutEMu",
	#"CutOffZ_trgMatch__SRBin",
        #"CutOffZ_trgMatch__SRBin",
	#"CutOnZ",
	#"CutEMuBT",
	#"CutPresel__nb",
	#"CutPresel__njets",
	#"CutPresel__MT2_PuppiMET",
	#"CutBVeto__MT2_PuppiMET",
	#"CutBTag__MT2_PuppiMET",
	#"CutEMu__other_mll_full",
	#"CutOffZ__PuppiMET",
	#"CutOffZ__Pt4l",
       	#"CutBVeto__SRBin",
	#"CutEMu__Yield",
	#"CutOffZTR__Yield",
	"CutOffZ__SRBin",
	"CutEMuMT2__SRBin",
	"CutBVeto__SRBin",
	#"CutOffZTR__",
	#"CutEMu__cos",
	#"CutPresel",
	#"CutOffZ__cos",
	#"CutOnZ",
	#"CutOffZ_trgMatch",
	#"CutEMuMT2_trgMatch",
	#"CutOnZ",
	#"CutEMuBT",
	#"CutEMu__",
        ]

    years = [
        "2016",
        "2016APV",
        "2017",
        "2018",
        "Run2",
        ]

    for year in years:
        for filterpattern in filterpatterns:
            plot(year, filterpattern)

