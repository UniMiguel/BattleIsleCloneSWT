# Swt-Praktikum

## Bemerkung zur Stilfibel
Wir verwenden die ungerische Notation. Dazu einige Details:

#### Klassennamen
Bspw:   `Hexagon`, `WasserHex` (Wasser hat von Hexagon geerbt)

* Klassennamen sollen zu Beginn immer groß geschrieben werden
* Bei Vererbung der Klasse muss erkennbar sein von welcher Klasse abgeleitet worden ist
* Abkürzungen erlaubt
* Folgewörter sollen mit Großbuchstaben beginnen
* Unterstriche und Zahlen ganz vermeiden

#### Attribut- und Methodennamen
Bspw: `int_hexPos`

* Attribut und Methoden sollen zu Beginn immer klein geschrieben werden
* Zweck soll am Namen erkennbar sein
* Zugehörigkeit soll am Namen erkennbar sein (mit Abkürzung des Klassennamens)
* Attribute die als Pointer genutzt werden, werden mit einem 'ptr_' vorne ergäntzt
* Datentyp sollte mit einem Unterstrich vorne angehängt werden
* Folgewörter sind mit Großbuchstaben (ohne Unterstriche) anzuhängen.
* Zahlen vermeiden

#### Aufbau und Konvenzionen des Quellcodes
Bspw:
```
    int funktion( int bla, int blub ) 
    {
        for( int a = 0; a < bla; a ++ )
        {
            cout << "tu irgendwas" << endl;
        }
        return bla * blub;
    }
```
* Operationszeichen (`+ - / < *`) werden durch ein Leerzeichen vor und nach angegeben. Außnahme Operationen die zusammengehören (Bspw `+= ++ && ||`)
* Anweisungsende `;` ohne voheriges Leerzeichen angeben
* In Funktions- und Operationsklammern `()` ist immer mindestens ein Leerzeichen nach `(` und vor
`)`. Ist die Klammer leer darf auf das Leerzeichen verzichtet werden. Vor dem öffnen der Klam-
mer `(` wird kein Leerzeichen angegeben.
* Die Geschweifteklammer `{ }` ist mit einem Zeilenumbruch nach der Funktions- und Operationsklam-
mern zu öffnen. Geschlossen wird diese in der Folgezeile auf selber Höhe wie die Funktion der
diese öffnet. Geschweifteklammern müssen benutzt werden.
* Das Innere einer Funktion, Operation oder Klasse ist nach der Geschweiftenklammer `{` um eine
Zeile und ein Tab versetzt zu beginnen. Diese Struktur ist Baumförming fortzuführen.
* Globalevariabeln mit `const` deklarieren und auf „const-correctness“ Achten.
* Nach einer Funktion, Methode oder Klasse muss eine leere Zeile sein.
* Es wird nur wichtige Informationen kommentiert und das über der jeweiligen Funktion

#### Definition und Deklaration
Jede Klasse hat seine eigene **hpp** und **cpp** Datei. 
Die Klasse wird im Header deklariert und der Header benötigt ein geeigneten Wächter. Der Wächter hat den Namen der Klasse, der komplett groß geschrieben ist und wird mit einem `_H` beendet. 
Die abgeleitete Klassen dürfen, wenn diese nicht zu groß sind, in die Elternklasse.

#### Sprache 
Die verwendete Sprache für die Bezeichnungen ist **Englisch**.

#### Patterns
Es sollen geeignete Muster verwendet werden um Probleme zu lösen. Es gibt viele gute und bewährte Lösungsansätzte auf die zugegriffen werden soll.

#### sonstiges
*Viel Spaß beim Programmieren ;P*



