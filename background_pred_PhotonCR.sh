declare -a years=(2016 2017 2018)
prefix="/home/users/bsathian/ZMet/histsthreeyears"

for year in "${years[@]}"
do
    mkdir -p $prefix/PhotonCR/$year
    for sampleName in GammaData WJets-EWKSub WGamma-EWKSub TTJets-2lep-EWKSub TTJets-1lep-EWKSub SingleTop-EWKSub
    do

        echo "nohup ./ZMETLooper $sampleName configs/threeyears/PhotonCR/run_modes.conf $year zmet_datasets_$year.txt > PhotonCR-$sampleName-$year.out &"
        nohup ./ZMETLooper $sampleName configs/threeyears/PhotonCR/run_modes.conf $year zmet_datasets_$year.txt > PhotonCR-$sampleName-$year.out &
    done
done
wait
echo "Adding up three years worth of histograms"
mkdir -p $prefix/PhotonCR/combined

for sampleName in GammaData WJets-EWKSub WGamma-EWKSub TTJets-2lep-EWKSub TTJets-1lep-EWKSub SingleTop-EWKSub
do
    hadd -f -k $prefix/PhotonCR/combined/$sampleName.root $prefix/PhotonCR/2016/$sampleName.root $prefix/PhotonCR/2017/$sampleName.root $prefix/PhotonCR/2018/$sampleName.root

done
