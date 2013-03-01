#include <iostream>
#include <vector>
#include <algorithm>
#include <iterator>
#include <vector>
#include <sstream>

using namespace std;

typedef int cyfra;

char c2i(char c) {
    switch(c) {
        case '0': return 0;
        case '1': return 1;
        case '2': return 2;
        case '3': return 3;
        case '4': return 4;
        case '5': return 5;
        case '6': return 6;
        case '7': return 7;
        case '8': return 8;
        case '9': return 9;
        case 'A': return 10;
        case 'B': return 11;
        case 'C': return 12;
        case 'D': return 13;
        case 'E': return 14;
        case 'F': return 15;
    }
}

char i2c(char i) {
    switch(i) {
        case 0: return '0';
        case 1: return '1';
        case 2: return '2';
        case 3: return '3';
        case 4: return '4';
        case 5: return '5';
        case 6: return '6';
        case 7: return '7';
        case 8: return '8';
        case 9: return '9';
        case 10: return 'A';
        case 11: return 'B';
        case 12: return 'C';
        case 13: return 'D';
        case 14: return 'E';
        case 15: return 'F';
    }
}

class Liczba {
    public:
        int znak;
        int b;
        Liczba();
        vector<cyfra> cyfry;
        friend ostream & operator<< (ostream &wyjscie, const Liczba &s);
        friend istream & operator>> (istream &wejscie, Liczba &s);
};

Liczba::Liczba() {
    cyfry.reserve(20);
    b = 10;
    znak = 0;
}

// porownanie
int compare(Liczba u, Liczba v) {
    int n = u.cyfry.size();
    int m = v.cyfry.size();
    if (n != m) return n > m ? 1 : -1;
    for (int i=n-1;i>=0;i--) {
        if (u.cyfry[i] == v.cyfry[i]) continue;
        return u.cyfry[i] > v.cyfry[i] ? 1 : -1;
    }
    return 0;
}

// POMOCNICZA; ODEJMOWANIE
Liczba substract_simple(Liczba u, Liczba v) {
    vector<cyfra> temp1(u.cyfry);
    vector<cyfra> temp2(v.cyfry);
    int n = temp1.size()>=temp2.size()?temp1.size():temp2.size();
    for (int i=temp1.size();i<n;i++) temp1.push_back(0);
    for (int i=temp2.size();i<n;i++) temp2.push_back(0);
    int i = 0,c = 0, t;
    Liczba w;
    w.cyfry.clear();
    while (i != n) {
        t = (temp1[i]-temp2[i]+c) % u.b;
        if (t < 0) w.cyfry.push_back(t+u.b);
        else w.cyfry.push_back(t);
        c = (temp1[i]-temp2[i]+c) / u.b;
        if (t < 0) c--;
        i++;
    }
    while (1) {
        if (w.cyfry.size() > 0 && w.cyfry[w.cyfry.size()-1] != 0) break;
        else w.cyfry.pop_back();
    }
    return w;
}

// SUMOWANIE LICZB
Liczba add(Liczba u, Liczba v) {
    vector<cyfra> temp1(u.cyfry);
    vector<cyfra> temp2(v.cyfry);
    int n = temp1.size()>=temp2.size()?temp1.size():temp2.size();
    for (int i=temp1.size();i<n;i++) temp1.push_back(0);
    for (int i=temp2.size();i<n;i++) temp2.push_back(0);
    int i = 0,c = 0;
    Liczba w;
    w.cyfry.clear();
    if (u.znak == v.znak) {
        w.znak = u.znak;
        while (i != n) {
            w.cyfry.push_back((temp1[i]+temp2[i]+c)%u.b);
            c = (temp1[i]+temp2[i]+c) / u.b;
            i++;
        }
        if (c > 0) w.cyfry.push_back(c);
    } else {
        if (compare(u,v) >= 0) {
            w = substract_simple(u,v);
            w.znak = u.znak;
        } else {
            w = substract_simple(v,u);
            w.znak = v.znak;
        }
    }
    return w;
}

// SUMOWANIE LICZB (ADD2)
/* Liczba add2(Liczba u, Liczba v) {
    vector<cyfra> temp1(u.cyfry);
    vector<cyfra> temp2(v.cyfry);
    int n = temp1.size()>=temp2.size()?temp1.size():temp2.size();
    for (int i=temp1.size();i<n;i++) temp1.push_back(0);
    for (int i=temp2.size();i<n;i++) temp2.push_back(0);

    int i = n-1;
    int j,t;
    Liczba w;
    w.cyfry = vector<cyfra>(n+1,0);
    while (i >= 0) {
        t = temp1[i]+temp2[i];
        w.cyfry[i] = t % u.b;
        j = i;
        while (t >= u.b) {
            j++;
            t = w.cyfry[j] + 1;
            w.cyfry[j] = t % u.b;
        }
        i--;
    }
    if (w.cyfry[w.cyfry.size()-1] == 0) w.cyfry.pop_back();
    return w;
} */

// ODEJMOWANIE LICZB
Liczba substract(Liczba u, Liczba v) {
    v.znak = -v.znak;
    return add(u,v);
}

ostream & operator << (ostream &wyjscie, const Liczba &s) {
    if (s.znak == -1) wyjscie << '-';
    for (int i=s.cyfry.size()-1;i>=0;i--) {
        wyjscie << i2c(s.cyfry[i]);
    }
    return wyjscie;
}

istream & operator >> (istream &wejscie, Liczba &s) {
    s.cyfry.clear();
    string tmp;
    wejscie >> tmp;
    const char *cstr = tmp.c_str();
    int i;
    if (cstr[0] == '-') {
        i = 1;
        s.znak = -1;
    } else {
        i = 0;
        s.znak = 1;
    }
    for (;i<tmp.length();i++) {
        s.cyfry.push_back(c2i(cstr[i]));
    }
    reverse(s.cyfry.begin(),s.cyfry.end());
    return wejscie;
}

int main() {
    int base;
    Liczba a;
    Liczba b;

    cout << "PODAJ BAZE: "; cin >> base;
    a.b = base;
    b.b = base;

    while (1) {
        cout << "PODAJ 1. LICZBE: "; cin >> a;
        cout << "PODAJ 2. LICZBE: "; cin >> b;
        cout << endl;
        cout << "SUMOWANIE LICZB:   " << add(a,b) << endl;
        cout << "ODEJMOWANIE LICZB: " << substract(a,b) << endl;
        cout << endl;
    }

    return 0;
}
