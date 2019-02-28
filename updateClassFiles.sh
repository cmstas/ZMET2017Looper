
updateClassFiles_BabyPath=$1
if [[ $# < 1 ]]
then
  echo "./updateClassFiles.sh /path/to/example/baby.root"
  read -e -p "Example Baby Path:" updateClassFiles_BabyPath
fi

if [[ ! -a $updateClassFiles_BabyPath ]]
then
  echo "The file $1 could not be found."
  echo "./updateClassFiles.sh /path/to/example/baby.root"
else
  pushd /home/users/bhashemi/Projects/GIT/Software/
  git pull 
  git checkout root6
  cd makeCMS3ClassFiles
  root -l -b -q "makeCMS3ClassFiles.C(\"$updateClassFiles_BabyPath\", \"t\", \"ZMET2016\", \"zmet\", \"phys\")"
  mv ZMET2016.* /home/users/bhashemi/Projects/GIT/ZMETBabyLooper2017/
  popd
fi