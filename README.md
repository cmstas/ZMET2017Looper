# 2017 ZMET Baby Looper

Baby looper for the 2017 ZMET Analysis.

This repository contains the code that runs on all the 2017 ZMET babies

The babies are made with the [ZMET2016 code in the cmstas group](https://github.com/cmstas/ZMET2016). 

Use branch "Send"

Running the full prediction:

. funcs.sh

makeAllForDir configs/prediction/DileptonData all
makeAllForDir configs/prediction/FS all
makeAllForDir configs/prediction/PhotonData all
makeAllForDir configs/prediction/ZNu all

You can run `numjobs v` to check the progress. After PhotonData is finished, you can run the VPTRWT branch, which will automatically make the reweight histograms for you:

makeAllForDir configs/prediction/PhotonData_VPTRWT all

Once all those finish, you can run the final configs that make the plots. These are in `configs/prediction/Final_for_paper`, so run

makeAllForDir configs/prediction/Final_for_paper plots

Everything automatically output to directories set in ConfigHelper.C, plots are automatically generated and dropped into your ~/public_html. 
