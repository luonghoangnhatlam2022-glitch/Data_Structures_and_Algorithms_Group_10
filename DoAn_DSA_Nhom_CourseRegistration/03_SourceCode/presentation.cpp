#pragma once
#include <iostream>
#include <string>
#include <vector>

#include "dsa_core.cpp"
using namespace std;

// ========================================================
// TANG PRESENTATION: GIAO DIEN DONG LENH (CONSOLE CLI)
// ========================================================

// 1. In menu cac chuc nang ra man hinh
void in_menu() {
    cout << "\n============================================\n";
    cout << "     HE THONG DANG KY HOC PHAN & WAITLIST   \n";
    cout << "============================================\n";
    cout << "1. Tra cuu thong tin Sinh vien \n";
    cout << "2. Tra cuu thong tin Hoc phan \n";
    cout << "3. Dang ky Mon hoc \n";
    cout << "4. Huy dang ky Mon hoc chinh thuc \n";
    cout << "5. Rut khoi Danh sach cho \n";
    cout << "6. Xem Lich su Thao tac gan nhat \n";
    cout << "0. Luu du lieu va Thoat chuong trinh\n";
    cout << "============================================\n";
    cout << "Nhap lua chon cua ban (0 - 6): ";
}

// 2. Chuc nang 1: Tra cuu sinh vien (MC1)
void man_hinh_tim_sinh_vien() {
    string ma_sv;
    cout << "-> Nhap MSSV can tra cuu: ";
    cin >> ma_sv;

    SinhVien* sv = tim_sinh_vien(ma_sv);
    if (sv != nullptr) {
        cout << "[Ket qua] MSSV: " << sv->mssv << "\n";
        cout << "          Ho va ten: " << sv->ho_ten << "\n";
        cout << "          Khoa/Nganh: " << sv->ma_nganh << "\n";
    } else {
        cout << "[Loi] Khong tim thay sinh vien co MSSV: " << ma_sv << "\n";
    }
}

// 3. Chuc nang 2: Tra cuu mon hoc (MC1)
void man_hinh_tim_hoc_phan() {
    string ma_mh;
    cout << "-> Nhap Ma mon hoc can tra cuu: ";
    cin >> ma_mh;

    HocPhan* hp = tim_hoc_phan(ma_mh);
    if (hp != nullptr) {
        cout << "[Ket qua] Ma hoc phan: " << hp->ma_mon << "\n";
        cout << "          Ten mon hoc: " << hp->ten_mon << "\n";
        cout << "          Si so hien tai: " << hp->si_so_hien_tai << " / " << hp->si_so_toi_da << "\n";

        // Hien thi them so luong nguoi dang cho neu co
        int so_nguoi_cho = ds_cho[ma_mh].hang_doi.size();
        cout << "          So sinh vien dang cho: " << so_nguoi_cho << "\n";
    } else {
        cout << "[Loi] Khong tim thay ma mon hoc: " << ma_mh << "\n";
    }
}

// 4. Chuc nang 3: Dang ky mon hoc (MC2)
void man_hinh_dang_ky() {
    string ma_mh, ma_sv;
    cout << "-> Nhap Ma mon hoc muon dang ky: ";
    cin >> ma_mh;
    cout << "-> Nhap MSSV: ";
    cin >> ma_sv;

    string ket_qua = dang_ky_mon(ma_mh, ma_sv);

    if (ket_qua == "THANH_CONG_CHINH_THUC") {
        cout << "[Thong bao] Dang ky thanh cong vao lop chinh thuc!\n";
    } else if (ket_qua == "THANH_CONG_VAO_HANG_CHO") {
        int vi_tri = ds_cho[ma_mh].hang_doi.size();
        cout << "[Thong bao] Lop da day si so! Ban da duoc xep vao Danh sach cho o vi tri so " << vi_tri << ".\n";
    } else if (ket_qua == "DA_DANG_KY_CHINH_THUC") {
        cout << "[Loi] Sinh vien da co ten trong danh sach lop chinh thuc!\n";
    } else if (ket_qua == "DA_NAM_TRONG_DANH_SACH_CHO") {
        cout << "[Loi] Sinh vien da co ten trong hang doi cho cua mon nay!\n";
    } else if (ket_qua == "LOI_MON_KHONG_TON_TAI") {
        cout << "[Loi] Ma mon hoc khong ton tai!\n";
    } else if (ket_qua == "LOI_SINH_VIEN_KHONG_TON_TAI") {
        cout << "[Loi] Ma sinh vien khong ton tai!\n";
    }
}

// 5. Chuc nang 4: Huy mon chinh thuc (MC2 Trigger)
void man_hinh_huy_mon() {
    string ma_mh, ma_sv;
    cout << "-> Nhap Ma mon hoc muon huy: ";
    cin >> ma_mh;
    cout << "-> Nhap MSSV: ";
    cin >> ma_sv;

    string ket_qua = huy_mon_chinh_thuc(ma_mh, ma_sv);

    if (ket_qua == "HUY_THANH_CONG") {
        cout << "[Thong bao] Da huy mon thanh cong (khong co sinh vien nao trong hang cho).\n";
    } else if (ket_qua.rfind("DA_HUY_VA_DON_SINH_VIEN_", 0) == 0) {
        string sv_don_len = ket_qua.substr(24);
        cout << "[Thong bao] Huy mon thanh cong! Sinh vien [" << sv_don_len
             << "] tu danh sach cho da duoc tu dong don len lop chinh thuc.\n";
    } else if (ket_qua == "SINH_VIEN_KHONG_CO_TRONG_LOP") {
        cout << "[Loi] Sinh vien khong theo hoc mon nay!\n";
    } else if (ket_qua == "LOI_MON_KHONG_TON_TAI") {
        cout << "[Loi] Ma mon hoc khong ton tai!\n";
    }
}

// 6. Chuc nang 5: Rut khoi danh sach cho (CR1)
void man_hinh_rut_hang_cho() {
    string ma_mh, ma_sv;
    cout << "-> Nhap Ma mon hoc: ";
    cin >> ma_mh;
    cout << "-> Nhap MSSV can rut khoi hang cho: ";
    cin >> ma_sv;

    bool thanh_cong = rut_khoi_hang_cho(ma_mh, ma_sv);

    if (thanh_cong) {
        cout << "[Thong bao] Da rut ten khoi danh sach cho thanh cong!\n";
    } else {
        cout << "[Loi] Sinh vien khong co trong danh sach cho hoac ma mon khong hop le!\n";
    }
}

// 7. Chuc nang 6: Xem lich su thao tac gan day (CR2)
void man_hinh_xem_lich_su() {
    int k;
    cout << "-> Nhap so thao tac gan nhat muon xem: ";
    if (!(cin >> k) || k <= 0) {
        cout << "[Loi] So luong khong hop le!\n";
        return;
    }

    vector<LichSu> nhat_ky = lay_lich_su_gan_day(k);

    cout << "\n----- DANH SACH " << nhat_ky.size() << " THAO TAC GAN NHAT -----\n";
    if (nhat_ky.empty()) {
        cout << "(Chua co thao tac nao duoc ghi nhan)\n";
        return;
    }

    for (const auto& entry : nhat_ky) {
        cout << "[" << entry.thoi_gian << "] "
             << "Hanh dong: " << entry.hanh_dong
             << " | MSSV: " << entry.mssv
             << " | Ma mon: " << entry.ma_mon << "\n";
    }
    cout << "-------------------------------------------\n";
}

// 8. Vong lap dieu khien chinh cua giao dien
void chay_giao_dien() {
    int lua_chon = -1;
    while (true) {
        in_menu();
        if (!(cin >> lua_chon)) {
            cin.clear();
            cin.ignore(10000, '\n');
            cout << "[Loi] Vui long nhap so tu 0 den 6!\n";
            continue;
        }

        if (lua_chon == 0) {
            cout << "\nDang chuan bi thoat chuong trinh...\n";
            break;
        }

        switch (lua_chon) {
            case 1: man_hinh_tim_sinh_vien(); break;
            case 2: man_hinh_tim_hoc_phan(); break;
            case 3: man_hinh_dang_ky(); break;
            case 4: man_hinh_huy_mon(); break;
            case 5: man_hinh_rut_hang_cho(); break;
            case 6: man_hinh_xem_lich_su(); break;
            default:
                cout << "[Loi] Lua chon khong hop le, vui long chon lai!\n";
                break;
        }
    }
}