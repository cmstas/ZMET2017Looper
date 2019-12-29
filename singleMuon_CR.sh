declare -a years=(2016 2017 2018)
prefix="/home/users/bsathian/ZMet/histsthreeyears/"
sample=("MuonData" "TTJets-2lep" "TTJets-1lep" "SingleTop" "WGamma" "WJets" "GammaJets")
#sample=("GammaJets")

mkdir -p $prefix/EWKCR/combined
for year in "${years[@]}"
do
    mkdir -p $prefix/EWKCR/$year
    for sampleName in "${sample[@]}"
    do
        echo "nohup ./ZMETLooper $sampleName configs/threeyears/EWKCR/run_modes.conf $year zmet_datasets_$year.txt > EWKCR-$sampleName-$year.out &"
        nohup ./ZMETLooper $sampleName configs/threeyears/EWKCR/run_modes.conf $year zmet_datasets_$year.txt > EWKCR-$sampleName-$year.out &
    done
    wait
done
wait

for sampleName in "${sample[@]}"
do
    hadd -f -k $prefix/EWKCR/combined/$sampleName.root $prefix/EWKCR/2016/$sampleName.root $prefix/EWKCR/2017/$sampleName.root $prefix/EWKCR/2018/$sampleName.root
done
