#include <iomanip>
#include <cstring>
#include <stdlib.h>
#include <stdio.h>
#include <fstream>
#include <iostream>
#include <clocale>

using namespace std;

struct Arac {
    char plaka[20];
    char marka[50];
    char model[50];
    char sahib_ad[80];
    char sahib_soyad[80];
};

void AracEkle();
void AracListeleme();
void AracArama();
void AracSil();
void AracDuzenle();

int main() {
    setlocale(LC_ALL, "Turkish");

    char anamenu;
    do {
        system("cls");
        cout << "|-------Otopark Otomasyonu------|" << endl;
        cout << "|          Se�im Yapiniz        |" << endl;
        cout << "|      1- Ara� Ekleme           |" << endl;
        cout << "|      2- Ara� Listeleme        |" << endl;
        cout << "|      3- Ara� Arama            |" << endl;
        cout << "|      4- Ara� Sil              |" << endl;
        cout << "|      5- Ara� Duzenle          |" << endl;
        cout << "|--------------------------------|" << endl;

        char secim;
        cin >> secim;

        switch (secim) {
            case '1':
                AracEkle();
                break;
            case '2':
                AracListeleme();
                break;
            case '3':
                AracArama();
                break;
            case '4':
                AracSil();
                break;
            case '5':
                AracDuzenle();
                break;
            default:
                cout << "Ge�ersiz se�im!" << endl;
                break;
        }

        cout << "Anamenuye Donmek icin: a basin, �ikmak i�in: c" << endl;
        cin >> anamenu;

    } while (anamenu == 'a' || anamenu == 'A');

    return 0;
}

Arac arac;

void AracEkle() {
    ofstream yaz("arac.dat", ios::binary | ios::app);
    if (!yaz) {
        cout << "Dosya a��lmad�!" << endl;
        return;
    }

    char secim;
    int adet = 0;

    do {
        cin.ignore();
        cout << "Ara� Plaka Giriniz: ";
        cin.getline(arac.plaka, sizeof(arac.plaka));
        cout << "Ara� Marka Giriniz: ";
        cin.getline(arac.marka, sizeof(arac.marka));
        cout << "Ara� Model Giriniz: ";
        cin.getline(arac.model, sizeof(arac.model));
        cout << "Ara� Sahibi Ad Giriniz: ";
        cin.getline(arac.sahib_ad, sizeof(arac.sahib_ad));
        cout << "Ara� Sahibi Soyad Giriniz: ";
        cin.getline(arac.sahib_soyad, sizeof(arac.sahib_soyad));

        yaz.write((char*)&arac, sizeof(arac));
        adet++;

        cout << "Baska Arac Eklemek Istermisiniz? (E/H): ";
        cin >> secim;
        cin.ignore();

    } while (secim == 'e' || secim == 'E');

    cout << adet << " adet arac eklendi." << endl;
    yaz.close();
}

void AracListeleme() {
    ifstream oku("arac.dat", ios::binary);
    if (!oku) {
        cout << "Dosya a��lmad�!" << endl;
        return;
    }

    oku.seekg(0, ios::end);
    int kayitsayisi = oku.tellg() / sizeof(arac);
    oku.seekg(0, ios::beg);

    cout << "Toplam Ara� Kayit Sayisi: " << kayitsayisi << endl;

    if (kayitsayisi > 0) {
        for (int i = 0; i < kayitsayisi; i++) {
            oku.read((char*)&arac, sizeof(arac));

            cout << i + 1 << ". Ara� Bilgileri:" << endl;
            cout << "Plaka: " << arac.plaka << endl;
            cout << "Marka: " << arac.marka << endl;
            cout << "Model: " << arac.model << endl;
            cout << "Sahibi Ad: " << arac.sahib_ad << endl;
            cout << "Sahibi Soyad: " << arac.sahib_soyad << endl;
        }
    } else {
        cout << "Kayit Bulunamadi..." << endl;
    }

    oku.close();
}

void AracArama() {
    ifstream oku("arac.dat", ios::binary);
    if (!oku) {
        cout << "Dosya a��lmad�!" << endl;
        return;
    }

    oku.seekg(0, ios::end);
    int kayitsayisi = oku.tellg() / sizeof(arac);
    oku.seekg(0, ios::beg);

    cin.ignore();
    cout << "Aranacak Ara� Plakasi Giriniz: ";
    char plaka[20];
    cin.getline(plaka, sizeof(plaka));

    bool bulundu = false;

    for (int i = 0; i < kayitsayisi; i++) {
        oku.read((char*)&arac, sizeof(arac));

        if (strcmp(arac.plaka, plaka) == 0) {
            cout << "Ara� Bilgileri:" << endl;
            cout << "Plaka: " << arac.plaka << endl;
            cout << "Marka: " << arac.marka << endl;
            cout << "Model: " << arac.model << endl;
            cout << "Sahibi Ad: " << arac.sahib_ad << endl;
            cout << "Sahibi Soyad: " << arac.sahib_soyad << endl;
            bulundu = true;
            break;
        }
    }

    if (!bulundu) {
        cout << "Kayit Bulunamadi..." << endl;
    }

    oku.close();
}

void AracSil() {
    ifstream oku("arac.dat", ios::binary);
    if (!oku) {
        cout << "Dosya a��lmad�!" << endl;
        return;
    }

    ofstream yedek("yedek.dat", ios::binary);
    if (!yedek) {
        cout << "Yedek dosya olu�turulamad�!" << endl;
        oku.close();
        return;
    }

    cout << "Silinecek Ara� Plakasi Giriniz: ";
    char plaka[20];
    cin.ignore();
    cin.getline(plaka, sizeof(plaka));

    bool bulundu = false;

    while (oku.read((char*)&arac, sizeof(arac))) {
        if (strcmp(arac.plaka, plaka) != 0) {
            yedek.write((char*)&arac, sizeof(arac));
        } else {
            bulundu = true;
        }
    }

    oku.close();
    yedek.close();

    if (bulundu) {
        remove("arac.dat");
        rename("yedek.dat", "arac.dat");
        cout << "Kayit Silindi." << endl;
    } else {
        remove("yedek.dat");
        cout << "Kayit Bulunamadi." << endl;
    }
}

void AracDuzenle() {
    ifstream oku("arac.dat", ios::binary);
    if (!oku) {
        cout << "Dosya a��lmad�!" << endl;
        return;
    }

    ofstream yedek("yedek.dat", ios::binary);
    if (!yedek) {
        cout << "Yedek dosya olu�turulamad�!" << endl;
        oku.close();
        return;
    }

    cout << "Duzenlenecek Ara� Plakasi Giriniz: ";
    char plaka[20];
    cin.ignore();
    cin.getline(plaka, sizeof(plaka));

    bool bulundu = false;

    while (oku.read((char*)&arac, sizeof(arac))) {
        if (strcmp(arac.plaka, plaka) == 0) {
            bulundu = true;
            cout << "Yeni Ara� Bilgilerini Giriniz:" << endl;
            cout << "Plaka: ";
            cin.getline(arac.plaka, sizeof(arac.plaka));
            cout << "Marka: ";
            cin.getline(arac.marka, sizeof(arac.marka));
            cout << "Model: ";
            cin.getline(arac.model, sizeof(arac.model));
            cout << "Sahibi Ad: ";
            cin.getline(arac.sahib_ad, sizeof(arac.sahib_ad));
            cout << "Sahibi Soyad: ";
            cin.getline(arac.sahib_soyad, sizeof(arac.sahib_soyad));
        }
        yedek.write((char*)&arac, sizeof(arac));
    }

    oku.close();
    yedek.close();

    if (bulundu) {
        remove("arac.dat");
        rename("yedek.dat", "arac.dat");
        cout << "Kayit Duzenlendi." << endl;
    } else {
        remove("yedek.dat");
        cout << "Kayit Bulunamadi." << endl;
    }
}

