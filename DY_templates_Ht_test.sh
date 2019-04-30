for SR in ZJets-0100 ZJets-100200 ZJets-200400 ZJets-400600 ZJets-600800 ZJets-8001200 ZJets-12002500 ZJets-2500Inf
do
    nohup ./ZMETLooper $SR configs/2017/Templates/closure/inclusive/run_modes.conf > ht_test-$SR.txt &

done
