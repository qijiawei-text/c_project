#!/bin/bash
#go over all the yaml files and find modules for url path.
FILES=../yaml/*.yaml
#FILES=../yaml/com-oplink-auth.yaml
rm module_path.json
YAML_URL_LIST_FILE=yaml_url_list.tmp

for f in $FILES
do
  filename=${f##*/}
  filename_no_suffix=${filename%.*}
  echo -e "$filename_no_suffix: [" >> module_path.json
  #find first match of data path
  cat $f | grep "/data/" > $YAML_URL_LIST_FILE
  #find match of rpc path
  cat $f | grep "/operations" >> $YAML_URL_LIST_FILE
  #remove redundant lines
  sed -i -e '/={/d' $YAML_URL_LIST_FILE
  #remove extra in yang module name
  #sed -i -e 's/\.yaml//g; s/\.\.\/yaml\///g; s/:$//g' $YAML_URL_LIST_FILE
  sed -i -e 's/\/:$//g' $YAML_URL_LIST_FILE
  sed -i -e 's/:$//g' $YAML_URL_LIST_FILE
  cat $YAML_URL_LIST_FILE >> module_path.json
  #flag the last line, which has no , to be handled at end
  sed -i -e '$ s/.*[a-z0-9]/&++/' module_path.json
  echo -e "  ]," >> module_path.json
done

# clear tmp file
[ -f $YAML_URL_LIST_FILE ] && rm $YAML_URL_LIST_FILE 

#flag the last line, which has no , to be handled at end
sed -i -e '$ s/.*/&++/' module_path.json
echo -e "}" >> module_path.json
#add " and remove ,++ in the last element of array
sed -i -e 's/^[ \t]*//; s/^.*[a-z]/"&/; s/.*[a-z0-9]/&\"\,/; s/\,\:/\:/g; s/\,++//g' module_path.json
#indentation
sed -i -e 's/^/  /; s/  }/}/; s/  {/{/' module_path.json

#add special customized file
mv module_path.json temp
cat module_path_special.json temp > module_path.json
cat module_path.json
rm temp

sudo cp module_path.json /etc/rest
