#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

// Nhúng trực tiếp file core.cpp để nạp dữ liệu vào ds_sinh_vien và ds_hoc_phan
#include "dsa_core.cpp"

using namespace std;

// ========================================================
// TANG PERSISTENCE: DOC DU LIEU TU FILE CSV NAP VAO RAM
// ========================================================

// 1. Doc file danh sach sinh vien (students.csv)
// Dinh dang mong doi: mssv,ho_ten,ma_nganh
bool nap_du_lieu_sinh_vien(string duong_dan_file) {
    ifstream tep_tin(duong_dan_file);
    if (!tep_tin.is_open()) {
        cout << "[Loi] Khong the mo file: " << duong_dan_file << endl;
        return false;
    }

    string dong_du_lieu;
    // Doc bo dong tieu de dau tien (header)
    getline(tep_tin, dong_du_lieu);

    int dem = 0;
    while (getline(tep_tin, dong_du_lieu)) {
        if (dong_du_lieu.empty()) continue;

        stringstream tach_chuoi(dong_du_lieu);
        string ma_sv, ten_sv, nganh;

        // Tach cac cot duoc phan cach bang dau phay
        if (getline(tach_chuoi, ma_sv, ',') &&
            getline(tach_chuoi, ten_sv, ',') &&
            getline(tach_chuoi, nganh, ',')) {

            SinhVien sv;
            sv.mssv = ma_sv;
            sv.ho_ten = ten_sv;
            sv.ma_nganh = nganh;

            // Nap truc tiep vao bang bam O(1) tren RAM
            ds_sinh_vien[ma_sv] = sv;
            dem++;
        }
    }

    tep_tin.close();
    cout << "[Thanh cong] Da nap " << dem << " sinh vien vao RAM." << endl;
    return true;
}

// 2. Doc file danh sach hoc phan (courses.csv)
// Dinh dang mong doi: ma_mon,ten_mon,si_so_toi_da
bool nap_du_lieu_hoc_phan(string duong_dan_file) {
    ifstream tep_tin(duong_dan_file);
    if (!tep_tin.is_open()) {
        cout << "[Loi] Khong the mo file: " << duong_dan_file << endl;
        return false;
    }

    string dong_du_lieu;
    // Doc bo dong tieu de dau tien (header)
    getline(tep_tin, dong_du_lieu);

    int dem = 0;
    while (getline(tep_tin, dong_du_lieu)) {
        if (dong_du_lieu.empty()) continue;

        stringstream tach_chuoi(dong_du_lieu);
        string ma_mh, ten_mh, chuoi_si_so;

        // Tach cac cot duoc phan cach bang dau phay
        if (getline(tach_chuoi, ma_mh, ',') &&
            getline(tach_chuoi, ten_mh, ',') &&
            getline(tach_chuoi, chuoi_si_so, ',')) {

            HocPhan hp;
            hp.ma_mon = ma_mh;
            hp.ten_mon = ten_mh;
            hp.si_so_toi_da = stoi(chuoi_si_so);
            hp.si_so_hien_tai = 0; // Luc khoi dong si so hien tai mac dinh la 0

            // Nap truc tiep vao bang bam O(1) tren RAM
            ds_hoc_phan[ma_mh] = hp;
            dem++;
        }
    }

    tep_tin.close();
    cout << "[Thanh cong] Da nap " << dem << " hoc phan vao RAM." << endl;
    return true;
}

// 3. Luu nhat ky he thong ra file CSV truoc khi tat chuong trinh
bool luu_nhat_ky_ra_file(string duong_dan_file) {
    ofstream tep_tin(duong_dan_file);
    if (!tep_tin.is_open()) {
        cout << "[Loi] Khong the ghi file nhat ky: " << duong_dan_file << endl;
        return false;
    }

    // Ghi tieu de
    tep_tin << "thoi_gian,hanh_dong,mssv,ma_mon\n";

    // Duyet qua toan bo vector lich su va ghi dong
    for (const auto& log : nhat_ky_he_thong) {
        tep_tin << log.thoi_gian << ","
                << log.hanh_dong << ","
                << log.mssv << ","
                << log.ma_mon << "\n";
    }

    tep_tin.close();
    cout << "[Thanh cong] Da luu nhat ky hoat dong ra file." << endl;
    return true;
}