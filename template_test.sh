SR=$1
year=2017
histsprefix="/home/users/bsathian/ZMet/histstemplate_comparison/$SR/"
for sample in cleanGamma cleanZ MCTemplates
do
    echo "nohup ./ZMETLooper all configs/template_comparison/$SR/$sample/run_modes.conf 2017 zmet_datasets_2017.txt > template_comparison_$SR\_$sample.out &"
    nohup ./ZMETLooper all configs/template_comparison/$SR/$sample/run_modes.conf 2017 zmet_datasets_2017.txt > template_comparison_$SR\_$sample.out &
done
wait

#Add GammaData to create GammaData-EWKSub
hadd -f -k $histsprefix/cleanGamma/$year/GammaData-EWKSub.root $histsprefix/cleanGamma/$year/GammaData.root $histsprefix/cleanGamma/$year/WGamma-EWKSub.root $histsprefix/cleanGamma/$year/WJets-EWKSub.root $histsprefix/cleanGamma/$year/TTJets-2lep-EWKSub.root $histsprefix/cleanGamma/$year/TTJets-1lep-EWKSub.root $histsprefix/cleanGamma/$year/SingleTop-EWKSub.root

#copy MCTemplates ZJets and GammaJets into reweightMCGamma folder
mkdir -p $histsprefix/reweightMCGamma/$year/
cp $histsprefix/MCTemplates/$year/ZJets.root $histsprefix/reweightMCGamma/$year/ZJets-MC.root
cp $histsprefix/MCTemplates/$year/GammaJets.root $histsprefix/reweightMCGamma/$year/GammaJets-MC.root

for sample in reweightGamma reweightMCGamma
do
    echo "nohup ./ZMETLooper all configs/template_comparison/$SR/$sample/run_modes.conf 2017 zmet_datasets_2017.txt > template_comparison_$SR\_$sample.out &"
    nohup ./ZMETLooper all configs/template_comparison/$SR/$sample/run_modes.conf 2017 zmet_datasets_2017.txt > template_comparison_$SR\_$sample.out &
done
