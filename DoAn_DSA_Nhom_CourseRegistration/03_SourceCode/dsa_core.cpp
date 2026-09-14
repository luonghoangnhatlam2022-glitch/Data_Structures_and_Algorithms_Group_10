#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <list>
#include <unordered_map>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>

using namespace std;
using ll = long long;

// ==========================================
// 1. KHAI BAO CAC KIEU DU LIEU (STRUCT)
// ==========================================

// Thong tin sinh vien
struct SinhVien {
    string mssv;
    string ho_ten;
    string ma_nganh;
};

// Thong tin hoc phan
struct HocPhan {
    string ma_mon;
    string ten_mon;
    int si_so_toi_da;
    int si_so_hien_tai;
};

// Lich su thao tac (CR2)
struct LichSu {
    string thoi_gian;
    string hanh_dong;  // DANG_KY, VAO_HANG_CHO, RUT_HANG_CHO, HUY_MON, DON_LEN_CHINH_THUC
    string mssv;
    string ma_mon;
};

// Quan ly danh sach cho cua 1 mon hoc (MC2 & CR1)
// Ket hop: std::list (hang doi FIFO) + std::unordered_map (dinh vi iterator O(1))
struct DanhSachCho {
    list<string> hang_doi; // Thu tu sinh vien xep hang
    unordered_map<string, list<string>::iterator> vi_tri_node; // mssv -> con tro trong list
};

// ==========================================
// 2. DU LIEU LUU TREN RAM (IN-MEMORY)
// ==========================================

// MC1: Bang bam tra cuu chinh xac theo ma dinh danh O(1)
unordered_map<string, SinhVien> ds_sinh_vien;
unordered_map<string, HocPhan> ds_hoc_phan;

// Danh sach sinh vien hoc chinh thuc cua tung mon: ma_mon -> vector<mssv>
unordered_map<string, vector<string>> ds_chinh_thuc;

// MC2 & CR1: Danh sach cho cua tung mon: ma_mon -> DanhSachCho
unordered_map<string, DanhSachCho> ds_cho;

// CR2: Ngan xep luu vet cac thao tac vua dien ra
vector<LichSu> nhat_ky_he_thong;

// ==========================================
// 3. CAC HAM TIEN ICH NOI BO
// ==========================================

// Lay thoi gian hien tai dang chuoi de ghi log
string lay_thoi_gian_hien_tai() {
    auto hien_tai = chrono::system_clock::now();
    auto thoi_gian_c = chrono::system_clock::to_time_t(hien_tai);
    stringstream ss;
    ss << put_time(localtime(&thoi_gian_c), "%Y-%m-%d %H:%M:%S");
    return ss.str();
}

// Ghi mot hanh dong vao nhat ky (CR2)
void ghi_nhat_ky(string hanh_dong, string mssv, string ma_mon) {
    nhat_ky_he_thong.push_back({ lay_thoi_gian_hien_tai(), hanh_dong, mssv, ma_mon });
}

// ==========================================
// 4. CAC THAO TAC NGHIEP VU CHINH
// ==========================================

// MC1: Tra cuu thong tin sinh vien theo MSSV - O(1)
SinhVien* tim_sinh_vien(string mssv) {
    if (ds_sinh_vien.find(mssv) != ds_sinh_vien.end()) {
        return &ds_sinh_vien[mssv];
    }
    return nullptr;
}

// MC1: Tra cuu thong tin hoc phan theo ma mon - O(1)
HocPhan* tim_hoc_phan(string ma_mon) {
    if (ds_hoc_phan.find(ma_mon) != ds_hoc_phan.end()) {
        return &ds_hoc_phan[ma_mon];
    }
    return nullptr;
}

// MC2: Dang ky hoc phan (Tu dong day vao waitlist neu lop het cho)
string dang_ky_mon(string ma_mon, string mssv) {
    // 1. Kiem tra ton tai
    if (ds_hoc_phan.find(ma_mon) == ds_hoc_phan.end()) return "LOI_MON_KHONG_TON_TAI";
    if (ds_sinh_vien.find(mssv) == ds_sinh_vien.end()) return "LOI_SINH_VIEN_KHONG_TON_TAI";

    // 2. Kiem tra da co trong lop chinh thuc chua
    auto& lop_chinh_thuc = ds_chinh_thuc[ma_mon];
    for (const string& id : lop_chinh_thuc) {
        if (id == mssv) return "DA_DANG_KY_CHINH_THUC";
    }

    // 3. Kiem tra da nam trong danh sach cho chua
    auto& hang_cho = ds_cho[ma_mon];
    if (hang_cho.vi_tri_node.find(mssv) != hang_cho.vi_tri_node.end()) {
        return "DA_NAM_TRONG_DANH_SACH_CHO";
    }

    HocPhan& hp = ds_hoc_phan[ma_mon];

    // 4. Neu con cho -> cho vao danh sach chinh thuc
    if (hp.si_so_hien_tai < hp.si_so_toi_da) {
        hp.si_so_hien_tai++;
        lop_chinh_thuc.push_back(mssv);
        ghi_nhat_ky("DANG_KY", mssv, ma_mon);
        return "THANH_CONG_CHINH_THUC";
    }

    // 5. Neu da het cho -> xep vao hang doi danh sach cho (FIFO)
    hang_cho.hang_doi.push_back(mssv);
    auto it = --hang_cho.hang_doi.end();
    hang_cho.vi_tri_node[mssv] = it; // Luu iterator de xoa O(1) ve sau
    ghi_nhat_ky("VAO_HANG_CHO", mssv, ma_mon);
    return "THANH_CONG_VAO_HANG_CHO";
}

// MC2 (Trigger): Sinh vien huy mon chinh thuc
// Neu co nguoi trong hang cho -> tu dong don nguoi nop som nhat len thay the
string huy_mon_chinh_thuc(string ma_mon, string mssv) {
    if (ds_hoc_phan.find(ma_mon) == ds_hoc_phan.end()) return "LOI_MON_KHONG_TON_TAI";

    auto& lop_chinh_thuc = ds_chinh_thuc[ma_mon];
    int vi_tri = -1;
    for (int i = 0; i < (int)lop_chinh_thuc.size(); ++i) {
        if (lop_chinh_thuc[i] == mssv) {
            vi_tri = i;
            break;
        }
    }

    // Sinh vien khong co trong lop chinh thuc
    if (vi_tri == -1) return "SINH_VIEN_KHONG_CO_TRONG_LOP";

    // Xoa sinh vien khoi lop
    lop_chinh_thuc.erase(lop_chinh_thuc.begin() + vi_tri);
    ghi_nhat_ky("HUY_MON", mssv, ma_mon);

    HocPhan& hp = ds_hoc_phan[ma_mon];
    auto& hang_cho = ds_cho[ma_mon];

    // Kiem tra danh sach cho: neu co nguoi, lay nguoi dau tien vao lop (FIFO)
    if (!hang_cho.hang_doi.empty()) {
        string mssv_duoc_chon = hang_cho.hang_doi.front();
        hang_cho.hang_doi.pop_front();
        hang_cho.vi_tri_node.erase(mssv_duoc_chon);

        lop_chinh_thuc.push_back(mssv_duoc_chon);
        ghi_nhat_ky("DON_LEN_CHINH_THUC", mssv_duoc_chon, ma_mon);
        return "DA_HUY_VA_DON_SINH_VIEN_" + mssv_duoc_chon;
    } else {
        hp.si_so_hien_tai--;
        return "HUY_THANH_CONG";
    }
}

// CR1: Sinh vien chu dong rut khoi danh sach cho o bat ky vi tri nao - O(1)
bool rut_khoi_hang_cho(string ma_mon, string mssv) {
    if (ds_hoc_phan.find(ma_mon) == ds_hoc_phan.end()) return false;

    auto& hang_cho = ds_cho[ma_mon];
    auto it = hang_cho.vi_tri_node.find(mssv);
    if (it == hang_cho.vi_tri_node.end()) return false; // Khong co trong hang cho

    // Xoa node truc tiep khoi list bang iterator trong O(1)
    hang_cho.hang_doi.erase(it->second);
    // Xoa khoi bang bam trong O(1)
    hang_cho.vi_tri_node.erase(it);

    ghi_nhat_ky("RUT_HANG_CHO", mssv, ma_mon);
    return true;
}

// CR2: Lay ra K thao tac gan nhat (theo thu tu thoi gian dao nguoc)
vector<LichSu> lay_lich_su_gan_day(int so_luong) {
    vector<LichSu> ket_qua;
    int dem = 0;
    for (auto it = nhat_ky_he_thong.rbegin(); it != nhat_ky_he_thong.rend() && dem < so_luong; ++it, ++dem) {
        ket_qua.push_back(*it);
    }
    return ket_qua;
}