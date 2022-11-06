# Licznik GPS

## Założenia projektowe

1. Odświeżanie GPS - częstotliwość odświeżania zależna od prędkości poruszania się. Gdy ktoś idzie spokojnym tempem nie trzeba często odświeżać aby mieć dokładne dane, jeśli ktoś szybko jedzie rowerem częstotliwość pobierania danych GPS powinna się zwiększyć.  Przy założeniu że osoba chodzi z prędkością 6 km/h to wówczas w ciągu sekundy przejdzie 1,66 metra więc pomiar co sekundę zapewnia nam dobrą dokładność. Natomiast poruszając się rowerem z prędkością 35 km/h z każdą sekunda pokonuje się niemal 10 metrów zatem częstotliwość pomiarów należy zwiększyć do około 4 pomiarów na sekundę. Natomiast jeśli chcemy uzyskać mniejszą różnicę w przebytym dystansie częstotliwość należy zwiększyć.
2. Możliwość odczytania położenia geograficznego z modułu GPS. Na podstawie zebranych danych wyliczanie: kierunku poruszania się (azymut), obecnej prędkości, przebytego dystansu, średniej prędkości na całym dystansie, spalonych kalorii
3. Prezentacja danych ma odbywać się na wyświetlaczu który musi pomieścić powyższe pomiary. Optymalnie by było gdyby wyświetlacz miałby niskie zużycie energii, tak by nie musieć używać dużej baterii. Warto też pomyśleć o dobrym działaniu w dużym słońcu tak aby wyświetlane dane były widoczne. Dane na wyświetlaczu powinny zmieniać się do około 0,5-1 sekundy.
4. Odpowiednia wielkość pamięci podręcznej jednostki obliczeniowej i pamięci trwałej tak aby pamięć podręczna była w stanie pomieścić wszystkie dane potrzebne do obecnych obliczeń a pamięć trwała mogła przechować dane do momentu synchronizacji z urządzeniem mobilnym. 
5. Obsługa za pomocą przycisków, start, stop, reset, sync
6. Bateria będąca w stanie zapewnić działanie urządzenia przez co najmniej kilka godzin ciągłej pracy.
7. Opcjonalne przekazanie zgranych danych (średnia prędkość, przebyty dystans, spalone kalorie) przez bluetooth do urządzenia z systemem Android

## Podział zadań

**Kuba Cz**: zadbanie o odpowiednią baterię, napisanie obsługi bluetooth, odpowiednie zapisywanie danych w pamięci,

**Kuba G**: zadbanie o odpowiedni moduł gps, obsługa komunikacji z modułem gps, zadbanie o odpowiednie działanie przycisków

**Piotrek**: obliczanie danych do wyświetlania,  zadbanie o wyświetlacz i odpowiednią komunikację z wyświetlaczem

## Funkcje i odpowiednie testy
|Funkcja                                 |Test        |
|------------------------                |------------|
|Poprawna obsługa wyświetlacza           | Empiryczne sprawdzenie czy wyświetlacz w poprawny sposób wypisuje to co zostanie mu zadane|
|Wyświetlanie danych                     | Empiryczne sprawdzenie poprawności schematu wyświetlanych danych|
|Poprawna obsługa modułu GPS             | Wypisanie danych GPS na wcześniej przetesotwany ekran|
|Odczywanie pozycji GPS                  | Wyjście w teren i sprawdzenie działania modułu z rzeczywistym, sprawnym GPS|
|Obliczenia                              | Sprawdzenie poprawności obliczeń poprzez przekazanie systemowi wcześniej spreparowanych i przeliczonych danych, porównanie wyników|
|Bateria                               | Sprawdzenie długości działania urządzenia w najbardziej pesymistycznym scenariuszu, czyli przy najczęstszym odświeżaniu modułu GPS|
|Poprawna komunikacja z modułem Bluetooth| Wysłanie danych z głównego urządzenia na moduł bluetooth 
|Poprawne przesyłanie danych po Bluetooth| Sprawdzenie poprawności wcześniej wysłanych danych poprzez aplikację pozwalającą na czytanie danych z bluetooth
|Poprawna obsługa danych w pamięci       | Zapisanie, odczytanie i sprawdzenie poprawności danych 

## Wymagania systemowe

|Sprzęt     | Wymagania|
|-----------| ---------|
|Moduł GPS| Optymalnie częstotliwość pracy od 1 do około 10-15 Hz. Komunikacja po powszechnie stosowanych protokołach komunikacji. UART lub SPI lub I2C. Zapewniający dosyć dużą dokładność, maksymalnie 1m odchyłu od rzeczywistego położenia. Pozwoli to na w miarę dokładne dane pomiarowe.| 
|Moduł bluetooth| Zasięg przesyłu danych nie jest potrzebny bardzo długi, wystarczy kilka mertów. Powinien obsługiwać UART lub SPI lub I2C. Powinien też być w miarę możliwości energooszczędny|
|Wyświetlacz| W idealnym przypadku kilka linijek i ponad 20 kolumn. Pozwoli to na estetyczne wyświetlenie wszystkich danych. Możliwie na zasadzie e-ink pozwalający na dobre działanie w słońcu i niskie zużycie baterii. Komunikacja po standerdowych protokołach|
|Bateria| Wystarczająco pojemna aby układ mógł pracować przez kilka/kilkanaście godzin. Obecnie bez konkternych wymagań odnośnie wymiarów fizycznych. Prototyp obecie tego nie wymaga.|
|Main board| Płytka powinna być na tyle szybka żeby być w stanie wykonać obliczenia na czas. Pownna też mieć na tyle dużo pinów aby dało się podpiąć do niej wszystkie urządzenia i zapewnić odpowiednią komuniakcję 

## Schemat blokowy

## Wstępny schedule

| Data | Zadanie |
|------|---------| 
|17.11.2022| Znalezienie odpowiedniego sprzętu i jego ewentualny zakup.|
|24.11.2022| Dopasowanie odpowiedniej baterii i zasilania |
|1.12.2022 | Odpowiednie oprogramowania wyświetlacza |
|8.12.2022 | Odpowiednie oprogramowanie modułu GPS |
|15.12.2022| Stworzenie algorytmów wyliczających dane i dostosowanie wyświetlacza|
|22.12.2022| Oprogramowanie odpowiedniego działania modułu Bluetooth |
|Do deadline projektu| Dalsze testy, poprawki i usprawnianie działania |

## Analiza / Rozwiązanie
- przedstawienie schematu ideowego / blokowego

## Notatki

- Założyć system kontroli wersji, pokazać system kontroli wersji 
- zrobić tabelkę z wymiganiami i testami do wymagań
- oszacować ile dane zadanie powinno zająć w ludzio-dniach  
- prepare system requirements
- poor but realistic schedule of creating projects
- functions should be assigned to hardware
- create block diagram
-E ink jest dobry jeśli nasza treść sie rzadko zmienia 
- uważać na zasilanie (napisać na schemacie napięcia) tak aby wszystko było odpowiednio zasilone
- można opisać RX TX
-oznaczając numery nóżek trzeba przyjąć odpowiedni standard (czyje RX czyje TX) np RT i TX z bluetooth BT_RX BT_TX
- przetwornica buckboost jeśli nasze zasilanie z akumulatora może oscylować wokół 3,3, może zejść poniżej 3,3 i step down sobie nie poradzi
- przygotować jeden dwa slajdy, schematy blokowe, można wpisać hardware ale nie musi być, na schematach opisywać, pokazywać funkcje


![image](https://user-images.githubusercontent.com/88560899/199722592-fd2eb058-cdfc-40b0-ac53-511aaf10b4da.png)
