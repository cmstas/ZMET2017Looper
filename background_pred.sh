#NOTE : I could've obviously added 3 years worth of stuff into a single TChain and sat back
#but turns out with a little effort I can loop over every year separately and then combine whenever needed

declare -a years=(2016 2017 2018)
prefix="/home/users/bsathian/ZMet/histsthreeyears/"
#Templates derivation
echo "Running over gamma MC and Z MC"
for year in "${years[@]}"
do
    for sampleName in ZJets GammaJets
    do
        echo "nohup ./ZMETLooper $sampleName configs/threeyears/MCTemplates/run_modes.conf $year zmet_datasets_$year.txt > MCTemplates-$sampleName-$year.out &"
        nohup ./ZMETLooper $sampleName configs/threeyears/MCTemplates/run_modes.conf $year zmet_datasets_$year.txt > MCTemplates-$sampleName-$year.out &
    done
done
wait

#DERIVING vpt_rwt
#vpt_rwt needs to be done on 140 fb^-1 of data. So we combine three years
#worth of histograms into a folder, hadd it, and send the combination back to reweight. Then we combine the reweighted GJets to get closure plots

echo "Deriving vpt_rwt and making closure histograms"
mkdir -p $prefix/MCTemplates/combined

#hadd the copied stuff. HARDCODING THIS MOFO
echo "hadd -f -k $prefix/MCTemplates/combined/GammaJets.root $prefix/MCTemplates/2016/GammaJets.root $prefix/MCTemplates/2017/GammaJets.root $prefix/MCTemplates/2018/GammaJets.root"

hadd -f -k $prefix/MCTemplates/combined/GammaJets.root $prefix/MCTemplates/2016/GammaJets.root $prefix/MCTemplates/2017/GammaJets.root $prefix/MCTemplates/2018/GammaJets.root

echo "hadd -f -k $prefix/MCTemplates/combined/ZJets.root $prefix/MCTemplates/2016/ZJets.root $prefix/MCTemplates/2017/ZJets.root $prefix/MCTemplates/2018/ZJets.root"

hadd -f -k $prefix/MCTemplates/combined/ZJets.root $prefix/MCTemplates/2016/ZJets.root $prefix/MCTemplates/2017/ZJets.root $prefix/MCTemplates/2018/ZJets.root

for year in "${years[@]}"
do
    for sampleName in ZJets GammaJets
    do
        cp $prefix/MCTemplates/combined/$sampleName.root $prefix/MCTemplates/$year/$sampleName-combined.root
    done
done

#Now with the full histograms in the respective folders, do the vpt_rwt
sampleName="GammaJets_reweight"

for year in "${years[@]}"
do
    echo "nohup ./ZMETLooper $sampleName configs/threeyears/MCTemplates/run_modes.conf $year zmet_datasets_$year.txt > MCTemplates-$sampleName-$year.out &"
    nohup ./ZMETLooper $sampleName configs/threeyears/MCTemplates/run_modes.conf $year zmet_datasets_$year.txt > MCTemplates-$sampleName-$year.out &
done
wait
#hadd the reweighted GammaJets and put them in combined folder

echo "hadd -f -k $prefix/MCTemplates/combined/$sampleName.root $prefix/MCTemplates/2016/$sampleName.root $prefix/MCTemplates/2017/$sampleName.root $prefix/MCTemplates/2018/$sampleName.root"

hadd -f -k $prefix/MCTemplates/combined/$sampleName.root $prefix/MCTemplates/2016/$sampleName.root $prefix/MCTemplates/2017/$sampleName.root $prefix/MCTemplates/2018/$sampleName.root
