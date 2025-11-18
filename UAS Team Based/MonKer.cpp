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

    cout << "\n--- Tambah Pengeluaran ---\n";
    cout << "Kategori: ";
    string kat; getline(cin, kat);
    if (!validKategori(kat)) {
        cout << "Kategori tidak valid.\n";
        return;
    }

    cout << "Nominal: ";
    string sNom; getline(cin, sNom);
    if (!validNominal(sNom)) {
        cout << "Nominal tidak valid.\n";
        return;
    }

    cout << "Tanggal (dd-mm-yyyy): ";
    string t; getline(cin, t);
    if (!validTanggal(t)) {
        cout << "Tanggal tidak valid.\n";
        return;
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
    cout << "\n--- Total Pengeluaran (Rentang) ---\n";

    cout << "Tanggal Awal: ";
    string a; getline(cin, a);
    if (!validTanggal(a)) { cout << "Tanggal salah.\n"; return; }

    cout << "Tanggal Akhir: ";
    string b; getline(cin, b);
    if (!validTanggal(b)) { cout << "Tanggal salah.\n"; return; }

    int kA = tanggalToKey(a);
    int kB = tanggalToKey(b);
    if (kA > kB) swap(kA, kB);

    int total = 0;

    tampilHeader();
    int no = 1;

    for (int i = 0; i < jumlahData; i++) {
        int key = tanggalToKey(tanggalArr[i]);
        if (key >= kA && key <= kB) {
            cout << setw(4) << no++ << " | "
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
    cout << "\n--- Cari Pengeluaran ---\n";
    cout << "Cari berdasarkan:\n1. Kategori\n2. Tanggal\nPilih: ";

    string p; getline(cin, p);

    if (p == "1") {
        cout << "Masukkan kategori: ";
        string key; getline(cin, key);

        key = toLowerStr(key);

        tampilHeader();
        int no = 1;

        for (int i = 0; i < jumlahData; i++) {
            if (toLowerStr(kategoriArr[i]) == key) {
                cout << setw(4) << no++ << " | "
                     << left << setw(20) << kategoriArr[i] << " | "
                     << right << setw(10) << nominalArr[i] << " | "
                     << " " << tanggalArr[i] << "\n";
            }
        }

    } else if (p == "2") {
        cout << "Tanggal (dd-mm-yyyy): ";
        string t; getline(cin, t);

        tampilHeader();
        int no = 1;

        for (int i = 0; i < jumlahData; i++) {
            if (tanggalArr[i] == t) {
                cout << setw(4) << no++ << " | "
                     << left << setw(20) << kategoriArr[i] << " | "
                     << right << setw(10) << nominalArr[i] << " | "
                     << " " << tanggalArr[i] << "\n";
            }
        }
    }
}


// ================== MENU 5 ===================
void sortPengeluaran() {
    if (jumlahData == 0) {
        cout << "Belum ada data.\n";
        return;
    }

    cout << "\nSort berdasarkan:\n1. Nominal\n2. Tanggal\n3. Kategori\nPilih: ";
    string p; getline(cin, p);

    cout << "Urutan (1. Ascending, 2. Descending): ";
    string o; getline(cin, o);
    bool asc = (o == "1");

    // Bubble Sort
    for (int i = 0; i < jumlahData - 1; i++) {
        for (int j = 0; j < jumlahData - i - 1; j++) {

            bool cond = false;

            if (p == "1") { // nominal
                cond = asc ? (nominalArr[j] > nominalArr[j+1])
                           : (nominalArr[j] < nominalArr[j+1]);
            } 
            else if (p == "2") { // tanggal
                cond = asc ? (tanggalToKey(tanggalArr[j]) > tanggalToKey(tanggalArr[j+1]))
                           : (tanggalToKey(tanggalArr[j]) < tanggalToKey(tanggalArr[j+1]));
            }
            else { // kategori
                cond = asc ? (toLowerStr(kategoriArr[j]) > toLowerStr(kategoriArr[j+1]))
                           : (toLowerStr(kategoriArr[j]) < toLowerStr(kategoriArr[j+1]));
            }

            if (cond) {
                swap(kategoriArr[j], kategoriArr[j+1]);
                swap(nominalArr[j], nominalArr[j+1]);
                swap(tanggalArr[j], tanggalArr[j+1]);
            }
        }
    }

    simpanData(); // Auto-save setelah sort
    cout << "Sort selesai dan tersimpan!\n";

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
        cout << "Belum ada data.\n";
        return;
    }

    cout << "\n--- Hapus Data ---\n";
    tampilHeader();
    for (int i = 0; i < jumlahData; i++) {
        cout << setw(4) << (i+1) << " | "
             << left << setw(20) << kategoriArr[i] << " | "
             << right << setw(10) << nominalArr[i] << " | "
             << " " << tanggalArr[i] << "\n";
    }

    cout << "\nHapus nomor berapa? ";
    string s; getline(cin, s);
    if (!validNominal(s)) return;

    int idx = stoi(s) - 1;

    if (idx < 0 || idx >= jumlahData) {
        cout << "Nomor tidak ada.\n";
        return;
    }

    for (int i = idx; i < jumlahData - 1; i++) {
        kategoriArr[i] = kategoriArr[i+1];
        nominalArr[i] = nominalArr[i+1];
        tanggalArr[i] = tanggalArr[i+1];
    }

    jumlahData--;
    simpanData(); // Auto-save setelah hapus data
    cout << "Data terhapus dan tersimpan!\n";
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