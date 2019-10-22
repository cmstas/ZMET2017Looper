prefix="/home/users/bsathian/ZMet/histsthreeyears/"
modes=("CV" "JES_up" "JES_dn" "GenMET")
sample=("T5ZZ" "TChiZZ" "TChiWZ" "TChiHZ")

for  mode in "${modes[@]}"
do
    for sampleName in "${sample[@]}"
    do
        hadd -f -k $prefix/fastsim/$mode/combined/$sampleName.root $prefix/fastsim/$mode/2016/$sampleName.root $prefix/fastsim/$mode/2017/$sampleName.root $prefix/fastsim/$mode/2018/$sampleName.root
    done
done

