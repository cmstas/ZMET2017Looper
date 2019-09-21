SR=$1
histsprefix="/home/users/bsathian/histstemplate_comparison/$SR/"
for sample in cleanGamma cleanZ MCTemplates
do
    echo "nohup ./ZMETLooper all configs/template_comparison/$SR/$sample/run_modes.conf 2017 zmet_datasets_2017.txt > template_comparison_$SR\_$sample.out &"
    nohup ./ZMETLooper all configs/template_comparison/$SR/$sample/run_modes.conf 2017 zmet_datasets_2017.txt > template_comparison_$SR\_$sample.out &
done
wait

#Add GammaData to create GammaData-EWKSub
hadd -f -k $histsprefix/cleanGamma/GammaData-EWKSub.root $histsprefix/cleanGamma/GammaData.root $histsprefix/cleanGamma/WGamma-EWKSub.root $histsprefix/cleanGamma/WJets-EWKSub.root $histsprefix/cleanGamma/TTJets-2lep-EWKSub.root $histsprefix/cleanGamma/TTJets-1lep-EWKSub.root $histsprefix/cleanGamma/SingleTop-EWKSub.root
mkdir -p $histsprefix/reweightMCGamma/
cp $histsprefix/MCTemplates/ZJets.root $histsprefix/reweightMCGamma/ZJets-MC.root
cp $histsprefix/MCTemplates/GammaJets.root $histsprefix/reweightMCGamma/GammaJets-MC.root

for sample in reweightGamma reweightMCGamma
do
    echo "nohup ./ZMETLooper all configs/template_comparison/$SR/$sample/run_modes.conf 2017 zmet_datsets_2017.txt > template_comparison_$SR\_$sample.out &"
    nohup ./ZMETLooper all configs/template_comparison/$SR/$sample/run_modes.conf 2017 zmet_datsets_2017.txt > template_comparison_$SR\_$sample.out &
done
