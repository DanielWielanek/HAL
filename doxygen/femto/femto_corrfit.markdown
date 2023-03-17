\page femto_corrfit Corrfit tutorial
# Generation of pair file
First step of analysis is generation of file with pairs, this can be done by using standard code for HBT analysis. But instead using a standard code for analysis you should use Hal::FemtoDumpPairAna.
# Generation of scripts
When you get you pair file you should create a configuration files. For this type:
\code{.sh}
hal-corrfit 0
\endcode
This script will create following structure:
* ana.C - macro that calcualte the CF
* corrfit_conf.xml - file with corrfit configuration
* jobs - directory for storing job files
* files - directory for storying calculated CF's
In principle you should modify only two files:
## config_file
This file contains configuration parameters. At the begin we say to the corrfit for how many parameters we want to generate map.
In the example below we want to have maps with Rout/Rside/Rlong=1,2,3,4,5,6,7,8,9,10 fm. This mean generation of 1000 correlation functions.
\code{.xml}
<CorrfitConfig>
<Parameters>
        <Param name="R_{out}" min="1" max="10" points="10"></Param>
        <Param name="R_{side}" min="1" max="10" points="10"></Param>
        <Param name="R_{long}" min="1" max="10" points="10"></Param>
</Parameters>
/endcode
<!-- full path to file with pairs-->
\endcode
Then we specify the path to the pair file.
\code{.xml}
<PairFile>/lustre/nyx/cbm/users/wielanek/corrfit/dumpMc.root</PairFile>
<!-- optional part, use to configure dump pair analysis-->
\endcode
Next step is defining the CF, here the CF is a 3D correlation function in PRF frame. This function contaisn 50 bins from k*=0 up to 0.5 GeV/c. 
\code{.xml}
<DumpAnalysisConf>
        <CorrelationFunction>
                <Name>CF</Name>
                <Frame>EKinematics::kPRF</Frame>
                <Type>Femto3DCF</Type>
<!-- optional part, used only for spherical harmonics-->
                <L>3</L>
                <Xaxis bins="50" min="0.0" max="0.50"></Xaxis>
                <Yaxis bins="50" min="0.0" max="0.50"></Yaxis>
                <Zaxis bins="50" min="0.0" max="0.50"></Zaxis>
        </CorrelationFunction>
\endcode        
In code below we say that we want to generate a gassian 3-dimensional source in LCMS frame.
* The job multiplicity factor mean how many maps will be generated per job (however this function doesn't work yet !).
* Weight multiplicy factor say how many freezouts are generated per pair. For each generation new HBT weight is calculated and pair is filled into CF.
* Preprocessing multiplicity factor - might be used if user want to reuse pair few times by calling preprocess pair few times.
* Calc mode say what kind of data will be used for calculation of CF. S - use signal pairs. B - use background pairs, S+B use signal pairs for numerator and background pairs for denominator.
* Ingore sign - ignore momentum sign when fill the CF.
     
\code{.xml}        
        <FreezoutGenerator>Hal::FemtoFreezoutGeneratorLCMS</FreezoutGenerator>
        <SourceModel>Hal::FemtoSourceModelGauss3D</SourceModel>
        <CalcOptions>
                <JobMultiplyFactor>1</JobMultiplyFactor>
                <WeightMultiplyFactor>1</WeightMultiplyFactor>
                <PreprocessMultiplyFactor>1</PreprocessMultiplyFactor>
                <!-- S/B/S+B for S(signal) B (background)  B+S (both)-->
                <CalcMode>S</CalcMode>
                <IgnoreSign>kTRUE</IgnoreSign>
        </CalcOptions>
\endcode        
Next step is configuration of the weight algorithm.
\code{.xml}             
        <WeightConf>
                <Type>Hal::FemtoWeightGeneratorLednicky</Type>
                <QuantumOn>kTRUE</QuantumOn>
                <StronOn>kFALSE</StronOn>
                <CoulombOn>kFALSE</CoulombOn>
                <PairType>211;211</PairType>
        </WeightConf>
</DumpAnalysisConf>
</CorrfitConfig>
\endcode

## job file

In job file you should change
\code{.cpp}
Hal::CorrFitDumpedPairAna *task = new Hal::CorrFitDumpedPairAna(job_index);
\endcode
to 
\code{.cpp}
Hal::CorrFitDumpedPairAnaHorizontal *task = new Hal::CorrFitDumpedPairAnaHorizontal(job_index);
\endcode

## map generation

Now you can generate the map. For doing this you should call ana.C N-times for each sample in map (in this case 1000). In each job the JOB_ID_HAL is a number of point 
that should be generated (in our case 0-999). You can use hal-jobs for running simulation or your bash scripts.

## merging map
To merge map you have to enter the place with directory with xml configuration file an type:

\code{.sh}
hal-corrfit 1
\endcode
This will merge all generated CF's into one large map file. If there will be problem with missing point in map you will be warned.

