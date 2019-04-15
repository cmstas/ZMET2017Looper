for hyp_type in ee mumu emu
do
    for configName in DoubleMuon DoubleElectron MuonElectron DYJets TTJets SingleTop TTZ TTW WW WZTo2L2Q WZTo3LNu ZZ2L ZZ4L
    do
        echo "nohup ./ZMETLooper $configName configs/2017/DoubleLepton/$hyp_type/run_modes.conf > $configName-$hyp_type.txt &"
        nohup ./ZMETLooper $configName configs/2017/DoubleLepton/$hyp_type/run_modes.conf > $configName-$hyp_type.txt &
    done
done
