OpenAPI describes and documents RESTful APIs. The Swagger definition generated with our tool is meant to be compliant with RESTCONF specification.

yang directory contains the yang model and YAML generating scripts using yang2swagger tool (github.com/oplinkoms/yang2swagger).

base: Oplink APIs based on standandar yang model
msft: Microsoft APIs

steps:
1) Check yang files in directory apps\restapi\openapi\basic_yang
    Note: Add new yang files or changed existed yang files

2) generate yaml
    a. modify yang2yaml.sh (apps\restapi\openapi\yang2yaml)when basic_yang changes or  yaml need to regenerate.
    b. execute yang2yaml.sh

3) generate json configuration files
   a. modify *_special.json when changed
   b. execute map_all.sh
   c. check list_key.json, string_type.json and number_type.json
      Note: manual check json format because of yang changes, maybe need to delete some invalid format.
           eg: delete them in list_key.json
	       "sequence-id       -> /acl/acl-sets/acl-set[name=current()/../../../../set-name",[type=current()/../../../../type]/acl-entries/acl-entry/sequence-id
           "type       -> ../../../../../../acl-sets/acl-set[name=current()/../set-name",/config/type

4) over
