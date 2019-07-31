declare -a years=(2016 2017 2018)
#Templates derivation
echo "Running over gamma MC and Z MC"
for year in "${years[@]}"
do
    for sampleName in ZJets GammaJets
    do
        echo "nohup ./ZMETLooper $sampleName configs/threeyears/MCTemplates/run_modes.conf $year zmet_datasets_$year.txt > MCTemplates-$sampleName-$year.out &"
        nohup ./ZMETLooper $sampleName configs/threeyears/MCTemplates/run_modes.conf $year zmet_datasets_$year.txt > MCTemplates-$sampleName-$year.out &
    done
done
wait

echo "Deriving vpt_rwt and making closure histograms"
sampleName = GammaJets_reweight

for year in "${years[@]}"
do
    echo "nohup ./ZMETLooper $sampleName configs/threeyears/MCTemplates/run_modes.conf $year zmet_datasets_$year.txt > MCTemplates-$sampleName-$year.out &"
    nohup ./ZMETLooper $sampleName configs/threeyears/MCTemplates/run_modes.conf $year zmet_datasets_$year.txt > MCTemplates-$sampleName-$year.out &
done
