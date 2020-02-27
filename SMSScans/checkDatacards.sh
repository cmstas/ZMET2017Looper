signalmod=$(echo "$1" | awk '{print tolower($0)}')

prefix="/home/users/bsathian/ZMet/ZMETBabyLooper2017/SMSScans"
mkdir -p $prefix/datacard_checks/$signalmod
pushd $prefix/datacard_checks/$signalmod

declare -a files
declare -a logfilenames
declare -a pullfilenames
declare -a directories
if [ "$signalmod" == "t5zz" ]
then
    dir=$prefix/DataCards/T5ZZ
    declare -A masses
    masses[1600]=700
    masses[1800]=1000
    masses[2000]=600
    counter=1
    for mG in "${!masses[@]}"; do
        mLSP=${masses[$mG]}
        mkdir -p ${mG}_${mLSP}
        directories[counter]=${mG}_${mLSP}

        files[counter]="$dir/datacard_all_mGluino_${mG}_mLSP_${mLSP}_.txt"
        logfilenames[counter]="log_${mG}_${mLSP}"
        pullfilenames[counter]="pulls_${mG}_${mLSP}"
        counter=$((counter+1))
    done
elif [ "$signalmod" == "tchiwz" ]
then
    dir=$prefix/DataCards/TChiWZ
    declare -A masses
    masses[400]=200
    masses[600]=300
    masses[700]=400
    counter=1
    for mG in "${!masses[@]}"; do
        mLSP=${masses[$mG]}
        mkdir -p ${mG}_${mLSP}
        directories[counter]=${mG}_${mLSP}

        files[counter]="$dir/datacard_all_mGluino_${mG}_mLSP_${mLSP}_.txt"
        logfilenames[counter]="log_${mG}_${mLSP}"
        pullfilenames[counter]="pulls_${mG}_${mLSP}"
        counter=$((counter+1))
    done

elif [ "$signalmod" == "tchizz" ]
then
    dir=$prefix/DataCards/TChiZZ
    declare -a masses
    masses[1]=300
    masses[2]=600
    masses[3]=800
    counter=1
    for mChi in "${masses[@]}"; do
        mkdir -p ${mChi}
        directories[counter]=${mChi}

        files[counter]="$dir/datacard_all_mChi_${mChi}_.txt"
        logfilenames[counter]="log_${mChi}"
        pullfilenames[counter]="pulls_${mChi}"
        counter=$((counter+1))
    done

elif [ "$signalmod" == "tchihz" ]
then
    dir=$prefix/DataCards/TChiHZ
    masses[1]=300
    masses[2]=600
    masses[3]=800
    counter=1
    for mChi in "${masses[@]}"; do
        mkdir -p ${mChi}
        directories[counter]=${mChi}

        files[counter]="$dir/datacard_all_mChi_${mChi}_.txt"
        logfilenames[counter]="log_${mChi}"
        pullfilenames[counter]="pulls_${mChi}"
        counter=$((counter+1))
    done
fi

cmssw_prefix="/home/users/bsathian/ZMet/babymaker/CMSSW_9_4_9/babymaker/ZMET2015/limitcode/CMSSW_7_4_7/"
if  [ hash combine 2>/dev/null ]; then
    echo "combine not found! Using CMSSW_7_4_7 combine"
    pushd $cmssw_prefix
    eval `scramv1 runtime -sh`
    popd
fi

#Do the checks here
counter=1
diffLocation="$cmssw_prefix/src/HiggsAnalysis/CombinedLimit/test/diffNuisances.py"

for file in "${files[@]}"
do
    pushd ${directories[counter]}
    echo "combine -M MaxLikelihoodFit -t -1 --expectSignal 0 --saveWithUncertainties --saveOverallShapes --numToysForShapes 200 --plots $file 1>${logfilenames[counter]}_nosignal.out 2>&1"
    combine -M MaxLikelihoodFit -t -1 --expectSignal 0  --saveWithUncertainties --saveOverallShapes --numToysForShapes 200 --plots $file 1>${logfilenames[counter]}_nosignal.out 2>&1
    mkdir plots_b_only
    mv *.png plots_b_only

    echo "python $diffLocation mlfit.root --abs 1>${pullfilenames[counter]}_nosignal.out 2>&1"
    python $diffLocation mlfit.root --abs 1>${pullfilenames[counter]}_nosignal.out 2>&1

    mv mlfit.root mlfit_b_only.root

    echo "combine -M MaxLikelihoodFit -t -1 --expectSignal 1 --saveWithUncertainties --saveOverallShapes --numToysForShapes 200 --plots $file 1>${logfilenames[counter]}_withsignal.out 2>&1"
    combine -M MaxLikelihoodFit -t -1 --expectSignal 1  --saveWithUncertainties --saveOverallShapes --numToysForShapes 200 --plots $file 1>${logfilenames[counter]}_withsignal.out 2>&1
    mkdir plots_sb
    mv *.png plots_sb

    echo "python $diffLocation mlfit.root --abs 1>${pullfilenames[counter]}_withsignal.out 2>&1"
    python $diffLocation mlfit.root --abs 1>${pullfilenames[counter]}_withsignal.out 2>&1

    mv mlfit.root mlfit_sb.root
    echo "python $prefix/plot_covariance_sb.py"
    python $prefix/plot_covariance_sb.py

    popd
    counter=$((counter+1))
done
popd

#correlation stuff
pushd /home/users/bsathian/babymaker/CMSSW_9_4_9
eval `scramv1 runtime -sh`
popd

counter=1
for file in "${files[@]}"
do
    pushd ${directories[counter]}

    echo "python $prefix/plot_covariance.py"
    python $prefix/plot_covariance.py

    echo "python $prefix/plot_covariance_sb.py"
    python $prefix/plot_covariance_sb.py
    popd
done
popd
