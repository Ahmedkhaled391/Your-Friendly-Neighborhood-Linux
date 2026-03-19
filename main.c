#include <gtk/gtk.h>
#include <unistd.h>
#include <string.h>
#include "app/window.h"
#include "logic/actions.h"

int main(int argc, char **argv)
{
   
    char exe_path[1024];
    char base_dir[1024];
    ssize_t len = readlink("/proc/self/exe", exe_path, sizeof(exe_path) - 1);
    if (len > 0)
    {
        exe_path[len] = '\0';

        
        const char *last_slash = strrchr(exe_path, '/');
        if (last_slash)
        {
            size_t dir_len = last_slash - exe_path;
            if (dir_len < sizeof(base_dir))
            {
                strncpy(base_dir, exe_path, dir_len);
                base_dir[dir_len] = '\0';
                set_scripts_base_path(base_dir);
            }
        }
    }

    GtkApplication *app = gtk_application_new(
        "com.yourfriendlylinux.app",
        G_APPLICATION_DEFAULT_FLAGS);
    g_signal_connect(app, "activate", G_CALLBACK(on_activate), NULL);
    int status = g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref(app);
    return status;
}