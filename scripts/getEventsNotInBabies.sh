function getEventsNotInBabies {
  MAXNUM=50
  num_low=`cat $1 | grep -n "Events that weren't in your babies" | cut -d':' -f1`
  num_high=`cat $1 | grep -n "Num events passed" | cut -d':' -f1`

  num_low=$((num_low+1))
  num_high=$((num_high-1))

  while [[ $(($num_low+$MAXNUM)) -le $num_high ]]
  do
    echo -n "ch->Scan(\"evt_event:evt_run:evt_lumiBlock\", \""
    echo `cat $1 | sed -n "${num_low},$(($num_low+$MAXNUM))p" | awk '{print " (evt_event==" $2 " && evt_run==" $4 " && evt_lumiBlock==" $6 " ) ||"}' | xargs` "\")"
    num_low=$(($num_low+$MAXNUM))
  done
  
  echo -n "ch->Scan(\"evt_event:evt_run:evt_lumiBlock\", \""
  echo `cat $1 | sed -n "${num_low},${num_high}p" | awk '{print " (evt_event==" $2 " && evt_run==" $4 " && evt_lumiBlock==" $6 " ) ||"}' | xargs` "\")"

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

