# asd
Repo containing all the exercises done to prepare the exam asd@unitn held by Alberto Montresor

## libs
These few scripts will give you an edge in the asd projects!

Link them to `/usr/local/bin` with `ln -s`.

### Workflow

`buildpr->genin->viewin->crun->genplt->viewplt`

testall, testone, loadtest, checktest are redundant and provide a `-n` option to not use p4merge.

- `buildpr`

Sets up the directory to this structure:

```
--dataset_<pr>/
  |--input/inputi.txt
  |--output/outputi.txt
  |--plt/
      |--rplti.png

--randset_<pr>/
  |--input/rinputi.txt
  |--plt/
      |--rplti.png
  |--num

--input.txt
--output.txt
--info.txt
--plt.png
--<pr>.cpp
```

- `pcrun [-d <number> | -r [number]] [-p]`  
Merges all utilities in one cli script!

- `viewin <-d <number> | -r <number>>`

- `viewplt <-d <number> | -r <number>>`

- `genin`

- `genplt [-d <number> | -r [number]] [-I]`

- `testone <number> [-n]`

- `testall [-n]`

- `checktest [-n]`

- `loadtest`
