#NOTE : I could've obviously added 3 years worth of stuff into a single TChain and sat back
#but turns out with a little effort I can loop over every year separately and then combine whenever needed

declare -a years=(2016 2017 2018)
prefix="/home/users/bsathian/ZMet/histsthreeyears/"
subdirectory=$1 #for b-tag or b-veto stuff
for year in "${years[@]}"
do
    for sampleName in Data MC
    do
        echo "nohup ./ZMETLooper $sampleName configs/threeyears/kappa_studies/$subdirectory/run_modes.conf $year zmet_datasets_$year.txt > kappa-$sampleName-$subdirectory-$year.out &"
        nohup ./ZMETLooper $sampleName configs/threeyears/kappa_studies/$subdirectory/run_modes.conf $year zmet_datasets_$year.txt > kappa-$sampleName-$subdirectory-$year.out &
    done
done
wait

echo "Adding three years histograms together"
mkdir -p $prefix/kappa_studies/$subdirectory/combined
for sampleName in Data MC
do
    hadd -f -k $prefix/kappa_studies/$subdirectory/combined/$sampleName.root $prefix/kappa_studies/$subdirectory/2016/$sampleName.root $prefix/kappa_studies/$subdirectory/2017/$sampleName.root $prefix/kappa_studies/$subdirectory/2018/$sampleName.root
done
