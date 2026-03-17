#include "page_setup.h"
#include "pages.h"
#include "../widgets/card.h"

GtkWidget *create_page_setup(GtkWindow *parent)
{
    GtkWidget *page = make_page();
    page_add_title(page, "System Setup");
    page_add(page, create_card(
                       "Install Essential Packages",
                       "Installs common tools like curl, git, htop, build-essential.",
                       "install_essentials", parent));
    page_add(page, create_card(
                       "Update System",
                       "Runs a full apt update and upgrade to keep your system current.",
                       "update_system", parent));
    page_add(page, create_card(
                       "Install Multimedia Codecs",
                       "Installs VLC, GIMP, FFmpeg and restricted multimedia extras.",
                       "install_multimedia", parent));
    return page;
}