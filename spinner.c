#include <gtk/gtk.h>
#include <string.h>
#include <unistd.h>

gboolean on_timeout(gpointer data) {
  gtk_widget_queue_draw(GTK_WIDGET(data));
  return G_SOURCE_CONTINUE;
}

gboolean on_draw(GtkWidget *widget, cairo_t *cr, gpointer data) {
  GtkStyleContext *context;

  context = gtk_widget_get_style_context(widget);
  gtk_render_background(context, cr, 0, 0, gtk_widget_get_allocated_width(widget), gtk_widget_get_allocated_height(widget));

  return FALSE;
}

void show_help() {
  g_print("Usage: loadspinner [TEXT] [FONTSIZE] [WAIT_TIME]\n");
  g_print("TEXT: The text to display in the label.\n");
  g_print("FONTSIZE: The font size of the label.\n");
  g_print("WAIT_TIME: The wait time in milliseconds before showing the window.\n");
}

int main(int argc, char *argv[]) {
  if (argc == 2 && strcmp(argv[1], "--help") == 0) {
    show_help();
    return 0;
  }

  if (argc != 4) {
    g_print("Error: Invalid number of arguments.\n");
    g_print("Use './program_name --help' for usage information.\n");
    return 1;
  }

  GtkWidget *window;
  GtkWidget *spinner;
  GtkWidget *label;
  GtkWidget *box;
  guint timeout_id;

  gtk_init(&argc, &argv);

  unsigned int waitTime = atoi(argv[3]);
  usleep(waitTime * 1000);

  window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_window_set_decorated(GTK_WINDOW(window), FALSE);
  gtk_window_set_skip_taskbar_hint(GTK_WINDOW(window), TRUE);
  gtk_window_set_skip_pager_hint(GTK_WINDOW(window), TRUE);
  gtk_window_set_default_size(GTK_WINDOW(window), 300, 200); // Set the desired size of the window

  GdkScreen *screen = gtk_window_get_screen(GTK_WINDOW(window));
  GdkVisual *visual = gdk_screen_get_rgba_visual(screen);
  if (visual != NULL) {
    gtk_widget_set_visual(window, visual);
    gtk_widget_set_app_paintable(window, TRUE);
  }

  g_signal_connect(G_OBJECT(window), "destroy", G_CALLBACK(gtk_main_quit), NULL);
  g_signal_connect(G_OBJECT(window), "draw", G_CALLBACK(on_draw), NULL);

  box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
  gtk_container_set_border_width(GTK_CONTAINER(box), 20);

  spinner = gtk_spinner_new();
  gtk_widget_set_size_request(spinner, 100, 100); // Set the size of the spinner widget
  gtk_widget_set_halign(spinner, GTK_ALIGN_CENTER);
  gtk_widget_set_valign(spinner, GTK_ALIGN_CENTER);
  gtk_spinner_start(GTK_SPINNER(spinner));
  gtk_box_pack_start(GTK_BOX(box), spinner, TRUE, TRUE, 0);

  // Extract the arguments for the label and font size
  char *loginText = argv[1];
  int fontSize = atoi(argv[2]);

  label = gtk_label_new(loginText);
  PangoFontDescription *font_desc = pango_font_description_new(); // Create a new font description object
  pango_font_description_set_family(font_desc, "Sans"); // Set the font family
  pango_font_description_set_size(font_desc, fontSize * PANGO_SCALE); // Set the font size
  gtk_widget_override_font(label, font_desc);
  gtk_widget_set_halign(label, GTK_ALIGN_CENTER);
  gtk_widget_set_valign(label, GTK_ALIGN_CENTER);
  gtk_box_pack_start(GTK_BOX(box), label, TRUE, TRUE, 0);

  gtk_container_add(GTK_CONTAINER(window), box);
  gtk_widget_show_all(window);

  timeout_id = g_timeout_add(1000, on_timeout, window);

  gtk_main();

  g_source_remove(timeout_id);

  return 0;
}

