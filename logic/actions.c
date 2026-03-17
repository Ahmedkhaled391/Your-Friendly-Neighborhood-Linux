#include "actions.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

static char *base_path = NULL;
static char script_path[1024];

void set_scripts_base_path(const char *path)
{
    if (base_path)
        free(base_path);
    base_path = strdup(path);
}

const char *get_script(const char *action_id)
{
    if (!base_path)
        return NULL;

    const char *script_name = NULL;
    const char *prefix = "bash ";

    if (strcmp(action_id, "install_essentials") == 0)
    {
        script_name = "install_essentials.sh";
        prefix = "pkexec bash ";
    }
    else if (strcmp(action_id, "enable_tweaks") == 0)
    {
        script_name = "enable_tweaks.sh";
        prefix = "pkexec bash ";
    }
    else if (strcmp(action_id, "clean_cache") == 0)
    {
        script_name = "clean_cache.sh";
        prefix = "pkexec bash ";
    }
    else if (strcmp(action_id, "change_appearance") == 0)
    {
        script_name = "change_appearance.sh";
        prefix = "bash ";
    }
    else if (strcmp(action_id, "check_sysinfo") == 0)
    {
        script_name = "check_sysinfo.sh";
        prefix = "bash ";
    }
    else if (strcmp(action_id, "update_system") == 0)
    {
        script_name = "update_system.sh";
        prefix = "pkexec bash ";
    }
    else if (strcmp(action_id, "install_multimedia") == 0)
    {
        script_name = "install_multimedia.sh";
        prefix = "pkexec bash ";
    }
    else
    {
        return NULL;
    }

    snprintf(script_path, sizeof(script_path), "%s\"%s/scripts/%s\"", prefix, base_path, script_name);
    return script_path;
}