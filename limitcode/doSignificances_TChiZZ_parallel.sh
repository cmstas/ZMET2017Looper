#!/bin/bash

function process_datacard ()
{

	datacardname=`basename $1`
	MASS1=$(echo "$datacardname" | awk '{split($0,a,"_"); print a[4]}')

	if [ $MASS1 -ge $MINMASS ]; then

		if [ -e ${INDIR}/datacard_all_mChi_${MASS1}_.txt ]; then
			echo "Running command: nice -n 10 combine -M Significance -n mChi${MASS1}_ ${INDIR}/datacard_all_mChi_${MASS1}_.txt --uncapped 1 --rMin -1 > log/limit_mChi${MASS1}.txt 2>&1"
			nice -n 10 combine -M Significance -n mChi${MASS1}_ ${INDIR}/datacard_all_mChi_${MASS1}_.txt --uncapped 1 --rMin -1  > log/limit_mChi${MASS1}.txt 2>&1

                     python <<EOL 
 
from __future__ import print_function 
import ROOT as r 
import os 
 
try: 
    f = r.TFile("higgsCombinemChi${MASS1}_.Significance.mH120.root") 
    t = f.limit 
except: 
    print("trying brute force! combine -M Significance -n mChi${MASS1}_ -d ${INDIR}/datacard_all_mChi_${MASS1}_.txt --uncapped 1 --rMin -1 --bruteForce > log/limit_mChi${MASS1}.txt 2>&1") 
    os.system("combine -M Significance -n mChi${MASS1}_ ${INDIR}/datacard_all_mChi_${MASS1}_.txt --uncapped 1 --rMin -1  > log/limit_mChi${MASS1}.txt 2>&1") 
EOL
		fi

		if [ -e higgsCombinemChi${MASS1}_.Significance.mH120.root ]; then
			mv higgsCombinemChi${MASS1}_.Significance.mH120.root "limit_${MODEL}_${MASS1}.root"
			echo "limit_${MODEL}_${MASS1}.root" >> list_$MODEL.txt
		fi
	fi

}

MODEL=TChiZZ
#INDIR=/home/users/olivito/zmet_dev/ZMET2016/ZMET2015/limitcode/datacards_TChiZZ_230317/
INDIR=/home/users/bsathian/ZMet/ZMETBabyLooper2017/SMSScans/DataCards/TChiZZ/
MINMASS=100

OUTDIR=significance_${MODEL}_070317_paralleltest
OWD=`pwd`

declare -a cards=(`ls ${INDIR}/datacard_*mC*.txt`)

 #need to combine cards from multiple signal regions if necessary
 for i in "${cards[@]}"
 do
    mG=$(echo "$i" | awk '{split($0,a,"_"); print a[3]"_"a[4]"_"}')
    if [ ! -e "$INDIR/datacard_all_$mG.txt" ]; then
        combineCards.py -S "$INDIR/datacard_"*"_$mG.txt" > "$INDIR/datacard_all_$mG.txt"
    fi
 done

if [ ! -d "$OUTDIR" ]; then
  mkdir -p "$OUTDIR"
fi

if [ ! -d "$OUTDIR/log" ]; then
  mkdir -p "$OUTDIR/log"
fi

cp make_allRValues.C $OUTDIR
pushd $OUTDIR

rm -f list_$MODEL.txt

counter=0
declare -a combinedCards=(`ls ${INDIR}/datacard_all_mC*.txt`)
PID_LIST=''
for i in "${combinedCards[@]}"
do
    process_datacard $i & pid=$!
    PID_LIST+=" $pid";
    if [[ "$counter" -ge 7 ]]; then
	trap "kill $PID_LIST" SIGINT
	wait ${PID_LIST}
	counter=0
	PID_LIST=''
    else
	counter=$((counter+1))
    fi
done

#root -b -q make_allRValues.C+\(\"$MODEL\",\"list_$MODEL.txt\"\)
#cp r-values_$MODEL.root $OWD
popd