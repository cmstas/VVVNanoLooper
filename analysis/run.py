#!/bin/env python

import os

#skimversion = "v9"
#skimdir = "/home/users/phchang/work/vvv/WWZRun3NanoLooper/vvvtree/{skimversion}".format(skimversion=skimversion)

skimversion = "WWZ_4L"
skimdir = "/home/users/kdownham/Triboson/VVVNanoLooper/vvvtree/110422" 

#____________________________________________________________________________________________
def main():

    # Create mappign between "process group" -> "list of ROOT files"
    files = get_groupping()

    # Configuration of how to split the jobs for ZZ and WWZ
    ZZ_njobs = 9

    # Clear the text file that holds the command lines
    jobs_command_file = open(".jobs.txt", "w")

    # Print the command line jobs for entire Run 2
    for proc in proc_groups:
        inputs_allyear = []
        for year in years:
            inputs = ",".join(files[year][proc])
            inputs_allyear += files[year][proc]
            #------------------------------------------
            if proc == "ZZ":
                for i in xrange(ZZ_njobs):
                    jobs_command_file.write(get_command(proc, inputs, year, ZZ_njobs, i) + "\n")
            else:
                jobs_command_file.write(get_command(proc, inputs, year) + "\n")
        #------------------------------------------
        inputs_allyear = ",".join(inputs_allyear)
        if proc == "ZZ":
            for i in xrange(ZZ_njobs):
                jobs_command_file.write(get_command(proc, inputs_allyear, "Run2", ZZ_njobs, i) + "\n")
        else:
            jobs_command_file.write(get_command(proc, inputs_allyear, "Run2") + "\n")

    # Do for the data
    jobs_command_file.write(get_command("data", "\\\"{skimdir}/*Run2016*HIPM*.root\\\"".format(skimdir=skimdir), "2016APV") + "\n")
    jobs_command_file.write(get_command("data", "\\\"{skimdir}/*Run2016*-UL2016*.root\\\"".format(skimdir=skimdir), "2016") + "\n")
    jobs_command_file.write(get_command("data", "\\\"{skimdir}/*Run2017*.root\\\"".format(skimdir=skimdir), "2017") + "\n")
    jobs_command_file.write(get_command("data", "\\\"{skimdir}/*Run2018*.root\\\"".format(skimdir=skimdir), "2018") + "\n")
    jobs_command_file.write(get_command("data", "\\\"{skimdir}/*Run201*.root\\\"".format(skimdir=skimdir), "Run2") + "\n")

    jobs_command_file.close()

    # Run the jobs!
    os.system("xargs.sh .jobs.txt")

    # Hadd ZZ and WWZ's output histograms that were ran in chunks
    print("")
    print("Hadding ZZ and WWZ output rootfiles ......")
    for year in years + ["Run2"]:
        os.system("hadd -f outputs/{0}/ZZ.root outputs/{0}/ZZ_*.root > outputs/{0}/ZZ.log 2>&1".format(year))
        os.system("hadd -f outputs/{0}/WWZ.root outputs/{0}/ZHWWZ.root outputs/{0}/NonResWWZ.root > outputs/{0}/WWZ.log 2>&1".format(year))
        os.system("hadd -f outputs/{0}/NonWWZ.root outputs/{0}/WWW.root outputs/{0}/WZZ.root outputs/{0}/ZZZ.root > outputs/{0}/NonWWZ.log 2>&1".format(year))

    print("Done!")


#____________________________________________________________________________________________
# Get Command
def get_command(proc, inputs, year, njobs=0, idx=0):
    if njobs > 0:
        rtn_str = "rm -f outputs/{}/{}_{}.root;".format(year, proc, idx)
    else:
        rtn_str = "rm -f outputs/{}/{}.root;".format(year, proc)
    rtn_str += "mkdir -p outputs/{};".format(year)
    rtn_str += "./doAnalysis -i {} ".format(inputs)
    if njobs > 0:
        rtn_str += "-j {} -I {} ".format(njobs, idx)
        rtn_str += "-t t -o outputs/{}/{}_{}.root > outputs/{}/{}_{}.log 2>&1".format(year, proc, idx, year, proc, idx)
    else:
        rtn_str += "-t t -o outputs/{}/{}.root > outputs/{}/{}.log 2>&1".format(year, proc, year, proc)
    return rtn_str

#____________________________________________________________________________________________
# Create mappign between "process group" -> "list of ROOT files"
def get_groupping():
    files = {}
    for year in years:
        files[year] = {}
        for proc_group in proc_groups:
            files[year][proc_group] = []

    # Loop over processes and put the corresponding root files in to proc_group lists
    for proc in process.keys():
        for year in years:
            key = proc+"_"+year
            if "WWZ_4l_2016APV" == key: continue
            if "WWZ_2016" == key: continue
            if "WWZ_2017" == key: continue
            if "WWZ_2018" == key: continue
            # add to the list in the grouping
            files[year][process[proc]].append(mc[key].format(skimdir=skimdir, skimversion=skimversion))

    return files

#____________________________________________________________________________________________
# Different eras defined
years=["2016APV", "2016", "2017", "2018"]

#____________________________________________________________________________________________
# List of different process groups
proc_groups = [ "WWW", "NonResWWZ", "ZHWWZ", "WZZ", "ZZZ", "Higgs", "ZZ", "TTZ", "WZ", "Other", ]

#____________________________________________________________________________________________
# Process grouping definition
process={
"DYM10"               : "Other",
"DYM50"               : "Other",
"GGZZ2e2m"            : "ZZ",
"GGZZ2e2t"            : "ZZ",
"GGZZ2m2t"            : "ZZ",
"GGZZ4e"              : "ZZ",
"GGZZ4m"              : "ZZ",
"GGZZ4t"              : "ZZ",
"GGZHWW_WW2l"         : "ZHWWZ",
"ZHWW_4l"             : "ZHWWZ",
"SSWW"                : "Other",
"ST_schan_lep"        : "Other",
"ST_antitop_tchan"    : "Other",
"ST_top_tchan"        : "Other",
"ST_antitop_nohad_tW" : "Other",
"ST_top_nohad_tW"     : "Other",
"TT2l"                : "Other",
"TThad"               : "Other",
"TT1l"                : "Other",
"TTWlv"               : "Other",
"TTWqq"               : "Other",
"TTZll_M10"           : "TTZ",
"TTZll_M1"            : "TTZ",
"TTZqq"               : "TTZ",
"Wlv"                 : "Other",
"WWlvlv"              : "Other",
"WWW_2l"              : "WWW",
"WWZ_4l"              : "NonResWWZ",
"WWZ"                 : "NonResWWZ",
"WZ2q2l"              : "WZ",
"WZ3l1v"              : "WZ",
"WZZ"                 : "WZZ",
"ZZ2l2v"              : "ZZ",
"ZZ2l2q"              : "ZZ",
"ZZ4l"                : "ZZ",
"ZZZ"                 : "ZZZ",
"tZq"                 : "Other",
"ttHNonbb"            : "Higgs",
"ttHbb"               : "Higgs",
"VHToNonbb"           : "Higgs",
}

#____________________________________________________________________________________________
# Data root files and their short names
#"ee_Bv2_2016APV" : "{skimdir}/DoubleEG_Run2016B-ver2_HIPM_UL2016_MiniAODv2_NanoAODv9-v3_NANOAOD_{skimversion}.root",
#"ee_C_2016APV" : "{skimdir}/DoubleEG_Run2016C-HIPM_UL2016_MiniAODv2_NanoAODv9-v2_NANOAOD_{skimversion}.root",
#"ee_D_2016APV" : "{skimdir}/DoubleEG_Run2016D-HIPM_UL2016_MiniAODv2_NanoAODv9-v2_NANOAOD_{skimversion}.root",
#"ee_E_2016APV" : "{skimdir}/DoubleEG_Run2016E-HIPM_UL2016_MiniAODv2_NanoAODv9-v2_NANOAOD_{skimversion}.root",
#"ee_F_2016APV" : "{skimdir}/DoubleEG_Run2016F-HIPM_UL2016_MiniAODv2_NanoAODv9-v2_NANOAOD_{skimversion}.root",
#"ee_F_2016" : "{skimdir}/DoubleEG_Run2016F-UL2016_MiniAODv2_NanoAODv9-v1_NANOAOD_{skimversion}.root",
#"ee_G_2016" : "{skimdir}/DoubleEG_Run2016G-UL2016_MiniAODv2_NanoAODv9-v1_NANOAOD_{skimversion}.root",
#"ee_H_2016" : "{skimdir}/DoubleEG_Run2016H-UL2016_MiniAODv2_NanoAODv9-v1_NANOAOD_{skimversion}.root",
#"em_Bv1_2016APV" : "{skimdir}/MuonEG_Run2016B-ver1_HIPM_UL2016_MiniAODv2_NanoAODv9-v2_NANOAOD_{skimversion}.root",
#"em_Bv2_2016APV" : "{skimdir}/MuonEG_Run2016B-ver2_HIPM_UL2016_MiniAODv2_NanoAODv9-v2_NANOAOD_{skimversion}.root",
#"em_C_2016APV" : "{skimdir}/MuonEG_Run2016C-HIPM_UL2016_MiniAODv2_NanoAODv9-v2_NANOAOD_{skimversion}.root",
#"em_D_2016APV" : "{skimdir}/MuonEG_Run2016D-HIPM_UL2016_MiniAODv2_NanoAODv9-v2_NANOAOD_{skimversion}.root",
#"em_E_2016APV" : "{skimdir}/MuonEG_Run2016E-HIPM_UL2016_MiniAODv2_NanoAODv9-v2_NANOAOD_{skimversion}.root",
#"em_F_2016APV" : "{skimdir}/MuonEG_Run2016F-HIPM_UL2016_MiniAODv2_NanoAODv9-v2_NANOAOD_{skimversion}.root",
#"em_F_2016" : "{skimdir}/MuonEG_Run2016F-UL2016_MiniAODv2_NanoAODv9-v1_NANOAOD_{skimversion}.root",
#"em_G_2016" : "{skimdir}/MuonEG_Run2016G-UL2016_MiniAODv2_NanoAODv9-v1_NANOAOD_{skimversion}.root",
#"em_H_2016" : "{skimdir}/MuonEG_Run2016H-UL2016_MiniAODv2_NanoAODv9-v1_NANOAOD_{skimversion}.root",
#"mm_Bv1_2016APV" : "{skimdir}/DoubleMuon_Run2016B-ver1_HIPM_UL2016_MiniAODv2_NanoAODv9-v2_NANOAOD_{skimversion}.root",
#"mm_Bv2_2016APV" : "{skimdir}/DoubleMuon_Run2016B-ver2_HIPM_UL2016_MiniAODv2_NanoAODv9-v2_NANOAOD_{skimversion}.root",
#"mm_C_2016APV" : "{skimdir}/DoubleMuon_Run2016C-HIPM_UL2016_MiniAODv2_NanoAODv9-v2_NANOAOD_{skimversion}.root",
#"mm_D_2016APV" : "{skimdir}/DoubleMuon_Run2016D-HIPM_UL2016_MiniAODv2_NanoAODv9-v2_NANOAOD_{skimversion}.root",
#"mm_E_2016APV" : "{skimdir}/DoubleMuon_Run2016E-HIPM_UL2016_MiniAODv2_NanoAODv9-v2_NANOAOD_{skimversion}.root",
#"mm_F_2016APV" : "{skimdir}/DoubleMuon_Run2016F-HIPM_UL2016_MiniAODv2_NanoAODv9-v2_NANOAOD_{skimversion}.root",
#"mm_F_2016" : "{skimdir}/DoubleMuon_Run2016F-UL2016_MiniAODv2_NanoAODv9-v1_NANOAOD_{skimversion}.root",
#"mm_G_2016" : "{skimdir}/DoubleMuon_Run2016G-UL2016_MiniAODv2_NanoAODv9-v2_NANOAOD_{skimversion}.root",
#"mm_H_2016" : "{skimdir}/DoubleMuon_Run2016H-UL2016_MiniAODv2_NanoAODv9-v1_NANOAOD_{skimversion}.root",
#"ee_B_2017" : "{skimdir}/DoubleEG_Run2017B-UL2017_MiniAODv2_NanoAODv9-v1_NANOAOD_{skimversion}.root",
#"ee_C_2017" : "{skimdir}/DoubleEG_Run2017C-UL2017_MiniAODv2_NanoAODv9-v1_NANOAOD_{skimversion}.root",
#"ee_D_2017" : "{skimdir}/DoubleEG_Run2017D-UL2017_MiniAODv2_NanoAODv9-v1_NANOAOD_{skimversion}.root",
#"ee_E_2017" : "{skimdir}/DoubleEG_Run2017E-UL2017_MiniAODv2_NanoAODv9-v1_NANOAOD_{skimversion}.root",
#"ee_F_2017" : "{skimdir}/DoubleEG_Run2017F-UL2017_MiniAODv2_NanoAODv9-v1_NANOAOD_{skimversion}.root",
#"mm_B_2017" : "{skimdir}/DoubleMuon_Run2017B-UL2017_MiniAODv2_NanoAODv9-v1_NANOAOD_{skimversion}.root",
#"mm_C_2017" : "{skimdir}/DoubleMuon_Run2017C-UL2017_MiniAODv2_NanoAODv9-v1_NANOAOD_{skimversion}.root",
#"mm_D_2017" : "{skimdir}/DoubleMuon_Run2017D-UL2017_MiniAODv2_NanoAODv9-v1_NANOAOD_{skimversion}.root",
#"mm_E_2017" : "{skimdir}/DoubleMuon_Run2017E-UL2017_MiniAODv2_NanoAODv9-v1_NANOAOD_{skimversion}.root",
#"mm_F_2017" : "{skimdir}/DoubleMuon_Run2017F-UL2017_MiniAODv2_NanoAODv9-v1_NANOAOD_{skimversion}.root",
#"em_B_2017" : "{skimdir}/MuonEG_Run2017B-UL2017_MiniAODv2_NanoAODv9-v1_NANOAOD_{skimversion}.root",
#"em_C_2017" : "{skimdir}/MuonEG_Run2017C-UL2017_MiniAODv2_NanoAODv9-v1_NANOAOD_{skimversion}.root",
#"em_D_2017" : "{skimdir}/MuonEG_Run2017D-UL2017_MiniAODv2_NanoAODv9-v1_NANOAOD_{skimversion}.root",
#"em_E_2017" : "{skimdir}/MuonEG_Run2017E-UL2017_MiniAODv2_NanoAODv9-v1_NANOAOD_{skimversion}.root",
#"em_F_2017" : "{skimdir}/MuonEG_Run2017F-UL2017_MiniAODv2_NanoAODv9-v1_NANOAOD_{skimversion}.root",
#"eg_A_2018" : "{skimdir}/EGamma_Run2018A-UL2018_MiniAODv2_NanoAODv9-v1_NANOAOD_{skimversion}.root",
#"eg_B_2018" : "{skimdir}/EGamma_Run2018B-UL2018_MiniAODv2_NanoAODv9-v1_NANOAOD_{skimversion}.root",
#"eg_C_2018" : "{skimdir}/EGamma_Run2018C-UL2018_MiniAODv2_NanoAODv9-v1_NANOAOD_{skimversion}.root",
#"eg_D_2018" : "{skimdir}/EGamma_Run2018D-UL2018_MiniAODv2_NanoAODv9-v3_NANOAOD_{skimversion}.root",
#"em_A_2018" : "{skimdir}/MuonEG_Run2018A-UL2018_MiniAODv2_NanoAODv9-v1_NANOAOD_{skimversion}.root",
#"em_B_2018" : "{skimdir}/MuonEG_Run2018B-UL2018_MiniAODv2_NanoAODv9-v1_NANOAOD_{skimversion}.root",
#"em_C_2018" : "{skimdir}/MuonEG_Run2018C-UL2018_MiniAODv2_NanoAODv9-v1_NANOAOD_{skimversion}.root",
#"em_D_2018" : "{skimdir}/MuonEG_Run2018D-UL2018_MiniAODv2_NanoAODv9-v1_NANOAOD_{skimversion}.root",
#"mm_A_2018" : "{skimdir}/DoubleMuon_Run2018A-UL2018_MiniAODv2_NanoAODv9-v1_NANOAOD_{skimversion}.root",
#"mm_B_2018" : "{skimdir}/DoubleMuon_Run2018B-UL2018_MiniAODv2_NanoAODv9-v1_NANOAOD_{skimversion}.root",
#"mm_C_2018" : "{skimdir}/DoubleMuon_Run2018C-UL2018_MiniAODv2_NanoAODv9-v1_NANOAOD_{skimversion}.root",
#"mm_D_2018" : "{skimdir}/DoubleMuon_Run2018D-UL2018_MiniAODv2_NanoAODv9-v2_NANOAOD_{skimversion}.root",
}

#____________________________________________________________________________________________
# MC root files and their short names
mc={
#"DYM10_2016APV" : "{skimdir}/DYJetsToLL_M-10to50_TuneCP5_13TeV-madgraphMLM-pythia8_RunIISummer20UL16NanoAODAPVv9-106X_mcRun2_asymptotic_preVFP_v11-v1_NANOAODSIM_{skimversion}.root",
#"DYM10_2016"    : "{skimdir}/DYJetsToLL_M-10to50_TuneCP5_13TeV-madgraphMLM-pythia8_RunIISummer20UL16NanoAODv9-106X_mcRun2_asymptotic_v17-v1_NANOAODSIM_{skimversion}.root",
#"DYM10_2017"    : "{skimdir}/DYJetsToLL_M-10to50_TuneCP5_13TeV-madgraphMLM-pythia8_RunIISummer20UL17NanoAODv9-106X_mc2017_realistic_v9-v1_NANOAODSIM_{skimversion}.root",
#"DYM10_2018"    : "{skimdir}/DYJetsToLL_M-10to50_TuneCP5_13TeV-madgraphMLM-pythia8_RunIISummer20UL18NanoAODv9-106X_upgrade2018_realistic_v16_L1v1-v1_NANOAODSIM_{skimversion}.root",
#"DYM50_2016APV" : "{skimdir}/DYJetsToLL_M-50_TuneCP5_13TeV-madgraphMLM-pythia8_RunIISummer20UL16NanoAODAPVv9-106X_mcRun2_asymptotic_preVFP_v11-v1_NANOAODSIM_{skimversion}.root",
#"DYM50_2016"    : "{skimdir}/DYJetsToLL_M-50_TuneCP5_13TeV-madgraphMLM-pythia8_RunIISummer20UL16NanoAODv9-106X_mcRun2_asymptotic_v17-v1_NANOAODSIM_{skimversion}.root",
#"DYM50_2017"    : "{skimdir}/DYJetsToLL_M-50_TuneCP5_13TeV-madgraphMLM-pythia8_RunIISummer20UL17NanoAODv9-106X_mc2017_realistic_v9-v1_NANOAODSIM_{skimversion}.root",
#"DYM50_2018"    : "{skimdir}/DYJetsToLL_M-50_TuneCP5_13TeV-madgraphMLM-pythia8_RunIISummer20UL18NanoAODv9-106X_upgrade2018_realistic_v16_L1v1-v1_NANOAODSIM_{skimversion}.root",
"GGZZ2e2m_2016APV" : "{skimdir}/GluGluToContinToZZTo2e2mu_TuneCP5_13TeV-mcfm701-pythia8_RunIISummer20UL16NanoAODAPVv9-106X_mcRun2_asymptotic_preVFP_v11-v2_NANOAODSIM_{skimversion}.root",
"GGZZ2e2m_2016" : "{skimdir}/GluGluToContinToZZTo2e2mu_TuneCP5_13TeV-mcfm701-pythia8_RunIISummer20UL16NanoAODv9-106X_mcRun2_asymptotic_v17-v1_NANOAODSIM_{skimversion}.root",
"GGZZ2e2m_2017" : "{skimdir}/GluGluToContinToZZTo2e2mu_TuneCP5_13TeV-mcfm701-pythia8_RunIISummer20UL17NanoAODv9-106X_mc2017_realistic_v9-v2_NANOAODSIM_{skimversion}.root",
"GGZZ2e2m_2018" : "{skimdir}/GluGluToContinToZZTo2e2mu_TuneCP5_13TeV-mcfm701-pythia8_RunIISummer20UL18NanoAODv9-106X_upgrade2018_realistic_v16_L1v1-v2_NANOAODSIM_{skimversion}.root",
"GGZZ2e2t_2016APV" : "{skimdir}/GluGluToContinToZZTo2e2tau_TuneCP5_13TeV-mcfm701-pythia8_RunIISummer20UL16NanoAODAPVv9-106X_mcRun2_asymptotic_preVFP_v11-v2_NANOAODSIM_{skimversion}.root",
"GGZZ2e2t_2016" : "{skimdir}/GluGluToContinToZZTo2e2tau_TuneCP5_13TeV-mcfm701-pythia8_RunIISummer20UL16NanoAODv9-106X_mcRun2_asymptotic_v17-v1_NANOAODSIM_{skimversion}.root",
"GGZZ2e2t_2017" : "{skimdir}/GluGluToContinToZZTo2e2tau_TuneCP5_13TeV-mcfm701-pythia8_RunIISummer20UL17NanoAODv9-106X_mc2017_realistic_v9-v2_NANOAODSIM_{skimversion}.root",
"GGZZ2e2t_2018" : "{skimdir}/GluGluToContinToZZTo2e2tau_TuneCP5_13TeV-mcfm701-pythia8_RunIISummer20UL18NanoAODv9-106X_upgrade2018_realistic_v16_L1v1-v2_NANOAODSIM_{skimversion}.root",
"GGZZ2m2t_2016APV" : "{skimdir}/GluGluToContinToZZTo2mu2tau_TuneCP5_13TeV-mcfm701-pythia8_RunIISummer20UL16NanoAODAPVv9-106X_mcRun2_asymptotic_preVFP_v11-v2_NANOAODSIM_{skimversion}.root",
"GGZZ2m2t_2016" : "{skimdir}/GluGluToContinToZZTo2mu2tau_TuneCP5_13TeV-mcfm701-pythia8_RunIISummer20UL16NanoAODv9-106X_mcRun2_asymptotic_v17-v1_NANOAODSIM_{skimversion}.root",
"GGZZ2m2t_2017" : "{skimdir}/GluGluToContinToZZTo2mu2tau_TuneCP5_13TeV-mcfm701-pythia8_RunIISummer20UL17NanoAODv9-106X_mc2017_realistic_v9-v2_NANOAODSIM_{skimversion}.root",
"GGZZ2m2t_2018" : "{skimdir}/GluGluToContinToZZTo2mu2tau_TuneCP5_13TeV-mcfm701-pythia8_RunIISummer20UL18NanoAODv9-106X_upgrade2018_realistic_v16_L1v1-v2_NANOAODSIM_{skimversion}.root",
"GGZZ4e_2016APV" : "{skimdir}/GluGluToContinToZZTo4e_TuneCP5_13TeV-mcfm701-pythia8_RunIISummer20UL16NanoAODAPVv9-106X_mcRun2_asymptotic_preVFP_v11-v2_NANOAODSIM_{skimversion}.root",
"GGZZ4e_2016" : "{skimdir}/GluGluToContinToZZTo4e_TuneCP5_13TeV-mcfm701-pythia8_RunIISummer20UL16NanoAODv9-106X_mcRun2_asymptotic_v17-v2_NANOAODSIM_{skimversion}.root",
"GGZZ4e_2017" : "{skimdir}/GluGluToContinToZZTo4e_TuneCP5_13TeV-mcfm701-pythia8_RunIISummer20UL17NanoAODv9-106X_mc2017_realistic_v9-v2_NANOAODSIM_{skimversion}.root",
"GGZZ4e_2018" : "{skimdir}/GluGluToContinToZZTo4e_TuneCP5_13TeV-mcfm701-pythia8_RunIISummer20UL18NanoAODv9-106X_upgrade2018_realistic_v16_L1v1-v2_NANOAODSIM_{skimversion}.root",
"GGZZ4m_2016APV" : "{skimdir}/GluGluToContinToZZTo4mu_TuneCP5_13TeV-mcfm701-pythia8_RunIISummer20UL18NanoAODv9-106X_upgrade2018_realistic_v16_L1v1-v2_NANOAODSIM_{skimversion}.root",
"GGZZ4m_2016" : "{skimdir}/GluGluToContinToZZTo4mu_TuneCP5_13TeV-mcfm701-pythia8_RunIISummer20UL16NanoAODv9-106X_mcRun2_asymptotic_v17-v2_NANOAODSIM_{skimversion}.root",
"GGZZ4m_2017" : "{skimdir}/GluGluToContinToZZTo4mu_TuneCP5_13TeV-mcfm701-pythia8_RunIISummer20UL17NanoAODv9-106X_mc2017_realistic_v9-v2_NANOAODSIM_{skimversion}.root",
"GGZZ4m_2018" : "{skimdir}/GluGluToContinToZZTo4mu_TuneCP5_13TeV-mcfm701-pythia8_RunIISummer20UL18NanoAODv9-106X_upgrade2018_realistic_v16_L1v1-v2_NANOAODSIM_{skimversion}.root",
"GGZZ4t_2016APV" : "{skimdir}/GluGluToContinToZZTo4tau_TuneCP5_13TeV-mcfm701-pythia8_RunIISummer20UL16NanoAODAPVv9-106X_mcRun2_asymptotic_preVFP_v11-v2_NANOAODSIM_{skimversion}.root",
"GGZZ4t_2016" : "{skimdir}/GluGluToContinToZZTo4tau_TuneCP5_13TeV-mcfm701-pythia8_RunIISummer20UL16NanoAODv9-106X_mcRun2_asymptotic_v17-v1_NANOAODSIM_{skimversion}.root",
"GGZZ4t_2017" : "{skimdir}/GluGluToContinToZZTo4tau_TuneCP5_13TeV-mcfm701-pythia8_RunIISummer20UL17NanoAODv9-106X_mc2017_realistic_v9-v2_NANOAODSIM_{skimversion}.root",
"GGZZ4t_2018" : "{skimdir}/GluGluToContinToZZTo4tau_TuneCP5_13TeV-mcfm701-pythia8_RunIISummer20UL18NanoAODv9-106X_upgrade2018_realistic_v16_L1v1-v2_NANOAODSIM_{skimversion}.root",
"GGZHWW_WW2l_2016APV": "{skimdir}/GluGluZH_HToWWTo2L2Nu_M125_13TeV_powheg_pythia8_TuneCP5_PSweights_RunIISummer20UL16NanoAODAPVv9-106X_mcRun2_asymptotic_preVFP_v11-v2_NANOAODSIM_{skimversion}.root",
"GGZHWW_WW2l_2016": "{skimdir}/GluGluZH_HToWWTo2L2Nu_M125_13TeV_powheg_pythia8_TuneCP5_PSweights_RunIISummer20UL16NanoAODv9-106X_mcRun2_asymptotic_v17-v2_NANOAODSIM_{skimversion}.root",
"GGZHWW_WW2l_2017" : "{skimdir}/GluGluZH_HToWWTo2L2Nu_M125_13TeV_powheg_pythia8_TuneCP5_PSweights_RunIISummer20UL17NanoAODv9-106X_mc2017_realistic_v9-v2_NANOAODSIM_{skimversion}.root",
"GGZHWW_WW2l_2018" : "{skimdir}/GluGluZH_HToWWTo2L2Nu_M125_13TeV_powheg_pythia8_TuneCP5_PSweights_RunIISummer20UL18NanoAODv9-106X_upgrade2018_realistic_v16_L1v1-v2_NANOAODSIM_{skimversion}.root",
"ZHWW_4l_2016APV" : "{skimdir}/HZJ_HToWWTo2L2Nu_ZTo2L_M-125_TuneCP5_13TeV-powheg-jhugen727-pythia8_RunIISummer20UL16NanoAODAPVv9-106X_mcRun2_asymptotic_preVFP_v11-v2_NANOAODSIM_{skimversion}.root",
"ZHWW_4l_2016" : "{skimdir}/HZJ_HToWWTo2L2Nu_ZTo2L_M-125_TuneCP5_13TeV-powheg-jhugen727-pythia8_RunIISummer20UL16NanoAODv9-106X_mcRun2_asymptotic_v17-v2_NANOAODSIM_{skimversion}.root",
"ZHWW_4l_2017" : "{skimdir}/HZJ_HToWWTo2L2Nu_ZTo2L_M-125_TuneCP5_13TeV-powheg-jhugen727-pythia8_RunIISummer20UL17NanoAODv9-106X_mc2017_realistic_v9-v2_NANOAODSIM_{skimversion}.root",
"ZHWW_4l_2018" : "{skimdir}/HZJ_HToWWTo2L2Nu_ZTo2L_M-125_TuneCP5_13TeV-powheg-jhugen727-pythia8_RunIISummer20UL18NanoAODv9-106X_upgrade2018_realistic_v16_L1v1-v2_NANOAODSIM_{skimversion}.root",
#"SSWW_2016APV" : "{skimdir}/SSWW_TuneCP5_13TeV-madgraph-pythia8_RunIISummer20UL16NanoAODAPVv9-106X_mcRun2_asymptotic_preVFP_v11-v1_NANOAODSIM_{skimversion}.root",
#"SSWW_2016" : "{skimdir}/SSWW_TuneCP5_13TeV-madgraph-pythia8_RunIISummer20UL16NanoAODv9-106X_mcRun2_asymptotic_v17-v1_NANOAODSIM_{skimversion}.root",
#"SSWW_2017" : "{skimdir}/SSWW_TuneCP5_13TeV-madgraph-pythia8_RunIISummer20UL17NanoAODv9-106X_mc2017_realistic_v9-v1_NANOAODSIM_{skimversion}.root",
#"SSWW_2018" : "{skimdir}/SSWW_TuneCP5_13TeV-madgraph-pythia8_RunIISummer20UL18NanoAODv9-106X_upgrade2018_realistic_v16_L1v1-v1_NANOAODSIM_{skimversion}.root",
#"ST_schan_lep_2016APV" : "{skimdir}/ST_s-channel_4f_leptonDecays_TuneCP5_13TeV-amcatnlo-pythia8_RunIISummer20UL16NanoAODAPVv9-106X_mcRun2_asymptotic_preVFP_v11-v1_NANOAODSIM_{skimversion}.root",
#"ST_schan_lep_2016" : "{skimdir}/ST_s-channel_4f_leptonDecays_TuneCP5_13TeV-amcatnlo-pythia8_RunIISummer20UL16NanoAODv9-106X_mcRun2_asymptotic_v17-v1_NANOAODSIM_{skimversion}.root",
#"ST_schan_lep_2017" : "{skimdir}/ST_s-channel_4f_leptonDecays_TuneCP5_13TeV-amcatnlo-pythia8_RunIISummer20UL17NanoAODv9-106X_mc2017_realistic_v9-v1_NANOAODSIM_{skimversion}.root",
#"ST_schan_lep_2018" : "{skimdir}/ST_s-channel_4f_leptonDecays_TuneCP5_13TeV-amcatnlo-pythia8_RunIISummer20UL18NanoAODv9-106X_upgrade2018_realistic_v16_L1v1-v1_NANOAODSIM_{skimversion}.root",
#"ST_antitop_tchan_2016APV" : "{skimdir}/ST_t-channel_antitop_4f_InclusiveDecays_TuneCP5_13TeV-powheg-madspin-pythia8_RunIISummer20UL16NanoAODAPVv9-106X_mcRun2_asymptotic_preVFP_v11-v1_NANOAODSIM_{skimversion}.root",
#"ST_antitop_tchan_2016" : "{skimdir}/ST_t-channel_antitop_4f_InclusiveDecays_TuneCP5_13TeV-powheg-madspin-pythia8_RunIISummer20UL16NanoAODv9-106X_mcRun2_asymptotic_v17-v1_NANOAODSIM_{skimversion}.root",
#"ST_antitop_tchan_2017" : "{skimdir}/ST_t-channel_antitop_4f_InclusiveDecays_TuneCP5_13TeV-powheg-madspin-pythia8_RunIISummer20UL17NanoAODv9-106X_mc2017_realistic_v9-v1_NANOAODSIM_{skimversion}.root",
#"ST_antitop_tchan_2018" : "{skimdir}/ST_t-channel_antitop_4f_InclusiveDecays_TuneCP5_13TeV-powheg-madspin-pythia8_RunIISummer20UL18NanoAODv9-106X_upgrade2018_realistic_v16_L1v1-v1_NANOAODSIM_{skimversion}.root",
#"ST_top_tchan_2016APV" : "{skimdir}/ST_t-channel_top_4f_InclusiveDecays_TuneCP5_13TeV-powheg-madspin-pythia8_RunIISummer20UL16NanoAODAPVv9-106X_mcRun2_asymptotic_preVFP_v11-v1_NANOAODSIM_{skimversion}.root",
#"ST_top_tchan_2016" : "{skimdir}/ST_t-channel_top_4f_InclusiveDecays_TuneCP5_13TeV-powheg-madspin-pythia8_RunIISummer20UL16NanoAODv9-106X_mcRun2_asymptotic_v17-v1_NANOAODSIM_{skimversion}.root",
#"ST_top_tchan_2017" : "{skimdir}/ST_t-channel_top_4f_InclusiveDecays_TuneCP5_13TeV-powheg-madspin-pythia8_RunIISummer20UL17NanoAODv9-106X_mc2017_realistic_v9-v1_NANOAODSIM_{skimversion}.root",
#"ST_top_tchan_2018" : "{skimdir}/ST_t-channel_top_4f_InclusiveDecays_TuneCP5_13TeV-powheg-madspin-pythia8_RunIISummer20UL18NanoAODv9-106X_upgrade2018_realistic_v16_L1v1-v1_NANOAODSIM_{skimversion}.root",
#"ST_antitop_nohad_tW_2016APV" : "{skimdir}/ST_tW_antitop_5f_NoFullyHadronicDecays_TuneCP5_13TeV-powheg-pythia8_RunIISummer20UL16NanoAODAPVv9-106X_mcRun2_asymptotic_preVFP_v11-v1_NANOAODSIM_{skimversion}.root",
#"ST_antitop_nohad_tW_2016" : "{skimdir}/ST_tW_antitop_5f_NoFullyHadronicDecays_TuneCP5_13TeV-powheg-pythia8_RunIISummer20UL16NanoAODv9-106X_mcRun2_asymptotic_v17-v1_NANOAODSIM_{skimversion}.root",
#"ST_antitop_nohad_tW_2017" : "{skimdir}/ST_tW_antitop_5f_NoFullyHadronicDecays_TuneCP5_13TeV-powheg-pythia8_RunIISummer20UL17NanoAODv9-106X_mc2017_realistic_v9-v1_NANOAODSIM_{skimversion}.root",
#"ST_antitop_nohad_tW_2018" : "{skimdir}/ST_tW_antitop_5f_NoFullyHadronicDecays_TuneCP5_13TeV-powheg-pythia8_RunIISummer20UL18NanoAODv9-106X_upgrade2018_realistic_v16_L1v1-v1_NANOAODSIM_{skimversion}.root",
#"ST_top_nohad_tW_2016APV" : "{skimdir}/ST_tW_top_5f_NoFullyHadronicDecays_TuneCP5_13TeV-powheg-pythia8_RunIISummer20UL16NanoAODAPVv9-106X_mcRun2_asymptotic_preVFP_v11-v1_NANOAODSIM_{skimversion}.root",
#"ST_top_nohad_tW_2016" : "{skimdir}/ST_tW_top_5f_NoFullyHadronicDecays_TuneCP5_13TeV-powheg-pythia8_RunIISummer20UL16NanoAODv9-106X_mcRun2_asymptotic_v17-v1_NANOAODSIM_{skimversion}.root",
#"ST_top_nohad_tW_2017" : "{skimdir}/ST_tW_top_5f_NoFullyHadronicDecays_TuneCP5_13TeV-powheg-pythia8_RunIISummer20UL17NanoAODv9-106X_mc2017_realistic_v9-v1_NANOAODSIM_{skimversion}.root",
#"ST_top_nohad_tW_2018" : "{skimdir}/ST_tW_top_5f_NoFullyHadronicDecays_TuneCP5_13TeV-powheg-pythia8_RunIISummer20UL18NanoAODv9-106X_upgrade2018_realistic_v16_L1v1-v1_NANOAODSIM_{skimversion}.root",
#"TT2l_2016APV" : "{skimdir}/TTTo2L2Nu_TuneCP5_13TeV-powheg-pythia8_RunIISummer20UL16NanoAODAPVv9-106X_mcRun2_asymptotic_preVFP_v11-v1_NANOAODSIM_{skimversion}.root",
#"TT2l_2016" : "{skimdir}/TTTo2L2Nu_TuneCP5_13TeV-powheg-pythia8_RunIISummer20UL16NanoAODv9-106X_mcRun2_asymptotic_v17-v1_NANOAODSIM_{skimversion}.root",
#"TT2l_2017" : "{skimdir}/TTTo2L2Nu_TuneCP5_13TeV-powheg-pythia8_RunIISummer20UL17NanoAODv9-106X_mc2017_realistic_v9-v1_NANOAODSIM_{skimversion}.root",
#"TT2l_2018" : "{skimdir}/TTTo2L2Nu_TuneCP5_13TeV-powheg-pythia8_RunIISummer20UL18NanoAODv9-106X_upgrade2018_realistic_v16_L1v1-v1_NANOAODSIM_{skimversion}.root",
#"TThad_2016APV" : "{skimdir}/TTToHadronic_TuneCP5_13TeV-powheg-pythia8_RunIISummer20UL16NanoAODAPVv9-106X_mcRun2_asymptotic_preVFP_v11-v1_NANOAODSIM_{skimversion}.root",
#"TThad_2016" : "{skimdir}/TTToHadronic_TuneCP5_13TeV-powheg-pythia8_RunIISummer20UL16NanoAODv9-106X_mcRun2_asymptotic_v17-v1_NANOAODSIM_{skimversion}.root",
#"TThad_2017" : "{skimdir}/TTToHadronic_TuneCP5_13TeV-powheg-pythia8_RunIISummer20UL17NanoAODv9-106X_mc2017_realistic_v9-v1_NANOAODSIM_{skimversion}.root",
#"TThad_2018" : "{skimdir}/TTToHadronic_TuneCP5_13TeV-powheg-pythia8_RunIISummer20UL18NanoAODv9-106X_upgrade2018_realistic_v16_L1v1-v1_NANOAODSIM_{skimversion}.root",
#"TT1l_2016APV" : "{skimdir}/TTToSemiLeptonic_TuneCP5_13TeV-powheg-pythia8_RunIISummer20UL16NanoAODAPVv9-106X_mcRun2_asymptotic_preVFP_v11-v1_NANOAODSIM_{skimversion}.root",
#"TT1l_2016" : "{skimdir}/TTToSemiLeptonic_TuneCP5_13TeV-powheg-pythia8_RunIISummer20UL16NanoAODv9-106X_mcRun2_asymptotic_v17-v1_NANOAODSIM_{skimversion}.root",
#"TT1l_2017" : "{skimdir}/TTToSemiLeptonic_TuneCP5_13TeV-powheg-pythia8_RunIISummer20UL17NanoAODv9-106X_mc2017_realistic_v9-v1_NANOAODSIM_{skimversion}.root",
#"TT1l_2018" : "{skimdir}/TTToSemiLeptonic_TuneCP5_13TeV-powheg-pythia8_RunIISummer20UL18NanoAODv9-106X_upgrade2018_realistic_v16_L1v1-v1_NANOAODSIM_{skimversion}.root",
#"TTWlv_2016APV" : "{skimdir}/TTWJetsToLNu_TuneCP5_13TeV-amcatnloFXFX-madspin-pythia8_RunIISummer20UL16NanoAODAPVv9-106X_mcRun2_asymptotic_preVFP_v11-v2_NANOAODSIM_{skimversion}.root",
#"TTWlv_2016" : "{skimdir}/TTWJetsToLNu_TuneCP5_13TeV-amcatnloFXFX-madspin-pythia8_RunIISummer20UL16NanoAODv9-106X_mcRun2_asymptotic_v17-v1_NANOAODSIM_{skimversion}.root",
#"TTWlv_2017" : "{skimdir}/TTWJetsToLNu_TuneCP5_13TeV-amcatnloFXFX-madspin-pythia8_RunIISummer20UL17NanoAODv9-106X_mc2017_realistic_v9-v1_NANOAODSIM_{skimversion}.root",
#"TTWlv_2018" : "{skimdir}/TTWJetsToLNu_TuneCP5_13TeV-amcatnloFXFX-madspin-pythia8_RunIISummer20UL18NanoAODv9-106X_upgrade2018_realistic_v16_L1v1-v1_NANOAODSIM_{skimversion}.root",
#"TTWqq_2016APV" : "{skimdir}/TTWJetsToQQ_TuneCP5_13TeV-amcatnloFXFX-madspin-pythia8_RunIISummer20UL16NanoAODAPVv9-106X_mcRun2_asymptotic_preVFP_v11-v2_NANOAODSIM_{skimversion}.root",
#"TTWqq_2016" : "{skimdir}/TTWJetsToQQ_TuneCP5_13TeV-amcatnloFXFX-madspin-pythia8_RunIISummer20UL16NanoAODv9-106X_mcRun2_asymptotic_v17-v1_NANOAODSIM_{skimversion}.root",
#"TTWqq_2017" : "{skimdir}/TTWJetsToQQ_TuneCP5_13TeV-amcatnloFXFX-madspin-pythia8_RunIISummer20UL17NanoAODv9-106X_mc2017_realistic_v9-v1_NANOAODSIM_{skimversion}.root",
#"TTWqq_2018" : "{skimdir}/TTWJetsToQQ_TuneCP5_13TeV-amcatnloFXFX-madspin-pythia8_RunIISummer20UL18NanoAODv9-106X_upgrade2018_realistic_v16_L1v1-v1_NANOAODSIM_{skimversion}.root",
#"TTZll_M10_2016APV" : "{skimdir}/TTZToLLNuNu_M-10_TuneCP5_13TeV-amcatnlo-pythia8_RunIISummer20UL16NanoAODAPVv9-106X_mcRun2_asymptotic_preVFP_v11-v1_NANOAODSIM_{skimversion}.root",
#"TTZll_M10_2016" : "{skimdir}/TTZToLLNuNu_M-10_TuneCP5_13TeV-amcatnlo-pythia8_RunIISummer20UL16NanoAODv9-106X_mcRun2_asymptotic_v17-v1_NANOAODSIM_{skimversion}.root",
#"TTZll_M10_2017" : "{skimdir}/TTZToLLNuNu_M-10_TuneCP5_13TeV-amcatnlo-pythia8_RunIISummer20UL17NanoAODv9-106X_mc2017_realistic_v9-v1_NANOAODSIM_{skimversion}.root",
#"TTZll_M10_2018" : "{skimdir}/TTZToLLNuNu_M-10_TuneCP5_13TeV-amcatnlo-pythia8_RunIISummer20UL18NanoAODv9-106X_upgrade2018_realistic_v16_L1v1-v1_NANOAODSIM_{skimversion}.root",
#"TTZll_M1_2016APV": "{skimdir}/TTZToLL_M-1to10_TuneCP5_13TeV-amcatnlo-pythia8_RunIISummer20UL16NanoAODAPVv9-106X_mcRun2_asymptotic_preVFP_v11-v1_NANOAODSIM_{skimversion}.root",
#"TTZll_M1_2016": "{skimdir}/TTZToLL_M-1to10_TuneCP5_13TeV-amcatnlo-pythia8_RunIISummer20UL16NanoAODv9-106X_mcRun2_asymptotic_v17-v1_NANOAODSIM_{skimversion}.root",
#"TTZll_M1_2017": "{skimdir}/TTZToLL_M-1to10_TuneCP5_13TeV-amcatnlo-pythia8_RunIISummer20UL17NanoAODv9-106X_mc2017_realistic_v9-v1_NANOAODSIM_{skimversion}.root",
#"TTZll_M1_2018" : "{skimdir}/TTZToLL_M-1to10_TuneCP5_13TeV-amcatnlo-pythia8_RunIISummer20UL18NanoAODv9-106X_upgrade2018_realistic_v16_L1v1-v1_NANOAODSIM_{skimversion}.root",
#"TTZqq_2016APV" : "{skimdir}/TTZToQQ_TuneCP5_13TeV-amcatnlo-pythia8_RunIISummer20UL16NanoAODAPVv9-106X_mcRun2_asymptotic_preVFP_v11-v1_NANOAODSIM_{skimversion}.root",
#"TTZqq_2016" : "{skimdir}/TTZToQQ_TuneCP5_13TeV-amcatnlo-pythia8_RunIISummer20UL16NanoAODv9-106X_mcRun2_asymptotic_v17-v1_NANOAODSIM_{skimversion}.root",
#"TTZqq_2017" : "{skimdir}/TTZToQQ_TuneCP5_13TeV-amcatnlo-pythia8_RunIISummer20UL17NanoAODv9-106X_mc2017_realistic_v9-v1_NANOAODSIM_{skimversion}.root",
#"TTZqq_2018" : "{skimdir}/TTZToQQ_TuneCP5_13TeV-amcatnlo-pythia8_RunIISummer20UL18NanoAODv9-106X_upgrade2018_realistic_v16_L1v1-v1_NANOAODSIM_{skimversion}.root",
#"Wlv_2016APV" : "{skimdir}/WJetsToLNu_TuneCP5_13TeV-amcatnloFXFX-pythia8_RunIISummer20UL16NanoAODAPVv9-106X_mcRun2_asymptotic_preVFP_v11-v2_NANOAODSIM_{skimversion}.root",
#"Wlv_2016" : "{skimdir}/WJetsToLNu_TuneCP5_13TeV-madgraphMLM-pythia8_RunIISummer20UL16NanoAODv9-106X_mcRun2_asymptotic_v17-v1_NANOAODSIM_{skimversion}.root",
#"Wlv_2017" : "{skimdir}/WJetsToLNu_TuneCP5_13TeV-madgraphMLM-pythia8_RunIISummer20UL17NanoAODv9-106X_mc2017_realistic_v9-v1_NANOAODSIM_{skimversion}.root",
#"Wlv_2018" : "{skimdir}/WJetsToLNu_TuneCP5_13TeV-madgraphMLM-pythia8_RunIISummer20UL18NanoAODv9-106X_upgrade2018_realistic_v16_L1v1-v1_NANOAODSIM_{skimversion}.root",
#
#
#
# "WWlvqq_2018" : "{skimdir}/WWTo1L1Nu2Q_4f_TuneCP5_13TeV-amcatnloFXFX-pythia8_RunIISummer20UL18NanoAODv9-106X_upgrade2018_realistic_v16_L1v1-v1_NANOAODSIM_{skimversion}.root",
#"WWlvlv_2016APV" : "{skimdir}/WWTo2L2Nu_TuneCP5_13TeV-powheg-pythia8_RunIISummer20UL16NanoAODAPVv9-106X_mcRun2_asymptotic_preVFP_v11-v1_NANOAODSIM_{skimversion}.root",
#"WWlvlv_2016" : "{skimdir}/WWTo2L2Nu_TuneCP5_13TeV-powheg-pythia8_RunIISummer20UL16NanoAODv9-106X_mcRun2_asymptotic_v17-v1_NANOAODSIM_{skimversion}.root",
#"WWlvlv_2017" : "{skimdir}/WWTo2L2Nu_TuneCP5_13TeV-powheg-pythia8_RunIISummer20UL17NanoAODv9-106X_mc2017_realistic_v9-v2_NANOAODSIM_{skimversion}.root",
#"WWlvlv_2018" : "{skimdir}/WWTo2L2Nu_TuneCP5_13TeV-powheg-pythia8_RunIISummer20UL18NanoAODv9-106X_upgrade2018_realistic_v16_L1v1-v2_NANOAODSIM_{skimversion}.root",
#
#
#
# "WW4q_2018" : "{skimdir}/WWTo4Q_4f_TuneCP5_13TeV-amcatnloFXFX-pythia8_RunIISummer20UL18NanoAODv9-106X_upgrade2018_realistic_v16_L1v1-v2_NANOAODSIM_{skimversion}.root",
#"WWW_2l_2016APV" : "{skimdir}/WWW_4F_DiLeptonFilter_TuneCP5_13TeV-amcatnlo-pythia8_RunIISummer20UL16NanoAODAPVv9-106X_mcRun2_asymptotic_preVFP_v11-v2_NANOAODSIM_{skimversion}.root",
#"WWW_2l_2016" : "{skimdir}/WWW_4F_DiLeptonFilter_TuneCP5_13TeV-amcatnlo-pythia8_RunIISummer20UL16NanoAODv9-106X_mcRun2_asymptotic_v17-v1_NANOAODSIM_{skimversion}.root",
#"WWW_2l_2017" : "{skimdir}/WWW_4F_DiLeptonFilter_TuneCP5_13TeV-amcatnlo-pythia8_RunIISummer20UL17NanoAODv9-106X_mc2017_realistic_v9-v1_NANOAODSIM_{skimversion}.root",
#"WWW_2l_2018" : "{skimdir}/WWW_4F_DiLeptonFilter_TuneCP5_13TeV-amcatnlo-pythia8_RunIISummer20UL18NanoAODv9-106X_upgrade2018_realistic_v16_L1v1-v1_NANOAODSIM_{skimversion}.root",
# "WWW_2016APV" : "{skimdir}/WWW_4F_TuneCP5_13TeV-amcatnlo-pythia8_RunIISummer20UL16NanoAODAPVv9-106X_mcRun2_asymptotic_preVFP_v11-v1_NANOAODSIM_{skimversion}.root",
# "WWW_2016" : "{skimdir}/WWW_4F_TuneCP5_13TeV-amcatnlo-pythia8_RunIISummer20UL16NanoAODv9-106X_mcRun2_asymptotic_v17-v1_NANOAODSIM_{skimversion}.root",
# "WWW_2017" : "{skimdir}/WWW_4F_TuneCP5_13TeV-amcatnlo-pythia8_RunIISummer20UL17NanoAODv9-106X_mc2017_realistic_v9-v1_NANOAODSIM_{skimversion}.root",
# "WWW_2018" : "{skimdir}/WWW_4F_TuneCP5_13TeV-amcatnlo-pythia8_RunIISummer20UL18NanoAODv9-106X_upgrade2018_realistic_v16_L1v1-v1_NANOAODSIM_{skimversion}.root",
# "WWW_ext_2016APV" : "{skimdir}/WWW_4F_TuneCP5_13TeV-amcatnlo-pythia8_RunIISummer20UL16NanoAODAPVv9-106X_mcRun2_asymptotic_preVFP_v11_ext1-v1_NANOAODSIM_{skimversion}.root",
# "WWW_ext_2016" : "{skimdir}/WWW_4F_TuneCP5_13TeV-amcatnlo-pythia8_RunIISummer20UL16NanoAODv9-106X_mcRun2_asymptotic_v17_ext1-v1_NANOAODSIM_{skimversion}.root",
# "WWW_ext_2017" : "{skimdir}/WWW_4F_TuneCP5_13TeV-amcatnlo-pythia8_RunIISummer20UL17NanoAODv9-106X_mc2017_realistic_v9_ext1-v2_NANOAODSIM_{skimversion}.root",
# "WWW_ext_2018" : "{skimdir}/WWW_4F_TuneCP5_13TeV-amcatnlo-pythia8_RunIISummer20UL18NanoAODv9-106X_upgrade2018_realistic_v16_L1v1_ext1-v2_NANOAODSIM_{skimversion}.root",
# Does not seem to have been produced for APV why?
"WWZ_4l_2016" : "{skimdir}/WWZJetsTo4L2Nu_4F_TuneCP5_13TeV-amcatnlo-pythia8_RunIISummer20UL16NanoAODv9-106X_mcRun2_asymptotic_v17-v2_NANOAODSIM_{skimversion}.root",
"WWZ_4l_2017" : "{skimdir}/WWZJetsTo4L2Nu_4F_TuneCP5_13TeV-amcatnlo-pythia8_RunIISummer20UL17NanoAODv9-106X_mc2017_realistic_v9-v2_NANOAODSIM_{skimversion}.root",
"WWZ_4l_2018" : "{skimdir}/WWZJetsTo4L2Nu_4F_TuneCP5_13TeV-amcatnlo-pythia8_RunIISummer20UL18NanoAODv9-106X_upgrade2018_realistic_v16_L1v1-v2_NANOAODSIM_{skimversion}.root",
"WWZ_2016APV" : "{skimdir}/WWZ_4F_TuneCP5_13TeV-amcatnlo-pythia8_RunIISummer20UL16NanoAODAPVv9-106X_mcRun2_asymptotic_preVFP_v11-v1_NANOAODSIM_{skimversion}.root",
#"WWZ_2016" : "{skimdir}/WWZ_4F_TuneCP5_13TeV-amcatnlo-pythia8_RunIISummer20UL16NanoAODv9-106X_mcRun2_asymptotic_v17-v1_NANOAODSIM_{skimversion}.root",
#"WWZ_2017" : "{skimdir}/WWZ_4F_TuneCP5_13TeV-amcatnlo-pythia8_RunIISummer20UL17NanoAODv9-106X_mc2017_realistic_v9-v1_NANOAODSIM_{skimversion}.root",
#"WWZ_2018" : "{skimdir}/WWZ_4F_TuneCP5_13TeV-amcatnlo-pythia8_RunIISummer20UL18NanoAODv9-106X_upgrade2018_realistic_v16_L1v1-v1_NANOAODSIM_{skimversion}.root",
# "WWZ_ext_2016APV" : "{skimdir}/WWZ_4F_TuneCP5_13TeV-amcatnlo-pythia8_RunIISummer20UL16NanoAODAPVv9-106X_mcRun2_asymptotic_preVFP_v11_ext1-v1_NANOAODSIM_{skimversion}.root",
# "WWZ_ext_2016" : "{skimdir}/WWZ_4F_TuneCP5_13TeV-amcatnlo-pythia8_RunIISummer20UL16NanoAODv9-106X_mcRun2_asymptotic_v17_ext1-v1_NANOAODSIM_{skimversion}.root",
# "WWZ_ext_2017" : "{skimdir}/WWZ_4F_TuneCP5_13TeV-amcatnlo-pythia8_RunIISummer20UL17NanoAODv9-106X_mc2017_realistic_v9_ext1-v2_NANOAODSIM_{skimversion}.root",
# "WWZ_ext_2018" : "{skimdir}/WWZ_4F_TuneCP5_13TeV-amcatnlo-pythia8_RunIISummer20UL18NanoAODv9-106X_upgrade2018_realistic_v16_L1v1_ext1-v2_NANOAODSIM_{skimversion}.root",
# "WW_2016APV" : "{skimdir}/WW_TuneCP5_13TeV-pythia8_RunIISummer20UL16NanoAODAPVv9-106X_mcRun2_asymptotic_preVFP_v11-v1_NANOAODSIM_{skimversion}.root",
# "WW_2016" : "{skimdir}/WW_TuneCP5_13TeV-pythia8_RunIISummer20UL16NanoAODv9-106X_mcRun2_asymptotic_v17-v1_NANOAODSIM_{skimversion}.root",
# "WW_2017" : "{skimdir}/WW_TuneCP5_13TeV-pythia8_RunIISummer20UL17NanoAODv9-106X_mc2017_realistic_v9-v1_NANOAODSIM_{skimversion}.root",
# "WW_2018" : "{skimdir}/WW_TuneCP5_13TeV-pythia8_RunIISummer20UL18NanoAODv9-106X_upgrade2018_realistic_v16_L1v1-v1_NANOAODSIM_{skimversion}.root",
#
#
#
# "WZ1l2q_2018" : "{skimdir}/WZTo1L1Nu2Q_4f_TuneCP5_13TeV-amcatnloFXFX-pythia8_RunIISummer20UL18NanoAODv9-106X_upgrade2018_realistic_v16_L1v1-v1_NANOAODSIM_{skimversion}.root",
#
#
#
# "WZ1l3v_2018" : "{skimdir}/WZTo1L3Nu_4f_TuneCP5_13TeV-amcatnloFXFX-pythia8_RunIISummer20UL18NanoAODv9-106X_upgrade2018_realistic_v16_L1v1-v1_NANOAODSIM_{skimversion}.root",
#"WZ2q2l_2016APV" : "{skimdir}/WZTo2Q2L_mllmin4p0_TuneCP5_13TeV-amcatnloFXFX-pythia8_RunIISummer20UL16NanoAODAPVv9-106X_mcRun2_asymptotic_preVFP_v11-v2_NANOAODSIM_{skimversion}.root",
#"WZ2q2l_2016" : "{skimdir}/WZTo2Q2L_mllmin4p0_TuneCP5_13TeV-amcatnloFXFX-pythia8_RunIISummer20UL16NanoAODv9-106X_mcRun2_asymptotic_v17-v2_NANOAODSIM_{skimversion}.root",
#"WZ2q2l_2017" : "{skimdir}/WZTo2Q2L_mllmin4p0_TuneCP5_13TeV-amcatnloFXFX-pythia8_RunIISummer20UL17NanoAODv9-106X_mc2017_realistic_v9-v2_NANOAODSIM_{skimversion}.root",
#"WZ2q2l_2018" : "{skimdir}/WZTo2Q2L_mllmin4p0_TuneCP5_13TeV-amcatnloFXFX-pythia8_RunIISummer20UL18NanoAODv9-106X_upgrade2018_realistic_v16_L1v1-v1_NANOAODSIM_{skimversion}.root",
#"WZ3l1v_2016APV" : "{skimdir}/WZTo3LNu_TuneCP5_13TeV-amcatnloFXFX-pythia8_RunIISummer20UL16NanoAODAPVv9-106X_mcRun2_asymptotic_preVFP_v11-v1_NANOAODSIM_{skimversion}.root",
#"WZ3l1v_2016" : "{skimdir}/WZTo3LNu_TuneCP5_13TeV-amcatnloFXFX-pythia8_RunIISummer20UL16NanoAODv9-106X_mcRun2_asymptotic_v17-v1_NANOAODSIM_{skimversion}.root",
#"WZ3l1v_2017" : "{skimdir}/WZTo3LNu_TuneCP5_13TeV-amcatnloFXFX-pythia8_RunIISummer20UL17NanoAODv9-106X_mc2017_realistic_v9-v2_NANOAODSIM_{skimversion}.root",
#"WZ3l1v_2018" : "{skimdir}/WZTo3LNu_TuneCP5_13TeV-amcatnloFXFX-pythia8_RunIISummer20UL18NanoAODv9-106X_upgrade2018_realistic_v16_L1v1-v2_NANOAODSIM_{skimversion}.root",
#"WZZ_2016APV" : "{skimdir}/WZZ_TuneCP5_13TeV-amcatnlo-pythia8_RunIISummer20UL16NanoAODAPVv9-106X_mcRun2_asymptotic_preVFP_v11-v1_NANOAODSIM_{skimversion}.root",
#"WZZ_2016" : "{skimdir}/WZZ_TuneCP5_13TeV-amcatnlo-pythia8_RunIISummer20UL16NanoAODv9-106X_mcRun2_asymptotic_v17-v1_NANOAODSIM_{skimversion}.root",
#"WZZ_2017" : "{skimdir}/WZZ_TuneCP5_13TeV-amcatnlo-pythia8_RunIISummer20UL17NanoAODv9-106X_mc2017_realistic_v9-v1_NANOAODSIM_{skimversion}.root",
#"WZZ_2018" : "{skimdir}/WZZ_TuneCP5_13TeV-amcatnlo-pythia8_RunIISummer20UL18NanoAODv9-106X_upgrade2018_realistic_v16_L1v1-v1_NANOAODSIM_{skimversion}.root",
# "WZZ_ext_2016APV" : "{skimdir}/WZZ_TuneCP5_13TeV-amcatnlo-pythia8_RunIISummer20UL16NanoAODAPVv9-106X_mcRun2_asymptotic_preVFP_v11_ext1-v1_NANOAODSIM_{skimversion}.root",
# "WZZ_ext_2016" : "{skimdir}/WZZ_TuneCP5_13TeV-amcatnlo-pythia8_RunIISummer20UL16NanoAODv9-106X_mcRun2_asymptotic_v17_ext1-v1_NANOAODSIM_{skimversion}.root",
# "WZZ_ext_2017" : "{skimdir}/WZZ_TuneCP5_13TeV-amcatnlo-pythia8_RunIISummer20UL17NanoAODv9-106X_mc2017_realistic_v9_ext1-v2_NANOAODSIM_{skimversion}.root",
# "WZZ_ext_2018" : "{skimdir}/WZZ_TuneCP5_13TeV-amcatnlo-pythia8_RunIISummer20UL18NanoAODv9-106X_upgrade2018_realistic_v16_L1v1_ext1-v2_NANOAODSIM_{skimversion}.root",
#"WZ_2016APV" : "{skimdir}/WZ_TuneCP5_13TeV-pythia8_RunIISummer20UL16NanoAODAPVv9-106X_mcRun2_asymptotic_preVFP_v11-v1_NANOAODSIM_{skimversion}.root",
#"WZ_2016" : "{skimdir}/WZ_TuneCP5_13TeV-pythia8_RunIISummer20UL16NanoAODv9-106X_mcRun2_asymptotic_v17-v1_NANOAODSIM_{skimversion}.root",
#"WZ_2017" : "{skimdir}/WZ_TuneCP5_13TeV-pythia8_RunIISummer20UL17NanoAODv9-106X_mc2017_realistic_v9-v1_NANOAODSIM_{skimversion}.root",
#"WZ_2018" : "{skimdir}/WZ_TuneCP5_13TeV-pythia8_RunIISummer20UL18NanoAODv9-106X_upgrade2018_realistic_v16_L1v1-v1_NANOAODSIM_{skimversion}.root",
"ZZ2l2v_2016APV" : "{skimdir}/ZZTo2L2Nu_TuneCP5_13TeV_powheg_pythia8_RunIISummer20UL16NanoAODAPVv9-106X_mcRun2_asymptotic_preVFP_v11-v1_NANOAODSIM_{skimversion}.root",
"ZZ2l2v_2016" : "{skimdir}/ZZTo2L2Nu_TuneCP5_13TeV_powheg_pythia8_RunIISummer20UL16NanoAODv9-106X_mcRun2_asymptotic_v17-v1_NANOAODSIM_{skimversion}.root",
"ZZ2l2v_2017" : "{skimdir}/ZZTo2L2Nu_TuneCP5_13TeV_powheg_pythia8_RunIISummer20UL17NanoAODv9-106X_mc2017_realistic_v9-v1_NANOAODSIM_{skimversion}.root",
"ZZ2l2v_2018" : "{skimdir}/ZZTo2L2Nu_TuneCP5_13TeV_powheg_pythia8_RunIISummer20UL18NanoAODv9-106X_upgrade2018_realistic_v16_L1v1-v1_NANOAODSIM_{skimversion}.root",
"ZZ2l2q_2016APV" : "{skimdir}/ZZTo2Q2L_mllmin4p0_TuneCP5_13TeV-amcatnloFXFX-pythia8_RunIISummer20UL16NanoAODAPVv9-106X_mcRun2_asymptotic_preVFP_v11-v1_NANOAODSIM_{skimversion}.root",
"ZZ2l2q_2016" : "{skimdir}/ZZTo2Q2L_mllmin4p0_TuneCP5_13TeV-amcatnloFXFX-pythia8_RunIISummer20UL16NanoAODv9-106X_mcRun2_asymptotic_v17-v1_NANOAODSIM_{skimversion}.root",
"ZZ2l2q_2017" : "{skimdir}/ZZTo2Q2L_mllmin4p0_TuneCP5_13TeV-amcatnloFXFX-pythia8_RunIISummer20UL17NanoAODv9-106X_mc2017_realistic_v9-v1_NANOAODSIM_{skimversion}.root",
"ZZ2l2q_2018" : "{skimdir}/ZZTo2Q2L_mllmin4p0_TuneCP5_13TeV-amcatnloFXFX-pythia8_RunIISummer20UL18NanoAODv9-106X_upgrade2018_realistic_v16_L1v1-v1_NANOAODSIM_{skimversion}.root",
#
#
#
#"ZZ2q2v_2018" : "{skimdir}/ZZTo2Q2Nu_TuneCP5_13TeV-amcatnloFXFX-pythia8_RunIISummer20UL18NanoAODv9-106X_upgrade2018_realistic_v16_L1v1-v1_NANOAODSIM_{skimversion}.root",
"ZZ4l_2016APV" : "{skimdir}/ZZTo4L_TuneCP5_13TeV_powheg_pythia8_RunIISummer20UL16NanoAODAPVv9-106X_mcRun2_asymptotic_preVFP_v11-v1_NANOAODSIM_{skimversion}.root",
"ZZ4l_2016" : "{skimdir}/ZZTo4L_TuneCP5_13TeV_powheg_pythia8_RunIISummer20UL16NanoAODv9-106X_mcRun2_asymptotic_v17-v1_NANOAODSIM_{skimversion}.root",
"ZZ4l_2017" : "{skimdir}/ZZTo4L_TuneCP5_13TeV_powheg_pythia8_RunIISummer20UL17NanoAODv9-106X_mc2017_realistic_v9-v2_NANOAODSIM_{skimversion}.root",
"ZZ4l_2018" : "{skimdir}/ZZTo4L_TuneCP5_13TeV_powheg_pythia8_RunIISummer20UL18NanoAODv9-106X_upgrade2018_realistic_v16_L1v1-v2_NANOAODSIM_{skimversion}.root",
#
#
#
#"ZZ4q_2018" : "{skimdir}/ZZTo4Q_5f_TuneCP5_13TeV-amcatnloFXFX-pythia8_RunIISummer20UL18NanoAODv9-106X_upgrade2018_realistic_v16_L1v1-v1_NANOAODSIM_{skimversion}.root",
#"ZZZ_2016APV" : "{skimdir}/ZZZ_TuneCP5_13TeV-amcatnlo-pythia8_RunIISummer20UL16NanoAODAPVv9-106X_mcRun2_asymptotic_preVFP_v11-v1_NANOAODSIM_{skimversion}.root",
#"ZZZ_2016" : "{skimdir}/ZZZ_TuneCP5_13TeV-amcatnlo-pythia8_RunIISummer20UL16NanoAODv9-106X_mcRun2_asymptotic_v17-v1_NANOAODSIM_{skimversion}.root",
#"ZZZ_2017" : "{skimdir}/ZZZ_TuneCP5_13TeV-amcatnlo-pythia8_RunIISummer20UL17NanoAODv9-106X_mc2017_realistic_v9-v1_NANOAODSIM_{skimversion}.root",
#"ZZZ_2018" : "{skimdir}/ZZZ_TuneCP5_13TeV-amcatnlo-pythia8_RunIISummer20UL18NanoAODv9-106X_upgrade2018_realistic_v16_L1v1-v1_NANOAODSIM_{skimversion}.root",
# "ZZZ_ext_2016APV" : "{skimdir}/ZZZ_TuneCP5_13TeV-amcatnlo-pythia8_RunIISummer20UL16NanoAODAPVv9-106X_mcRun2_asymptotic_preVFP_v11_ext1-v1_NANOAODSIM_{skimversion}.root",
# "ZZZ_ext_2016" : "{skimdir}/ZZZ_TuneCP5_13TeV-amcatnlo-pythia8_RunIISummer20UL16NanoAODv9-106X_mcRun2_asymptotic_v17_ext1-v1_NANOAODSIM_{skimversion}.root",
# "ZZZ_ext_2017" : "{skimdir}/ZZZ_TuneCP5_13TeV-amcatnlo-pythia8_RunIISummer20UL17NanoAODv9-106X_mc2017_realistic_v9_ext1-v2_NANOAODSIM_{skimversion}.root",
# "ZZZ_ext_2018" : "{skimdir}/ZZZ_TuneCP5_13TeV-amcatnlo-pythia8_RunIISummer20UL18NanoAODv9-106X_upgrade2018_realistic_v16_L1v1_ext1-v2_NANOAODSIM_{skimversion}.root",
# "ZZ_2016APV" : "{skimdir}/ZZ_TuneCP5_13TeV-pythia8_RunIISummer20UL16NanoAODAPVv9-106X_mcRun2_asymptotic_preVFP_v11-v1_NANOAODSIM_{skimversion}.root",
# "ZZ_2016" : "{skimdir}/ZZ_TuneCP5_13TeV-pythia8_RunIISummer20UL16NanoAODv9-106X_mcRun2_asymptotic_v17-v1_NANOAODSIM_{skimversion}.root",
# "ZZ_2017" : "{skimdir}/ZZ_TuneCP5_13TeV-pythia8_RunIISummer20UL17NanoAODv9-106X_mc2017_realistic_v9-v1_NANOAODSIM_{skimversion}.root",
# "ZZ_2018" : "{skimdir}/ZZ_TuneCP5_13TeV-pythia8_RunIISummer20UL18NanoAODv9-106X_upgrade2018_realistic_v16_L1v1-v1_NANOAODSIM_{skimversion}.root",
#"tZq_2016APV" : "{skimdir}/tZq_ll_4f_ckm_NLO_TuneCP5_13TeV-amcatnlo-pythia8_RunIISummer20UL16NanoAODAPVv9-106X_mcRun2_asymptotic_preVFP_v11-v1_NANOAODSIM_{skimversion}.root",
#"tZq_2016" : "{skimdir}/tZq_ll_4f_ckm_NLO_TuneCP5_13TeV-amcatnlo-pythia8_RunIISummer20UL16NanoAODv9-106X_mcRun2_asymptotic_v17-v1_NANOAODSIM_{skimversion}.root",
#"tZq_2017" : "{skimdir}/tZq_ll_4f_ckm_NLO_TuneCP5_13TeV-amcatnlo-pythia8_RunIISummer20UL17NanoAODv9-106X_mc2017_realistic_v9-v1_NANOAODSIM_{skimversion}.root",
#"tZq_2018" : "{skimdir}/tZq_ll_4f_ckm_NLO_TuneCP5_13TeV-amcatnlo-pythia8_RunIISummer20UL18NanoAODv9-106X_upgrade2018_realistic_v16_L1v1-v1_NANOAODSIM_{skimversion}.root",
#"ttHNonbb_2016APV" : "{skimdir}/ttHJetToNonbb_M125_TuneCP5_13TeV_amcatnloFXFX_madspin_pythia8_RunIISummer20UL16NanoAODAPVv9-106X_mcRun2_asymptotic_preVFP_v11-v1_NANOAODSIM_{skimversion}.root",
#"ttHNonbb_2016" : "{skimdir}/ttHJetToNonbb_M125_TuneCP5_13TeV_amcatnloFXFX_madspin_pythia8_RunIISummer20UL16NanoAODv9-106X_mcRun2_asymptotic_v17-v1_NANOAODSIM_{skimversion}.root",
#"ttHNonbb_2017" : "{skimdir}/ttHJetToNonbb_M125_TuneCP5_13TeV_amcatnloFXFX_madspin_pythia8_RunIISummer20UL17NanoAODv9-106X_mc2017_realistic_v9-v1_NANOAODSIM_{skimversion}.root",
#"ttHNonbb_2018" : "{skimdir}/ttHJetToNonbb_M125_TuneCP5_13TeV_amcatnloFXFX_madspin_pythia8_RunIISummer20UL18NanoAODv9-106X_upgrade2018_realistic_v16_L1v1-v1_NANOAODSIM_{skimversion}.root",
#"ttHbb_2016APV" : "{skimdir}/ttHJetTobb_M125_TuneCP5_13TeV_amcatnloFXFX_madspin_pythia8_RunIISummer20UL16NanoAODAPVv9-106X_mcRun2_asymptotic_preVFP_v11-v1_NANOAODSIM_{skimversion}.root",
#"ttHbb_2016" : "{skimdir}/ttHJetTobb_M125_TuneCP5_13TeV_amcatnloFXFX_madspin_pythia8_RunIISummer20UL16NanoAODv9-106X_mcRun2_asymptotic_v17-v1_NANOAODSIM_{skimversion}.root",
#"ttHbb_2017" : "{skimdir}/ttHJetTobb_M125_TuneCP5_13TeV_amcatnloFXFX_madspin_pythia8_RunIISummer20UL17NanoAODv9-106X_mc2017_realistic_v9-v1_NANOAODSIM_{skimversion}.root",
#"ttHbb_2018" : "{skimdir}/ttHJetTobb_M125_TuneCP5_13TeV_amcatnloFXFX_madspin_pythia8_RunIISummer20UL18NanoAODv9-106X_upgrade2018_realistic_v16_L1v1-v1_NANOAODSIM_{skimversion}.root",
#"VHToNonbb_2016APV" : "{skimdir}/VHToNonbb_M125_TuneCP5_13TeV-amcatnloFXFX_madspin_pythia8_RunIISummer20UL16NanoAODAPVv9-106X_mcRun2_asymptotic_preVFP_v11-v2_NANOAODSIM_{skimversion}.root",
#"VHToNonbb_2016" : "{skimdir}/VHToNonbb_M125_TuneCP5_13TeV-amcatnloFXFX_madspin_pythia8_RunIISummer20UL16NanoAODv9-106X_mcRun2_asymptotic_v17-v2_NANOAODSIM_{skimversion}.root",
#"VHToNonbb_2017" : "{skimdir}/VHToNonbb_M125_TuneCP5_13TeV-amcatnloFXFX_madspin_pythia8_RunIISummer20UL17NanoAODv9-106X_mc2017_realistic_v9-v2_NANOAODSIM_{skimversion}.root",
#"VHToNonbb_2018" : "{skimdir}/VHToNonbb_M125_TuneCP5_13TeV-amcatnloFXFX_madspin_pythia8_RunIISummer20UL18NanoAODv9-106X_upgrade2018_realistic_v16_L1v1-v2_NANOAODSIM_{skimversion}.root",
}
#"VHToNonbb_2018" : "{skimdir}/VHToNonbb_M125_TuneCP5_13TeV-amcatnloFXFX_madspin_pythia8_RunIISummer20UL18NanoAODv9-106X_upgrade2018_realistic_v16_L1v1-v2_NANOAODSIM_{skimversion}.root",
}

if __name__ == "__main__":

    main()
