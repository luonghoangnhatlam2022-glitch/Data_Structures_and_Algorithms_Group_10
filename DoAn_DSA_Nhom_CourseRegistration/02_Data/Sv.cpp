#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>

using namespace std;

vector<int> m = {19, 7, 24, 8, 13, 26, 22, 9, 14, 17, 28, 3};

void Ngay_Thang(vector<pair<int, int>> &a, int n)
{
    int seedn = 0, seedt = 0, mn = 0, mt = 0;
    int i = 0;
    while (i < n)
    {
        seedt = (seedt + m[mt]) * m[mt] % 12 + 1;
        mt = (mt + 1) % 12;
        while (true)
        {
            seedn = (seedn + m[mn]) * m[mn] % 31 + 1;
            mn = (mn + 1) % 12;
            if (seedn <= 28)
                break;
            if (seedt == 1 || seedt == 3 || seedt == 5 || seedt == 7 || seedt == 8 || seedt == 10 || seedt == 12)
                break;
            if (seedt != 4 && seedn <= 30)
                break;
        }
        a[i] = {seedn, seedt};
    }
}
void Ho_Ten(vector<string> &s, int n)
{
    ifstream file("ten.csv");
    string line;
    vector<string> Ho, Lnam, Lnu;
    vector<pair<string, int>> Ten;
    int i = 0;
    while (getline(file, line))
    {
        stringstream ss(line);
        string item;
        vector<string> row;

        while (getline(ss, item, ','))
        {
            row.push_back(item);
        }
        if (i < 10)
        {
            Ho.push_back(row[0]);
        }
        else if (i < 20)
        {
            Lnam.push_back(row[0]);
        }
        else if (i < 30)
        {
            Lnu.push_back(row[0]);
        }
        else
        {
            if (row.size() >= 2)
            {
                Ten.push_back({row[0], stoi(row[1])});
            }
        }
        i++;
    }
    file.close();
    int j = 0;

    int seedHo = 0;
    int seedLot = 2;
    int seedDoDai = 1;

    auto update_seed = [&](int &seed, int x, int y = 0)
    {
        seed = ((seed + m[j]) * m[j]) % x + y;
    };

    int soVongLap = n / 100;
    for (int i = 0; i < 100; ++i)
    {
        for (int k = 0; k < soVongLap; ++k)
        {

            // 1. Cập nhật seed Độ dài và tính độ dài tên
            // Truyền x = 10 (có thể điều chỉnh) để lấy biến thiên, sau đó mod 3 + 2
            seedDoDai = ((seedDoDai - 2 + m[j]) * m[j]) % 3 + 2; // Sẽ ra 2, 3, hoặc 4

            // 2. Lấy Tên (từ trên xuống theo i) và kiểm tra giới tính
            string ten = Ten[i].first;
            int gioi_tinh = Ten[i].second; // 0: nam nữ đều được, 1: nam, 2: nữ

            if (gioi_tinh == 0)
            {
                // j ở đây đang là j hiện tại sau khi đã bị dịch chuyển bởi update_seed
                gioi_tinh = (i + k) % 2 + 1; // Đảm bảo luôn ra 1 (nam) hoặc 2 (nữ)
            }

            // 3. Lấy Họ (x=10 vì mảng Ho có 10 phần tử)
            update_seed(seedHo, 10, 0);
            string ho1 = Ho[seedHo];
            string ho2 = "";

            // Nếu độ dài = 4 thì chạy seed lấy thêm họ 2
            if (seedDoDai == 4)
            {
                j = (j + 1) % 12;
                update_seed(seedHo, 10, 0);
                ho2 = Ho[seedHo];
            }

            // 4. Lấy Lót (Nếu độ dài >= 3)
            string lot = "";
            if (seedDoDai >= 3)
            {
                do
                {
                    update_seed(seedLot, 10, 0); // x=10 vì mảng lót có 10 phần tử
                    j = (j + 1) % 12;
                    if (gioi_tinh == 1)
                    {
                        lot = Lnam[seedLot];
                    }
                    else
                    {
                        lot = Lnu[seedLot];
                    }
                } while (lot == ten);
            }

            // 5. Nối chuỗi tạo tên hoàn chỉnh
            string full_name = ho1;
            if (seedDoDai == 4)
            {
                full_name += " " + ho2;
            }
            if (seedDoDai >= 3)
            {
                full_name += " " + lot;
            }
            full_name += " " + ten;

            // Push vào vector tổng
            s.push_back(full_name);
            j = (j + 1) % 12;
        }
    }
}

int main()
{
    vector<string> name;
    vector<pair<int, int>> sinh;
    vector<string> nganh = {"CNTT", "ATTT", "KTDL", "AI"};
    int n = 200;
    Ho_Ten(name, n);
    for (int i = 0; i < n; i++)
    {
        cout << name[i] << endl;
    }
}