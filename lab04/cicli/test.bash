declare -a index=(
  0 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19
);

for i in "${index[@]}"; do
  cp "dataset_cicli/cicli/input/input$i.txt" "input.txt";
  crun "cicli.cpp";
  if diff "output.txt" "dataset_cicli/cicli/output/output$i.txt";
    then echo "$i good";
    else echo "$i bad";
    p4merge "output.txt" "dataset_cicli/cicli/output/output$i.txt";
  fi
done;