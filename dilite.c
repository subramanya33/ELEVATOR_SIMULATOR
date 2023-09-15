#include <stdio.h>
#include <stdlib.h>
#include <gtk/gtk.h>

GtkWidget *floorLabels[4]; // Updated to have 4 floor labels
GtkWidget *currentFloorEntry;
GtkWidget *floor1Entry;
GtkWidget *floor2Entry;
GtkWidget *resultLabel;
int currentFloor = 1;

int calculate_shortest_path(int currentFloor, int targetFloor1, int targetFloor2) {
    int distanceToFloor1 = abs(currentFloor - targetFloor1);
    int distanceToFloor2 = abs(currentFloor - targetFloor2);

    if (distanceToFloor1 < distanceToFloor2 ||
        (distanceToFloor1 == distanceToFloor2 && targetFloor1 < targetFloor2)) {
        return targetFloor1;
    } else {
        return targetFloor2;
    }
}

void move_elevator(int targetFloor) {
    currentFloor = targetFloor;

    for (int floor = 0; floor < 4; floor++) {
        if (floor == currentFloor - 1) {
            gtk_label_set_text(GTK_LABEL(floorLabels[3 - floor]), "Elevator"); // Reverse order
        } else {
            gtk_label_set_text(GTK_LABEL(floorLabels[3 - floor]), "------");
        }
    }
}

void floor_button_clicked(GtkWidget *widget, gpointer data) {
    int targetFloor = GPOINTER_TO_INT(data);
    move_elevator(targetFloor);

    const char *message = g_strdup_printf("Elevator moving to floor %d", targetFloor);
    gtk_label_set_text(GTK_LABEL(resultLabel), message);
    g_free((gpointer)message);
}

void calculate_and_move_elevator() {
    int requestedFloor = atoi(gtk_entry_get_text(GTK_ENTRY(currentFloorEntry)));
    move_elevator(requestedFloor);

    const char *message = g_strdup_printf("Elevator moving to floor %d", requestedFloor);
    gtk_label_set_text(GTK_LABEL(resultLabel), message);
    g_free((gpointer)message);

    int floor1 = atoi(gtk_entry_get_text(GTK_ENTRY(floor1Entry)));
    int floor2 = atoi(gtk_entry_get_text(GTK_ENTRY(floor2Entry)));

    int nearestFloor = calculate_shortest_path(requestedFloor, floor1, floor2);
    move_elevator(nearestFloor);

    message = g_strdup_printf("Elevator moving to floor %d", nearestFloor);
    gtk_label_set_text(GTK_LABEL(resultLabel), message);
    g_free((gpointer)message);
}

int main(int argc, char *argv[]) {
    gtk_init(&argc, &argv);

    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);

    for (int i = 4; i >= 1; i--) {
        floorLabels[3 - i] = gtk_label_new("------"); // Reverse order
        gtk_box_pack_start(GTK_BOX(vbox), floorLabels[3 - i], FALSE, FALSE, 0);

        GtkWidget *floorButton = gtk_button_new_with_label(g_strdup_printf("Floor %d", i));
        g_signal_connect(floorButton, "clicked", G_CALLBACK(floor_button_clicked), GINT_TO_POINTER(i));
        
        // Set the button width to half of the previous width (e.g., 50)
        gtk_widget_set_size_request(floorButton, 50, -1); // Use -1 to maintain the height

        gtk_box_pack_start(GTK_BOX(vbox), floorButton, FALSE, FALSE, 0);
    }

    currentFloorEntry = gtk_entry_new();
    GtkWidget *currentFloorLabel = gtk_label_new("Enter current floor:");
    gtk_box_pack_start(GTK_BOX(vbox), currentFloorLabel, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(vbox), currentFloorEntry, FALSE, FALSE, 0);
    floor2Entry = gtk_entry_new();
    GtkWidget *floor2Label = gtk_label_new("Enter person 1's requested floor:");
    gtk_box_pack_start(GTK_BOX(vbox), floor2Label, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(vbox), floor2Entry, FALSE, FALSE, 0);
    floor1Entry = gtk_entry_new();
    GtkWidget *floor1Label = gtk_label_new("Enter person 2's requested floor:");
    gtk_box_pack_start(GTK_BOX(vbox), floor1Label, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(vbox), floor1Entry, FALSE, FALSE, 0);



    resultLabel = gtk_label_new("");
    gtk_box_pack_start(GTK_BOX(vbox), resultLabel, FALSE, FALSE, 0);

    GtkWidget *calculateButton = gtk_button_new_with_label("Move Elevator");
    g_signal_connect(calculateButton, "clicked", G_CALLBACK(calculate_and_move_elevator), NULL);
    gtk_box_pack_start(GTK_BOX(vbox), calculateButton, FALSE, FALSE, 0);

    gtk_container_add(GTK_CONTAINER(window), vbox);

    gtk_widget_show_all(window);

    // Main event loop
    while (1) {
        if (gtk_events_pending()) {
            gtk_main_iteration();
        }
        // You can add additional code here if needed.
    }

    return 0;
}