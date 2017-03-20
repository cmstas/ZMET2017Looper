function getEventsNotInBabies {
  num_low=`cat $1 | grep -n "Events that weren't in your babies" | cut -d':' -f1`
  num_high=`cat $1 | grep -n "Num events passed" | cut -d':' -f1`

  sed -n '${num_low},${num_high}p'
}

if [[ $# < 1 ]]
then
  echo "getEventsNotInBabies path/to/output/of/single/sample"
  return 1
elif [[ ! -a $1 ]]
then
  echo "Could not find file: $1"
  return 1
else
  getEventsNotInBabies $1
fi

