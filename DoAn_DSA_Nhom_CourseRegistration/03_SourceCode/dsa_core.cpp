
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