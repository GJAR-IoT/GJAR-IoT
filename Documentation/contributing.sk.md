# Ako prispieť ku GJAR IoT

*(Predpokladané znalosti: práca s Gitom, GitHubom - v prípade potreby pozri dokumentáciu: [EN](https://git-scm.com/book/en/v2), [CZ](https://git-scm.com/book/cs/v2).)*

*Tento návod je o prispievaní do kódu a pod. Pre písanie dokumentácie, pozri *Ako písať dokumentáciu*.*

0. Vytvor fork relevantnej časti projektu (jeden z repozitárov [GJAR-IoT](https://github.com/GJAR-IoT/GJAR-IoT), [GJAR-IoT-Backend](https://github.com/GJAR-IoT/GJAR-IoT-Backend), [GJAR-IoT-Frontend](https://github.com/GJAR-IoT/GJAR-IoT-Frontend), [GJAR-IoT-Node](https://github.com/GJAR-IoT/GJAR-IoT-Node)). Ak nepotrebuješ manuálne vytvoriť changelog alebo upravovať všeobecnú dokumentáciu (súčasťou ktorej je aj táto časť), pravdepodobne nepotrebuješ fork repozitára GJAR-IoT.
1. (Dobrovoľné, ale odporúčané.) Vytvor si pre svoje zmeny novú vetvu, so zmysluplným a popisným názvom (v ňom použi konkrétny názov veci, na ktorej pracuješ).
2. Urob zmeny, píš kód, zmeny ukladaj do Gitu (commit).
3. Odošli zmeny (push) do svojho forku.
4. Vytvor Pull Request (PR), ktorá obsahuje (nepovinný, ale odprúčaný) všeobecný popis a účel zmien, po ktorom nasleduje **povinný** záznam pre changelog - pozri nižšie.

## Formát PR

Na začiatku PR by mal byť všeobecný popis, obsahujúci cieľ zmien. Ak sa prijatím zmien má uzavrieť jedna alebo viac issues, tento popis by mal obsahovať frázu `Fixes #<číslo issue>, #<ďalšie>`, obsakazujúcu na všetky dotknuté issues. Tento popis je možné vynechať, ak nepridáva žiadne informácie, ktoré nie sú obsiahnuté nižšie.

Nasleduje **povinný** záznam pre changelog vo formáte:

```md
Changelog:
+ pridané veci
+ začínajú znamienkom plus (a medzera)
- odobraté mínusom
* zmeny, opravy hviezdičkou
```

Keďže z toho bude changelog, jednotlivé položky musia byť jasné, jednoznačné, popisné a stručné. Ak nevieš jednoznačne, či ide o pridanú alebo odobratú vec, najskôr je najvhodnejšia hviezdička ("zmena").
