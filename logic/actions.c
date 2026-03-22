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
        script_name = "appearance/change_appearance_dark.sh";
        prefix = "bash ";
    }
    else if (strcmp(action_id, "change_appearance_light") == 0)
    {
        script_name = "appearance/change_appearance_light.sh";
        prefix = "bash ";
    }
    else if (strcmp(action_id, "change_appearance_blue") == 0)
    {
        script_name = "appearance/change_appearance_blue.sh";
        prefix = "bash ";
    }
    else if (strcmp(action_id, "change_appearance_green") == 0)
    {
        script_name = "appearance/change_appearance_green.sh";
        prefix = "bash ";
    }
    else if (strcmp(action_id, "change_appearance_orange") == 0)
    {
        script_name = "appearance/change_appearance_orange.sh";
        prefix = "bash ";
    }
    else if (strcmp(action_id, "change_appearance_purple") == 0)
    {
        script_name = "appearance/change_appearance_purple.sh";
        prefix = "bash ";
    }
    else if (strcmp(action_id, "change_appearance_yellow") == 0)
    {
        script_name = "appearance/change_appearance_yellow.sh";
        prefix = "bash ";
    }
    else if (strcmp(action_id, "reset_appearance_default") == 0)
    {
        script_name = "appearance/reset_appearance_default.sh";
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
    else if (strcmp(action_id, "enable_firewall") == 0)
    {
        script_name = "enable_firewall.sh";
        prefix = "pkexec bash ";
    }
    else if (strcmp(action_id, "disable_firewall") == 0)
    {
        script_name = "disable_firewall.sh";
        prefix = "pkexec bash ";
    }
    else if (strcmp(action_id, "backup_run") == 0)
    {
        script_name = "monthly_backup.sh"
                      " run";
        prefix = "bash ";
        snprintf(script_path, sizeof(script_path), "%s\"%s/scripts/monthly_backup.sh\" run", prefix, base_path);
        return script_path;
    }
    else if (strcmp(action_id, "backup_install_auto") == 0)
    {
        snprintf(script_path, sizeof(script_path), "bash \"%s/scripts/monthly_backup.sh\" install-auto", base_path);
        return script_path;
    }
    else if (strcmp(action_id, "backup_remove_auto") == 0)
    {
        snprintf(script_path, sizeof(script_path), "bash \"%s/scripts/monthly_backup.sh\" remove-auto", base_path);
        return script_path;
    }
    else if (strcmp(action_id, "install_cron") == 0)
    {
        snprintf(script_path, sizeof(script_path), "pkexec bash \"%s/scripts/install_cron.sh\"", base_path);
        return script_path;
    }
    else
    {
        return NULL;
    }

    snprintf(script_path, sizeof(script_path), "%s\"%s/scripts/%s\"", prefix, base_path, script_name);
    return script_path;
}