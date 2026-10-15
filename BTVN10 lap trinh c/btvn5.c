#include <stdio.h>
#include <string.h>

#define MAX_TICKETS 5

struct Ticket {
    int ticket_id;
    char event_name[50];
    double price;
    int is_vip;
};

double calculate_ticket_price(double price, int is_vip);
int find_ticket(const struct Ticket list[], int total_tickets, int ticket_id);
void display_tickets(const struct Ticket list[], int total_tickets);
int add_ticket(struct Ticket list[], int total_tickets, struct Ticket new_ticket);
int delete_ticket(struct Ticket list[], int total_tickets, int ticket_id);

int main(void) {
    struct Ticket list[MAX_TICKETS] = {
        {101, "VPOP Star Arena", 500000.0, 1},
        {102, "Music Festival", 300000.0, 0}
    };

    int total_tickets = 2;
    int index;
    struct Ticket new_ticket;

    printf("========== TICKETGO ==========\n");

    printf("\n1. Danh sach ve ban dau\n");
    display_tickets(list, total_tickets);

    printf("\n2. Tinh tien ve VIP\n");
    printf("Gia goc: %.0f VND\n", 500000.0);
    printf("Gia sau phu thu VIP: %.0f VND\n",
           calculate_ticket_price(500000.0, 1));

    printf("\n3. Them ve moi\n");

    new_ticket.ticket_id = 103;
    strcpy(new_ticket.event_name, "Rock Concert");
    new_ticket.price = 400000.0;
    new_ticket.is_vip = 1;
    total_tickets = add_ticket(list, total_tickets, new_ticket);

    new_ticket.ticket_id = 104;
    strcpy(new_ticket.event_name, "Summer Festival");
    new_ticket.price = 250000.0;
    new_ticket.is_vip = 0;
    total_tickets = add_ticket(list, total_tickets, new_ticket);

    new_ticket.ticket_id = 105;
    strcpy(new_ticket.event_name, "Live Show");
    new_ticket.price = 350000.0;
    new_ticket.is_vip = 0;
    total_tickets = add_ticket(list, total_tickets, new_ticket);

    printf("\nDanh sach sau khi them 3 ve:\n");
    display_tickets(list, total_tickets);

    printf("\n4. Thu them ve khi danh sach da day\n");
    new_ticket.ticket_id = 106;
    strcpy(new_ticket.event_name, "Extra Concert");
    new_ticket.price = 200000.0;
    new_ticket.is_vip = 0;
    total_tickets = add_ticket(list, total_tickets, new_ticket);

    printf("\nTong so ve hien tai: %d\n", total_tickets);

    printf("\n5. Xoa ve co ma khong ton tai\n");
    total_tickets = delete_ticket(list, total_tickets, 999);

    printf("\n6. Xoa ve hop le co ma 102\n");
    total_tickets = delete_ticket(list, total_tickets, 102);

    printf("\n7. Danh sach sau khi xoa\n");
    display_tickets(list, total_tickets);

    index = find_ticket(list, total_tickets, 102);
    printf("\nVi tri cua ve 102 sau khi xoa: %d\n", index);

    printf("\nTong so ve cuoi cung: %d\n", total_tickets);

    return 0;
}

double calculate_ticket_price(double price, int is_vip) {
    if (is_vip == 1) {
        return price * 1.2;
    }

    return price;
}

int find_ticket(const struct Ticket list[], int total_tickets, int ticket_id) {
    int i;

    if (total_tickets <= 0) {
        return -1;
    }

    for (i = 0; i < total_tickets; i++) {
        if (list[i].ticket_id == ticket_id) {
            return i;
        }
    }

    return -1;
}

void display_tickets(const struct Ticket list[], int total_tickets) {
    int i;
    double final_price;

    if (total_tickets == 0) {
        printf("Danh sach ve dang rong!\n");
        return;
    }

    printf("--------------------------------------------------------------------------------\n");
    printf("%-10s %-25s %15s %-10s %15s\n",
           "Ma ve", "Ten su kien", "Gia goc", "Loai ve", "Gia thanh toan");
    printf("--------------------------------------------------------------------------------\n");

    for (i = 0; i < total_tickets; i++) {
        final_price = calculate_ticket_price(list[i].price, list[i].is_vip);

        printf("%-10d %-25s %15.0f %-10s %15.0f\n",
               list[i].ticket_id,
               list[i].event_name,
               list[i].price,
               list[i].is_vip == 1 ? "VIP" : "Thuong",
               final_price);
    }

    printf("--------------------------------------------------------------------------------\n");
}

int add_ticket(struct Ticket list[], int total_tickets, struct Ticket new_ticket) {
    if (total_tickets >= MAX_TICKETS) {
        printf("Danh sach ve da day!\n");
        return total_tickets;
    }

    if (total_tickets < 0) {
        printf("So luong ve khong hop le!\n");
        return total_tickets;
    }

    if (find_ticket(list, total_tickets, new_ticket.ticket_id) != -1) {
        printf("Ma ve %d da ton tai!\n", new_ticket.ticket_id);
        return total_tickets;
    }

    list[total_tickets] = new_ticket;
    total_tickets++;

    printf("Them ve %d thanh cong!\n", new_ticket.ticket_id);

    return total_tickets;
}

int delete_ticket(struct Ticket list[], int total_tickets, int ticket_id) {
    int index;
    int i;

    if (total_tickets == 0) {
        printf("Danh sach ve dang rong, khong the xoa!\n");
        return total_tickets;
    }

    index = find_ticket(list, total_tickets, ticket_id);

    if (index == -1) {
        printf("Khong tim thay ma ve %d!\n", ticket_id);
        return total_tickets;
    }

    for (i = index; i < total_tickets - 1; i++) {
        list[i] = list[i + 1];
    }

    total_tickets--;

    printf("Xoa ve %d thanh cong!\n", ticket_id);

    return total_tickets;
}

