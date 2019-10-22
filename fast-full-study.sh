declare -a years=(2016 2017 2018)
prefix="/home/users/bsathian/ZMet/histsthreeyears/"
sample=("TChiWZfullsim" "TChiWZfastsim")

mkdir -p $prefix/TChiWZ_full_fast_studies/combined/
for year in "${years[@]}"
do
    mkdir -p $prefix/TChiWZ_full_fast_studies/$year
    for sampleName in "${sample[@]}"
    do
    	echo "nohup ./ZMETLooper $sampleName configs/threeyears/TChiWZ_full_fast_studies/run_modes.conf $year zmet_datasets_$year.txt > full_fast_studies-$sampleName-$year.out &"
        nohup ./ZMETLooper $sampleName configs/threeyears/TChiWZ_full_fast_studies/run_modes.conf $year zmet_datasets_$year.txt > full_fast_studies-$sampleName-$year.out &
    done
done
wait

#hadd the fastsim histograms
for mode in "${modes[@]}"
do
    for sampleName in "${sample[@]}"
    do
        hadd -f -k $prefix/TChiWZ_full_fast_studies/combined/$sampleName.root $prefix/TChiWZ_full_fast_studies/2016/$sampleName.root $prefix/TChiWZ_full_fast_studies/2017/$sampleName.root $prefix/TChiWZ_full_fast_studies/2018/$sampleName.root
    done
done
