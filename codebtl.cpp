#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//Cau truc du lieu 
typedef struct {
    char maSV[20];
    char hoTen[50];
    char gioiTinh[10];
    int namSinh;
    float toan, ly, hoa;
    float diemTB;
} SinhVien;
//Cau truc danh sach lien ket 
typedef struct Node {
    SinhVien sv;
    struct Node* next;
} Node;

typedef struct {
    Node* head;
} DanhSach;
//Ham nhap sinh vien 
void nhapSV(SinhVien* sv) {
    printf("Nhap ma SV: ");
    scanf("%s", sv->maSV);
    printf("Nhap ho ten: ");
    fflush(stdin);
    fgets(sv->hoTen, sizeof(sv->hoTen), stdin);
    sv->hoTen[strcspn(sv->hoTen, "\n")] = '\0';
    printf("Nhap gioi tinh: ");
    scanf("%s", sv->gioiTinh);
    printf("Nhap nam sinh: ");
    scanf("%d", &sv->namSinh);
    printf("Nhap diem Toan, Ly, Hoa: ");
    scanf("%f%f%f", &sv->toan, &sv->ly, &sv->hoa);
    sv->diemTB = (sv->toan + sv->ly + sv->hoa) / 3;
}
//Cap phat bo nho 
Node* taoNode(SinhVien sv) {
    Node* p = (Node*)malloc(sizeof(Node));
    p->sv = sv;
    p->next = NULL;
    return p;
}
//Ham them sinh vien 
void themSV(DanhSach* ds, SinhVien sv) {
    Node* p = taoNode(sv);
    if (ds->head == NULL) {
        ds->head = p;
    } else {
        Node* temp = ds->head;
        while (temp->next != NULL)
            temp = temp->next;
        temp->next = p;
    }
}
//Ham hien thi sinh vien 
void hienThi(DanhSach ds) {
    Node* p = ds.head;
    printf("\n%-10s %-10s %-10s %-10s %-6s %-6s %-6s %-6s\n",
           "MaSV", "HoTen", "GioiTinh", "NamSinh", "Toan", "Ly", "Hoa", "DTB");
    while (p != NULL) {
        SinhVien sv = p->sv;
        printf("%-10s %-10s %-10s %-10d %-6.2f %-6.2f %-6.2f %-6.2f\n",
               sv.maSV, sv.hoTen, sv.gioiTinh, sv.namSinh,
               sv.toan, sv.ly, sv.hoa, sv.diemTB);
        p = p->next;
    }
}
//Ham tinh diem trung binh 
float tinhDTB(DanhSach ds) {
    Node* p = ds.head;
    int count = 0;
    float tong = 0;
    while (p != NULL) {
        tong += p->sv.diemTB;
        count++;
        p = p->next;
    }
    return count == 0 ? 0 : tong / count;
}
//Ham loc sinh vien gioi 
DanhSach locSinhVienGioi(DanhSach ds) {
    DanhSach gioi;
    gioi.head = NULL;
    Node* p = ds.head;
    while (p != NULL) {
        if (p->sv.diemTB >= 8.0)
            themSV(&gioi, p->sv);
        p = p->next;
    }
    return gioi;
}
//Ham xoa sinh vien 
int xoaSinhVien(DanhSach* ds, const char* maSV) {
    Node* p = ds->head;
    Node* prev = NULL;

    while (p != NULL) {
        if (strcmp(p->sv.maSV, maSV) == 0) {
            if (prev == NULL) {
                ds->head = p->next;
            } else {
                prev->next = p->next;
            }
            free(p);
            return 1;
        }
        prev = p;
        p = p->next;
    }
    return 0;
}
//Ham ghi sinh vien gioi ra file 
void ghiSinhVienGioiRaFile(DanhSach ds, const char* filename) {
    FILE* f = fopen(filename, "w");
    if (f == NULL) {
        printf("Khong the mo file de ghi!\n");
        return;
    }

    Node* p = ds.head;
    fprintf(f, "%-10s %-10s %-10s %-10s %-6s %-6s %-6s %-6s\n",
            "MaSV", "HoTen", "GioiTinh", "NamSinh", "Toan", "Ly", "Hoa", "DTB");
    while (p != NULL) {
        if (p->sv.diemTB >= 8.0) {
            SinhVien sv = p->sv;
            fprintf(f, "%-10s %-10s %-10s %-10d %-6.2f %-6.2f %-6.2f %-6.2f\n",
                    sv.maSV, sv.hoTen, sv.gioiTinh, sv.namSinh,
                    sv.toan, sv.ly, sv.hoa, sv.diemTB);
        }
        p = p->next;
    }
    fclose(f);
    printf("Da ghi danh sach sinh vien gioi vao file %s\n", filename);
}

void giaiPhong(DanhSach* ds) {
    Node* p = ds->head;
    while (p != NULL) {
        Node* temp = p;
        p = p->next;
        free(temp);
    }
    ds->head = NULL;
}
//Ham chuc nang chinh 
int main() {
    DanhSach ds;
    ds.head = NULL;

    int choice;
    do {
        printf("\n================== MENU ==================\n");
        printf("1. Nhap danh sach sinh vien\n");
        printf("2. Hien thi danh sach\n");
        printf("3. Tinh diem TB lop\n");
        printf("4. Loc sinh vien gioi (DTB >= 8.0)\n");
        printf("5. Xoa sinh vien theo ma SV\n");
        printf("6. Ghi danh sach sinh vien gioi vao file\n");
        printf("7. Thoat\n");
        printf("Chon chuc nang: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: {
                int n;
                printf("Nhap so sinh vien: ");
                scanf("%d", &n);
                for (int i = 0; i < n; i++) {
                    printf("\n-- Sinh vien thu %d --\n", i + 1);
                    SinhVien sv;
                    nhapSV(&sv);
                    themSV(&ds, sv);
                }
                break;
            }
            case 2:
                hienThi(ds);
                break;
            case 3:
                printf("Diem trung binh ca lop: %.2f\n", tinhDTB(ds));
                break;
            case 4: {
                DanhSach gioi = locSinhVienGioi(ds);
                printf("Danh sach sinh vien gioi:");
                hienThi(gioi);
                giaiPhong(&gioi);
                break;
            }
            case 5: {
                char maXoa[20];
                printf("Nhap ma sinh vien can xoa: ");
                scanf("%s", maXoa);
                if (xoaSinhVien(&ds, maXoa)) {
                    printf("Xoa sinh vien thanh cong.\n");
                } else {
                    printf("Khong tim thay sinh vien co ma %s.\n", maXoa);
                }
                break;
            }
            case 6: {
                ghiSinhVienGioiRaFile(ds, "sinhvien_gioi.txt");
                break;
            }
            case 7:
                printf("Ket thuc chuong trinh.\n");
                break;
            default:
                printf("Lua chon khong hop le!\n");
        }

    } while (choice != 7);

    giaiPhong(&ds);
    return 0;
}
