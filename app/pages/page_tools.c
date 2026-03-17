#include "page_tools.h"
#include "pages.h"
#include "../widgets/card.h"

GtkWidget *create_page_tools(GtkWindow *parent)
{
    GtkWidget *page = make_page();
    page_add_title(page, "System Tools");
    page_add(page, create_card(
                       "Check System Information",
                       "Displays OS version, kernel info, CPU details, memory and disk usage.",
                       "check_sysinfo", parent));
    return page;
}