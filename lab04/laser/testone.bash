#!/bin/bash
cp "dataset_laser/laser/input/input$1.txt" "input.txt";
crunt "laser.cpp";
head -n1 "output.txt" > "output1.txt";
if diff "output1.txt" "dataset_laser/laser/output/output$1.txt";
  then
    echo "$1 good";
  else
    echo "$1 bad";
    echo "$1" > ./.graphs/num
    case $2 in
      y | yes | image | print) ./print_graph.py;;
    esac
    nohup p4merge "output.txt" "dataset_laser/laser/output/output$1.txt" &
fi