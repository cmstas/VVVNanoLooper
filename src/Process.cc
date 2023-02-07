#include "Process.h"

void Process()
{
    //==============================================
    // Process:
    // This function gets called during the event looping.
    // This is where one sets the variables used for the category .
    //==============================================

    // Process_Common is always called regardless of the category
    Process_Common();

    // Then depending on the analysis mode, different "Process" runs

    //std::cout << "Debug Process 1" << std::endl;

    switch (ana.looperMode)
    {
        case AnalysisConfig::k4LepMET: Process_4LepMET(); break;
    }

    //std::cout << "Debug Process 2" << std::endl;

    // At this point, variables are all computed and set
    // Adding a check for a given event
    if (nt.run() == 1 and nt.luminosityBlock() == 131 and nt.event() == 130004){
	ana.cutflow.printCuts();

    }

    //std::cout << "Debug Process 3" << std::endl;

    // Now fill all the histograms that are booked!
    ana.cutflow.fill();

    //std::cout << "Debug Process 4" << std::endl;

    // If there are certain things people wish to do "Post" processing of the cutflows and histogramming
    // For example this is where one would write out TTree
    if (ana.write_tree)
    {
        switch (ana.looperMode)
        {
            case AnalysisConfig::k4LepMET: PostProcess_4LepMET(); break;
        }
    }
    //std::cout << "Debug Process 5" << std::endl;

    // Reset all the variables!
    ana.tx.clear();
}
