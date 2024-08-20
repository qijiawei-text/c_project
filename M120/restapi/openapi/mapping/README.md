1) url to yang module mapping:
when construct xpath of yang from REST request, we need to identify which yang module it is target to. module_map.sh go over all REST API yaml files and generate module_path.json which contains REST url paths belong to a yang module.

2) list key name:
when REST url path pass in the key for a list, it does not contain the key name.It has the format of list_name = key_value. But xpath is in the format of list_name[key-name=keyvalue]. list_key_map.sh to generate the key names for each list.

5) map_all.sh call all above scripts.
