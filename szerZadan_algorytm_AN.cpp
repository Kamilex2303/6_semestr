#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <cstdio>

using namespace std;

// wezel
typedef struct vertice {
    int id; // numer
    int time; // czas wykonania
    int l; // etykieta; najwczesniejsze wykonanie
    int m; // maszyna
    vector <vertice*> prev; // nastepnicy
    vector <vertice*> next; // poprzednicy
} vertice;

int maxtime = 0;
int m = 0;

// drukowanie tablicy
void print(vector <vertice*> wezly) {
    int n = wezly.size();
    printf("\n");
    printf("  Z  | czas |  l  | m \n");
    printf("-----+------+-----+----\n");
    for (int i=0;i<n;i++) {
        vertice *v = wezly[i];
        printf(" Z%02i |  %3i | %3i | %2i \n",v->id,v->time,v->l,v->m+1);
        if (m < v->m) m = v->m;
    }
    printf("Ilosc potrzebnych maszyn  : %2i.\n",m+1);
    printf("Makszymalny czas wykonania: %2i.\n",maxtime);
}

// max czasu i etykiet poprzednikow
int max(vertice *v) {
    int n = v->prev.size();
    int m = 0;
    for (int i=0;i<n;i++) {
        if (m == 0) {
            m = v->prev[i]->time + v->prev[i]->l;
            continue;
        }
        if (m < v->prev[i]->time + v->prev[i]->l) m = v->prev[i]->time + v->prev[i]->l;
    }
    return m;
}

// przypisanie do maszyn
void machines(vector<vertice*> wezly) {
    int n = wezly.size();
    maxtime = 0;

    for (int i=0;i<n;i++) {
        vertice *v = wezly[i];
        if (maxtime < v->l+v->time) maxtime = v->l+v->time;
    }

    int maszyny[maxtime][maxtime];
    for (int i=0;i<maxtime;i++)
        for (int j=0;j<maxtime;j++) maszyny[i][j] = 0;

    for (int i=0;i<n;i++) {
        vertice *v = wezly[i];
        int j = 0;
        while (maszyny[j][v->l] == 1) j++;
        for (int k=0;k<v->time;k++) {
            maszyny[j][v->l+k] = 1;
        }
        v->m = j;
    }
}

// pomocnicza
bool porownaj(vertice *u, vertice *v) {
    return u->id < v->id;
}

// algorytm AN
void algorytm_AN(vector <vertice*> wezly) {
    int n = wezly.size();

    // szuka tych bez poprzednikow
    vector<vertice*> pierwsi;
    pierwsi.reserve(n);
    for (int i=0;i<n;i++) {
        vertice *v = wezly[i];
        if (v->prev.size() == 0) pierwsi.push_back(v);
    }

    // pseudo-kolejka, najpierw pierwsi a potem ich nastepnicy itd.
    vector<vertice*> kolejka;
    kolejka.reserve(n);
    kolejka.insert(kolejka.end(),pierwsi.begin(),pierwsi.end());
    for (int i=0;i<kolejka.size();i++) {
        vertice *v = kolejka[i];
        for (int j=0;j<v->next.size();j++) {
            vertice *u = v->next[j];
            vector<vertice*>::iterator it = find(kolejka.begin(), kolejka.end(), u);
            if (it == kolejka.end()) kolejka.push_back(u);
        }
    }

    for (int i=0;i<kolejka.size();i++) cout << kolejka[i]->id << endl;

    sort (kolejka.begin(), kolejka.end(), porownaj);
    // przypisanie etykiet
    for (int i=0;i<kolejka.size();i++) {
        vertice *v = kolejka[i];
        v->l = max(v);
    }

    machines(wezly);
}

// funkcja to tworzenia rysunku
void rysunek(vector<vertice*> wezly) {
    int n = wezly.size();

    int width = 800 / maxtime;
    int height = 32;

    ofstream plik;
    plik.open("rysunek.svg");
    plik << "<svg xmlns=\"http://www.w3.org/2000/svg\" version=\"1.1\">";

    for (int i=0;i<n;i++) {
        plik << "<g>";
        vertice *v = wezly[i];

        plik << "<rect x=\"" << width*v->l << "\" y=\"" << v->m*height << "\" width=\"" << width*v->time << "\" height=\"" << height << "\" style=\"fill:white;stroke-width:1;stroke:black\" >" << "</rect>" << endl;
        plik << "<text x=\"" << width*v->l + (width/3)*v->time << "\" y=\"" << (height/2)+height*v->m << "\" font-family=\"Arial\" font-size=\"16\" fill=\"black\">Z" << v->id << "</text>" << endl;

        //cout << "x = " << width*v->l << " y = " << v->m*height << " width = " << width*v->time << " height = " << height << endl;
        plik << "</g>" << endl;
    }
    plik << "<line x1=\"0\" y1=\"" << (m+1)*height + height/2 << "\" x2=\"" << 800 << "\" y2=\"" << (m+1)*height + height/2 << "\" style=\"stroke:black;stroke-width:2\"/>" << endl;
    for (int i=0;i<maxtime+1;i++) {
        plik << "<line x1=\"" << width*i << "\" y1=\"" << (m+1)*height + height/4 << "\" x2=\"" << width*i << "\" y2=\"" << (m+1)*height + height*3/4 << "\" style=\"stroke:black;stroke-width:2\"/>" << endl;
    }
    plik << "<text x=\"" << 800 << "\" y=\"" << (m+2)*height << "\" font-family=\"Arial\" font-size=\"16\" fill=\"black\">" << maxtime << "</text>" << endl;
    plik << "</svg>" << endl;
    plik.close();
    cout << "Zapisano 'rysunek.svg'.\n";
}

int main() {
    int n;
    cout << "Ile zadan? "; cin >> n;

    vector <vertice*> wezly;
    wezly.reserve(n);

    vertice w[n];
    for (int i=0;i<n;i++) {
        w[i].id = i+1;
        cout << "Podaj czas " << i+1 << ". zadania: "; cin >> w[i].time;
        w[i].l = 0;
        w[i].m = 0;
        wezly.push_back(&w[i]);
    }

    int u = 0,v = 0;
    cout << "-1 konczy" << endl;
    while (1) {
        cout << "Podaj sciezke: ";
        cin >> u; if (u == -1) break;
        cin >> v;
        w[u-1].next.push_back(&w[v-1]);
        w[v-1].prev.push_back(&w[u-1]);
    }

    algorytm_AN(wezly);
    print(wezly);
    rysunek(wezly);
    return 0;
}
