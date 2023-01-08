#!/bin/bash
declare -a index=(
  0 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19
);
declare ngood=0;
declare nbad=0;

for i in "${index[@]}"; do
  cp "dataset_laser/laser/input/input$i.txt" "input.txt";
  crunt "laser.cpp";
  head -n1 "output.txt" > "output1.txt";
  if diff "output1.txt" "dataset_laser/laser/output/output$i.txt";
    then
      echo "$i good";
      ngood=$((ngood+1));
    else
      echo "$i bad";
      nbad=$((nbad+1));
      case $1 in
        n | no | fast ) ;;
        *) p4merge "output.txt" "dataset_laser/laser/output/output$i.txt";
      esac
  fi
done
  echo "=======";
  echo "$ngood/$((ngood+nbad))"