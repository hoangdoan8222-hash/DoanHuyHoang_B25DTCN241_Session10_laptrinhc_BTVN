#include <stdio.h>
#include <string.h>

#define MAX_VE 50

struct VeSuKien {
    int maVe;
    char tenSuKien[50];
    char loaiVe[20];
    float giaGoc;
    int isEarlyBird;
    int daCheckIn;
};

float tinhGiaVe(float giaGoc, int isEarlyBird);
int timVeTheoMa(const struct VeSuKien dsVe[], int soLuongVe, int maVe);
int phatHanhVe(struct VeSuKien dsVe[], int soLuongVe, struct VeSuKien veMoi);
void checkInVe(struct VeSuKien dsVe[], int soLuongVe, int maVe);
int huyVe(struct VeSuKien dsVe[], int soLuongVe, int maVe);
void hienThiDanhSach(const struct VeSuKien dsVe[], int soLuongVe);

int main() {
    struct VeSuKien dsVe[MAX_VE] = {
        {1001, "Live Concert VIBE 2025", "VIP", 2000000.0f, 1, 0},
        {1002, "Live Concert VIBE 2025", "GA", 800000.0f, 0, 0},
        {1003, "Live Concert VIBE 2025", "Zone A", 1200000.0f, 1, 0}
    };

    struct VeSuKien veMoi = {
        1004, "Live Concert VIBE 2025", "VIP", 2500000.0f, 0, 0
    };

    int soLuongVe = 3;

    printf("=== HE THONG QUAN LY VE LIVE CONCERT VIBE 2025 ===\n");

    printf("\n1. DANH SACH VE BAN DAU\n");
    hienThiDanhSach(dsVe, soLuongVe);

    printf("\n2. PHAT HANH VE MOI\n");
    soLuongVe = phatHanhVe(dsVe, soLuongVe, veMoi);

    printf("\n3. CHECK-IN VE 1001\n");
    checkInVe(dsVe, soLuongVe, 1001);

    printf("\n4. THU QUET LAI VE 1001\n");
    checkInVe(dsVe, soLuongVe, 1001);

    printf("\n5. HUY VE 1002\n");
    soLuongVe = huyVe(dsVe, soLuongVe, 1002);

    printf("\n6. THU CHECK-IN MA VE KHONG TON TAI 9999\n");
    checkInVe(dsVe, soLuongVe, 9999);

    printf("\n7. DANH SACH VE SAU CAC THAO TAC\n");
    hienThiDanhSach(dsVe, soLuongVe);

    return 0;
}

float tinhGiaVe(float giaGoc, int isEarlyBird) {
    if (isEarlyBird == 1) {
        return giaGoc * 0.85f;
    }

    return giaGoc;
}

int timVeTheoMa(const struct VeSuKien dsVe[], int soLuongVe, int maVe) {
    int i;

    for (i = 0; i < soLuongVe; i++) {
        if (dsVe[i].maVe == maVe) {
            return i;
        }
    }

    return -1;
}

int phatHanhVe(struct VeSuKien dsVe[], int soLuongVe, struct VeSuKien veMoi) {
    if (soLuongVe >= MAX_VE) {
        printf("[LOI] Danh sach da day, khong the phat hanh them ve!\n");
        return soLuongVe;
    }

    if (timVeTheoMa(dsVe, soLuongVe, veMoi.maVe) != -1) {
        printf("[LOI] Ma ve %d da ton tai!\n", veMoi.maVe);
        return soLuongVe;
    }

    veMoi.daCheckIn = 0;
    dsVe[soLuongVe] = veMoi;
    soLuongVe++;

    printf("[THANH CONG] Da phat hanh ve %d (%s).\n",
           veMoi.maVe, veMoi.loaiVe);

    return soLuongVe;
}

void checkInVe(struct VeSuKien dsVe[], int soLuongVe, int maVe) {
    int viTri;

    if (soLuongVe == 0) {
        printf("[LOI] Danh sach rong, khong co ve de check-in!\n");
        return;
    }

    viTri = timVeTheoMa(dsVe, soLuongVe, maVe);

    if (viTri == -1) {
        printf("[LOI] Ma ve %d khong ton tai trong he thong!\n", maVe);
        return;
    }

    if (dsVe[viTri].daCheckIn == 1) {
        printf("[CANH BAO] Ve %d da check-in truoc do! Tu choi quet lai.\n",
               maVe);
        return;
    }

    dsVe[viTri].daCheckIn = 1;

    printf("[THANH CONG] Check-in ve %d (%s) thanh cong!\n",
           maVe, dsVe[viTri].loaiVe);
}

int huyVe(struct VeSuKien dsVe[], int soLuongVe, int maVe) {
    int viTri;
    int i;

    if (soLuongVe == 0) {
        printf("[LOI] Danh sach rong, khong co ve de huy!\n");
        return soLuongVe;
    }

    viTri = timVeTheoMa(dsVe, soLuongVe, maVe);

    if (viTri == -1) {
        printf("[LOI] Khong tim thay ve %d de huy!\n", maVe);
        return soLuongVe;
    }

    for (i = viTri; i < soLuongVe - 1; i++) {
        dsVe[i] = dsVe[i + 1];
    }

    soLuongVe--;

    printf("[THANH CONG] Da huy ve %d.\n", maVe);

    return soLuongVe;
}

void hienThiDanhSach(const struct VeSuKien dsVe[], int soLuongVe) {
    int i;
    float giaThanhToan;

    if (soLuongVe == 0) {
        printf("Danh sach hien khong co ve nao.\n");
        return;
    }

    printf("-----------------------------------------------------------------------------------------------\n");
    printf("%-8s %-28s %-12s %15s %-12s %-12s\n",
           "Ma ve", "Su kien", "Loai ve", "Gia thanh toan",
           "Early Bird", "Check-in");
    printf("-----------------------------------------------------------------------------------------------\n");

    for (i = 0; i < soLuongVe; i++) {
        giaThanhToan = tinhGiaVe(dsVe[i].giaGoc, dsVe[i].isEarlyBird);

        printf("%-8d %-28s %-12s %15.0f %-12s %-12s\n",
               dsVe[i].maVe,
               dsVe[i].tenSuKien,
               dsVe[i].loaiVe,
               giaThanhToan,
               dsVe[i].isEarlyBird == 1 ? "Co" : "Khong",
               dsVe[i].daCheckIn == 1 ? "Da check-in" : "Chua");
    }

    printf("-----------------------------------------------------------------------------------------------\n");
    printf("Tong so ve hien tai: %d/%d\n", soLuongVe, MAX_VE);
}

