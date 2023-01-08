#!/bin/bash
cp "dataset_cicli/cicli/input/input$1.txt" "input.txt";
crun "cicli.cpp";
if diff "output.txt" "dataset_cicli/cicli/output/output$1.txt";
  then
    echo "$1 good";
  else
    echo "$1 bad";
    echo "$1" > ./.graphs/num
    case $2 in
      y | yes | image | print) ./print_graph.py;;
    esac
    nohup p4merge "output.txt" "dataset_cicli/cicli/output/output$1.txt" &
fi