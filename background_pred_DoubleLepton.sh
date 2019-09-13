declare -a years=(2016 2017 2018)
prefix="/home/users/bsathian/ZMet/histsthreeyears/"
sample=("Data" "DYJets" "TTJets" "SingleTop" "TTZ" "TTW" "WW" "WZTo2L2Q" "WZTo3LNu" "ZZ2L" "ZZ4L" "GluGluToZZ" "VVV")

#Do a hadd for the EWK Sample to get the total Templates prediction
mkdir -p $prefix/DoubleLepton/combined
hadd -f -k $prefix/DoubleLepton/combined/Templates.root $prefix/dataTemplates/combined/GammaData.root $prefix/dataTemplates/combined/WGamma-EWKSub.root $prefix/dataTemplates/combined/WJets-EWKSub.root $prefix/dataTemplates/combined/TTJets-1lep-EWKSub.root $prefix/dataTemplates/combined/TTJets-2lep-EWKSub.root $prefix/dataTemplates/combined/SingleTop-EWKSub.root

for year in "${years[@]}"
do
    mkdir -p $prefix/DoubleLepton/$year
    for sampleName in "${sample[@]}"
    do
        echo "nohup ./ZMETLooper $sampleName configs/threeyears/DoubleLepton/run_modes.conf $year zmet_datasets_$year.txt > DoubleLepton-$sampleName-$year.out &"
        nohup ./ZMETLooper $sampleName configs/threeyears/DoubleLepton/run_modes.conf $year zmet_datasets_$year.txt > DoubleLepton-$sampleName-$year.out &
    done
done
wait

#hadd the DoubleLepton histograms
for sampleName in "${sample[@]}"
do
    hadd -f -k $prefix/DoubleLepton/combined/$sampleName.root $prefix/DoubleLepton/2016/$sampleName.root $prefix/DoubleLepton/2017/$sampleName.root $prefix/DoubleLepton/2018/$sampleName.root
done
