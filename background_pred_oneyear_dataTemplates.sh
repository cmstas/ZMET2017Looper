year=()
prefix="/home/users/bsathian/ZMet/histsthreeyears/"
echo "Running over GammaData and EWKSub"

mkdir -p $prefix/dataTemplates/$year
cp $prefix/MCTemplates/$year/ZJets.root $prefix/dataTemplates/$year/ZJets_MC.root
cp $prefix/MCTemplates/$year/GammaJets.root $prefix/dataTemplates/$year/GammaJets_MC.root

for sampleName in GammaData WJets-EWKSub WGamma-EWKSub TTJets-2lep-EWKSub TTJets-1lep-EWKSub SingleTop-EWKSub
do
    echo "nohup ./ZMETLooper $sampleName configs/threeyears/dataTemplates/run_modes.conf $year zmet_datasets_$year.txt > dataTemplates-$sampleName-$year.out &"
    nohup ./ZMETLooper $sampleName configs/threeyears/dataTemplates/run_modes.conf $year zmet_datasets_$year.txt > dataTemplates-$sampleName-$year.out &
done
wait

