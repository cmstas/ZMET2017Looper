declare -a years=(2016 2017 2018)
prefix="/home/users/bsathian/ZMet/histsthreeyears/"
modes=("CV" "JES_up" "JES_dn" "GenMET")
sample=("T5ZZ" "TChiZZ" "TChiWZ" "TChiHZ")

for mode in "${modes[@]}"
do
    mkdir -p $prefix/fastsim/$mode/combined/
    for year in "${years[@]}"
    do
        mkdir -p $prefix/fastsim/$mode/$year
        for sampleName in "${sample[@]}"
        do
            echo "nohup ./ZMETLooper $sampleName configs/threeyears/fastsim/$mode/run_modes.conf $year zmet_datasets_$year.txt > fastsim-$sampleName-$year.out &"
            nohup ./ZMETLooper $sampleName configs/threeyears/fastsim/$mode/run_modes.conf $year zmet_datasets_$year.txt > fastsim-$sampleName-$mode-$year.out &
        done
    done
done
wait

#hadd the fastsim histograms
for mode in modes
do
    for sampleName in "${sample[@]}"
    do
        hadd -f -k $prefix/fastsim/$mode/combined/$sampleName.root $prefix/fastsim/$mode/2016/$sampleName.root $prefix/fastsim/$mode/2017/$sampleName.root $prefix/fastsim/$mode/2018/$sampleName.root
    done
done
