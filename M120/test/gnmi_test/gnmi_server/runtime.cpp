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

#include "runtime.h"


using namespace std;
using namespace libyang;

static void
print_loaded_module(std::shared_ptr<libyang::Context> ctx)
{
    cout << "=================================================="
         << endl;
    for (auto it : ctx->get_module_iter())
        cout << string(it->name()) << endl;
    cout << "=================================================="
         << endl;
}

/* install - download module and load it in our libyang context. */
void
RuntimeSrCallback::install(const char *module_name, const char *revision)
{
    libyang::S_Module mod;
    string str;

    /* Is module already loaded with libyang? */
    mod = ctx->get_module(module_name, revision);
    if (mod != nullptr) {
        cout << "Module was already loaded: "
             << module_name << "@" << revision;
        return;
    }

    /* Download module from sysrepo */
    try {
        cout << "Download " << module_name << " from sysrepo";
        str = sr_sess->get_schema(module_name, revision, nullptr, SR_SCHEMA_YANG);
    } catch (const exception &exc) {
        cout << exc.what();
        return;
    }

    /* parse module */
    try {
        cout << "Parse " << module_name << " with libyang";
        mod = ctx->parse_module_mem(str.c_str(), LYS_IN_YANG);
    } catch (const exception &exc) {
        cout << exc.what();
        return;
    }
}

/* module_install - Actions performed after sysrepo install/uninstall module
 * event. */
void
RuntimeSrCallback::module_install(const char *module_name, const char *revision,
                                  sr_module_state_t state, void *private_ctx)
{
    (void)private_ctx;

    if (ctx == nullptr) {
        cout << "Context can not be null";
        return;
    }

    switch (state) {
    case SR_MS_UNINSTALLED:
        cout << "Impossible to remove a module at runtime";
        break;

    case SR_MS_IMPORTED:
    case SR_MS_IMPLEMENTED:
        cout << "Install " << module_name;
        install(module_name, revision);
        print_loaded_module(ctx);
        break;

    default:
        cout << "Unknown state";
    }
}

void
RuntimeSrCallback::feature_enable(const char *module_name,
                                  const char *feature_name, bool enable,
                                  void *private_ctx)
{
    (void)private_ctx; (void)enable;
    cout << "Impossible to enable/disable feature "
         << string(feature_name) << " of "
         << string(module_name)
         << " at runtime";
}

