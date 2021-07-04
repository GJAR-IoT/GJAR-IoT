# Ako písať dokumentáciu

## Jazyk

Ideál je, aby všetka dokumentácia bola dvojjazyčná: [SK](./), [EN](../en/).
Vo všeobecnosti, všetky informácie musia byť dostupné po slovensky. Zároveň, keďže projekt je verejne dostupný na Githube, aj za účelom prezentácie jeho účastníkov, najmenej všetky externe prístupné časti projektu musia byť zdokumentované aj po anglicky (napr. všeobecné informácie o projekte a jeho štruktúre, o fungovaní organizácie, a tiež o používaní a API volaniach).

## Formát a štruktúra dokumentácie

Dokumentácia je písaná vo formáte **Markdown** a nasleduje štruktúru projektu - teda každá časť (Backend, Frontend, Hardware/Node) má svoju vlastnú dokumentáciu. Cieľom je, aby sa každá časť projektu vedela rozvíjať nezávisle.

Dokumentácia je rozdelená na jednotlivé súbory (stránky), ktoré tvoria tematicky samostatné jednotky (napr. tento návod na písanie dokumentácie je v inom súbore ako [popis štruktúry](structure.md) projektu).

Súbory sú prepojené odkazmi. Externé odkazy obsahujú celú cestu, tie interné používajú **relatívne** cesty, vďaka čomu je možné jednoduchšie odkazovať napr. na správnu jazykovú verziu dokumentácie komponentu (dopomáhajú tomu symbolické odkazy, pozri napr. `./Backend`).
