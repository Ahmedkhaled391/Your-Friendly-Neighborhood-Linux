#include "page_custom.h"
#include "pages.h"
#include "../widgets/card.h"

GtkWidget *create_page_custom(GtkWindow *parent)
{
    GtkWidget *page = make_page();
    page_add_title(page, "Customization");
    page_add(page, create_card(
                       "Apply Dark Theme",
                       "Switches to Adwaita-dark for a consistent dark UI.",
                       "change_appearance", parent));
    return page;
}