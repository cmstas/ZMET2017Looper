#Derive vpt_rwt
echo "Deriving vpt_rwt and checking closure"
for sampleName in ZJets GammaJets
do
    echo "nohup ./ZMETLooper $sampleName configs/2017/DoubleLepton_narrow/MCTemplates/run_modes.conf > MCTemplates-$sampleName.out &"

    nohup ./ZMETLooper $sampleName configs/2017/DoubleLepton_narrow/MCTemplates/run_modes.conf > MCTemplates-$sampleName.out &
done
wait
#plot closure
echo "Closure histograms"
sampleName=GammaJets_reweight
nohup ./ZMETLooper $sampleName configs/2017/DoubleLepton_narrow/MCTemplates/run_modes.conf > MCTemplates-$sampleName.out &

mkdir -p ../hists2017/DoubleLepton_narrow/dataTemplates
for sampleName in ZJets GammaJets
do
    cp ../hists2017/DoubleLepton_narrow/MCTemplates/$sampleName.root ../hists2017/DoubleLepton_narrow/dataTemplates/$(sampleName)_MC.root
done
#Do reweight of GammaData and EWKSub MCs
for sampleName in GammaData WJets-EWKSub WGamma-EWKSub TTJets-2lep-EWKSub TTJets-1lep-EWKSub SingleTop-EWKSub
do
    echo "nohup ./ZMETLooper $sampleName configs/2017/DoubleLepton_narrow/dataTemplates/run_modes.conf > dataTemplates-$sampleName.out &"
    nohup ./ZMETLooper $sampleName configs/2017/DoubleLepton_narrow/dataTemplates/run_modes.conf > dataTemplates-$sampleName.out &
done

