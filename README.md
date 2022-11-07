main:
    In main se creeaza tabelul de puteri perfecte incepand de la 2 pana la numarul de reduceri,
    de asemenea se creeaza si un vector de structuri thread_arguments care va pastra informatia
    despre fiecare thread. Threadurile cu id mai mic decat numarul de maperi vor fi threadurile
    folosite pentru mapper, celelalte vor fi pentru reducer.
vector:
    Vector este o structura care pastreaza un int* si dimensiunea acestuia.
    *return_power_table:
        Functia care creeaza tabelul de puteri, incepand cu puterea 2 pana la nr_reducer + 2.
        Aceasta va pastra toate puterile perfecte pana la INT_MAX.
    *get_index_from_power_table:
        Cauta un numar in tabelul de puteri, si daca il gaseste(numarul are putere perfecta) 
        atunci adauga numarul in lista corespunzatoare puterii in care a fost gasit.Un numar 
        poate fi putere perfecta pentru mai multe numere, de aceea nu se opreste la prima 
        gasire a numarului. Algoritmul foloseste un binarySearch pentru a optimiza timpul 
        intrucat tabelul de puteri poate deveni destul de mare.
    *merge_Vector:
        Concateneaza 2 vectori intr-unul singur.
    *add_elem_in_list:
        Adauga un element in lista unui vector(in lista int* din structura).Se realoca spatiu 
        si se adauga noul element, daca acesta nu este deja acolo.Functie folosita de reducer
        pentru a sterge duplicatele.
    *vector_destroy:
        Sterge memoria folosita de un vector.
thread_arguments:
    *thread_distribution:
        Citeste cu threadurile pentru mapper din fisierul de test fisierele pe care trebuie sa 
        le citeasca in continuare. S-a adaugat un mutex pentru ca mai multe threaduri sa nu
        citeasca in acelasi timp din acelasi fisier.
        Se apeleaza functia mapper si reducer corespunzatoare. Daca un thread de mapper a terminat
        de citit din fisier, va verifica daca mai poate citi din fisierul test.txt si in caz afirmativ,
        va citi fisierul urmator.
    *mapper:
        Citeste linie cu linie din fisierul de inptut, si daca gaseste o putere perfecta o adauga in 
        lista cu puteri perfecte a thread-ului curent. Intrucat un thread poate citi mai multe fisiere, 
        datele din fisierele urmatoare vor fi adaugate in aceleasi liste de la fisierele trecute.
    *reducer:
        Trece prin listele de puteri perfecte salvate de fiecare mapper si calculeaza numarul lor fara 
        duplicate, apoi le afiseaza in fisierul de output.
    *thread_arguments_destroy:
        Elibereaza memoria structurii thread_arguments.

