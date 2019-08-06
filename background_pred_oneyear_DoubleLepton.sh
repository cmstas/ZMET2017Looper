year=$1
prefix="/home/users/bsathian/ZMet/histsthreeyears/"
sample=("Data" "DYJets" "TTJets" "SingleTop" "TTZ" "TTW" "WW" "WZTo2L2Q" "WZTo3LNu" "ZZ2L" "ZZ4L" "GluGluToZZ" "VVV")

#Do a hadd for the EWK Sample to get the total Templates prediction
mkdir -p $prefix/DoubleLepton/$year
hadd -f -k $prefix/DoubleLepton/$year/Templates.root $prefix/dataTemplates/$year/GammaData.root $prefix/dataTemplates/$year/WGamma-EWKSub.root $prefix/dataTemplates/$year/WJets-EWKSub.root $prefix/dataTemplates/$year/TTJets-1lep-EWKSub.root $prefix/dataTemplates/$year/TTJets-2lep-EWKSub.root $prefix/dataTemplates/$year/SingleTop-EWKSub.root

for sampleName in "${sample[@]}"
do
    echo "nohup ./ZMETLooper $sampleName configs/threeyears/DoubleLepton/run_modes.conf $year zmet_datasets_$year.txt > DoubleLepton-$sampleName-$year.out &"
    nohup ./ZMETLooper $sampleName configs/threeyears/DoubleLepton/run_modes.conf $year zmet_datasets_$year.txt > DoubleLepton-$sampleName-$year.out &
done
wait
