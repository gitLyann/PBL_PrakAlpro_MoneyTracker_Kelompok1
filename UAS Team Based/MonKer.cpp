#include <bits/stdc++.h>
using namespace std;

const int MAX_DATA = 500;
const string DATA_FILE = "monke_data.txt";

// Data arrays
string kategoriArr[MAX_DATA];
int nominalArr[MAX_DATA];
string tanggalArr[MAX_DATA]; // "dd-mm-yyyy"
int jumlahData = 0;

// --- Prototype Functions ---
void mainMenu();
void tambahPengeluaran();
void tampilPengeluaran();
void totalPengeluaranRentang();
void cariPengeluaran();
void sortPengeluaran();
void hapusPengeluaran();

bool validTanggal(const string &t);
bool validNominal(const string &s);
bool validKategori(const string &s);
int  tanggalToKey(const string &t);
string toLowerStr(const string &s);

// --- File Management Functions ---
void muatData();
void simpanData();

// ---------- File Management ----------
void muatData() {
    ifstream file(DATA_FILE);
    if (!file.is_open()) {
        // File belum ada, tidak masalah
        return;
    }

    jumlahData = 0;
    string line;
    
    while (getline(file, line) && jumlahData < MAX_DATA) {
        // Format: kategori|nominal|tanggal
        size_t pos1 = line.find('|');
        size_t pos2 = line.rfind('|');
        
        if (pos1 != string::npos && pos2 != string::npos && pos1 != pos2) {
            kategoriArr[jumlahData] = line.substr(0, pos1);
            nominalArr[jumlahData] = stoi(line.substr(pos1 + 1, pos2 - pos1 - 1));
            tanggalArr[jumlahData] = line.substr(pos2 + 1);
            jumlahData++;
        }
    }
    
    file.close();
    cout << "Data berhasil dimuat! Total: " << jumlahData << " pengeluaran.\n";
}

void simpanData() {
    ofstream file(DATA_FILE);
    if (!file.is_open()) {
        cout << "Gagal menyimpan data!\n";
        return;
    }

    for (int i = 0; i < jumlahData; i++) {
        file << kategoriArr[i] << "|" 
             << nominalArr[i] << "|" 
             << tanggalArr[i] << "\n";
    }
    
    file.close();
}

// ---------- Utility ----------
string toLowerStr(const string &s) {
    string r = s;
    for (char &c : r) c = tolower(c);
    return r;
}

bool validKategori(const string &s) {
    if (s.empty()) return false;
    for (char c : s) {
        if (!(isalpha(c) || c==' ')) return false;
    }
    return true;
}

bool validNominal(const string &s) {
    if (s.empty()) return false;
    for (char c : s) if (!isdigit(c)) return false;
    return true;
}

bool validTanggal(const string &t) {
    if (t.size() != 10) return false;
    if (t[2] != '-' || t[5] != '-') return false;

    for (int i = 0; i < 10; i++) {
        if (i==2 || i==5) continue;
        if (!isdigit(t[i])) return false;
    }

    int dd = stoi(t.substr(0,2));
    int mm = stoi(t.substr(3,2));
    int yy = stoi(t.substr(6,4));

    if (mm < 1 || mm > 12) return false;

    int dim[] = {0,31,28,31,30,31,30,31,31,30,31,30,31};
    if (mm == 2) {
        bool leap = (yy%4==0 && yy%100!=0) || (yy%400==0);
        if (leap) dim[2] = 29;
    }

    return (dd >= 1 && dd <= dim[mm]);
}

int tanggalToKey(const string &t) {
    int dd = stoi(t.substr(0,2));
    int mm = stoi(t.substr(3,2));
    int yy = stoi(t.substr(6,4));
    return yy * 10000 + mm * 100 + dd;
}

void tampilHeader() {
    cout << left << setw(4) << "No" << " | "
         << left << setw(20) << "Kategori" << " | "
         << right << setw(10) << "Nominal" << " | "
         << " Tanggal\n";
    cout << "----+----------------------+------------+------------\n";
}


// ================== MAIN MENU ===================
void mainMenu() {
    while (true) {
        cout << "\n=== MonKe - Money Tracker ===\n";
        cout << "1. Tambah Pengeluaran\n";
        cout << "2. Tampil Pengeluaran (Terbaru Sampai Terlama)\n";
        cout << "3. Total Pengeluaran (Rentang Tanggal)\n";
        cout << "4. Cari Pengeluaran\n";
        cout << "5. Sort Pengeluaran\n";
        cout << "6. Hapus Pengeluaran\n";
        cout << "7. Keluar\n";
        cout << "Pilih menu [1-7]: ";

        string pilih;
        getline(cin, pilih);

        if (pilih == "1") tambahPengeluaran();
        else if (pilih == "2") tampilPengeluaran();
        else if (pilih == "3") totalPengeluaranRentang();
        else if (pilih == "4") cariPengeluaran();
        else if (pilih == "5") sortPengeluaran();
        else if (pilih == "6") hapusPengeluaran();
        else if (pilih == "7") {
            simpanData();
            cout << "\nData tersimpan! Terima kasih telah menggunakan MonKe!\n";
            break;
        } 
        else cout << "Pilihan tidak valid!\n";
    }
}


// ================== MENU 1 ===================
void tambahPengeluaran() {
    if (jumlahData >= MAX_DATA) {
        cout << "Kapasitas penuh!\n";
        return;
    }

    string kat, sNom, t;

    cout << "\n--- Tambah Pengeluaran ---\n";

    // Input Kategori
    while (true){
        cout << "Kategori: ";
        getline(cin, kat);

        if (validKategori(kat)) break; 
        cout << "Kategori tidak valid.\n";
        
    }

    // Input Nominal
    while (true){
        cout << "Nominal: ";
        getline(cin, sNom);

        if (validNominal(sNom)) break; 
        cout << "Nominal tidak valid.\n";
    }

    // Input Tanggal
    while (true){
        cout << "Tanggal (dd-mm-yyyy): ";
        getline(cin, t);

        if (validTanggal(t)) break; 
        cout << "Tanggal tidak valid.\n";
    }

    kategoriArr[jumlahData] = kat;
    nominalArr[jumlahData] = stoi(sNom);
    tanggalArr[jumlahData] = t;
    jumlahData++;

    simpanData(); // Auto-save setelah tambah data
    cout << "Pengeluaran berhasil ditambahkan dan tersimpan!\n";
}


// ================== MENU 2 ===================
void tampilPengeluaran() {
    cout << "\n--- Daftar Pengeluaran (Terbaru Sampai Terlama) ---\n";
    if (jumlahData == 0) {
        cout << "Belum ada data.\n";
        return;
    }

    tampilHeader();
    int nomor = 1;

    for (int i = jumlahData - 1; i >= 0; i--) {
        cout << setw(4) << nomor++ << " | "
             << left << setw(20) << kategoriArr[i] << " | "
             << right << setw(10) << nominalArr[i] << " | "
             << " " << tanggalArr[i] << "\n";
    }
}


// ================== MENU 3 ===================
void totalPengeluaranRentang() {
    if (jumlahData == 0) {
        cout << "Belum ada data pengeluaran.\n";
        return;
    }

    cout << "\n--- Total Pengeluaran (Rentang Tanggal) ---\n";

    string tAwal, tAkhir;

    // ---------------- INPUT TANGGAL AWAL ----------------
    while (true) {
        cout << "Tanggal Awal (dd-mm-yyyy): ";
        getline(cin, tAwal);

        if (validTanggal(tAwal)) break;
        cout << "Tanggal awal tidak valid! Format harus dd-mm-yyyy.\n";
    }

    // ---------------- INPUT TANGGAL AKHIR ----------------
    while (true) {
        cout << "Tanggal Akhir (dd-mm-yyyy): ";
        getline(cin, tAkhir);

        if (validTanggal(tAkhir)) break;
        cout << "Tanggal akhir tidak valid! Format harus dd-mm-yyyy.\n";
    }

    // Convert to comparable key
    int keyAwal = tanggalToKey(tAwal);
    int keyAkhir = tanggalToKey(tAkhir);

    if (keyAwal > keyAkhir) swap(keyAwal, keyAkhir);

    // ---------------- HITUNG TOTAL ----------------
    int total = 0, nomor = 1;

    cout << "\nData dalam rentang:\n";
    tampilHeader();

    for (int i = 0; i < jumlahData; i++) {
        int keyNow = tanggalToKey(tanggalArr[i]);

        if (keyNow >= keyAwal && keyNow <= keyAkhir) {
            cout << setw(4) << nomor++ << " | "
                 << left << setw(20) << kategoriArr[i] << " | "
                 << right << setw(10) << nominalArr[i] << " | "
                 << " " << tanggalArr[i] << "\n";
            total += nominalArr[i];
        }
    }

    cout << "\nTotal Pengeluaran Periode: Rp " << total << "\n";
}

// ================== MENU 4 ===================
void cariPengeluaran() {
    if (jumlahData == 0) {
        cout << "Belum ada data.\n";
        return;
    }

    cout << "\n--- Cari Pengeluaran ---\n";

    string pilih;

    // ---------------- PILIH MENU VALID ----------------
    while (true) {
        cout << "Cari berdasarkan:\n";
        cout << "1. Kategori\n";
        cout << "2. Tanggal (dd-mm-yyyy)\n";
        cout << "Pilih (1/2): ";
        getline(cin, pilih);

        if (pilih == "1" || pilih == "2") break;

        cout << "Pilihan tidak valid! Silakan pilih 1 atau 2.\n";
    }

    // CARI BERDASARKAN KATEGORI 
    if (pilih == "1") {
        string key;

        // Input kategori valid
        while (true) {
            cout << "Masukkan kategori: ";
            getline(cin, key);

            if (validKategori(key)) break;

            cout << "Kategori tidak valid! Hanya huruf dan spasi.\n";
        }

        // Lowercase untuk pencocokan
        string keyL = toLowerStr(key);

        cout << "\nHasil pencarian untuk kategori \"" << key << "\" :\n";

        tampilHeader();
        int no = 1;

        for (int i = 0; i < jumlahData; i++) {
            if (toLowerStr(kategoriArr[i]) == keyL) {
                cout << setw(4) << no++ << " | "
                     << left << setw(20) << kategoriArr[i] << " | "
                     << right << setw(10) << nominalArr[i] << " | "
                     << " " << tanggalArr[i] << "\n";
            }
        }

        // Jika tidak ada data
        if (no == 1) {
            cout << "Tidak ada data dengan kategori tersebut.\n";
        }

        return;
    }

    // CARI BERDASARKAN TANGGAL
    else if (pilih == "2") {
        string tgl;

        // Input tanggal valid
        while (true) {
            cout << "Masukkan tanggal (dd-mm-yyyy): ";
            getline(cin, tgl);

            if (validTanggal(tgl)) break;
            cout << "Format tanggal salah! Contoh: 05-11-2024\n";
        }

        cout << "\nHasil pencarian tanggal " << tgl << ":\n";

        tampilHeader();
        int no = 1;

        for (int i = 0; i < jumlahData; i++) {
            if (tanggalArr[i] == tgl) {
                cout << setw(4) << no++ << " | "
                     << left << setw(20) << kategoriArr[i] << " | "
                     << right << setw(10) << nominalArr[i] << " | "
                     << " " << tanggalArr[i] << "\n";
            }
        }

        if (no == 1) {
            cout << "Tidak ada data pada tanggal tersebut.\n";
        }

        return;
    }
}



// ================== MENU 5 ===================
void sortPengeluaran() {
    if (jumlahData == 0) {
        cout << "Belum ada data.\n";
        return;
    }

    cout << "\n--- Sort Pengeluaran ---\n";

    string pilihSort;

    // -----------------------------------------
    //       Validasi pilihan menu sort
    // -----------------------------------------
    while (true) {
        cout << "Sort berdasarkan:\n";
        cout << "1. Nominal\n";
        cout << "2. Tanggal\n";
        cout << "3. Kategori\n";
        cout << "Pilih (1/2/3): ";
        getline(cin, pilihSort);

        if (pilihSort == "1" || pilihSort == "2" || pilihSort == "3") break;

        cout << "Pilihan tidak valid! Silakan pilih 1/2/3.\n";
    }


    // -----------------------------------------
    //       Validasi ascending/descending
    // -----------------------------------------
    string pilihOrder;
    bool asc = true;

    while (true) {
        cout << "Urutan:\n";
        cout << "1. Ascending (kecil ke besar)\n";
        cout << "2. Descending (besar ke kecil)\n";
        cout << "Pilih (1/2): ";
        getline(cin, pilihOrder);

        if (pilihOrder == "1") { asc = true; break; }
        if (pilihOrder == "2") { asc = false; break; }

        cout << "Pilihan urutan tidak valid!\n";
    }

    //Bubble Sort
    for (int i = 0; i < jumlahData - 1; i++) {
        for (int j = 0; j < jumlahData - i - 1; j++) {

            bool perluSwap = false;

            // Sort berdasarkan Nominal
            if (pilihSort == "1") {
                perluSwap = asc ? (nominalArr[j] > nominalArr[j+1])
                                : (nominalArr[j] < nominalArr[j+1]);
            }

            // Sort berdasarkan Tanggal (dd-mm-yyyy → key)
            else if (pilihSort == "2") {
                int key1 = tanggalToKey(tanggalArr[j]);
                int key2 = tanggalToKey(tanggalArr[j+1]);
                perluSwap = asc ? (key1 > key2)
                                : (key1 < key2);
            }

            // Sort berdasarkan Kategori (case-insensitive)
            else if (pilihSort == "3") {
                string a = toLowerStr(kategoriArr[j]);
                string b = toLowerStr(kategoriArr[j+1]);
                perluSwap = asc ? (a > b)
                                : (a < b);
            }

            // Jika memenuhi kondisi maka tukar semua field
            if (perluSwap) {
                swap(kategoriArr[j], kategoriArr[j+1]);
                swap(nominalArr[j], nominalArr[j+1]);
                swap(tanggalArr[j], tanggalArr[j+1]);
            }
        }
    }

    // Auto-save setelah sort
    simpanData();

    cout << "\nData berhasil diurutkan!\n";

    //Tampilkan hasil setelah sorting
    tampilHeader();
    for (int i = 0; i < jumlahData; i++) {
        cout << setw(4) << (i+1) << " | "
             << left << setw(20) << kategoriArr[i] << " | "
             << right << setw(10) << nominalArr[i] << " | "
             << " " << tanggalArr[i] << "\n";
    }
}



// ================== MENU 6 ===================
void hapusPengeluaran() {
    if (jumlahData == 0) {
        cout << "\nBelum ada data untuk dihapus.\n";
        return;
    }

    cout << "\n--- Hapus Pengeluaran ---\n";
    tampilHeader();

    for (int i = 0; i < jumlahData; i++) {
        cout << setw(4) << (i + 1) << " | "
             << left << setw(20) << kategoriArr[i] << " | "
             << right << setw(10) << nominalArr[i] << " | "
             << " " << tanggalArr[i] << "\n";
    }

    cout << "\nMasukkan nomor data yang ingin dihapus: ";
    string input;
    getline(cin, input);

    // Validasi input angka
    if (!validNominal(input)) {
        cout << "Nomor tidak valid. Kembali ke menu utama.\n";
        return;
    }

    int idx = stoi(input) - 1;

    if (idx < 0 || idx >= jumlahData) {
        cout << "Nomor tidak ada dalam data. Kembali ke menu utama.\n";
        return;
    }

    // --- KONFIRMASI HAPUS ---
    cout << "\nAnda yakin ingin menghapus data berikut?\n";
    cout << kategoriArr[idx] << " | "
         << nominalArr[idx] << " | "
         << tanggalArr[idx] << "\n";
    cout << "Konfirmasi (y/n): ";

    string konfirmasi;
    getline(cin, konfirmasi);

    if (konfirmasi != "y" && konfirmasi != "Y") {
        cout << "Penghapusan dibatalkan. Kembali ke menu utama.\n";
        return;
    }

    // --- PROSES HAPUS ---
    for (int i = idx; i < jumlahData - 1; i++) {
        kategoriArr[i] = kategoriArr[i + 1];
        nominalArr[i] = nominalArr[i + 1];
        tanggalArr[i] = tanggalArr[i + 1];
    }

    jumlahData--;

    simpanData(); // auto-save
    cout << "Data berhasil dihapus!\n";
}



// ================== MAIN ===================
int main() {
    ios::sync_with_stdio(false);
    cin.tie(&cout);

    cout << "=== MonKe - Money Tracker ===\n";
    cout << "Memuat data...\n";
    muatData();

    mainMenu();
    return 0;
}
