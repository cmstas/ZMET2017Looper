for configName in DoubleMuon DoubleElectron MuonElectron DYJets TTJets SingleTop TTZ TTW WW WZTo2L2Q WZTo3LNu ZZ2L ZZ4L
do
    nohup ./ZMETLooper $configName configs/2017/DoubleLepton/run_modes.conf > $configName.txt &
done

