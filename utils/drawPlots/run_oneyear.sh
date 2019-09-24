year=$1
config_file=../../configs/oneyear/$year/statsplots.conf

for SR in SRA SRB SRC SRAb SRBb SRCb SRVZBoosted SRVZResolved SRHZ VRA VRB VRC VRWZBoosted VRWZResolved VRHZ
do
    ./drawPlots $config_file $SR
done
