#include <iostream>
#include <vector>
#include <ctime>
#include <cstdlib>
#include <cstring>
#include <string>
using namespace std;

const int NUMER_ILOSC = 13, KOLOR_ILOSC = 4, KUPKI_DU_ILOSC = 8;

class karta {
public:
    int numer;
    int kolor;
};

class Ruch {
public:
    int z_ktorej_kupki;
    int ktora_karte;
    int na_ktora_kupke;
};

Ruch zapytajORuch() {
    Ruch ruch;

    // Wczytaj z kt�rej kupki
    while (true) {
        cout << "Z ktorej kupki chcesz wziac karte? (1-12): ";
        if (cin >> ruch.z_ktorej_kupki && ruch.z_ktorej_kupki >= 1 && ruch.z_ktorej_kupki <= 12) {
            ruch.z_ktorej_kupki--;
            break;
        }
        else {
            cout << "Nieprawidlowy numer kupki! Sprobuj ponownie.\n";
            cin.clear();                // napraw stan cin
            cin.ignore(1000, '\n');      // odrzuc reszt� lini
        }
    }

    // Wczytaj kt�ra karta od ko�ca
    while (true) {
        cout << "Ktora karta od dolu? (1-n): ";
        if (cin >> ruch.ktora_karte && ruch.ktora_karte >= 1) {
            break;
        }
        else {
            cout << "Nieprawidlowa karta! Sprobuj ponownie.\n";
            cin.clear();
            cin.ignore(1000, '\n');
        }
    }

    // Wczytaj na kt�r� kupk�
    while (true) {
        cout << "Na ktora kupke chcesz przeniesc? (1-16): ";
        if (cin >> ruch.na_ktora_kupke && ruch.na_ktora_kupke >= 1 && ruch.na_ktora_kupke <= 16) {
            ruch.na_ktora_kupke--;
            break;
        }
        else {
            cout << "Nieprawidlowy numer kupki docelowej! Sprobuj ponownie.\n";
            cin.clear();
            cin.ignore(1000, '\n');
        }
    }

    return ruch;
}

string numerNaString(int numer) {
    if (numer == 0) return  " A";
    if (numer == 10) return " J";
    if (numer == 11) return " Q";
    if (numer == 12) return " K";
    if (numer + 1 == 10) return "10"; // Dziesi�tka
    return " " + to_string(numer + 1);
}

char kolorNaZnak(int kolor) {
    switch (kolor) {
    case 0: return 'P';
    case 1: return 'K';
    case 2: return 'T';
    case 3: return 'C';
    default: return '?';
    }
}
bool Sprawdz_wygrana(const vector<vector<karta>>& posortowane, const vector<vector<karta>>& kupki_DU) {
    // int liczba_kart = 0;
    // for (const auto& stos : posortowane) {
    //     liczba_kart += stos.size();
    // }
    // return liczba_kart == 52;
    int puste = 0, ulozone = 0, numer;
    bool po_kolei, wygrana;
    for(int i = 0 ; i < 8 ; i ++)
    {
        if(kupki_DU[i].empty())
            puste ++;
        else
        {
            po_kolei = true;
            numer = kupki_DU[i].back().numer;
            for(int j = 1 ; j < kupki_DU[1].size() ; j ++)
            {
                if(!(kupki_DU[i][kupki_DU[i].size() - j].numer == numer + 1))
                    po_kolei = false;
            }
            if(po_kolei)
                ulozone++;
        }
        wygrana = (puste + ulozone == 8);
    }
    if(wygrana)
        return true;
    int i = posortowane.size()-1;
    if(!(posortowane[0].empty() ||posortowane[1].empty()  || posortowane[2].empty() || posortowane[3].empty()))
    return posortowane[0].back().numer == 12 && posortowane[1].back().numer == 12 && posortowane[2].back().numer == 12 && posortowane[3].back().numer == 12;
    else
    return false;
}

void Wyswietl_plansze(vector<vector<karta>>& kupki_DU, vector<karta>& free_cells, vector<vector<karta>>& posortowane) {
    cout << "\n-- FREE CELLS (9-12) --\n";
    cout << "\n-- 9     10     11     12 --\n";
    for (int i = 0; i < 4; i++) {
        if (free_cells[i].numer >= 0)
            cout << "| " << numerNaString(free_cells[i].numer) << kolorNaZnak(free_cells[i].kolor) << " | ";
        else
            cout << "|    | ";
    }
    cout << "\n";

    cout << "\n-- SORTED STACKS (13-16) --\n";
     cout << "\n-- 13    14     15     16 --\n";
    for (int i = 0; i < 4; i++) {
        if (!posortowane[i].empty())
            cout << "| " << numerNaString(posortowane[i].back().numer) << kolorNaZnak(posortowane[i].back().kolor) << " | ";
        else
            cout << "|    | ";
    }
    cout << "\n";

    cout << "\n-- MAIN PILES (1-8)) --\n";
     cout << "\n-- 1    2    3    4    5    6    7    8 --\n";
    size_t maxKupka = 0;
    for (auto& kupka : kupki_DU)
        if (kupka.size() > maxKupka) maxKupka = kupka.size();

    for (size_t i = 0; i < maxKupka; i++) {
        for (size_t j = 0; j < kupki_DU.size(); j++) {
            if (i < kupki_DU[j].size())
                cout << "| " << numerNaString(kupki_DU[j][i].numer) << kolorNaZnak(kupki_DU[j][i].kolor);
            else
                cout << "|    ";
        }
        cout << "|\n";
    }
    cout << endl;
}

void Tasowanie_kart(vector<vector<karta>>& kupki_DU) {
    kupki_DU.resize(KUPKI_DU_ILOSC);
    bool talia[NUMER_ILOSC][KOLOR_ILOSC];
    karta k1;
    for (int i = 0; i < 13; i++)
        for (int j = 0; j < 4; j++)
            talia[i][j] = 1;

    int cnt = 52;
    for (int i = 0; i < 52; i++) {
        int numer, kolor, iterator;
        do {
            iterator = rand() % cnt;
            numer = iterator % NUMER_ILOSC;
            kolor = (iterator / NUMER_ILOSC) % KOLOR_ILOSC;
        } while (talia[numer][kolor] == 0);

        talia[numer][kolor] = 0;
        k1.kolor = kolor;
        k1.numer = numer;
        kupki_DU[i % 8].push_back(k1);
    }
}

bool Mozna_ruszyc(vector<karta> kupka_I, int ktora_karte) {
    bool mozna_ruszyc = true;
    int nr_karty = kupka_I[kupka_I.size() - ktora_karte].numer;
    int kolor_karty = kupka_I[kupka_I.size() - ktora_karte].kolor;

    for (int i = ktora_karte - 1; i > 0; i--) {
        if (kupka_I[kupka_I.size() - i].numer != nr_karty - 1 || kupka_I[kupka_I.size() - i].kolor % 2 == kolor_karty % 2)
            mozna_ruszyc = false;
        else {
            kolor_karty++;
            nr_karty--;
        }
    }
    if(ktora_karte == 1)
        mozna_ruszyc = true;
    return mozna_ruszyc;
}

bool Czy_legalny(vector<karta> kupka_I, vector<karta> kupka_II, int ktora_karte, bool czy_posortowane) {
    bool mozna_polozyc = false;
    bool mozna_ruszyc, legalny = false;

    if (kupka_I.empty()) return false;
    int I_size = kupka_I.size() - ktora_karte;
    int II_size = kupka_II.size() - 1;

    if (czy_posortowane) {
        if (!kupka_II.empty()) {
            // Musi by� ten sam kolor i numer dok�adnie o 1 wi�kszy
            return kupka_I[I_size].kolor == kupka_II[II_size].kolor &&
                kupka_I[I_size].numer == kupka_II[II_size].numer + 1;
        }
        else {
            // Na pusty stos mo�na po�o�y� tylko Asa
            return kupka_I[I_size].numer == 0;
        }
    }
    else {
        bool mozna_polozyc = false;
        if (!kupka_II.empty()) {
            if ((kupka_I[I_size].kolor % 2) != (kupka_II[II_size].kolor % 2) &&
                kupka_I[I_size].numer == kupka_II[II_size].numer - 1)
                mozna_polozyc = true;
        }
        else {
            if (ktora_karte == 1)
                mozna_polozyc = true;
        }
        if(kupka_II.empty())
            {
                mozna_polozyc = true;
            }
        bool mozna_ruszyc = Mozna_ruszyc(kupka_I, ktora_karte);
        return mozna_polozyc && mozna_ruszyc;
    }
}

bool Czy_ruch_legalny(const vector<vector<karta>>& kupki_DU, const vector<vector<karta>>& posortowane, const vector<karta>& free_cells, int z_ktorej_kupki, int ktora_karte, int na_ktora_kupke) {
    if (z_ktorej_kupki < 0 || z_ktorej_kupki >= 16 || na_ktora_kupke < 0 || na_ktora_kupke >= 16 || ktora_karte <= 0)
        return false;

    // 1�8: g��wne kupki
    // 9�12: FreeCells
    // 13�16: posortowane stosy

    vector<karta> z_kupki;
    if (z_ktorej_kupki < 8) {
        if (kupki_DU[z_ktorej_kupki].size() < ktora_karte) return false;
        z_kupki = kupki_DU[z_ktorej_kupki];
    }
    else if (z_ktorej_kupki >= 8 && z_ktorej_kupki < 12) {
        if (ktora_karte != 1) return false;
        if (free_cells[z_ktorej_kupki - 8].numer == -1) return false;
        z_kupki.push_back(free_cells[z_ktorej_kupki - 8]);
    }
    else return false; // nie mo�na rusza� z posortowanej kupki

    vector<karta> na_kupke;
    bool czy_posortowane = false;
    if (na_ktora_kupke < 8) {
        na_kupke = kupki_DU[na_ktora_kupke];
    }
    else if (na_ktora_kupke >= 8 && na_ktora_kupke < 12) {
        if (ktora_karte != 1) return false;
        if (free_cells[na_ktora_kupke - 8].numer != -1) return false;
        return true;
    }
    else if (na_ktora_kupke >= 12 && na_ktora_kupke < 16) {
        if (ktora_karte != 1) return false;
        na_kupke = posortowane[na_ktora_kupke - 12];
        czy_posortowane = true;
    }
    else return false;

    return Czy_legalny(z_kupki, na_kupke, ktora_karte, czy_posortowane);
}



void Przenies_karte(vector<karta>& kupka_I, vector<karta>& kupka_II, int od_ktorej) {
    if (od_ktorej > 0) {
        for (int i = od_ktorej; i > 0; i--) {
            kupka_II.push_back(kupka_I[kupka_I.size() - i]);
            kupka_I.erase(kupka_I.begin() + kupka_I.size() - i);
        }
    }
}

void Przenies_z_FreeCell(vector<karta>& free_cells, int z_ktorej, vector<karta>& na_kupke) {
    if (free_cells[z_ktorej].numer != -1) {
        na_kupke.push_back(free_cells[z_ktorej]);
        free_cells[z_ktorej] = { -1, -1 };
    }
}

void Przenies_do_FreeCell(vector<karta>& free_cells, int na_ktora, vector<karta>& kupka) {
    if (free_cells[na_ktora].numer == -1 && !kupka.empty()) {
        free_cells[na_ktora] = kupka.back();
        kupka.pop_back();
    }
}
bool Sprawdz_przegrana(const vector<vector<karta>>& kupki_DU, const vector<karta>& free_cells, const vector<vector<karta>>& posortowane) {
    for (int z = 0; z < 12; ++z) {  // 0-7: DU kupki, 8-11: FreeCell
        int max_karta = (z < 8 && !kupki_DU[z].empty()) ? kupki_DU[z].size() : 1;
        if (z >= 8 && free_cells[z - 8].numer == -1) continue; // pusty free cell - nic nie wyci�gniesz

        for (int ktora = 1; ktora <= max_karta; ++ktora) {
            for (int na = 0; na < 16; ++na) {
                if (z == na) continue; // nie przenosimy na to samo miejsce
                if (Czy_ruch_legalny(kupki_DU, posortowane, free_cells, z, ktora, na)) {
                    return false; // jaki� ruch jest mo�liwy
                }
            }
        }
    }
    return true; // �adnego ruchu - przegrana
}
int main() {
    srand(time(0));

    vector<vector<karta>> kupki_DU;
    vector<vector<karta>> posortowane(4);
    vector<karta> free_cells(4, { -1, -1 });

    Tasowanie_kart(kupki_DU);
    // for(int i = 0 ; i <= kupki_DU[7].size() + 2; i ++)
    // {
    //     kupki_DU[7].erase(kupki_DU[7].begin());
    // }
    // for(int i = 0 ; i < 5 ; i ++)
    // {
    //     kupki_DU[5][kupki_DU.size()  - i - 2].numer = 5 + i;
    //     kupki_DU[5][kupki_DU.size()  - i - 2].kolor = i % 4;
    // }
    // for(int i = 0 ; i < 4 ; i ++)
    // {
    //     karta card =  {12, 3};

    //     posortowane[i].push_back(card);

    // }
    Wyswietl_plansze(kupki_DU, free_cells, posortowane);

    while (true) {
        Ruch ruch = zapytajORuch();

        bool check = Czy_ruch_legalny(kupki_DU, posortowane, free_cells, ruch.z_ktorej_kupki, ruch.ktora_karte, ruch.na_ktora_kupke);
        if (Sprawdz_wygrana(posortowane, kupki_DU)) {
            cout << "Gratulacje! Wygrales gre!\n";
            break; // wyjscie z petli
        }
        if (Sprawdz_przegrana(kupki_DU, free_cells, posortowane)) {
            cout << "Niestety, brak mozliwych ruchow. Przegrales.\n";
            break;
        }
        if (check) {
            cout << "Ruch dozwolony! Wykonuje...\n";

            if (ruch.z_ktorej_kupki < 8) {
                if (ruch.na_ktora_kupke < 8)
                    Przenies_karte(kupki_DU[ruch.z_ktorej_kupki], kupki_DU[ruch.na_ktora_kupke], ruch.ktora_karte);
                else if (ruch.na_ktora_kupke < 12)
                    Przenies_do_FreeCell(free_cells, ruch.na_ktora_kupke - 8, kupki_DU[ruch.z_ktorej_kupki]);
                else
                    Przenies_karte(kupki_DU[ruch.z_ktorej_kupki], posortowane[ruch.na_ktora_kupke - 12], 1);
            }
            else if (ruch.z_ktorej_kupki >= 8 && ruch.z_ktorej_kupki < 12) {
                if (ruch.na_ktora_kupke < 8)
                    Przenies_z_FreeCell(free_cells, ruch.z_ktorej_kupki - 8, kupki_DU[ruch.na_ktora_kupke]);
                else
                    Przenies_z_FreeCell(free_cells, ruch.z_ktorej_kupki - 8, posortowane[ruch.na_ktora_kupke - 12]);
            }


        }
        else {
            cout << "Ruch niedozwolony!\n";
        }

        Wyswietl_plansze(kupki_DU, free_cells, posortowane);
    }

    return 0;
}

