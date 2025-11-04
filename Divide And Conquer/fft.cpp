// Clase: Topics on Advanced Algoritmos
// FFT
// Juliana Nieto
// Octubre 20

#include <iostream>
#include <vector>
#include <complex>
#include <sstream>
#include <iomanip>
#include <cmath>
using namespace std;


using cd = complex<double>;
const double PI = acos(-1.0);

void fft(vector<cd>& a, bool inversa) {
    int n = (int)a.size();

    // Permutación por inversión de bits
    for (int i = 1, j = 0; i < n; ++i) {
        int bit = n >> 1;
        for (; j & bit; bit >>= 1) j ^= bit;
        j ^= bit;
        if (i < j) swap(a[i], a[j]);
    }

    // Etapas de mariposas
    for (int len = 2; len <= n; len <<= 1) {
        double ang = 2 * PI / len * (inversa ? -1 : 1);
        cd wlen(cos(ang), sin(ang));
        for (int i = 0; i < n; i += len) {
            cd w(1);
            for (int j = 0; j < len / 2; ++j) {
                cd u = a[i + j];
                cd v = a[i + j + len / 2] * w;
                a[i + j] = u + v;
                a[i + j + len / 2] = u - v;
                w *= wlen;
            }
        }
    }
    if (inversa) for (cd& x : a) x /= n;
}

// Evita imprimir "-0.00" y limpia ruido numérico muy pequeño.
inline double limpiaCero(double x) {
    if (fabs(x) < 5e-3) return 0.0; // valores ~0 -> 0 exacto
    return x;
}
inline string fmt(double x) {
    x = limpiaCero(x);
    ostringstream os;
    os << fixed << setprecision(2) << x;
    if (os.str() == "-0.00") return "0.00";
    return os.str();
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    // Leer dos líneas de coeficientes
    string sA, sB;
    if (!getline(cin, sA)) return 0;
    if (!getline(cin, sB)) return 0;

    vector<double> A, B;
    {
        istringstream in(sA); double x;
        while (in >> x) A.push_back(x);
    }
    {
        istringstream in(sB); double x;
        while (in >> x) B.push_back(x);
    }
    int p = (int)A.size(), q = (int)B.size();
    if (p == 0 || q == 0) { cout << "\n\n\n"; return 0; }

    // N = siguiente potencia de 2 que cubre la convolución
    int need = p + q - 1;
    int N = 1; while (N < need) N <<= 1;

    // Copias como complejos
    vector<cd> fa(N), fb(N);
    for (int i = 0; i < p; ++i) fa[i] = cd(A[i], 0);
    for (int i = 0; i < q; ++i) fb[i] = cd(B[i], 0);

    // DFT(A) y DFT(B)
    vector<cd> FA = fa, FB = fb;
    fft(FA, false);
    fft(FB, false);

    // Imprimir DFT(A)
    for (int i = 0; i < N; ++i) {
        cout << "(" << fmt(FA[i].real()) << "," << fmt(FA[i].imag()) << ")";
        if (i + 1 < N) cout << " ";
    }
    cout << "\n";

    // Imprimir DFT(B)
    for (int i = 0; i < N; ++i) {
        cout << "(" << fmt(FB[i].real()) << "," << fmt(FB[i].imag()) << ")";
        if (i + 1 < N) cout << " ";
    }
    cout << "\n";

    // Producto puntual e IFFT para la convolución
    for (int i = 0; i < N; ++i) fa[i] = FA[i] * FB[i];
    fft(fa, true); // IFFT

    // Imprimir coeficientes del producto (sólo los necesarios)
    for (int i = 0; i < need; ++i) {
        cout << fmt(fa[i].real());
        if (i + 1 < need) cout << " ";
    }
    cout << "\n";
    return 0;
}