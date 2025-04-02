# Proiect

## Prezentare Generala

Acesta este un sistem simplu de management bancar implementat in C, care permite atat functionalitati pentru admin, cat si pentru utilizatori, cum ar fi gestionarea utilizatorilor, adaugarea/stergerea conturilor IBAN, transferul de bani, plata facturilor de utilitati si gestionarea conturilor utilizatorilor. Sistemul foloseste liste legate pentru a stoca informatiile utilizatorilor si conturilor si functioneaza cu doua roluri principale: admin si utilizator.

## Functionalitati
# Functionalitati Admin:
1. Adaugare Utilizator - Adaugarea unui utilizator nou prin introducerea numelui de utilizator, parolei si CNP-ului.
2. Adaugare cod IBAN - Adaugarea unui nou cont IBAN pentru un utilizator existent, cu moneda si sold initial.
3. Vizualizare Utilizatori - Afisarea tuturor utilizatorilor impreuna cu detaliile conturilor lor.
4. Stergere cod IBAN - Stergerea unui cont IBAN din profilul unui utilizator.
5. Editare Date Utilizator - Editarea informatiilor unui utilizator, cum ar fi numele de utilizator si parola, pe baza CNP-ului.
6. Schimbare Parola Admin - Schimbarea parolei admin-ului.
7. Deconectare - Iesirea din sesiunea de admin.

# Functionalitati Utilizator:

1. Selectare Cont pentru Operatiuni - Selectarea unui cont pentru diverse operatiuni, cum ar fi transferuri si plati.
2. Afisare Extras - Vizualizarea detaliilor contului, inclusiv soldul si moneda.
3. Transfer de Bani - Transferul de bani intre conturile utilizatorului sau catre un alt utilizator.
4. Plata Facturi Utilitati - Plata facturilor de utilitati din contul selectat.
5. Schimbare Parola - Schimbarea parolei utilizatorului.
6. Deconectare - Iesirea din sesiunea utilizatorului.


# Cum se Utilizeaza
1. Inceputul programului: La rularea aplicatiei, se va deschide un meniu principal care permite accesul la sesiuni de admin si utilizator.
2. Autentificare Admin: Adminul poate sa se conecteze folosind numele de utilizator si parola prestabilite (admin/admin123) si sa administreze utilizatorii si conturile.
3. Autentificare Utilizator: Un utilizator poate sa se conecteze folosind numele de utilizator si parola proprii, avand acces la functionalitatile conturilor sale.
4. Fisier de intrare: Fisierele users.txt si accounts.txt sunt citite la inceputul rularii aplicatiei pentru a popula sistemul cu datele corespunzatoare.
