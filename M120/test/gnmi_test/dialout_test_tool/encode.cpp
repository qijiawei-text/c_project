/*
 * Copyright 2020 Yohan Pipereau
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <exception>
#include <sysrepo-cpp/Struct.hpp>
#include <sysrepo-cpp/Sysrepo.hpp>
#include <libyang/Tree_Schema.hpp>
#include <libyang/Tree_Data.hpp>

/* #include <utils/log.h> */

#include "encode.h"
#include "runtime.h"




using namespace std;
using namespace libyang;
using sysrepo::Val;

/*
 * Wrapper to test wether the current Data Node is a key.
 * We know that by looking in the Schema Tree.
 * @param leaf Leaf Data Node
 */
static bool
isKey(S_Data_Node_Leaf_List leaf)
{
    S_Schema_Node_Leaf tmp = make_shared<Schema_Node_Leaf>(leaf->schema());

    if (tmp->is_key())
        return true;
    else
        return false;
}

/***************
* CRUD - READ *
***************/

static Json::Value
json_tree(sysrepo::S_Tree tree)
{
    sysrepo::S_Tree iter;
    Json::Value val;

    /* run through all siblings */
    for (iter = tree->first_child(); iter != nullptr; iter = iter->next()) {
        /* create sibling with "node" as a parent */
        switch (iter->type()) { /* follows RFC 7951 */
        /* JSON Number */
        case SR_UINT8_T:
            val[iter->name()] = iter->data()->get_uint8();
            break;
        case SR_UINT16_T:
            val[iter->name()] = iter->data()->get_uint16();
            break;
        case SR_UINT32_T:
            val[iter->name()] = iter->data()->get_uint32();
            break;
        case SR_INT8_T:
            val[iter->name()] = iter->data()->get_int8();
            break;
        case SR_INT16_T:
            val[iter->name()] = iter->data()->get_int16();
            break;
        case SR_INT32_T:
            val[iter->name()] = iter->data()->get_int32();
            break;

        /* JSON string */
        case SR_STRING_T:
            val[iter->name()] = iter->data()->get_string();
            break;
        case SR_INT64_T:
            val[iter->name()] = to_string(iter->data()->get_int64());
            break;
        case SR_UINT64_T:
            val[iter->name()] = to_string(iter->data()->get_uint64());
            break;
        case SR_DECIMAL64_T:
            val[iter->name()] = to_string(iter->data()->get_decimal64());
            break;
        case SR_IDENTITYREF_T:
            val[iter->name()] = iter->data()->get_identityref();
            break;
        case SR_INSTANCEID_T:
            val[iter->name()] = iter->data()->get_identityref();
            break;
        case SR_BINARY_T:
            val[iter->name()] = iter->data()->get_binary();
            break;
        case SR_BITS_T:
            val[iter->name()] = iter->data()->get_bits();
            break;
        case SR_ENUM_T:
            val[iter->name()] = iter->data()->get_enum();
            break;
        case SR_BOOL_T:
            val[iter->name()] = iter->data()->get_bool() ? "true" : "false";
            break;

        /* JSON arrays */
        case SR_LIST_T:
            val[iter->name()].append(json_tree(iter));
            break;
        case SR_LEAF_EMPTY_T:
            val[iter->name()].append("null");
            break;

        /* nested JSON */
        case SR_CONTAINER_T:
        case SR_CONTAINER_PRESENCE_T:
            val[iter->name()] = json_tree(iter);
            break;

        /* Unsupported types */
        case SR_ANYDATA_T:
        case SR_ANYXML_T:
            throw invalid_argument("unsupported ANYDATA and ANYXML types");
            break;

        default:
            cout << "Unknown tree node type";
            throw invalid_argument("Unknown tree node type");
        }
    }
    return val;
}


/*
 * Store YANG leaf in sysrepo datastore
 * @param node Describe a libyang Data Tree leaf or leaf list
 */
void
Encode::storeLeaf(libyang::S_Data_Node_Leaf_List node)
{
    shared_ptr<Val> sval;

    if (isKey(node)) {
        /* If node is a key create it first by setting parent path */
        cout << "leaf key: " << node->path();
        return;
    } else {
        cout << "leaf: " << node->path();
    }

    switch (node->value_type()) {
    case LY_TYPE_BINARY:        /* Any binary data */
        sval = make_shared<Val>(node->value()->binary(), SR_STRING_T);
        break;
    case LY_TYPE_STRING:        /* Human-readable string */
        sval = make_shared<Val>(node->value()->string(), SR_STRING_T);
        break;
    case LY_TYPE_BOOL:          /* "true" or "false" */
        sval = make_shared<Val>(static_cast<bool>(node->value()->bln()));
        break;
    case LY_TYPE_DEC64:         /* 64-bit signed decimal number */
        sval = make_shared<Val>(static_cast<double>(node->value()->dec64()));
        break;
    case LY_TYPE_INT8:          /* 8-bit signed integer */
        /*  sval = make_shared<Val>(node->value()->int8(), SR_INT8_T); */
        sval = make_shared<Val>(node->value()->int8());
        break;
    case LY_TYPE_UINT8:         /* 8-bit unsigned integer */
        /*  sval = make_shared<Val>(node->value()->uint8(), SR_UINT8_T); */
        sval = make_shared<Val>(node->value()->uint8());
        break;
    case LY_TYPE_INT16:         /* 16-bit signed integer */
        /*  sval = make_shared<Val>(node->value()->int16(), SR_INT16_T); */
        sval = make_shared<Val>(node->value()->int16());
        break;
    case LY_TYPE_UINT16:        /* 16-bit unsigned integer */
        /*  sval = make_shared<Val>(node->value()->uint16(), SR_UINT16_T); */
        sval = make_shared<Val>(node->value()->uint16());
        break;
    case LY_TYPE_INT32:         /* 32-bit signed integer */
        /* sval = make_shared<Val>(node->value()->int32(), SR_INT32_T); */
        sval = make_shared<Val>(node->value()->int32());
        break;
    case LY_TYPE_UINT32:        /* 32-bit unsigned integer */
        /* sval = make_shared<Val>(node->value()->uintu32(), SR_UINT32_T); */
        sval = make_shared<Val>(node->value()->uintu32());
        break;
    case LY_TYPE_INT64:         /* 64-bit signed integer */
        sval = make_shared<Val>(node->value()->int64(), SR_INT64_T);
        break;
    case LY_TYPE_UINT64:        /* 64-bit unsigned integer */
        /* sval = make_shared<Val>(node->value()->uint64(), SR_UINT64_T); */
        sval = make_shared<Val>(node->value()->uint64());
        break;
    case LY_TYPE_IDENT:         /* A reference to an abstract identity */
    {
        string str(node->value()->ident()->module()->name());
        str.append(":");
        str.append(node->value()->ident()->name());
        sval = make_shared<Val>(str.c_str(), SR_IDENTITYREF_T);
        break;
    }
    case LY_TYPE_ENUM:          /* Enumerated strings */
        sval = make_shared<Val>(node->value()->enm()->name(), SR_ENUM_T);
        break;
    case LY_TYPE_EMPTY:         /* A leaf that does not have any value */
        sval = make_shared<Val>(nullptr, SR_LEAF_EMPTY_T);
        break;
    case LY_TYPE_LEAFREF:       /* A reference to a leaf instance */
    {
        /* run again this function */
        S_Data_Node_Leaf_List leaf
            = make_shared<Data_Node_Leaf_List>(node->value()->leafref());
        storeLeaf(leaf);
        break;
    }

/* Unsupported types */
    case LY_TYPE_BITS:          /* A set of bits or flags */
        cout << "Unsupported BITS type";
        throw std::invalid_argument("Unsupported BITS type");
        break;
    case LY_TYPE_INST:          /* References a data tree node */
        cout << "Unsupported INSTANCE-IDENTIFIER type" << endl;
        throw std::invalid_argument("Unsupported INSTANCE-IDENTIFIER type");
        break;
    case LY_TYPE_UNION:         /* Choice of member types */
        cout << "Unsupported UNION type";
        throw std::invalid_argument("Unsupported UNION type");
        break;
    case LY_TYPE_DER:           /* Derived type */
        cout << "Unsupported DERIVED type";
        throw std::invalid_argument("Unsupported DERIVED type");
        break;
    case LY_TYPE_UNKNOWN:       /* Unknown type (used in edit-config leaves) */
        cout << "Unsupported UNKNOWN type";
        throw std::invalid_argument("Unsupported UNKNOWN type");
        break;
    default:
        cout << "UNKNOWN type";
        throw std::invalid_argument("Unknown type");
    }

    try {
        sr_sess->set_item(node->path().c_str(), sval);
    } catch (exception &exc) {
        cout << exc.what();
        throw; /* rethrow as caught */
    }
}

void
Encode::storeTree(libyang::S_Data_Node node)
{
    for (auto it : node->tree_dfs()) {
        /* Run through the entire tree, including siblinigs */

        switch (it->schema()->nodetype()) {
        case LYS_LEAF : /* Only LEAF & LEAF LIST hold values in sysrepo */
            {
                S_Data_Node_Leaf_List itleaf = make_shared<Data_Node_Leaf_List>(it);

                try {
                    storeLeaf(itleaf);
                } catch (std::string str) { /* triggered by sysepo::Val constructor */
                    cout << str;
                    throw invalid_argument("Internal error with JSON encoding");
                }
                break;
            }

        case LYS_LEAFLIST: /* Only LEAF & LEAF LIST hold values in sysrepo */
            cout << "Unsupported leaf-list: " << it->path();
            /* sysrepo does not seem to support leaf lists */
            break;

        case LYS_LIST: /* A list instance must be created before populating leaves */
        {
            try {
                shared_ptr<Val> sval = make_shared<Val>(nullptr, SR_LIST_T);
                sr_sess->set_item(it->path().c_str(), sval);
            } catch (exception &exc) {
                cout << exc.what();
                throw; /* rethrow as caught */
            }

            break;
        }

        default:
            break;
        }
    }
}

void
Encode::json_update(string data)
{
    S_Data_Node node;

    /* Parse input JSON, same options than netopeer2 edit-config */
    node = ctx->parse_data_mem(data.c_str(), LYD_JSON, LYD_OPT_EDIT |
                               LYD_OPT_STRICT);

    /* store Data Tree to sysrepo */
    storeTree(node);
}

/* Get sysrepo subtree data corresponding to XPATH */
vector<JsonData>
Encode::json_read(string xpath)
{
    sysrepo::S_Trees sr_trees;
    sysrepo::S_Tree sr_tree;
    vector<JsonData> json_vec;
    Json::StyledWriter styledwriter; /* pretty JSON */
    Json::FastWriter fastWriter; /* unreadable JSON */
    Json::Value val;
    JsonData tmp;
    string key_name, key_value;

    /* Get multiple subtree for YANG lists or one for other YANG types */
    sr_trees = sr_sess->get_subtrees(xpath.c_str());
    if (sr_trees == nullptr)
        throw invalid_argument("xpath not found");

    for (size_t i = 0; i < sr_trees->tree_cnt(); i++) {
        sr_tree = sr_trees->tree(i);
        val = json_tree(sr_tree);

        /*
         * Pass a pair containing key name and key value.
         * keys are always first element of children in sysrepo trees
         */
        if (sr_tree->type() == SR_LIST_T) {
            tmp.key.first = string(sr_tree->first_child()->name());
            tmp.key.second = val[tmp.key.first].asString();
            cout << tmp.key.first << ":" << tmp.key.second;
        }

        /* Print Pretty JSON message */
        /* cout << styledwriter.write(val); */

        tmp.data = styledwriter.write(val);

        /* Fast unreadable JSON message */
        /* tmp.data = fastWriter.write(val); */

        /* cout << tmp.data; */

        json_vec.push_back(tmp);
    }

    return json_vec;
}

Encode::Encode(shared_ptr<sysrepo::Session> sess)
    : sr_sess(sess)
{
    shared_ptr<sysrepo::Yang_Schemas> schemas; /* sysrepo YANG schemas supported */
    shared_ptr<RuntimeSrCallback> scb; /* pointer to callback class */
    sub = make_shared<sysrepo::Subscribe>(sr_sess); /* sysrepo subscriptions */
    S_Module mod;
    string str;
#if 0
    /* Libyang log level should be ERROR only */
    set_log_verbosity(LY_LLERR);

    /* 1. build libyang context */
    ctx = make_shared<Context>();

    /* Instantiate Callback class */
    scb = make_shared<RuntimeSrCallback>(ctx, sess);

    /* 2. get the list of schemas from sysrepo */
    try {
        schemas = sr_sess->list_schemas();
    } catch (const exception &exc) {
        cout << "encode error!" << endl;
        cout << exc.what();
        exit(1);
    }

    /* 3.1 Callback for missing modules */
    auto mod_c_cb = [this](const char *mod_name, const char *mod_rev,
                           const char *, const char *)->libyang::Context::mod_missing_cb_return {
        string str; S_Module mod;

        cout << "Importing missing dependency " << mod_name;
        str = this->sr_sess->get_schema(mod_name, mod_rev, NULL, SR_SCHEMA_YANG);

        try {
            mod = this->ctx->parse_module_mem(str.c_str(), LYS_IN_YANG);
        } catch (const exception &exc) {
            cout << exc.what();
        }

        return {LYS_IN_YANG, mod_name};
    };

    /* 3.2 register callback for missing YANG module */
    ctx->add_missing_module_callback(mod_c_cb);

    /* 4. Initialize our libyang context with modules and features
     * already loaded in sysrepo */
    for (unsigned int i = 0; i < schemas->schema_cnt(); i++) {
        string module_name = schemas->schema(i)->module_name();
        string revision = schemas->schema(i)->revision()->revision();

        mod = ctx->get_module(module_name.c_str(), revision.c_str());
        if (mod != nullptr) {
            cout << "Module was already loaded: "
                 << module_name << "@" << revision;
        } else {
            cout << "Download & parse module: "
                 << module_name << "@" << revision;

            /* 4.1 Download YANG model from sysrepo as in YANG format and parse it */
            try {
                str = sr_sess->get_schema(module_name.c_str(), revision.c_str(), NULL,
                                          SR_SCHEMA_YANG);
                mod = ctx->parse_module_mem(str.c_str(), LYS_IN_YANG);
            } catch (const exception &exc) {
                cout << exc.what();
                continue;
            }
        }

        /* 4.2 Load features loaded in sysrepo */
        for (size_t j = 0; j < schemas->schema(i)->enabled_feature_cnt(); j++) {
            string feature_name = schemas->schema(i)->enabled_features(j);

            cout << "Loading feature " << feature_name
                 << " in module " << mod->name();

            mod->feature_enable(feature_name.c_str());
        }
    }

    /* 5. subscribe for notifications about new modules */
    sub->module_install_subscribe(scb, ctx.get(), sysrepo::SUBSCR_DEFAULT);

    /* 6. subscribe for changes of features state */
    sub->feature_enable_subscribe(scb);

#endif
}

Encode::~Encode()
{
    cout << "Disconnect sysrepo session and Libyang context";
}

