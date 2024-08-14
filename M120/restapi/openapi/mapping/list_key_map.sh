#!/bin/bash
#go over all the yang model and find key of the list 

  #find the type of leaf node from yang model
  pyang -f tree ../../../../yang-model/oplink/*/*.yang | grep ']' > list_key.json 
  #remove prefix, yang module module name and ? mark
  sed -i -e 's/^.*+--..//g; s/^.*x--..//g; s/^.*://g; s/?//g' list_key.json 
  #remove white sapce at begining
  sed -i -e 's/^[ \t]*//' list_key.json 
  #remove duplicates
  sort list_key.json | uniq > temp && mv temp list_key.json
  #remove invalide lines
  sed -i -e '/}$/d' list_key.json
  # add string "
  sed -i -e 's/^/    \"/; s/* \[/\": \"/; s/\]/\",/' list_key.json

  #remove invalid lines
  sed -i -e '/ -> /d' list_key.json
  sed -i -e '/""/d' list_key.json
  sed -i -e '/current()/d' list_key.json 

  #remove , in last line
  sed -i -e '$ s/,//' list_key.json
  #add { and }
  sed -i -e '1s/^/{\n/' list_key.json
  echo -e  "}" >> list_key.json
  cat list_key.json

  sudo cp list_key.json /etc/rest
