
#include <gtk/gtk.h>

#include "opl3.h"


struct sc_widget {
	GtkWidget *hbox;
	GtkWidget *label;
	GtkWidget *sc;
};

struct op_widget {
	GtkWidget *trem;
	GtkWidget *vib;
	GtkWidget *sus;
	GtkWidget *ksr;
	struct sc_widget mul;
	struct sc_widget ksl;
	struct sc_widget tl;
	struct sc_widget ar;
	struct sc_widget dr;
	struct sc_widget sl;
	struct sc_widget rr;
	struct sc_widget ws;
};
	
struct ins_widget {
	GtkWidget *opl3;
	struct op_widget op[4];
	GtkWidget *fb;
	GtkWidget *algA;
	GtkWidget *algB;
};

static void callback(GtkWidget *widget, gpointer data)
{
	g_print ("Hello again - %s was pressed\n", (gchar *) data);
}



#define new_button(p,w,l) do { \
	w = gtk_button_new_with_label(l); \
	gtk_box_pack_start(GTK_BOX(p), (w), TRUE, TRUE, 0); \
	gtk_widget_show(w); \
} while (0)

#define new_check_button(p,w,l) do { \
	w = gtk_check_button_new_with_label(l); \
	gtk_box_pack_start(GTK_BOX(p), (w), TRUE, TRUE, 0); \
	gtk_widget_show(w); \
} while (0)

#define new_scale(p,w,b,l) do { \
	w.hbox = gtk_hbox_new(FALSE, 0); \
	w.sc = gtk_hscale_new_with_range(0,(1<<(b))-1,1); \
	w.label = gtk_label_new(l); \
	gtk_scale_set_value_pos(GTK_SCALE(w.sc), GTK_POS_RIGHT); \
	gtk_box_pack_start(GTK_BOX(w.hbox), (w.sc), TRUE, TRUE, 0); \
	gtk_box_pack_start(GTK_BOX(w.hbox), (w.label), TRUE, TRUE, 0); \
	gtk_box_pack_start(GTK_BOX(p), (w.hbox), TRUE, TRUE, 0); \
	gtk_widget_show(w.label); \
	gtk_widget_show(w.sc); \
	gtk_widget_show(w.hbox); \
} while (0)

int main(int argc, char *argv[])
{
	GtkWidget *window;
	GtkWidget *vbox1, *hbox1, *opbox[4];
	GtkWidget *play;
	struct ins_widget ins;
	int i;

	gtk_init(&argc, &argv);

	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title(GTK_WINDOW(window), "OPL3 instrument editor");
	gtk_container_set_border_width(GTK_CONTAINER(window), 10);

	vbox1 = gtk_vbox_new(FALSE, 0);
	gtk_container_add(GTK_CONTAINER(window), vbox1);

	hbox1 = gtk_hbox_new(FALSE, 0);

	gtk_box_pack_start(GTK_BOX(vbox1), hbox1, TRUE, TRUE, 0);

	for (i = 0; i < 4; i++) {
		opbox[i] = gtk_vbox_new(FALSE, 0);
		gtk_box_pack_start(GTK_BOX(hbox1), opbox[i], TRUE, TRUE, 0);

		new_check_button(opbox[i], ins.op[i].trem, "Tremolo");
		new_check_button(opbox[i], ins.op[i].vib, "Vibrato");
		new_check_button(opbox[i], ins.op[i].sus, "Sustain");
		new_check_button(opbox[i], ins.op[i].ksr, "Keyboard scaling");

		new_scale(opbox[i], ins.op[i].mul, 4, "Mul");
		new_scale(opbox[i], ins.op[i].ksl, 2, "KSL");
		new_scale(opbox[i], ins.op[i].tl, 6, "TL");
		new_scale(opbox[i], ins.op[i].ar, 4, "AR");
		new_scale(opbox[i], ins.op[i].dr, 4, "DR");
		new_scale(opbox[i], ins.op[i].sl, 4, "SL");
		new_scale(opbox[i], ins.op[i].rr, 4, "RR");
		new_scale(opbox[i], ins.op[i].ws, 3, "WS");

		gtk_widget_show(opbox[i]);
	}

	gtk_widget_show(hbox1);

	new_button(vbox1, play, "Play");

	gtk_widget_show(vbox1);

	gtk_widget_show(window);

	gtk_main();
    
	return 0;
}


