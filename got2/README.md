## corbec++

```
cat inputcor.txt | ./corbec++ > output.txt
```

```
p cep n m
```
oss: I lati devono essere 1...n e non 0...n-1

### automate
```
declare -a index=(
  0 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19
);

for i in "${index[@]}"; do
  cp "dataset_got2/input/input$i.txt" "input.txt";
  crun tocor.cpp;
  cat inputcor.txt | ./corbec++ > output.txt;
  cp "output.txt" "dataset_got2/output/output$i.txt";
done
```