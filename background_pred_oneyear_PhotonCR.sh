year=$1
prefix="/home/users/bsathian/ZMet/histsoneyear"

mkdir -p $prefix/PhotonCR/$year
for sampleName in GammaData GammaJets WJets-EWKSub WGamma-EWKSub TTJets-2lep-EWKSub TTJets-1lep-EWKSub SingleTop-EWKSub
do

    echo "nohup ./ZMETLooper $sampleName configs/oneyear/PhotonCR/run_modes.conf $year zmet_datasets_$year.txt > PhotonCR-$sampleName-$year.out &"
    nohup ./ZMETLooper $sampleName configs/oneyear/PhotonCR/run_modes.conf $year zmet_datasets_$year.txt > PhotonCR-$sampleName-$year.out &
    done
wait

