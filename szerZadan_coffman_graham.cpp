#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <cstdio>

using namespace std;

// wezel
typedef struct vertice {
    int id; // numer
    int L; // etykieta;
    int m; // maszyna
    int done; // ostatni czas wykonania
    vector <int> s_list; // lista s_list()
    vector <vertice*> prev; // nastepnicy
    vector <vertice*> next; // poprzednicy
} vertice;

int maxtime; // Cmax

void print(vector<vertice*> wezly) {
    int n = wezly.size();
    for (int i=0;i<n;i++) {
        vertice *w = wezly[i];
        cout << "Z" << w->id << " (" << w->L << ")" << ", maszyna: " << w->m+1 << ", czas rozpoczecia: " << w->done-1 << endl;
    }
}

bool nastepnicy(vertice *wezel) {
    int n = wezel->next.size();
    for (int i=0;i<n;i++)
        if (wezel->next[i]->L == 0) return false;
    return true;
}

vector<vertice*> zbiorA(vector<vertice*> wezly) {
    int n = wezly.size();
    vector <vertice*> A;
    for (int i=0;i<n;i++) {
        vertice *w = wezly[i];
        if (w->L == 0 && (nastepnicy(w) || w->next.size() == 0)) {
            A.push_back(w);
        }
    }
    return A;
}

void przypisz(vertice *wezel) {
    int n = wezel->next.size();
    for (int i=0;i<n;i++) wezel->s_list.push_back(wezel->next[i]->L);
}

void coffman_graham(vector<vertice*> wezly) {
    int n = wezly.size();

    for (int i=0;i<n;i++) {
        vertice *w = wezly[i];
        vector <vertice*> A = zbiorA(wezly);
        for (int j=0;j<A.size();j++) przypisz(A[j]);
        vertice *z = A[0];
        int minimum = z->s_list.size();
        for (int j=1;j<A.size();j++) {
            if (A[j]->s_list.size() < minimum) {
                z = A[j];
                minimum = z->s_list.size();
            }
        }
        z->L = i+1;
    }
}

bool porownaj(vertice *u, vertice *v) {
    return u->L >= v->L;
}

int ostatni(vertice *wezel) {
    int n = wezel->prev.size();
    int last = 0;
    for (int i=0;i<n;i++) {
        if (last < wezel->prev[i]->done) last = wezel->prev[i]->done;
    }
    return last;
}

void maszyny(vector<vertice*> wezly) {
    int n = wezly.size();
    sort(wezly.begin(), wezly.end(), porownaj);

    int machines[2][n];
    for (int i=0;i<2;i++)
        for (int j=0;j<n;j++)
            machines[i][j] = 0;

    for (int i=0;i<n;i++) {
        vertice *w = wezly[i];
        int j = ostatni(w);
        while(1) {
            if (machines[0][j] == 0 || machines[1][j] == 0) {
                if (machines[0][j] == 0) {
                    w->m = 0;
                    machines[0][j] = 1;
                } else {
                    w->m = 1;
                    machines[1][j] = 1;
                }
                break;
            } else j++;
        }
        w->done = j+1;
    }

    maxtime = 0;
    for (int i=0;i<n;i++) {
        if (maxtime < wezly[i]->done) maxtime = wezly[i]->done;
    }
}

void rysunek(vector<vertice *> wezly) {
    int n = wezly.size();
    int m = 2;

    int width = 100;
    int WIDTH = width * maxtime + 10;
    int height = 50;

    ofstream plik;
    plik.open("rysunek.svg");
    plik << "<svg xmlns=\"http://www.w3.org/2000/svg\" version=\"1.1\">";
    for (int i=0;i<n;i++) {
        plik << "<g>";
        vertice *v = wezly[i];

        plik << "<rect x=\"" << width*(v->done-1) << "\" y=\"" << v->m*height << "\" width=\"" << width << "\" height=\"" << height << "\" style=\"fill:white;stroke-width:1;stroke:black\" >" << "</rect>" << endl;
        plik << "<text x=\"" << width*(v->done-1) + (width/3) << "\" y=\"" << (height/2)+height*v->m << "\" font-family=\"Arial\" font-size=\"16\" fill=\"black\">Z" << v->id << "</text>" << endl;

        //cout << "x = " << width*v->l << " y = " << v->m*height << " width = " << width*v->time << " height = " << height << endl;
        plik << "</g>" << endl;
    }

    plik << "<line x1=\"0\" y1=\"" << (m)*height + 15 << "\" x2=\"" << WIDTH << "\" y2=\"" << (m)*height + 15 << "\" style=\"stroke:black;stroke-width:2\"/>" << endl;
    for (int i=0;i<maxtime+1;i++) {
        plik << "<line x1=\"" << width*i << "\" y1=\"" << (m)*height + 10 << "\" x2=\"" << width*i << "\" y2=\"" << (m)*height + 20 << "\" style=\"stroke:black;stroke-width:2\"/>" << endl;
    }
    plik << "<text x=\"" << WIDTH << "\" y=\"" << (m)*height + 10 << "\" font-family=\"Arial\" font-size=\"16\" fill=\"black\">" << maxtime << "</text>" << endl;
    plik << "</svg>" << endl;
    plik.close();
    cout << "Zapisano 'rysunek.svg'.\n";
}

bool cykl(int id, vertice *wezel) {
    if (wezel->id == id) return true;
    for (vector<vertice*>::iterator it = wezel->next.begin(); it != wezel->next.end(); ++it) {
        if (cykl(id,*it)) return true;
    }
    return false;
}

bool cykl(vertice *wezel) {
    for (vector<vertice*>::iterator it = wezel->next.begin(); it != wezel->next.end(); ++it) {
         if (cykl(wezel->id,*it)) return true;
    }
    return false;
}

int main() {
    int n;
    cout << "Ile zadan? "; cin >> n;
    vector <vertice*> wezly;
    wezly.reserve(n);

    for (int i=0;i<n;i++) {
        vertice *w = new vertice;
        w->id = i+1;
        w->L = 0;
        w->m = 0;
        w->done = 0;
        wezly.push_back(w);
    }

    int u = 0,v = 0;
    cout << "-1 konczy" << endl;
    while (1) {
        cout << "Podaj sciezke: ";
        cin >> u; if (u == -1) break;
        cin >> v;
        wezly[u-1]->next.push_back(wezly[v-1]);
        if (cykl(wezly[v-1])) {
            wezly[u-1]->next.erase(wezly[u-1]->next.end()-1);
            cout << "Wykryto cykl, ostatnia sciezka niezapisana." << endl;
            continue;
        }
        wezly[v-1]->prev.push_back(wezly[u-1]);
    }

    cout << endl;
    coffman_graham(wezly);
    maszyny(wezly);
    print(wezly);
    cout << endl;
    rysunek(wezly);

    return 0;
}
