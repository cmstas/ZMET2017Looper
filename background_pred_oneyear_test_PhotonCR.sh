year=$1
prefix="/home/users/bsathian/ZMet/histsoneyear"

mkdir -p $prefix/PhotonCR/$year
for sampleName in GammaData GammaJets WJets-EWKSub WGamma-EWKSub TTJets-2lep-EWKSub TTJets-1lep-EWKSub SingleTop-EWKSub
do

    echo "nohup ./ZMETLooper $sampleName $2 $year zmet_datasets_$year.txt > PhotonCR_test-$sampleName-$year.out &"
    nohup ./ZMETLooper $sampleName $2 $year zmet_datasets_$year.txt > PhotonCR_test-$sampleName-$year.out &
    done
wait

