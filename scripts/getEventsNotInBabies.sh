function getEventsNotInBabies {
  num_low=`cat $1 | grep -n "Events that weren't in your babies" | cut -d':' -f1`
  num_high=`cat $1 | grep -n "Num events passed" | cut -d':' -f1`

  num_low=$((num_low+1))
  num_high=$((num_high-1))

  while [[ $((num_low+30)) -leq $num_high ]]
  do
    echo -n "ch->Scan(\"evt_event:evt_run:evt_lumiBlock\", \""
    cat $1 | sed -n "${num_low},$((num_low+30))p" | awk '{print " (evt_event==" $2 " && evt_run==" $4 " && evt_lumiBlock==" $6 " ) ||"}' | xargs
    echo -n "\")"
    num_low=$((num_low+30))
  done
  
  echo -n "ch->Scan(\"evt_event:evt_run:evt_lumiBlock\", \""
  cat $1 | sed -n "${num_low},${num_high}p" | awk '{print " (evt_event==" $2 " && evt_run==" $4 " && evt_lumiBlock==" $6 " ) ||"}' | xargs
  echo -n "\")"

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

