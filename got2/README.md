## corbec++

```
cat inputcor.txt | ./corbec++ > output.txt
```

```
p cep n m
```

oss: I lati devono essere 1...n e non 0...n-1

### automate

- some more to do 12 13 15

```
declare -a index=(
  16 17
);

for i in "${index[@]}"; do
  cp "../dataset_got2/input/input$i.txt" "input.txt";
  crun tocor.cpp;
  cat inputcor.txt | timeout 4000s ./corbec++ > output.txt;
  echo "done $i";
  cp "output.txt" "../dataset_got2/output/output$i.txt";
done
```
