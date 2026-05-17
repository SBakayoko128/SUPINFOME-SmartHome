# SUPINFOME - Smart Home Arduino

Projet embarqué réalisé à SUPINFO dans le cadre du cours IoT (2ème année).
Simulation d'une maison connectée à l'aide de deux cartes Arduino Uno communicant via UART.

---

## Architecture

```
[ Arduino 1 - Contrôleur principal ]
        |
        | UART (Serial 9600 baud)
        |
[ Arduino 2 - Actionneur ]
```

**Arduino 1** gère l'interface utilisateur : écran LCD 16x2, clavier matriciel 4x4, capteur de température, buzzer.

**Arduino 2** gère les actionneurs : LED RGB, deux servomoteurs (porte + volet), moteur VMC, capteur d'humidité.

---

## Fonctionnalités

| Fonctionnalité | Déclencheur | Description |
|---|---|---|
| Chauffage automatique | Température < 19°C | Détection via capteur analogique, commande envoyée à Arduino 2 |
| VMC automatique | Humidité > 60% | Gérée en autonomie par Arduino 2 |
| Contrôle porte | Touches `5` / `B` | Ouverture / fermeture via servomoteur |
| Contrôle volet | Touche `3` | Toggle ouvert/fermé via servomoteur |
| LED RGB | Touches `C`, `D`, `#`, `0` | Rouge, Vert, Bleu, Blanc |
| Sonnette | Touche `A` | Séquence sonore via buzzer |
| Affichage LCD | Temps réel | Température + état du système |

---

## Communication UART

Les deux cartes échangent via des commandes sur un octet :

| Commande | Sens | Action |
|---|---|---|
| `R` | 1 → 2 | LED Rouge / Chauffage ON |
| `W` | 1 → 2 | LED Blanche / Chauffage OFF |
| `G` | 1 → 2 | LED Verte |
| `B` | 1 → 2 | LED Bleue |
| `O` | 1 → 2 | Ouvrir porte |
| `F` | 1 → 2 | Fermer porte |
| `V` | 1 → 2 | Toggle volet |
| `H` | 1 → 2 | Demande état VMC |
| `M` | 2 → 1 | VMC en marche |
| `S` | 2 → 1 | VMC arrêtée |

---

## Composants utilisés

- 2x Arduino Uno
- Écran LCD 16x2 (LiquidCrystal)
- Clavier matriciel 4x4 (Keypad)
- Capteur de température analogique (TMP)
- Capteur d'humidité analogique
- LED RGB (cathode commune)
- 2x Servomoteur SG90
- Moteur DC (VMC)
- Buzzer actif

---

## Structure du repo

```
├── Code Arduino 1.ino   # Contrôleur principal (LCD, clavier, temp, buzzer)
├── Code Arduino 2.ino   # Actionneur (LED, servos, VMC, humidité)
├── image.png            # Schéma de câblage
└── SUPINFOME_Documentation.pdf
```

---

## Technologies

- Langage : C/C++ (Arduino)
- Communication : UART série (9600 baud)
- Librairies : `LiquidCrystal`, `Keypad`, `Servo`
- Simulation : Tinkercad compatible
