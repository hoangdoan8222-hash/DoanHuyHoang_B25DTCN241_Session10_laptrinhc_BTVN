#include <stdio.h>
#include <string.h>

#define MAX_ZONES 5
#define MAX_QRS 10
#define MAX_BUY 4

typedef struct {
    int zone_id;
    char zone_name[50];
    float ticket_price;
    int total_capacity;
    int sold_tickets;
} TicketZone;

typedef struct {
    char qr_code[30];
    int zone_id;
    int is_used;
} QrCheckIn;

enum SaleStatus {
    SALE_SUCCESS,
    SALE_EMPTY_ZONES,
    SALE_ZONE_NOT_FOUND,
    SALE_INVALID_QUANTITY,
    SALE_INVALID_EARLY_BIRD,
    SALE_INVALID_ZONE_DATA,
    SALE_OVER_CAPACITY,
    SALE_INVALID_OUTPUT
};

enum CheckInStatus {
    QR_CHECKIN_SUCCESS,
    QR_LIST_EMPTY,
    QR_NOT_FOUND,
    QR_ALREADY_USED,
    QR_INVALID_DATA
};

int timKhuVuc(const TicketZone zones[], int total_zones, int zone_id);
float tinhTongTien(float ticket_price, int quantity, int is_early_bird);
enum SaleStatus banVe(
    TicketZone zones[],
    int total_zones,
    int zone_id,
    int quantity,
    int is_early_bird,
    float *total
);
int timQR(const QrCheckIn qrs[], int total_qrs, const char qr_code[]);
enum CheckInStatus checkInQR(
    QrCheckIn qrs[],
    int total_qrs,
    const char qr_code[]
);
void hienThiKhuVuc(const TicketZone zones[], int total_zones);
void inKetQuaBanVe(enum SaleStatus status, float total);
void inKetQuaCheckIn(enum CheckInStatus status);

int main(void) {
    TicketZone zones[MAX_ZONES] = {
        {101, "VIP", 2000000.0f, 5, 2},
        {102, "GA", 800000.0f, 10, 3},
        {103, "Zone A", 1200000.0f, 8, 4}
    };

    QrCheckIn qrs[MAX_QRS] = {
        {"QR-VIP-001", 101, 0},
        {"QR-GA-001", 102, 1},
        {"QR-ZONEA-001", 103, 0}
    };

    TicketZone emptyZones[1];
    QrCheckIn emptyQrs[1];

    int total_zones = 3;
    int total_qrs = 3;
    float total = 0.0f;

    printf("=== TICKETBOX LIVE CONCERT ===\n");

    printf("\n1. DANH SACH KHU VUC BAN DAU\n");
    hienThiKhuVuc(zones, total_zones);

    printf("\n2. BAN 2 VE VIP, CO EARLY BIRD\n");
    inKetQuaBanVe(
        banVe(zones, total_zones, 101, 2, 1, &total),
        total
    );

    printf("\n3. MUA SO LUONG AM (-2)\n");
    inKetQuaBanVe(
        banVe(zones, total_zones, 102, -2, 0, &total),
        total
    );

    printf("\n4. MUA 0 VE\n");
    inKetQuaBanVe(
        banVe(zones, total_zones, 102, 0, 0, &total),
        total
    );

    printf("\n5. MUA 5 VE, VUOT GIOI HAN\n");
    inKetQuaBanVe(
        banVe(zones, total_zones, 102, 5, 0, &total),
        total
    );

    printf("\n6. KHU VUC VIP CHI CON 1 CHO, MUA 2 VE\n");
    zones[0].sold_tickets = 4;
    inKetQuaBanVe(
        banVe(zones, total_zones, 101, 2, 0, &total),
        total
    );

    printf("\n7. MA KHU VUC KHONG TON TAI\n");
    inKetQuaBanVe(
        banVe(zones, total_zones, 999, 1, 0, &total),
        total
    );

    printf("\n8. CHECK-IN QR-VIP-001 LAN DAU\n");
    inKetQuaCheckIn(checkInQR(qrs, total_qrs, "QR-VIP-001"));

    printf("\n9. CHECK-IN LAI QR-VIP-001\n");
    inKetQuaCheckIn(checkInQR(qrs, total_qrs, "QR-VIP-001"));

    printf("\n10. CHECK-IN QR KHONG TON TAI\n");
    inKetQuaCheckIn(checkInQR(qrs, total_qrs, "QR-UNKNOWN"));

    printf("\n11. CHECK-IN KHI DANH SACH QR RONG\n");
    inKetQuaCheckIn(checkInQR(emptyQrs, 0, "QR-VIP-001"));

    printf("\n12. BAN VE KHI DANH SACH KHU VUC RONG\n");
    inKetQuaBanVe(
        banVe(emptyZones, 0, 101, 1, 0, &total),
        total
    );

    printf("\n13. TRANG THAI KHU VUC SAU KIEM THU\n");
    hienThiKhuVuc(zones, total_zones);

    return 0;
}

int timKhuVuc(const TicketZone zones[], int total_zones, int zone_id) {
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

float tinhTongTien(float ticket_price, int quantity, int is_early_bird) {
    float giaSauGiam;

    giaSauGiam = ticket_price;

    if (is_early_bird == 1) {
        giaSauGiam = ticket_price * 0.85f;
    }

    return giaSauGiam * quantity;
}

enum SaleStatus banVe(
    TicketZone zones[],
    int total_zones,
    int zone_id,
    int quantity,
    int is_early_bird,
    float *total
) {
    int viTri;
    float thanhTien;

    if (total == NULL) {
        return SALE_INVALID_OUTPUT;
    }

    *total = 0.0f;

    if (zones == NULL || total_zones <= 0) {
        return SALE_EMPTY_ZONES;
    }

    if (quantity < 1 || quantity > MAX_BUY) {
        return SALE_INVALID_QUANTITY;
    }

    if (is_early_bird != 0 && is_early_bird != 1) {
        return SALE_INVALID_EARLY_BIRD;
    }

    viTri = timKhuVuc(zones, total_zones, zone_id);

    if (viTri == -1) {
        return SALE_ZONE_NOT_FOUND;
    }

    if (zones[viTri].ticket_price < 0 ||
        zones[viTri].total_capacity < 0 ||
        zones[viTri].sold_tickets < 0 ||
        zones[viTri].sold_tickets > zones[viTri].total_capacity) {
        return SALE_INVALID_ZONE_DATA;
    }

    if (quantity >
        zones[viTri].total_capacity - zones[viTri].sold_tickets) {
        return SALE_OVER_CAPACITY;
    }

    thanhTien = tinhTongTien(
        zones[viTri].ticket_price,
        quantity,
        is_early_bird
    );

    zones[viTri].sold_tickets += quantity;
    *total = thanhTien;

    return SALE_SUCCESS;
}

int timQR(const QrCheckIn qrs[], int total_qrs, const char qr_code[]) {
    int i;

    if (qrs == NULL || qr_code == NULL || total_qrs <= 0) {
        return -1;
    }

    for (i = 0; i < total_qrs; i++) {
        if (strcmp(qrs[i].qr_code, qr_code) == 0) {
            return i;
        }
    }

    return -1;
}

enum CheckInStatus checkInQR(
    QrCheckIn qrs[],
    int total_qrs,
    const char qr_code[]
) {
    int viTri;

    if (qrs == NULL || total_qrs <= 0) {
        return QR_LIST_EMPTY;
    }

    if (qr_code == NULL || qr_code[0] == '\0') {
        return QR_INVALID_DATA;
    }

    viTri = timQR(qrs, total_qrs, qr_code);

    if (viTri == -1) {
        return QR_NOT_FOUND;
    }

    if (qrs[viTri].is_used != 0 && qrs[viTri].is_used != 1) {
        return QR_INVALID_DATA;
    }

    if (qrs[viTri].is_used == 1) {
        return QR_ALREADY_USED;
    }

    qrs[viTri].is_used = 1;

    printf("Ma khu vuc cua QR: %d\n", qrs[viTri].zone_id);

    return QR_CHECKIN_SUCCESS;
}

void hienThiKhuVuc(const TicketZone zones[], int total_zones) {
    int i;

    if (zones == NULL || total_zones <= 0) {
        printf("Danh sach khu vuc rong.\n");
        return;
    }

    printf("%-8s %-12s %-15s %-12s %-12s\n",
           "Zone ID", "Ten khu vuc", "Gia ve", "Suc chua", "Da ban");

    for (i = 0; i < total_zones; i++) {
        printf("%-8d %-12s %-15.0f %-12d %-12d\n",
               zones[i].zone_id,
               zones[i].zone_name,
               zones[i].ticket_price,
               zones[i].total_capacity,
               zones[i].sold_tickets);
    }
}

void inKetQuaBanVe(enum SaleStatus status, float total) {
    switch (status) {
        case SALE_SUCCESS:
            printf("[THANH CONG] Tong tien: %.0f VND\n", total);
            break;
        case SALE_EMPTY_ZONES:
            printf("[LOI] Danh sach khu vuc rong.\n");
            break;
        case SALE_ZONE_NOT_FOUND:
            printf("[LOI] Khong tim thay ma khu vuc.\n");
            break;
        case SALE_INVALID_QUANTITY:
            printf("[LOI] So luong mua phai tu 1 den 4 ve.\n");
            break;
        case SALE_INVALID_EARLY_BIRD:
            printf("[LOI] Gia tri Early Bird khong hop le.\n");
            break;
        case SALE_INVALID_ZONE_DATA:
            printf("[LOI] Du lieu khu vuc khong hop le.\n");
            break;
        case SALE_OVER_CAPACITY:
            printf("[LOI] Khong du suc chua, tu choi giao dich.\n");
            break;
        case SALE_INVALID_OUTPUT:
            printf("[LOI] Dia chi bien nhan ket qua khong hop le.\n");
            break;
        default:
            printf("[LOI] Trang thai giao dich khong xac dinh.\n");
    }
}

void inKetQuaCheckIn(enum CheckInStatus status) {
    switch (status) {
        case QR_CHECKIN_SUCCESS:
            printf("[THANH CONG] Check-in QR thanh cong.\n");
            break;
        case QR_LIST_EMPTY:
            printf("[LOI] Danh sach QR rong.\n");
            break;
        case QR_NOT_FOUND:
            printf("[LOI] Khong tim thay ma QR.\n");
            break;
        case QR_ALREADY_USED:
            printf("[CANH BAO] QR da duoc su dung, tu choi check-in lai.\n");
            break;
        case QR_INVALID_DATA:
            printf("[LOI] Du lieu QR khong hop le.\n");
            break;
        default:
            printf("[LOI] Trang thai check-in khong xac dinh.\n");
    }
}

