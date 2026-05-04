import random

def generuj_graf_trojspojny(liczba_wierzcholkow, nazwa_pliku="wejscie.txt"):
    if liczba_wierzcholkow < 4:
        print("Minimalna liczba wierzcholkow to 4.")
        return

    edges = [(1, 2), (1, 3), (1, 4), (2, 3), (2, 4), (3, 4)]
    faces = [(1, 2, 3), (1, 2, 4), (1, 3, 4), (2, 3, 4)]

    for i in range(5, liczba_wierzcholkow + 1):
        idx = random.randint(0, len(faces) - 1)
        u, v, w = faces.pop(idx)

        edges.extend([(u, i), (v, i), (w, i)])
        faces.extend([(u, v, i), (v, w, i), (w, u, i)])

    with open(nazwa_pliku, "w", encoding="utf-8") as f:
        for u, v in edges:
            waga = random.randint(1, 20)
            a, b = min(u, v), max(u, v) 
            f.write(f"v{a}v{b} {a} {b} {waga}\n")
            
    print(f"Sukces! Wygenerowano graf ({liczba_wierzcholkow} wierzcholkow) do pliku: {nazwa_pliku}")

generuj_graf_trojspojny(80)