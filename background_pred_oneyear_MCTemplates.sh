year=$1
prefix="/home/users/bsathian/ZMet/histsthreeyears/"
#Templates derivation
echo "Running over gamma MC and Z MC"
for sampleName in ZJets GammaJets
do
    echo "nohup ./ZMETLooper $sampleName configs/threeyears/MCTemplates/run_modes.conf $year zmet_datasets_$year.txt > MCTemplates-$sampleName-$year.out &"
    nohup ./ZMETLooper $sampleName configs/threeyears/MCTemplates/run_modes.conf $year zmet_datasets_$year.txt > MCTemplates-$sampleName-$year.out &
done
wait

#DERIVING vpt_rwt

#Now with the full histograms in the respective folders, do the vpt_rwt
sampleName="GammaJets_reweight"

    echo "nohup ./ZMETLooper $sampleName configs/threeyears/MCTemplates/run_modes.conf $year zmet_datasets_$year.txt > MCTemplates-$sampleName-$year.out &"
    nohup ./ZMETLooper $sampleName configs/threeyears/MCTemplates/run_modes.conf $year zmet_datasets_$year.txt > MCTemplates-$sampleName-$year.out &
wait


