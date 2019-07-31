declare -a years=(2016 2017 2018)
prefix="/home/users/bsathian/ZMet/histsthreeyears/"
sample=("Data" )
echo "Running over GammaData and EWKSub"

for year in "${years[@]}"
do
    mkdir -p $prefix/dataTemplates/$year
    cp $prefix/MCTemplates/ZJets.root $prefix/dataTemplates/year/ZJets_MC.root
        cp $prefix/MCTemplates/GammaJets.root $prefix/dataTemplates/year/GammaJets_MC.root
done

for year in "${years[@]}"
do
    for sampleName in GammaData WJets-EWKSub WGamma-EWKSub TTJets-2lep-EWKSub TTJets-1lep-EWKSub SingleTop-EWKSub
    do
        echo "nohup ./ZMETLooper $sampleName configs/threeyears/dataTemplates/run_modes.conf $year zmet_datasets_$year.txt > dataTemplates-$sampleName-$year.out &"
        nohup ./ZMETLooper $sampleName configs/threeyears/dataTemplates/run_modes.conf $year zmet_datasets_$year.txt > dataTemplates-$sampleName-$year.out &
    done
done
wait

echo "Adding up three years worth of histograms"
mkdir -p $prefix/dataTemplates/combined
for sampleName in GammaData WJets-EWKSub WGamma-EWKSub TTJets-2lep-EWKSub TTJets-1lep-EWKSub SingleTop-EWKSub
do
    hadd -f -k $prefix/dataTemplates/combined/$sampleName.root $prefix/dataTemplates/2016/$sampleName.root $prefix/dataTemplates/2017/$sampleName.root $prefix/dataTemplates/2018/$sampleName.root
done

