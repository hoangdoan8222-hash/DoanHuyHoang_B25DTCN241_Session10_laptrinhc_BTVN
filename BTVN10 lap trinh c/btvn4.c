#include <stdio.h>
#include <string.h>

#define MAX_ZONES 50
#define MAX_BUY 4
#define NAME_SIZE 50

typedef struct {
    int zone_id;
    char zone_name[NAME_SIZE];
    float ticket_price;
    int total_capacity;
    int sold_tickets;
    int is_early_bird;
} TicketZone;

int timZoneTheoMa(const TicketZone zones[], int total_zones, int zone_id);
int hienThiDanhSach(const TicketZone zones[], int total_zones);
int themZone(TicketZone zones[], int total_zones);
int capNhatGiaVe(TicketZone zones[], int total_zones);
int xoaZone(TicketZone zones[], int total_zones);
int kiemTraDatVe(const TicketZone zones[], int total_zones,
                 int zone_id, int quantity);
float tinhTongTien(const TicketZone *zone, int quantity);
void datVe(TicketZone zones[], int total_zones);
int docDong(char buffer[], int size);
int docSoNguyen(const char *prompt, int *value);
int docSoThuc(const char *prompt, float *value);
int docChuoi(const char *prompt, char buffer[], int size);
void hienThiMenu(void);

int main(void) {
    TicketZone zones[MAX_ZONES] = {
        {101, "Zone SVIP", 3000000.0f, 100, 0, 1},
        {102, "Zone VIP", 2000000.0f, 200, 0, 1},
        {103, "Zone GA", 800000.0f, 500, 0, 0},
        {104, "Zone Standard", 500000.0f, 800, 0, 0}
    };

    int total_zones = 4;
    int choice;
    int running = 1;

    while (running) {
        hienThiMenu();

        if (!docSoNguyen("Nhap lua chon: ", &choice)) {
            break;
        }

        switch (choice) {
            case 1:
                hienThiDanhSach(zones, total_zones);
                break;

            case 2:
                total_zones = themZone(zones, total_zones);
                break;

            case 3:
                capNhatGiaVe(zones, total_zones);
                break;

            case 4:
                total_zones = xoaZone(zones, total_zones);
                break;

            case 5:
                datVe(zones, total_zones);
                break;

            case 0:
                running = 0;
                printf("Da thoat chuong trinh.\n");
                break;

            default:
                printf("[LOI] Lua chon khong hop le. Vui long thu lai.\n");
        }
    }

    return 0;
}

void hienThiMenu(void) {
    printf("\n===============================================\n");
    printf("       TICKETBOX - VPOP STAR ARENA\n");
    printf("===============================================\n");
    printf("1. Xem danh sach khu vuc ve\n");
    printf("2. Them khu vuc ve moi\n");
    printf("3. Cap nhat gia ve khu vuc\n");
    printf("4. Xoa khu vuc ve\n");
    printf("5. Dat ve va in hoa don\n");
    printf("0. Thoat\n");
    printf("===============================================\n");
}

int docDong(char buffer[], int size) {
    size_t length;
    int ch;

    if (fgets(buffer, size, stdin) == NULL) {
        return 0;
    }

    length = strlen(buffer);

    if (length > 0 && buffer[length - 1] == '\n') {
        buffer[length - 1] = '\0';
    } else {
        ch = getchar();

        if (ch != '\n' && ch != EOF) {
            while (ch != '\n' && ch != EOF) {
                ch = getchar();
            }

            return -1;
        }
    }

    return 1;
}

int docSoNguyen(const char *prompt, int *value) {
    char buffer[100];
    char extra;
    int result;

    while (1) {
        printf("%s", prompt);
        result = docDong(buffer, sizeof(buffer));

        if (result == 0) {
            return 0;
        }

        if (result == -1 ||
            sscanf(buffer, " %d %c", value, &extra) != 1) {
            printf("[LOI] Vui long nhap mot so nguyen hop le.\n");
            continue;
        }

        return 1;
    }
}

int docSoThuc(const char *prompt, float *value) {
    char buffer[100];
    char extra;
    int result;

    while (1) {
        printf("%s", prompt);
        result = docDong(buffer, sizeof(buffer));

        if (result == 0) {
            return 0;
        }

        if (result == -1 ||
            sscanf(buffer, " %f %c", value, &extra) != 1 ||
            !(*value >= 0.0f) ||
            *value > 1000000000.0f) {
            printf("[LOI] Vui long nhap gia tri tu 0 den 1000000000.\n");
            continue;
        }

        return 1;
    }
}

int docChuoi(const char *prompt, char buffer[], int size) {
    int result;
    size_t i;

    while (1) {
        printf("%s", prompt);
        result = docDong(buffer, size);

        if (result == 0) {
            return 0;
        }

        if (result == -1) {
            printf("[LOI] Noi dung qua dai. Vui long nhap lai.\n");
            continue;
        }

        i = 0;
        while (buffer[i] == ' ' || buffer[i] == '\t') {
            i++;
        }

        if (buffer[i] == '\0') {
            printf("[LOI] Khong duoc de trong.\n");
            continue;
        }

        return 1;
    }
}

int timZoneTheoMa(const TicketZone zones[], int total_zones, int zone_id) {
    int i;

    if (zones == NULL || total_zones <= 0) {
        return -1;
    }

    for (i = 0; i < total_zones; i++) {
        if (zones[i].zone_id == zone_id) {
            return i;
        }
    }

    return -1;
}

int hienThiDanhSach(const TicketZone zones[], int total_zones) {
    int i;
    int remaining;

    if (zones == NULL || total_zones <= 0) {
        printf("Danh sach khu vuc ve dang rong.\n");
        return 0;
    }

    printf("\n-----------------------------------------------------------------------------------------------\n");
    printf("%-8s %-20s %15s %12s %12s %12s %-12s\n",
           "Ma zone", "Ten zone", "Gia ve", "Suc chua",
           "Da ban", "Con lai", "Early Bird");
    printf("-----------------------------------------------------------------------------------------------\n");

    for (i = 0; i < total_zones; i++) {
        remaining = zones[i].total_capacity - zones[i].sold_tickets;

        printf("%-8d %-20s %15.0f %12d %12d %12d %-12s\n",
               zones[i].zone_id,
               zones[i].zone_name,
               zones[i].ticket_price,
               zones[i].total_capacity,
               zones[i].sold_tickets,
               remaining,
               zones[i].is_early_bird == 1 ? "Co" : "Khong");
    }

    printf("-----------------------------------------------------------------------------------------------\n");
    return 1;
}

int themZone(TicketZone zones[], int total_zones) {
    TicketZone new_zone;

    if (zones == NULL) {
        printf("[LOI] Danh sach khu vuc khong hop le.\n");
        return total_zones;
    }

    if (total_zones >= MAX_ZONES) {
        printf("[LOI] Danh sach da dat gioi han %d zone.\n", MAX_ZONES);
        return total_zones;
    }

    if (!docSoNguyen("Nhap ma zone: ", &new_zone.zone_id)) {
        return total_zones;
    }

    if (new_zone.zone_id <= 0) {
        printf("[LOI] Ma zone phai lon hon 0.\n");
        return total_zones;
    }

    if (timZoneTheoMa(zones, total_zones, new_zone.zone_id) != -1) {
        printf("[LOI] Ma zone da ton tai.\n");
        return total_zones;
    }

    if (!docChuoi("Nhap ten zone: ", new_zone.zone_name,
                  sizeof(new_zone.zone_name))) {
        return total_zones;
    }

    if (!docSoThuc("Nhap gia ve: ", &new_zone.ticket_price)) {
        return total_zones;
    }

    if (!docSoNguyen("Nhap tong suc chua: ",
                     &new_zone.total_capacity)) {
        return total_zones;
    }

    if (new_zone.total_capacity <= 0) {
        printf("[LOI] Suc chua phai lon hon 0.\n");
        return total_zones;
    }

    if (!docSoNguyen("Ap dung Early Bird? (1: Co, 0: Khong): ",
                     &new_zone.is_early_bird)) {
        return total_zones;
    }

    if (new_zone.is_early_bird != 0 &&
        new_zone.is_early_bird != 1) {
        printf("[LOI] Early Bird chi nhan gia tri 0 hoac 1.\n");
        return total_zones;
    }

    new_zone.sold_tickets = 0;
    zones[total_zones] = new_zone;
    total_zones++;

    printf("[THANH CONG] Da them zone %d.\n", new_zone.zone_id);
    return total_zones;
}

int capNhatGiaVe(TicketZone zones[], int total_zones) {
    int zone_id;
    int vi_tri;
    float new_price;

    if (zones == NULL || total_zones <= 0) {
        printf("[LOI] Danh sach khu vuc ve dang rong.\n");
        return 0;
    }

    if (!docSoNguyen("Nhap ma zone can cap nhat: ", &zone_id)) {
        return 0;
    }

    vi_tri = timZoneTheoMa(zones, total_zones, zone_id);

    if (vi_tri == -1) {
        printf("[LOI] Khong tim thay zone %d.\n", zone_id);
        return 0;
    }

    if (!docSoThuc("Nhap gia ve moi: ", &new_price)) {
        return 0;
    }

    zones[vi_tri].ticket_price = new_price;

    printf("[THANH CONG] Da cap nhat gia zone %d thanh %.0f VND.\n",
           zone_id, new_price);

    return 1;
}

int xoaZone(TicketZone zones[], int total_zones) {
    int zone_id;
    int vi_tri;
    int i;

    if (zones == NULL || total_zones <= 0) {
        printf("[LOI] Danh sach khu vuc ve dang rong.\n");
        return total_zones;
    }

    if (!docSoNguyen("Nhap ma zone can xoa: ", &zone_id)) {
        return total_zones;
    }

    vi_tri = timZoneTheoMa(zones, total_zones, zone_id);

    if (vi_tri == -1) {
        printf("[LOI] Khong tim thay zone %d.\n", zone_id);
        return total_zones;
    }

    if (zones[vi_tri].sold_tickets > 0) {
        printf("[LOI] Khong the xoa zone da co ve duoc ban.\n");
        return total_zones;
    }

    for (i = vi_tri; i < total_zones - 1; i++) {
        zones[i] = zones[i + 1];
    }

    total_zones--;

    printf("[THANH CONG] Da xoa zone %d.\n", zone_id);
    return total_zones;
}

int kiemTraDatVe(const TicketZone zones[], int total_zones,
                 int zone_id, int quantity) {
    int vi_tri;

    if (zones == NULL || total_zones <= 0) {
        printf("[LOI] Danh sach zone dang rong.\n");
        return 0;
    }

    if (quantity < 1 || quantity > MAX_BUY) {
        printf("[LOI] Moi giao dich chi duoc mua tu 1 den %d ve.\n",
               MAX_BUY);
        return 0;
    }

    vi_tri = timZoneTheoMa(zones, total_zones, zone_id);

    if (vi_tri == -1) {
        printf("[LOI] Ma zone khong ton tai.\n");
        return 0;
    }

    if (zones[vi_tri].ticket_price < 0 ||
        zones[vi_tri].total_capacity <= 0 ||
        zones[vi_tri].sold_tickets < 0 ||
        zones[vi_tri].sold_tickets > zones[vi_tri].total_capacity ||
        (zones[vi_tri].is_early_bird != 0 &&
         zones[vi_tri].is_early_bird != 1)) {
        printf("[LOI] Du lieu zone khong hop le.\n");
        return 0;
    }

    if (quantity >
        zones[vi_tri].total_capacity - zones[vi_tri].sold_tickets) {
        printf("[LOI] Khong du ve trong khu vuc nay.\n");
        return 0;
    }

    return 1;
}

float tinhTongTien(const TicketZone *zone, int quantity) {
    float subtotal;

    subtotal = zone->ticket_price * quantity;

    if (zone->is_early_bird == 1) {
        subtotal = subtotal * 0.85f;
    }

    return subtotal;
}

void datVe(TicketZone zones[], int total_zones) {
    int zone_id;
    int quantity;
    int vi_tri;
    float total;

    if (zones == NULL || total_zones <= 0) {
        printf("[LOI] Danh sach zone dang rong.\n");
        return;
    }

    hienThiDanhSach(zones, total_zones);

    if (!docSoNguyen("Nhap ma zone muon mua: ", &zone_id)) {
        return;
    }

    if (!docSoNguyen("Nhap so luong ve (1-4): ", &quantity)) {
        return;
    }

    if (!kiemTraDatVe(zones, total_zones, zone_id, quantity)) {
        printf("[THAT BAI] Giao dich khong duoc thuc hien.\n");
        return;
    }

    vi_tri = timZoneTheoMa(zones, total_zones, zone_id);
    total = tinhTongTien(&zones[vi_tri], quantity);

    zones[vi_tri].sold_tickets += quantity;

    printf("\n===============================================\n");
    printf("              HOA DON TICKETBOX\n");
    printf("===============================================\n");
    printf("Su kien       : VPOP Star Arena\n");
    printf("Ma zone       : %d\n", zones[vi_tri].zone_id);
    printf("Ten zone      : %s\n", zones[vi_tri].zone_name);
    printf("So luong ve   : %d\n", quantity);
    printf("Gia goc/ve    : %.0f VND\n", zones[vi_tri].ticket_price);
    printf("Early Bird    : %s\n",
           zones[vi_tri].is_early_bird == 1 ? "Co" : "Khong");

    if (zones[vi_tri].is_early_bird == 1) {
        printf("Giam gia      : 15%%\n");
    } else {
        printf("Giam gia      : 0%%\n");
    }

    printf("TONG THANH TOAN: %.0f VND\n", total);
    printf("Ve da ban     : %d/%d\n",
           zones[vi_tri].sold_tickets,
           zones[vi_tri].total_capacity);
    printf("===============================================\n");
    printf("[THANH CONG] Dat ve thanh cong!\n");
}

