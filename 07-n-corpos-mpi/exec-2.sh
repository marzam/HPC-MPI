#!/bin/bash
rm *.txt
i=100
while [ $i -lt 10000 ];
do
  if [ $i -lt 1000 ]; then
    let i=i+100
  else
    let i=i+1000
  fi
  echo $i
  /usr/bin/time -ao n-bodies-openmp.txt ./n-bodies-openmp 100 $i 0 >> n-bodies-openmp-mem.txt
  /usr/bin/time -ao n-bodies-openmp-st.txt ./n-bodies-openmp-st 100 $i 0 >> n-bodies-openmp-mem-st.txt
  /usr/bin/time -ao n-bodies.txt ./n-bodies 100 $i 0 >> n-bodies-mem.txt
done
