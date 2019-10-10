declare -a years=(2016 2017 2018)
prefix="/home/users/bsathian/ZMet/histsthreeyears/"
sample=("T5ZZ" "TChiZZ" "TChiWZ" "TChiHZ")

mkdir -p $prefix/fastsim/combined

for year in "${years[@]}"
do
    mkdir -p $prefix/fastsim/$year
    for sampleName in "${sample[@]}"
    do
        echo "nohup ./ZMETLooper $sampleName configs/threeyears/fastsim/run_modes.conf $year zmet_datasets_$year.txt > fastsim-$sampleName-$year.out &"
        nohup ./ZMETLooper $sampleName configs/threeyears/fastsim/run_modes.conf $year zmet_datasets_$year.txt > fastsim-$sampleName-$year.out &
    done
done
wait

#hadd the fastsim histograms

for sampleName in "${sample[@]}"
do
    hadd -f -k $prefix/fastsim/combined/$sampleName.root $prefix/fastsim/2016/$sampleName.root $prefix/fastsim/2017/$sampleName.root $prefix/fastsim/2018/$sampleName.root
done
