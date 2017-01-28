pushd /home/users/bhashemi/Projects/GIT/Software/
git pull 
git checkout root6
cd makeCMS3ClassFiles
root -l -b -q "makeCMS3ClassFiles.C(\"/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-22-09/skims/gjetsht400_dr0p4_mgmlm.root\", \"t\", \"ZMET2016\", \"zmet\", \"phys\")"
mv ZMET2016.* /home/users/bhashemi/Projects/GIT/ZMETBabyLooper2017/
popd
