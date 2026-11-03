<?xml version="1.0" encoding="utf-8"?>
<!DOCTYPE TS>
<TS version="2.1" language="pl_PL">
<context>
    <name>AiFallbackArgParser</name>
    <message>
        <location filename="../libqnapi/src/parser/aifallbackargparser.cpp" line="32"/>
        <source>When no subtitles in the main language are found, generate them locally: transcribe the audio (whisper.cpp) and translate it with a local language model (llama.cpp). Needs a CUDA GPU, ffmpeg and the models next to the executable</source>
        <translation>Gdy nie znaleziono napisów w głównym języku, wygeneruj je lokalnie: transkrypcja dźwięku (whisper.cpp) i tłumaczenie lokalnym modelem językowym (llama.cpp). Wymaga karty graficznej z CUDA, ffmpeg i modeli obok pliku programu</translation>
    </message>
</context>
<context>
    <name>BackupLangArgParser</name>
    <message>
        <location filename="../libqnapi/src/parser/backuplangargparser.cpp" line="32"/>
        <source>You need to pass backup language code after %1 argument</source>
        <translation>Musisz podać kod języka zapasowego po argumencie %1</translation>
    </message>
    <message>
        <location filename="../libqnapi/src/parser/backuplangargparser.cpp" line="42"/>
        <source>Invalid backup language code passed: %1</source>
        <translation>Niepoprawny kod jezykowy: %1</translation>
    </message>
    <message>
        <location filename="../libqnapi/src/parser/backuplangargparser.cpp" line="50"/>
        <source>code</source>
        <translation>kod</translation>
    </message>
    <message>
        <location filename="../libqnapi/src/parser/backuplangargparser.cpp" line="51"/>
        <source>Alternative subtitles language</source>
        <translation>Zapasowy język pobierania napisów</translation>
    </message>
</context>
<context>
    <name>CliAiFallback::ConsoleListener</name>
    <message numerus="yes">
        <location filename="../cli/src/cliaifallback.cpp" line="41"/>
        <source>AI fallback: generating %1 subtitles for %n file(s)</source>
        <translation>
            <numerusform>Napisy AI: generowanie napisów (%1) dla %n pliku</numerusform>
            <numerusform>Napisy AI: generowanie napisów (%1) dla %n plików</numerusform>
            <numerusform>Napisy AI: generowanie napisów (%1) dla %n plików</numerusform>
        </translation>
    </message>
    <message>
        <location filename="../cli/src/cliaifallback.cpp" line="46"/>
        <source>AI fallback is unavailable, missing:</source>
        <translation>Napisy AI są niedostępne, brakuje:</translation>
    </message>
    <message>
        <location filename="../cli/src/cliaifallback.cpp" line="51"/>
        <source>Build the tools with scripts/build-ai-tools.bat and download the models with scripts/download-ai-models.ps1</source>
        <translation>Zbuduj narzędzia skryptem scripts/build-ai-tools.bat i pobierz modele skryptem scripts/download-ai-models.ps1</translation>
    </message>
    <message numerus="yes">
        <location filename="../cli/src/cliaifallback.cpp" line="75"/>
        <source>%n line(s) transcribed in %1</source>
        <translation>
            <numerusform>%n linia przepisana w %1</numerusform>
            <numerusform>%n linie przepisane w %1</numerusform>
            <numerusform>%n linii przepisanych w %1</numerusform>
        </translation>
    </message>
    <message>
        <location filename="../cli/src/cliaifallback.cpp" line="80"/>
        <source>AI fallback: nothing was transcribed</source>
        <translation>Napisy AI: nic nie zostało przepisane</translation>
    </message>
    <message>
        <location filename="../cli/src/cliaifallback.cpp" line="84"/>
        <source>Loading the translation model...</source>
        <translation>Wczytywanie modelu tłumaczącego...</translation>
    </message>
    <message>
        <location filename="../cli/src/cliaifallback.cpp" line="88"/>
        <source>Model loaded in %1</source>
        <translation>Model wczytany w %1</translation>
    </message>
    <message>
        <location filename="../cli/src/cliaifallback.cpp" line="92"/>
        <source>AI fallback: %1</source>
        <translation>Napisy AI: %1</translation>
    </message>
    <message>
        <location filename="../cli/src/cliaifallback.cpp" line="94"/>
        <source>Server log: %1</source>
        <translation>Log serwera: %1</translation>
    </message>
    <message>
        <location filename="../cli/src/cliaifallback.cpp" line="100"/>
        <source>Translating %1</source>
        <translation>Tłumaczenie %1</translation>
    </message>
    <message>
        <location filename="../cli/src/cliaifallback.cpp" line="114"/>
        <source>AI subtitles saved (%1)</source>
        <translation>Zapisano napisy AI (%1)</translation>
    </message>
    <message numerus="yes">
        <location filename="../cli/src/cliaifallback.cpp" line="117"/>
        <source>%n line(s) left untranslated</source>
        <translation>
            <numerusform>%n linia pozostała nieprzetłumaczona</numerusform>
            <numerusform>%n linie pozostały nieprzetłumaczone</numerusform>
            <numerusform>%n linii pozostało nieprzetłumaczonych</numerusform>
        </translation>
    </message>
    <message>
        <location filename="../cli/src/cliaifallback.cpp" line="130"/>
        <source>Transcribing %1</source>
        <translation>Transkrypcja %1</translation>
    </message>
</context>
<context>
    <name>CliMain</name>
    <message>
        <location filename="../cli/src/climain.cpp" line="37"/>
        <source>QNapi %1 (Qt version %2), %3</source>
        <translation>QNapi %1 (wersja Qt %2), %3</translation>
    </message>
    <message>
        <location filename="../cli/src/climain.cpp" line="52"/>
        <source>List of languages recognized by QNapi, including corresponding</source>
        <translation>Oto lista rozpoznawanych przez QNapi jezykow i odpowiadajacym</translation>
    </message>
    <message>
        <location filename="../cli/src/climain.cpp" line="53"/>
        <source>two-letter language codes:</source>
        <translation>im dwuliterowych kodow:</translation>
    </message>
    <message>
        <location filename="../cli/src/climain.cpp" line="68"/>
        <source>Current default subtitles language: %1 (%2)</source>
        <translation>Aktualnie ustawiony preferowany jezyk pobierania napisow: %1 (%2)</translation>
    </message>
    <message>
        <location filename="../cli/src/climain.cpp" line="73"/>
        <source>No alternative subtitles language has been set</source>
        <translation>Brak ustawionego zapasowego jezyka napisow</translation>
    </message>
    <message>
        <location filename="../cli/src/climain.cpp" line="75"/>
        <source>Current alternative subtitles language: %1 (%2)</source>
        <translation>Aktualnie ustawiony zapasowy jezyk napisow: %1 (%2)</translation>
    </message>
    <message>
        <location filename="../cli/src/climain.cpp" line="106"/>
        <source>QNapi: deleting temporary files...</source>
        <translation></translation>
    </message>
    <message>
        <location filename="../cli/src/climain.cpp" line="123"/>
        <source>QNapi: finished.</source>
        <translation></translation>
    </message>
    <message>
        <location filename="../cli/src/main.cpp" line="91"/>
        <source>Command line argument parsing error:</source>
        <translation></translation>
    </message>
</context>
<context>
    <name>CliSubtitlesDownloader</name>
    <message>
        <location filename="../cli/src/clisubtitlesdownloader.cpp" line="37"/>
        <source>Invalid path to p7zip executable: %1</source>
        <translation>Sciezka do programu p7zip jest nieprawidlowa: %1</translation>
    </message>
    <message>
        <location filename="../cli/src/clisubtitlesdownloader.cpp" line="44"/>
        <source>Can&apos;t write to temporary directory: %1</source>
        <translation>Nie mozna pisac do katalogu tymczasowego: %1</translation>
    </message>
    <message>
        <location filename="../cli/src/clisubtitlesdownloader.cpp" line="255"/>
        <source>%1 - already has subtitles in the main language, no AI needed</source>
        <translation>%1 - ma już napisy w głównym języku, AI niepotrzebne</translation>
    </message>
    <message>
        <location filename="../cli/src/clisubtitlesdownloader.cpp" line="260"/>
        <source>AI fallback: every file has subtitles already</source>
        <translation>Napisy AI: wszystkie pliki mają już napisy</translation>
    </message>
    <message>
        <source>Calculating checksums...</source>
        <translation type="vanished">Obliczanie sum kontrolnych...</translation>
    </message>
    <message>
        <source>Searching for subtitles [%1] (%2)...</source>
        <translation type="vanished">Szukanie napisów [%1] (%2)...</translation>
    </message>
    <message>
        <source>Searching for subtitles in alternative language [%1] (%2)...</source>
        <translation type="vanished">Szukanie napisów w języku zapasowym [%1] (%2)...</translation>
    </message>
    <message>
        <source>0)	Do not download subtitles for this video</source>
        <translation type="vanished">0)	Nie pobieraj napisow dla tego filmu</translation>
    </message>
    <message>
        <source>Found subtitles:</source>
        <translation type="vanished">Znaleziono napisy:</translation>
    </message>
    <message>
        <source> (good)</source>
        <translation type="vanished"> (dobre)</translation>
    </message>
    <message>
        <source> (bad)</source>
        <translation type="vanished"> (zle)</translation>
    </message>
    <message>
        <source>Select subtitles to download: </source>
        <translation type="vanished">Wybierz napisy do pobrania: </translation>
    </message>
    <message>
        <source>Downloading subtitles...</source>
        <translation type="vanished">Pobieranie napisow...</translation>
    </message>
    <message>
        <source>Unable to download subtitles!</source>
        <translation type="vanished">Nie udalo sie pobrac napisow!</translation>
    </message>
    <message>
        <source>Unpacking subtitles...</source>
        <translation type="vanished">Rozpakowywanie napisow...</translation>
    </message>
    <message>
        <source>Failed to unpack subtitles!</source>
        <translation type="vanished">Nie udalo sie rozpakowac napisow!</translation>
    </message>
    <message>
        <source>Post-processing subtitles file...</source>
        <translation type="vanished">Przetwarzanie napisow...</translation>
    </message>
    <message>
        <source>Adjusting subtitles...</source>
        <translation type="vanished">Dopasowywanie napisów...</translation>
    </message>
    <message>
        <source>Could not adjust subtitles!</source>
        <translation type="vanished">Nie udało się dopasować napisów!</translation>
    </message>
    <message>
        <source>Downloading subtitles for &apos;%1&apos; [%2/%3]</source>
        <translation type="vanished">Pobieranie napisow dla &apos;%1&apos; [%2/%3]</translation>
    </message>
    <message>
        <source>No permission to write to the directory &apos;%1&apos;!</source>
        <translation type="vanished">Brak uprawnień zapisu do katalogu &apos;%1&apos;!</translation>
    </message>
    <message>
        <source>Subtitles not found!</source>
        <translation type="vanished">Nie znaleziono napisow!</translation>
    </message>
    <message numerus="yes">
        <source>Processing of remaining %n file(s) was ignored due to critical error.</source>
        <translation type="vanished">
            <numerusform>Pobieranie pozostalego 1 pliku przerwane z powodu bledu.</numerusform>
            <numerusform>Pobieranie pozostalych %n plikow przerwane z powodu bledu.</numerusform>
            <numerusform>Pobieranie pozostalych %n plikow przerwane z powodu bledu.</numerusform>
        </translation>
    </message>
</context>
<context>
    <name>CliSubtitlesDownloader::ConsoleListener</name>
    <message>
        <location filename="../cli/src/clisubtitlesdownloader.cpp" line="68"/>
        <source>Processing %1 files, %2 at a time...</source>
        <translation>Przetwarzanie plików: %1, po %2 naraz...</translation>
    </message>
    <message>
        <location filename="../cli/src/clisubtitlesdownloader.cpp" line="77"/>
        <source>Downloading subtitles for &apos;%1&apos; [%2/%3]</source>
        <translation>Pobieranie napisow dla &apos;%1&apos; [%2/%3]</translation>
    </message>
    <message>
        <location filename="../cli/src/clisubtitlesdownloader.cpp" line="93"/>
        <source>Calculating checksums...</source>
        <translation>Obliczanie sum kontrolnych...</translation>
    </message>
    <message>
        <location filename="../cli/src/clisubtitlesdownloader.cpp" line="96"/>
        <source>Searching for subtitles [%1] (%2)...</source>
        <translation>Szukanie napisów [%1] (%2)...</translation>
    </message>
    <message>
        <location filename="../cli/src/clisubtitlesdownloader.cpp" line="101"/>
        <source>Searching for subtitles in alternative language [%1] (%2)...</source>
        <translation>Szukanie napisów w języku zapasowym [%1] (%2)...</translation>
    </message>
    <message>
        <location filename="../cli/src/clisubtitlesdownloader.cpp" line="106"/>
        <source>Search failed (%4) - retrying in %1 s (%2/%3)...</source>
        <translation>Wyszukiwanie nie powiodło się (%4) - ponowna próba za %1 s (%2/%3)...</translation>
    </message>
    <message>
        <location filename="../cli/src/clisubtitlesdownloader.cpp" line="113"/>
        <source>Downloading subtitles...</source>
        <translation>Pobieranie napisow...</translation>
    </message>
    <message>
        <location filename="../cli/src/clisubtitlesdownloader.cpp" line="116"/>
        <source>Unpacking subtitles...</source>
        <translation>Rozpakowywanie napisow...</translation>
    </message>
    <message>
        <location filename="../cli/src/clisubtitlesdownloader.cpp" line="119"/>
        <source>Post-processing subtitles file...</source>
        <translation>Przetwarzanie napisow...</translation>
    </message>
    <message>
        <location filename="../cli/src/clisubtitlesdownloader.cpp" line="122"/>
        <source>Adjusting subtitles...</source>
        <translation>Dopasowywanie napisów...</translation>
    </message>
    <message numerus="yes">
        <location filename="../cli/src/clisubtitlesdownloader.cpp" line="138"/>
        <source>%n file(s) were rate-limited; waiting %1 s before retrying...</source>
        <translation>
            <numerusform>%n plik objęty limitem zapytań; czekam %1 s przed ponowną próbą...</numerusform>
            <numerusform>%n pliki objęte limitem zapytań; czekam %1 s przed ponowną próbą...</numerusform>
            <numerusform>%n plików objętych limitem zapytań; czekam %1 s przed ponowną próbą...</numerusform>
        </translation>
    </message>
    <message>
        <location filename="../cli/src/clisubtitlesdownloader.cpp" line="151"/>
        <source>Select subtitles to download: </source>
        <translation>Wybierz napisy do pobrania: </translation>
    </message>
    <message>
        <location filename="../cli/src/clisubtitlesdownloader.cpp" line="170"/>
        <source>No permission to write to the directory &apos;%1&apos;!</source>
        <translation>Brak uprawnień zapisu do katalogu &apos;%1&apos;!</translation>
    </message>
    <message>
        <location filename="../cli/src/clisubtitlesdownloader.cpp" line="175"/>
        <source>Subtitles not found!</source>
        <translation>Nie znaleziono napisow!</translation>
    </message>
    <message>
        <location filename="../cli/src/clisubtitlesdownloader.cpp" line="179"/>
        <source>Unable to download subtitles!</source>
        <translation>Nie udalo sie pobrac napisow!</translation>
    </message>
    <message>
        <location filename="../cli/src/clisubtitlesdownloader.cpp" line="183"/>
        <source>Failed to unpack subtitles!</source>
        <translation>Nie udalo sie rozpakowac napisow!</translation>
    </message>
    <message>
        <location filename="../cli/src/clisubtitlesdownloader.cpp" line="187"/>
        <source>Could not adjust subtitles!</source>
        <translation>Nie udało się dopasować napisów!</translation>
    </message>
    <message>
        <location filename="../cli/src/clisubtitlesdownloader.cpp" line="203"/>
        <source>still rate-limited, try again later</source>
        <translation>nadal limit zapytań, spróbuj później</translation>
    </message>
    <message>
        <location filename="../cli/src/clisubtitlesdownloader.cpp" line="207"/>
        <source>could not download subtitles</source>
        <translation>nie udało się pobrać napisów</translation>
    </message>
    <message>
        <location filename="../cli/src/clisubtitlesdownloader.cpp" line="214"/>
        <source>0)	Do not download subtitles for this video</source>
        <translation>0)	Nie pobieraj napisow dla tego filmu</translation>
    </message>
    <message>
        <location filename="../cli/src/clisubtitlesdownloader.cpp" line="215"/>
        <source>Found subtitles:</source>
        <translation>Znaleziono napisy:</translation>
    </message>
    <message>
        <location filename="../cli/src/clisubtitlesdownloader.cpp" line="222"/>
        <source> (good)</source>
        <translation> (dobre)</translation>
    </message>
    <message>
        <location filename="../cli/src/clisubtitlesdownloader.cpp" line="224"/>
        <source> (bad)</source>
        <translation> (złe)</translation>
    </message>
</context>
<context>
    <name>Console</name>
    <message>
        <location filename="../libqnapi/src/utils/console.cpp" line="60"/>
        <source>You must enter a number!</source>
        <translation>Wpisz liczbe!</translation>
    </message>
    <message>
        <location filename="../libqnapi/src/utils/console.cpp" line="65"/>
        <source>You must enter a number between %1 and %2</source>
        <translation>Wprowadz liczbe miedzy %1 a %2</translation>
    </message>
</context>
<context>
    <name>DontShowListArgParser</name>
    <message>
        <location filename="../libqnapi/src/parser/dontshowlistargparser.cpp" line="32"/>
        <source>Never show a list of available subtitles</source>
        <translation>Nie pokazuj listy dostępnych napisów</translation>
    </message>
</context>
<context>
    <name>ExtensionArgParser</name>
    <message>
        <location filename="../libqnapi/src/parser/extensionargparser.cpp" line="32"/>
        <source>You need to pass subtitles extension after %1 argument</source>
        <translation>Należy podać rozszerzenie napisów po argumencie %1</translation>
    </message>
    <message>
        <location filename="../libqnapi/src/parser/extensionargparser.cpp" line="40"/>
        <source>Invalid target subtitles extension passed: %1</source>
        <translation>Podano niepoprawne rozszerzenie napisow: %1</translation>
    </message>
    <message>
        <location filename="../libqnapi/src/parser/extensionargparser.cpp" line="48"/>
        <source>ext</source>
        <translation>ext</translation>
    </message>
    <message>
        <location filename="../libqnapi/src/parser/extensionargparser.cpp" line="49"/>
        <source>Select target subtitles file extension</source>
        <translation>Rozszerzenie pliku z napisami</translation>
    </message>
</context>
<context>
    <name>FormatArgParser</name>
    <message>
        <location filename="../libqnapi/src/parser/formatargparser.cpp" line="32"/>
        <source>You need to pass subtitles format code after %1 argument</source>
        <translation>Należy podać format napisów po argumencie %1</translation>
    </message>
    <message>
        <location filename="../libqnapi/src/parser/formatargparser.cpp" line="40"/>
        <source>Invalid target subtitles format passed: %1</source>
        <translation>Podano niepoprawny format napisow: %1</translation>
    </message>
    <message>
        <location filename="../libqnapi/src/parser/formatargparser.cpp" line="49"/>
        <source>format</source>
        <translation>format</translation>
    </message>
    <message>
        <location filename="../libqnapi/src/parser/formatargparser.cpp" line="50"/>
        <source>Select target subtitles file format (one of mDVD, MPL2, SRT, TMP)</source>
        <translation>Docelowy format napisów (mDVD, MPL2, SRT, TMP)</translation>
    </message>
</context>
<context>
    <name>GetThread</name>
    <message>
        <location filename="../gui/src/forms/frmprogress.cpp" line="311"/>
        <source>Checking permissions of the video directory...</source>
        <translation>Sprawdzanie uprawnień do katalogu z filmem...</translation>
    </message>
    <message>
        <location filename="../gui/src/forms/frmprogress.cpp" line="529"/>
        <source>No permission to write to the directory &apos;%1&apos;!</source>
        <translation>Brak uprawnień zapisu do katalogu &apos;%1&apos;!</translation>
    </message>
    <message>
        <location filename="../gui/src/forms/frmprogress.cpp" line="315"/>
        <source>Calculating checksum of the file...</source>
        <translation>Obliczanie sumy kontrolnej pliku...</translation>
    </message>
    <message numerus="yes">
        <location filename="../gui/src/forms/frmprogress.cpp" line="286"/>
        <source>Processing %n file(s), %1 at a time...</source>
        <translation>
            <numerusform>Przetwarzanie %n pliku, po %1 naraz...</numerusform>
            <numerusform>Przetwarzanie %n plików, po %1 naraz...</numerusform>
            <numerusform>Przetwarzanie %n plików, po %1 naraz...</numerusform>
        </translation>
    </message>
    <message>
        <location filename="../gui/src/forms/frmprogress.cpp" line="319"/>
        <source>Searching for subtitles [%1] (%2)...</source>
        <translation>Szukanie napisów [%1] (%2)...</translation>
    </message>
    <message>
        <location filename="../gui/src/forms/frmprogress.cpp" line="325"/>
        <source>Searching for subtitles in alternative language [%1] (%2)...</source>
        <translation>Szukanie napisów w języku zapasowym [%1] (%2)...</translation>
    </message>
    <message>
        <location filename="../gui/src/forms/frmprogress.cpp" line="331"/>
        <source>Nothing found, retrying in %1 s (%2/%3)...</source>
        <translation>Nic nie znaleziono, ponowna próba za %1 s (%2/%3)...</translation>
    </message>
    <message>
        <location filename="../gui/src/forms/frmprogress.cpp" line="338"/>
        <source>Downloading subtitles file...</source>
        <translation>Pobieranie napisów dla pliku...</translation>
    </message>
    <message>
        <location filename="../gui/src/forms/frmprogress.cpp" line="342"/>
        <source>Unpacking subtitles file...</source>
        <translation>Rozpakowywanie napisów...</translation>
    </message>
    <message>
        <location filename="../gui/src/forms/frmprogress.cpp" line="346"/>
        <source>Post-processing subtitles...</source>
        <translation>Przetwarzanie napisów...</translation>
    </message>
    <message>
        <location filename="../gui/src/forms/frmprogress.cpp" line="350"/>
        <source>Adjusting subtitles...</source>
        <translation>Dopasowywanie napisów...</translation>
    </message>
    <message numerus="yes">
        <location filename="../gui/src/forms/frmprogress.cpp" line="379"/>
        <source>%n file(s) were rate-limited; waiting %1 s before retrying...</source>
        <translation>
            <numerusform>%n plik objęty limitem zapytań; czekam %1 s przed ponowną próbą...</numerusform>
            <numerusform>%n pliki objęte limitem zapytań; czekam %1 s przed ponowną próbą...</numerusform>
            <numerusform>%n plików objętych limitem zapytań; czekam %1 s przed ponowną próbą...</numerusform>
        </translation>
    </message>
    <message numerus="yes">
        <location filename="../gui/src/forms/frmprogress.cpp" line="410"/>
        <source> (+%n more)</source>
        <translation>
            <numerusform> (+%n więcej)</numerusform>
            <numerusform> (+%n więcej)</numerusform>
            <numerusform> (+%n więcej)</numerusform>
        </translation>
    </message>
    <message>
        <location filename="../gui/src/forms/frmprogress.cpp" line="414"/>
        <source>%1 of %2 files done, %3 in progress</source>
        <translation>Gotowe: %1 z %2 plików, w toku: %3</translation>
    </message>
    <message numerus="yes">
        <location filename="../gui/src/forms/frmprogress.cpp" line="447"/>
        <source>Generating %1 subtitles for %n file(s)...</source>
        <translation>
            <numerusform>Generowanie napisów (%1) dla %n pliku...</numerusform>
            <numerusform>Generowanie napisów (%1) dla %n plików...</numerusform>
            <numerusform>Generowanie napisów (%1) dla %n plików...</numerusform>
        </translation>
    </message>
    <message>
        <location filename="../gui/src/forms/frmprogress.cpp" line="457"/>
        <source>Transcribing the audio (%1/%2)...</source>
        <translation>Transkrypcja dźwięku (%1/%2)...</translation>
    </message>
    <message>
        <location filename="../gui/src/forms/frmprogress.cpp" line="464"/>
        <source>Transcribing the audio (%1/%2): %3%</source>
        <translation>Transkrypcja dźwięku (%1/%2): %3%</translation>
    </message>
    <message>
        <location filename="../gui/src/forms/frmprogress.cpp" line="479"/>
        <source>Loading the translation model...</source>
        <translation>Wczytywanie modelu tłumaczącego...</translation>
    </message>
    <message>
        <location filename="../gui/src/forms/frmprogress.cpp" line="490"/>
        <source>Translating (%1/%2)...</source>
        <translation>Tłumaczenie (%1/%2)...</translation>
    </message>
    <message>
        <location filename="../gui/src/forms/frmprogress.cpp" line="495"/>
        <source>Translating (%1/%2): line %3 of %4</source>
        <translation>Tłumaczenie (%1/%2): linia %3 z %4</translation>
    </message>
    <message>
        <location filename="../gui/src/forms/frmprogress.cpp" line="534"/>
        <source>still rate-limited, try again later</source>
        <translation>nadal limit zapytań, spróbuj później</translation>
    </message>
    <message>
        <location filename="../gui/src/forms/frmprogress.cpp" line="537"/>
        <source>Could not adjust subtitles!</source>
        <translation>Nie udało się dopasować napisów!</translation>
    </message>
    <message>
        <location filename="../gui/src/forms/frmprogress.cpp" line="591"/>
        <source>AI subtitles could not be generated, these files were not found:</source>
        <translation>Nie udało się wygenerować napisów AI, nie znaleziono tych plików:</translation>
    </message>
    <message>
        <location filename="../gui/src/forms/frmprogress.cpp" line="594"/>
        <source>Check the paths on the AI subtitles tab in Settings.</source>
        <translation>Sprawdź ścieżki na karcie Napisy AI w ustawieniach.</translation>
    </message>
    <message>
        <location filename="../gui/src/forms/frmprogress.cpp" line="596"/>
        <source>The translation model could not be started:</source>
        <translation>Nie udało się uruchomić modelu tłumaczącego:</translation>
    </message>
    <message>
        <location filename="../gui/src/forms/frmprogress.cpp" line="614"/>
        <source>(generated by AI)</source>
        <translation>(wygenerowane przez AI)</translation>
    </message>
    <message>
        <location filename="../gui/src/forms/frmprogress.cpp" line="618"/>
        <source>AI: %1</source>
        <translation>AI: %1</translation>
    </message>
</context>
<context>
    <name>GuiMain</name>
    <message>
        <location filename="../gui/src/guimain.cpp" line="101"/>
        <source>List of languages recognized by QNapi, including corresponding</source>
        <translation>Oto lista rozpoznawanych przez QNapi jezykow i odpowiadajacym</translation>
    </message>
    <message>
        <location filename="../gui/src/guimain.cpp" line="103"/>
        <source>two-letter language codes:</source>
        <translation>im dwuliterowych kodow:</translation>
    </message>
    <message>
        <location filename="../gui/src/guimain.cpp" line="118"/>
        <source>Current default subtitles language: %1 (%2)</source>
        <translation>Aktualnie ustawiony preferowany jezyk pobierania napisow: %1 (%2)</translation>
    </message>
    <message>
        <location filename="../gui/src/guimain.cpp" line="123"/>
        <source>No alternative subtitles language has been set</source>
        <translation>Brak ustawionego zapasowego jezyka napisow</translation>
    </message>
    <message>
        <location filename="../gui/src/guimain.cpp" line="125"/>
        <source>Current alternative subtitles language: %1 (%2)</source>
        <translation>Aktualnie ustawiony zapasowy jezyk napisow: %1 (%2)</translation>
    </message>
    <message>
        <location filename="../gui/src/guimain.cpp" line="134"/>
        <source>QNapi - command line argument parsing error</source>
        <translation>QNapi - błąd parsowania argumentu wiersza poleceń</translation>
    </message>
    <message>
        <location filename="../gui/src/guimain.cpp" line="175"/>
        <source>QNapi - CLI executable not found</source>
        <translation>QNapi - nie znaleziono wersji dla wiersza poleceń</translation>
    </message>
    <message>
        <location filename="../gui/src/guimain.cpp" line="176"/>
        <source>You have requested to run QNapi in command-line mode, but unfortunately command-line binary &apos;%1&apos; could not be found.</source>
        <translation>Zażądano uruchomienia QNapi w trybie wiersza poleceń, jednak nie znaleziono &apos;%1&apos;.</translation>
    </message>
</context>
<context>
    <name>HelpHelper</name>
    <message>
        <location filename="../libqnapi/src/utils/helphelper.cpp" line="29"/>
        <source>QNapi is distributed under the GNU General Public License v2.</source>
        <translation>QNapi rozprowadzany jest na warunkach licencji GNU General Public License v2.</translation>
    </message>
    <message>
        <location filename="../libqnapi/src/utils/helphelper.cpp" line="33"/>
        <source>Syntax: %1 [options] [list of files]</source>
        <translation>Skladnia: %1 [opcje] [lista plikow]</translation>
    </message>
    <message>
        <location filename="../libqnapi/src/utils/helphelper.cpp" line="34"/>
        <source>Available options:</source>
        <translation>Dostepne opcje:</translation>
    </message>
</context>
<context>
    <name>JobsArgParser</name>
    <message>
        <location filename="../libqnapi/src/parser/jobsargparser.cpp" line="31"/>
        <source>You need to pass the number of parallel downloads after %1 argument</source>
        <translation>Po argumencie %1 musisz podać liczbę równoczesnych pobrań</translation>
    </message>
    <message>
        <location filename="../libqnapi/src/parser/jobsargparser.cpp" line="41"/>
        <source>Invalid number of parallel downloads passed: %1 (expected 1 to %2)</source>
        <translation>Nieprawidłowa liczba równoczesnych pobrań: %1 (oczekiwano od 1 do %2)</translation>
    </message>
    <message>
        <location filename="../libqnapi/src/parser/jobsargparser.cpp" line="52"/>
        <source>count</source>
        <translation>liczba</translation>
    </message>
    <message>
        <location filename="../libqnapi/src/parser/jobsargparser.cpp" line="53"/>
        <source>Number of files to process in parallel (1 to %1, default %2). Subtitle services throttle requests that arrive one after another, so processing several files at once is dramatically faster</source>
        <translation>Liczba plików przetwarzanych równolegle (od 1 do %1, domyślnie %2). Serwisy z napisami ograniczają zapytania wysyłane jedno po drugim, więc przetwarzanie kilku plików naraz jest znacznie szybsze</translation>
    </message>
</context>
<context>
    <name>LangArgParser</name>
    <message>
        <location filename="../libqnapi/src/parser/langargparser.cpp" line="32"/>
        <source>You need to pass language code after %1 argument</source>
        <translation>Musisz podac kod jezyka po argumencie %1</translation>
    </message>
    <message>
        <location filename="../libqnapi/src/parser/langargparser.cpp" line="41"/>
        <source>Invalid language code passed: %1</source>
        <translation>Niepoprawny kod jezykowy: %1</translation>
    </message>
    <message>
        <location filename="../libqnapi/src/parser/langargparser.cpp" line="50"/>
        <source>code</source>
        <translation>kod</translation>
    </message>
    <message>
        <location filename="../libqnapi/src/parser/langargparser.cpp" line="50"/>
        <source>Preferred subtitles language</source>
        <translation>Preferowany język napisów</translation>
    </message>
</context>
<context>
    <name>LangSuffixArgParser</name>
    <message>
        <location filename="../libqnapi/src/parser/langsuffixargparser.cpp" line="31"/>
        <source>You need to pass the language suffix mode after %1 argument</source>
        <translation>Po argumencie %1 musisz podać tryb kodu języka</translation>
    </message>
    <message>
        <location filename="../libqnapi/src/parser/langsuffixargparser.cpp" line="47"/>
        <source>Invalid language suffix mode passed: %1 (expected none, 2 or 3)</source>
        <translation>Nieprawidłowy tryb kodu języka: %1 (oczekiwano none, 2 lub 3)</translation>
    </message>
    <message>
        <location filename="../libqnapi/src/parser/langsuffixargparser.cpp" line="57"/>
        <source>mode</source>
        <translation>tryb</translation>
    </message>
    <message>
        <location filename="../libqnapi/src/parser/langsuffixargparser.cpp" line="58"/>
        <source>Append the subtitles language to the file name, as media servers expect (none, 2 for two-letter e.g. movie.pl.srt, 3 for three-letter e.g. movie.pol.srt)</source>
        <translation>Dodaj język napisów do nazwy pliku, jak oczekują serwery multimediów (none; 2 - dwuliterowy, np. film.pl.srt; 3 - trzyliterowy, np. film.pol.srt)</translation>
    </message>
</context>
<context>
    <name>MaiaObject</name>
    <message>
        <location filename="../deps/libmaia/maia/maiaObject.cpp" line="314"/>
        <source>parse error: invalid xml-rpc. not conforming to spec.</source>
        <translation>błąd parsowania: niepoprawny xml-rpc. niezgodny ze specyfikacją.</translation>
    </message>
</context>
<context>
    <name>QNapiApp</name>
    <message>
        <location filename="../gui/src/qnapiapp.cpp" line="100"/>
        <location filename="../gui/src/qnapiapp.cpp" line="111"/>
        <location filename="../gui/src/qnapiapp.cpp" line="119"/>
        <location filename="../gui/src/qnapiapp.cpp" line="130"/>
        <source>Download subtitles</source>
        <translation>Pobierz napisy</translation>
    </message>
    <message>
        <location filename="../gui/src/qnapiapp.cpp" line="104"/>
        <source>Scan directories</source>
        <translation>Skanuj katalogi</translation>
    </message>
    <message>
        <location filename="../gui/src/qnapiapp.cpp" line="108"/>
        <source>Convert subtitles</source>
        <translation>Konwertuj napisy</translation>
    </message>
    <message>
        <location filename="../gui/src/qnapiapp.cpp" line="115"/>
        <location filename="../gui/src/qnapiapp.cpp" line="126"/>
        <location filename="../gui/src/qnapiapp.cpp" line="134"/>
        <source>Create an account</source>
        <translation>Załóż konto</translation>
    </message>
    <message>
        <location filename="../gui/src/qnapiapp.cpp" line="123"/>
        <source>Upload subtitles</source>
        <translation>Wyślij napisy</translation>
    </message>
    <message>
        <location filename="../gui/src/qnapiapp.cpp" line="138"/>
        <source>Settings</source>
        <translation>Ustawienia</translation>
    </message>
    <message>
        <location filename="../gui/src/qnapiapp.cpp" line="141"/>
        <source>About</source>
        <translation>O programie</translation>
    </message>
    <message>
        <location filename="../gui/src/qnapiapp.cpp" line="144"/>
        <source>Quit</source>
        <translation>Zakończ</translation>
    </message>
    <message>
        <location filename="../gui/src/qnapiapp.cpp" line="148"/>
        <source>NapiProjekt</source>
        <translation>NapiProjekt</translation>
    </message>
    <message>
        <location filename="../gui/src/qnapiapp.cpp" line="153"/>
        <source>OpenSubtitles</source>
        <translation>OpenSubtitles</translation>
    </message>
    <message>
        <location filename="../gui/src/qnapiapp.cpp" line="159"/>
        <source>Napisy24</source>
        <translation>Napisy24</translation>
    </message>
    <message>
        <location filename="../gui/src/qnapiapp.cpp" line="203"/>
        <source>Select one or more video files to download subtitles for</source>
        <translation>Wybierz jeden lub więcej plików z filmami</translation>
    </message>
</context>
<context>
    <name>QNapiOpenDialog</name>
    <message>
        <location filename="../gui/src/qnapiopendialog.cpp" line="26"/>
        <source>Video files (%1);;All files (*.*)</source>
        <translation>Filmy (%1);;Wszystkie pliki (*.*)</translation>
    </message>
    <message>
        <location filename="../gui/src/qnapiopendialog.cpp" line="29"/>
        <source>Subtitles files (%1);;All files (*.*)</source>
        <translation>Napisy (%1);;Wszystkie pliki (*.*)</translation>
    </message>
</context>
<context>
    <name>QObject</name>
    <message>
        <location filename="../libqnapi/src/engines/napiprojektdownloadengine.cpp" line="66"/>
        <source>&lt;b&gt;www.napiprojekt.pl&lt;/b&gt; subtitles download engine</source>
        <translation>Silnik pobierania napisów z serwisu &lt;b&gt;www.napiprojekt.pl&lt;/b&gt;</translation>
    </message>
    <message>
        <location filename="../libqnapi/src/engines/napiprojektdownloadengine.cpp" line="160"/>
        <location filename="../libqnapi/src/engines/napiprojektdownloadengine.cpp" line="227"/>
        <source>NapiProjekt did not respond in time.</source>
        <translation>NapiProjekt nie odpowiedział na czas.</translation>
    </message>
    <message>
        <location filename="../libqnapi/src/engines/napiprojektdownloadengine.cpp" line="161"/>
        <location filename="../libqnapi/src/engines/napiprojektdownloadengine.cpp" line="228"/>
        <source>Network error while contacting NapiProjekt: %1</source>
        <translation>Błąd sieci podczas łączenia z NapiProjektem: %1</translation>
    </message>
    <message>
        <location filename="../libqnapi/src/engines/napiprojektdownloadengine.cpp" line="243"/>
        <source>NapiProjekt is rate-limiting requests; try again in a few seconds.</source>
        <translation>NapiProjekt ogranicza liczbę zapytań; spróbuj ponownie za kilka sekund.</translation>
    </message>
    <message>
        <location filename="../libqnapi/src/engines/napisy24downloadengine.cpp" line="26"/>
        <source>&lt;b&gt;www.napisy24.pl&lt;/b&gt; subtitle download engine</source>
        <translation>Silnik pobierania napisów z serwisu &lt;b&gt;www.napisy24.pl&lt;/b&gt;</translation>
    </message>
    <message>
        <location filename="../libqnapi/src/engines/napisy24downloadengine.cpp" line="103"/>
        <source>Napisy24 did not respond in time.</source>
        <translation>Napisy24 nie odpowiedziały na czas.</translation>
    </message>
    <message>
        <location filename="../libqnapi/src/engines/napisy24downloadengine.cpp" line="104"/>
        <source>Network error while contacting Napisy24: %1</source>
        <translation>Błąd sieci podczas łączenia z Napisy24: %1</translation>
    </message>
    <message>
        <location filename="../libqnapi/src/engines/opensubtitlesdownloadengine.cpp" line="29"/>
        <source>&lt;b&gt;www.opensubtitles.org&lt;/b&gt; subtitles download engine</source>
        <translation>Silnik pobierania napisów z serwisu &lt;b&gt;www.opensubtitles.org&lt;/b&gt;</translation>
    </message>
    <message>
        <location filename="../libqnapi/src/engines/opensubtitlesdownloadengine.cpp" line="229"/>
        <source>OpenSubtitles returned a placeholder instead of subtitles because the session is anonymous. Enter your opensubtitles.org account in Options -&gt; OpenSubtitles.</source>
        <translation>OpenSubtitles zwrócił zastępczy plik zamiast napisów, ponieważ sesja jest anonimowa. Wpisz dane konta opensubtitles.org w Opcje -&gt; OpenSubtitles.</translation>
    </message>
    <message>
        <location filename="../libqnapi/src/engines/opensubtitlesdownloadengine.cpp" line="233"/>
        <source>OpenSubtitles returned a placeholder instead of subtitles for account &apos;%1&apos;. The account is logged in but is not entitled to download this subtitle (VIP membership or a higher download quota may be required).</source>
        <translation>OpenSubtitles zwrócił zastępczy plik zamiast napisów dla konta &apos;%1&apos;. Konto jest zalogowane, ale nie ma uprawnień do pobrania tych napisów (może być potrzebne konto VIP lub większy limit pobrań).</translation>
    </message>
    <message>
        <location filename="../libqnapi/src/engines/opensubtitlesdownloadengine.cpp" line="263"/>
        <source>OpenSubtitles did not respond in time.</source>
        <translation>OpenSubtitles nie odpowiedział na czas.</translation>
    </message>
    <message>
        <location filename="../libqnapi/src/engines/opensubtitlesdownloadengine.cpp" line="264"/>
        <source>Could not reach OpenSubtitles (%1).</source>
        <translation>Nie można połączyć się z OpenSubtitles (%1).</translation>
    </message>
    <message>
        <location filename="../libqnapi/src/engines/opensubtitlesdownloadengine.cpp" line="277"/>
        <source>OpenSubtitles refused an anonymous session (%1).</source>
        <translation>OpenSubtitles odrzucił anonimową sesję (%1).</translation>
    </message>
    <message>
        <location filename="../libqnapi/src/engines/opensubtitlesdownloadengine.cpp" line="279"/>
        <source>OpenSubtitles rejected the account &apos;%1&apos; (%2). Check the user name and password in Options.</source>
        <translation>OpenSubtitles odrzucił konto &apos;%1&apos; (%2). Sprawdź nazwę użytkownika i hasło w opcjach.</translation>
    </message>
    <message>
        <location filename="../gui/src/main.cpp" line="92"/>
        <source>First Run</source>
        <translation>Pierwsze uruchomienie</translation>
    </message>
    <message>
        <location filename="../gui/src/main.cpp" line="93"/>
        <source>This is the first launch of the program QNapi. Do you want to configure it now?</source>
        <translation>To jest pierwsze uruchomienie programu QNapi. Czy chcesz przeprowadzić konfigurację teraz?</translation>
    </message>
    <message>
        <location filename="../gui/src/guimain.cpp" line="218"/>
        <source>QNapi: deleting temporary files...</source>
        <translation>QNapi: usuwanie plików tymczasowych...</translation>
    </message>
    <message>
        <location filename="../gui/src/guimain.cpp" line="233"/>
        <source>QNapi: finished.</source>
        <translation>QNapi: zakończono.</translation>
    </message>
    <message>
        <location filename="../libqnapi/src/qnapi.cpp" line="111"/>
        <source>No subtitles found!</source>
        <translation>Nie znaleziono napisów!</translation>
    </message>
    <message>
        <location filename="../libqnapi/src/subconvert/subtitleconverter.cpp" line="164"/>
        <source>Subtitles downloaded and processed by QNapi|</source>
        <translation>Napisy pobrane i przetworzone programem QNapi|</translation>
    </message>
    <message>
        <location filename="../libqnapi/src/subtitlelanguage.cpp" line="63"/>
        <source>Albanian</source>
        <translation>Albański</translation>
    </message>
    <message>
        <location filename="../libqnapi/src/subtitlelanguage.cpp" line="67"/>
        <source>English</source>
        <translation>Angielski</translation>
    </message>
    <message>
        <location filename="../libqnapi/src/subtitlelanguage.cpp" line="71"/>
        <source>Arabic</source>
        <translation>Arabski</translation>
    </message>
    <message>
        <location filename="../libqnapi/src/subtitlelanguage.cpp" line="75"/>
        <source>Bulgarian</source>
        <translation>Bułgarski</translation>
    </message>
    <message>
        <location filename="../libqnapi/src/subtitlelanguage.cpp" line="79"/>
        <source>Chinese</source>
        <translation>Chiński</translation>
    </message>
    <message>
        <location filename="../libqnapi/src/subtitlelanguage.cpp" line="83"/>
        <source>Croatian</source>
        <translation>Chorwacki</translation>
    </message>
    <message>
        <location filename="../libqnapi/src/subtitlelanguage.cpp" line="87"/>
        <source>Czech</source>
        <translation>Czeski</translation>
    </message>
    <message>
        <location filename="../libqnapi/src/subtitlelanguage.cpp" line="91"/>
        <source>Danish</source>
        <translation>Duński</translation>
    </message>
    <message>
        <location filename="../libqnapi/src/subtitlelanguage.cpp" line="95"/>
        <source>Estonian</source>
        <translation>Estoński</translation>
    </message>
    <message>
        <location filename="../libqnapi/src/subtitlelanguage.cpp" line="99"/>
        <source>Finnish</source>
        <translation>Fiński</translation>
    </message>
    <message>
        <location filename="../libqnapi/src/subtitlelanguage.cpp" line="103"/>
        <source>French</source>
        <translation>Francuski</translation>
    </message>
    <message>
        <location filename="../libqnapi/src/subtitlelanguage.cpp" line="107"/>
        <source>Galician</source>
        <translation>Galicyjski</translation>
    </message>
    <message>
        <location filename="../libqnapi/src/subtitlelanguage.cpp" line="111"/>
        <source>Greek</source>
        <translation>Grecki</translation>
    </message>
    <message>
        <location filename="../libqnapi/src/subtitlelanguage.cpp" line="115"/>
        <source>Hebrew</source>
        <translation>Hebrajski</translation>
    </message>
    <message>
        <location filename="../libqnapi/src/subtitlelanguage.cpp" line="119"/>
        <source>Spanish</source>
        <translation>Hiszpanski</translation>
    </message>
    <message>
        <location filename="../libqnapi/src/subtitlelanguage.cpp" line="123"/>
        <source>Dutch</source>
        <translation>Holenderski</translation>
    </message>
    <message>
        <location filename="../libqnapi/src/subtitlelanguage.cpp" line="127"/>
        <source>Indonesian</source>
        <translation>Indonezyjski</translation>
    </message>
    <message>
        <location filename="../libqnapi/src/subtitlelanguage.cpp" line="131"/>
        <source>Japanese</source>
        <translation>Japoński</translation>
    </message>
    <message>
        <location filename="../libqnapi/src/subtitlelanguage.cpp" line="135"/>
        <source>Korean</source>
        <translation>Koreański</translation>
    </message>
    <message>
        <location filename="../libqnapi/src/subtitlelanguage.cpp" line="139"/>
        <source>Macedonian</source>
        <translation>Macedoński</translation>
    </message>
    <message>
        <location filename="../libqnapi/src/subtitlelanguage.cpp" line="143"/>
        <source>German</source>
        <translation>Niemiecki</translation>
    </message>
    <message>
        <location filename="../libqnapi/src/subtitlelanguage.cpp" line="147"/>
        <source>Norwegian</source>
        <translation>Norweski</translation>
    </message>
    <message>
        <location filename="../libqnapi/src/subtitlelanguage.cpp" line="151"/>
        <source>Occitan</source>
        <translation>Oksytański</translation>
    </message>
    <message>
        <location filename="../libqnapi/src/subtitlelanguage.cpp" line="155"/>
        <source>Persian (farsi)</source>
        <translation>Perski (farsi)</translation>
    </message>
    <message>
        <location filename="../libqnapi/src/subtitlelanguage.cpp" line="159"/>
        <source>Polish</source>
        <translation>Polski</translation>
    </message>
    <message>
        <location filename="../libqnapi/src/subtitlelanguage.cpp" line="163"/>
        <source>Portuguese</source>
        <translation>Portugalski</translation>
    </message>
    <message>
        <location filename="../libqnapi/src/subtitlelanguage.cpp" line="167"/>
        <source>Portuguese-BR</source>
        <translation>Portugalski-BR</translation>
    </message>
    <message>
        <location filename="../libqnapi/src/subtitlelanguage.cpp" line="171"/>
        <source>Russian</source>
        <translation>Rosyjski</translation>
    </message>
    <message>
        <location filename="../libqnapi/src/subtitlelanguage.cpp" line="175"/>
        <source>Romanian</source>
        <translation>Rumuński</translation>
    </message>
    <message>
        <location filename="../libqnapi/src/subtitlelanguage.cpp" line="179"/>
        <source>Serbian</source>
        <translation>Serbski</translation>
    </message>
    <message>
        <location filename="../libqnapi/src/subtitlelanguage.cpp" line="183"/>
        <source>Slovenian</source>
        <translation>Słoweński</translation>
    </message>
    <message>
        <location filename="../libqnapi/src/subtitlelanguage.cpp" line="187"/>
        <source>Swedish</source>
        <translation>Szwedzki</translation>
    </message>
    <message>
        <location filename="../libqnapi/src/subtitlelanguage.cpp" line="191"/>
        <source>Slovak</source>
        <translation>Słowacki</translation>
    </message>
    <message>
        <location filename="../libqnapi/src/subtitlelanguage.cpp" line="195"/>
        <source>Turkish</source>
        <translation>Turecki</translation>
    </message>
    <message>
        <location filename="../libqnapi/src/subtitlelanguage.cpp" line="199"/>
        <source>Vietnamese</source>
        <translation>Wietnamski</translation>
    </message>
    <message>
        <location filename="../libqnapi/src/subtitlelanguage.cpp" line="203"/>
        <source>Hungarian</source>
        <translation>Węgierski</translation>
    </message>
    <message>
        <location filename="../libqnapi/src/subtitlelanguage.cpp" line="207"/>
        <source>Italian</source>
        <translation>Włoski</translation>
    </message>
    <message>
        <location filename="../libqnapi/src/ai/aierror.h" line="33"/>
        <source>cancelled</source>
        <translation>anulowano</translation>
    </message>
    <message>
        <location filename="../libqnapi/src/ai/aisubtitlegenerator.cpp" line="311"/>
        <source>cannot write temporary file %1</source>
        <translation>nie można zapisać pliku tymczasowego %1</translation>
    </message>
    <message>
        <location filename="../libqnapi/src/ai/aisubtitlegenerator.cpp" line="328"/>
        <source>could not save the subtitles next to the movie</source>
        <translation>nie udało się zapisać napisów obok filmu</translation>
    </message>
    <message>
        <location filename="../libqnapi/src/ai/audioextractor.cpp" line="59"/>
        <source>Could not start ffmpeg: %1</source>
        <translation>Nie można uruchomić ffmpeg: %1</translation>
    </message>
    <message>
        <location filename="../libqnapi/src/ai/audioextractor.cpp" line="69"/>
        <source>ffmpeg timed out extracting audio</source>
        <translation>ffmpeg przekroczył limit czasu podczas wyodrębniania dźwięku</translation>
    </message>
    <message>
        <location filename="../libqnapi/src/ai/audioextractor.cpp" line="77"/>
        <source>ffmpeg could not extract audio (%1)</source>
        <translation>ffmpeg nie mógł wyodrębnić dźwięku (%1)</translation>
    </message>
    <message>
        <location filename="../libqnapi/src/ai/llamaserver.cpp" line="65"/>
        <source>No free local port for llama-server</source>
        <translation>Brak wolnego portu lokalnego dla llama-server</translation>
    </message>
    <message>
        <location filename="../libqnapi/src/ai/llamaserver.cpp" line="92"/>
        <source>Could not start llama-server: %1</source>
        <translation>Nie można uruchomić llama-server: %1</translation>
    </message>
    <message>
        <location filename="../libqnapi/src/ai/llamaserver.cpp" line="109"/>
        <source>llama-server exited while loading the model (%1)</source>
        <translation>llama-server zakończył działanie podczas wczytywania modelu (%1)</translation>
    </message>
    <message>
        <location filename="../libqnapi/src/ai/llamaserver.cpp" line="125"/>
        <source>llama-server did not become ready in time</source>
        <translation>llama-server nie był gotowy na czas</translation>
    </message>
    <message>
        <location filename="../libqnapi/src/ai/llamaserver.cpp" line="147"/>
        <source>llama-server is not running (%1)</source>
        <translation>llama-server nie działa (%1)</translation>
    </message>
    <message>
        <location filename="../libqnapi/src/ai/llamaserver.cpp" line="167"/>
        <source>llama-server did not answer in time</source>
        <translation>llama-server nie odpowiedział na czas</translation>
    </message>
    <message>
        <location filename="../libqnapi/src/ai/llamaserver.cpp" line="170"/>
        <source>llama-server request failed (%1 %2)</source>
        <translation>Zapytanie do llama-server nie powiodło się (%1 %2)</translation>
    </message>
    <message>
        <location filename="../libqnapi/src/ai/llamaserver.cpp" line="179"/>
        <source>llama-server returned no answer</source>
        <translation>llama-server nie zwrócił odpowiedzi</translation>
    </message>
    <message>
        <location filename="../libqnapi/src/ai/subtitletranslator.cpp" line="187"/>
        <source>the model&apos;s answer is not valid JSON</source>
        <translation>odpowiedź modelu nie jest poprawnym JSON-em</translation>
    </message>
    <message>
        <location filename="../libqnapi/src/ai/whispertranscriber.cpp" line="240"/>
        <source>Could not start whisper: %1</source>
        <translation>Nie można uruchomić whisper: %1</translation>
    </message>
    <message>
        <location filename="../libqnapi/src/ai/whispertranscriber.cpp" line="260"/>
        <source>whisper timed out</source>
        <translation>whisper przekroczył limit czasu</translation>
    </message>
    <message>
        <location filename="../libqnapi/src/ai/whispertranscriber.cpp" line="282"/>
        <source>whisper failed (%1)</source>
        <translation>whisper zakończył się błędem (%1)</translation>
    </message>
    <message>
        <location filename="../libqnapi/src/ai/whispertranscriber.cpp" line="287"/>
        <source>whisper produced no output file</source>
        <translation>whisper nie utworzył pliku wynikowego</translation>
    </message>
    <message>
        <location filename="../libqnapi/src/ai/whispertranscriber.cpp" line="295"/>
        <source>cannot read whisper&apos;s output (%1)</source>
        <translation>nie można odczytać wyniku whisper (%1)</translation>
    </message>
    <message>
        <location filename="../libqnapi/src/ai/whispertranscriber.cpp" line="301"/>
        <source>whisper found no speech in the audio</source>
        <translation>whisper nie znalazł mowy w nagraniu</translation>
    </message>
    <message>
        <location filename="../libqnapi/src/engines/subtitledownloadengine.cpp" line="72"/>
        <source>Could not open the video file &apos;%1&apos;: %2</source>
        <translation>Nie można otworzyć pliku wideo &apos;%1&apos;: %2</translation>
    </message>
</context>
<context>
    <name>QuietBatchArgParser</name>
    <message>
        <location filename="../libqnapi/src/parser/quietbatchargparser.cpp" line="33"/>
        <source>Download subtitles silently without showing any messages</source>
        <translation>Pobierz napisy nie wypisujac zadnych komunikatow (implikuje -d)</translation>
    </message>
</context>
<context>
    <name>RunCLIAppArgParser</name>
    <message>
        <location filename="../libqnapi/src/parser/runcliappargparser.cpp" line="37"/>
        <source>Run in command-line mode (requires separate CLI binary)</source>
        <translation>Uruchom w trybie wiersza poleceń (wymaga osobnego pliku wykonywalnego)</translation>
    </message>
</context>
<context>
    <name>ShowHelpArgParser</name>
    <message>
        <location filename="../libqnapi/src/parser/showhelpargparser.cpp" line="31"/>
        <source>Show help text</source>
        <translation>Pokaż tekst pomocy</translation>
    </message>
</context>
<context>
    <name>ShowHelpLanguagesArgParser</name>
    <message>
        <location filename="../libqnapi/src/parser/showhelplanguagesargparser.cpp" line="32"/>
        <source>List of available subtitles languages</source>
        <translation>Pokaż dostępne języki pobierania napisów</translation>
    </message>
</context>
<context>
    <name>ShowListArgParser</name>
    <message>
        <location filename="../libqnapi/src/parser/showlistargparser.cpp" line="33"/>
        <source>Always show a list of available subtitles</source>
        <translation>Zawsze pokazuj listę dostępnych napisów</translation>
    </message>
</context>
<context>
    <name>SubtitleMatcher</name>
    <message>
        <location filename="../libqnapi/src/subtitlematcher.cpp" line="128"/>
        <source>_copy</source>
        <translation>_kopia</translation>
    </message>
</context>
<context>
    <name>frmAbout</name>
    <message>
        <location filename="../gui/ui/frmabout.ui" line="20"/>
        <source>QNapi - About</source>
        <translation>QNapi - O programie</translation>
    </message>
    <message>
        <location filename="../gui/ui/frmabout.ui" line="64"/>
        <source>&lt;html&gt;&lt;head/&gt;&lt;body&gt;&lt;p&gt;QNapi is a program for automatic downloading and matching subtitles for movies. &lt;/p&gt;&lt;p&gt;The program is distributed under the GNU General Public License version second or later.&lt;/p&gt;&lt;p&gt;&lt;a href=&quot;http://qnapi.github.io&quot;&gt;&lt;span style=&quot; text-decoration: underline; color:#0000ff;&quot;&gt;http://qnapi.github.io&lt;/span&gt;&lt;/a&gt;&lt;/p&gt;&lt;p&gt;I thank all those who in any way supported the development of the project.&lt;/p&gt;&lt;/body&gt;&lt;/html&gt;</source>
        <translation>&lt;html&gt;&lt;head/&gt;&lt;body&gt;&lt;p&gt;QNapi jest programem do automatycznego pobierania i dopasowywania napisów do filmów. &lt;/p&gt;&lt;p&gt;Program rozprowadzany jest na licencji GNU General Public License w wersji drugiej lub późniejszej.&lt;/p&gt;&lt;p&gt;&lt;a href=&quot;http://qnapi.github.io&quot;&gt;&lt;span style=&quot; text-decoration: underline; color:#0000ff;&quot;&gt;http://qnapi.github.io&lt;/span&gt;&lt;/a&gt;&lt;/p&gt;&lt;p&gt;Dziękuję wszystkim osobom, które w jakikolwiek sposób wsparły rozwój projektu.&lt;/p&gt;&lt;/body&gt;&lt;/html&gt;</translation>
    </message>
    <message>
        <location filename="../gui/ui/frmabout.ui" line="95"/>
        <source>Qt version: x.y.z</source>
        <translation>Wersja Qt: x.y.z</translation>
    </message>
    <message>
        <location filename="../gui/ui/frmabout.ui" line="118"/>
        <source>The program uses www.napiprojekt.pl subtitle database</source>
        <translation>Program korzysta z bazy napisów www.napiprojekt.pl</translation>
    </message>
    <message>
        <location filename="../gui/ui/frmabout.ui" line="147"/>
        <source>The program uses www.opensubtitles.org subtitle database</source>
        <translation>Program korzysta z bazy napisów www.opensubtitles.org</translation>
    </message>
    <message>
        <location filename="../gui/ui/frmabout.ui" line="173"/>
        <source>The program uses www.napisy24.pl subtitle database</source>
        <translation>Program korzysta z bazy napisów www.napisy24.pl</translation>
    </message>
    <message>
        <location filename="../gui/ui/frmabout.ui" line="224"/>
        <source>QNapi version</source>
        <translation>Wersja QNapi</translation>
    </message>
</context>
<context>
    <name>frmConvert</name>
    <message>
        <location filename="../gui/ui/frmconvert.ui" line="14"/>
        <source>QNapi - Convert subtitles</source>
        <translation>QNapi - Konwertuj napisy</translation>
    </message>
    <message>
        <location filename="../gui/ui/frmconvert.ui" line="24"/>
        <source>Source subtitles file:</source>
        <translation>Źródłowy plik z napisami:</translation>
    </message>
    <message>
        <location filename="../gui/ui/frmconvert.ui" line="66"/>
        <source>Detected format:</source>
        <translation>Wykryto format:</translation>
    </message>
    <message>
        <location filename="../gui/ui/frmconvert.ui" line="109"/>
        <source>Target subtitles format:</source>
        <translation>Docelowy format napisów:</translation>
    </message>
    <message>
        <location filename="../gui/ui/frmconvert.ui" line="178"/>
        <source>Video framerate (FPS):</source>
        <translation>Liczba klatek na sekundę filmu (FPS):</translation>
    </message>
    <message>
        <location filename="../gui/ui/frmconvert.ui" line="216"/>
        <location filename="../gui/ui/frmconvert.ui" line="220"/>
        <location filename="../gui/ui/frmconvert.ui" line="395"/>
        <location filename="../gui/ui/frmconvert.ui" line="399"/>
        <location filename="../gui/ui/frmconvert.ui" line="468"/>
        <source>23.976</source>
        <translation>23,976</translation>
    </message>
    <message>
        <location filename="../gui/ui/frmconvert.ui" line="225"/>
        <location filename="../gui/ui/frmconvert.ui" line="404"/>
        <location filename="../gui/ui/frmconvert.ui" line="473"/>
        <source>24</source>
        <translation>24</translation>
    </message>
    <message>
        <location filename="../gui/ui/frmconvert.ui" line="230"/>
        <location filename="../gui/ui/frmconvert.ui" line="409"/>
        <location filename="../gui/ui/frmconvert.ui" line="478"/>
        <source>25</source>
        <translation>25</translation>
    </message>
    <message>
        <location filename="../gui/ui/frmconvert.ui" line="235"/>
        <location filename="../gui/ui/frmconvert.ui" line="414"/>
        <location filename="../gui/ui/frmconvert.ui" line="483"/>
        <source>29.97</source>
        <translation>29,97</translation>
    </message>
    <message>
        <location filename="../gui/ui/frmconvert.ui" line="240"/>
        <location filename="../gui/ui/frmconvert.ui" line="419"/>
        <location filename="../gui/ui/frmconvert.ui" line="488"/>
        <source>30</source>
        <translation>30</translation>
    </message>
    <message>
        <location filename="../gui/ui/frmconvert.ui" line="245"/>
        <location filename="../gui/ui/frmconvert.ui" line="424"/>
        <location filename="../gui/ui/frmconvert.ui" line="493"/>
        <source>50</source>
        <translation>50</translation>
    </message>
    <message>
        <location filename="../gui/ui/frmconvert.ui" line="250"/>
        <location filename="../gui/ui/frmconvert.ui" line="429"/>
        <location filename="../gui/ui/frmconvert.ui" line="498"/>
        <source>59.94</source>
        <translation>59.94</translation>
    </message>
    <message>
        <location filename="../gui/ui/frmconvert.ui" line="255"/>
        <location filename="../gui/ui/frmconvert.ui" line="434"/>
        <location filename="../gui/ui/frmconvert.ui" line="503"/>
        <source>60</source>
        <translation>60</translation>
    </message>
    <message>
        <location filename="../gui/ui/frmconvert.ui" line="268"/>
        <location filename="../gui/ui/frmconvert.ui" line="516"/>
        <source>Detect video framerate (FPS) from file</source>
        <translation>Wykryj docelowy FPS z pliku</translation>
    </message>
    <message>
        <location filename="../gui/ui/frmconvert.ui" line="287"/>
        <source>File extension for target subtitles file:</source>
        <translation>Docelowe rozszerzenie napisów:</translation>
    </message>
    <message>
        <location filename="../gui/ui/frmconvert.ui" line="323"/>
        <source>Default</source>
        <translation>Domyślne</translation>
    </message>
    <message>
        <location filename="../gui/ui/frmconvert.ui" line="328"/>
        <source>srt</source>
        <translation>srt</translation>
    </message>
    <message>
        <location filename="../gui/ui/frmconvert.ui" line="333"/>
        <source>sub</source>
        <translation>sub</translation>
    </message>
    <message>
        <location filename="../gui/ui/frmconvert.ui" line="338"/>
        <source>txt</source>
        <translation>txt</translation>
    </message>
    <message>
        <location filename="../gui/ui/frmconvert.ui" line="353"/>
        <source>Change FPS (frames/second) of subtitles</source>
        <translation>Zmień FPS (klatki/sekundę) napisów</translation>
    </message>
    <message>
        <location filename="../gui/ui/frmconvert.ui" line="376"/>
        <source>from</source>
        <translation>z</translation>
    </message>
    <message>
        <location filename="../gui/ui/frmconvert.ui" line="445"/>
        <source>to</source>
        <translation>na</translation>
    </message>
    <message>
        <location filename="../gui/ui/frmconvert.ui" line="535"/>
        <source>Shift subtitles</source>
        <translation>Przesuń napisy</translation>
    </message>
    <message>
        <location filename="../gui/ui/frmconvert.ui" line="579"/>
        <source> seconds</source>
        <translation> sekund</translation>
    </message>
    <message>
        <location filename="../gui/ui/frmconvert.ui" line="597"/>
        <source>Target subtitles file name:</source>
        <translation>Docelowa nazwa pliku:</translation>
    </message>
    <message>
        <location filename="../gui/ui/frmconvert.ui" line="657"/>
        <source>Convert</source>
        <translation>Konwertuj</translation>
    </message>
    <message>
        <location filename="../gui/src/forms/frmconvert.cpp" line="83"/>
        <source>Choose a subtitles file</source>
        <translation>Wybierz plik z napisami</translation>
    </message>
    <message>
        <location filename="../gui/src/forms/frmconvert.cpp" line="100"/>
        <source>incorrect</source>
        <translation>niepoprawny</translation>
    </message>
    <message>
        <location filename="../gui/src/forms/frmconvert.cpp" line="145"/>
        <source>Default (%1)</source>
        <translation>Domyślne (%1)</translation>
    </message>
    <message>
        <location filename="../gui/src/forms/frmconvert.cpp" line="174"/>
        <location filename="../gui/src/forms/frmconvert.cpp" line="196"/>
        <source>Select a video file</source>
        <translation>Wybierz plik z filmem</translation>
    </message>
    <message>
        <location filename="../gui/src/forms/frmconvert.cpp" line="248"/>
        <source>Subtitles converted</source>
        <translation>Przekonwertowano napisy</translation>
    </message>
    <message>
        <location filename="../gui/src/forms/frmconvert.cpp" line="249"/>
        <source>Changed subtitles format from &apos;%1&apos; to &apos;%2&apos;</source>
        <translation>Poprawnie zmieniono format napisów z &apos;%1&apos; na &apos;%2&apos;</translation>
    </message>
    <message>
        <location filename="../gui/src/forms/frmconvert.cpp" line="253"/>
        <source>An error occured while converting subtitles!</source>
        <translation>Wystąpił problem podczas zamiany formatu napisów!</translation>
    </message>
    <message>
        <location filename="../gui/src/forms/frmconvert.cpp" line="252"/>
        <source>Could not change subtitle format!</source>
        <translation>Problem podczas konwertowania napisów!</translation>
    </message>
</context>
<context>
    <name>frmListSubtitles</name>
    <message>
        <location filename="../gui/ui/frmlistsubtitles.ui" line="20"/>
        <source>QNapi - Select subtitles from the list</source>
        <translation>QNapi - wybierz napisy z listy</translation>
    </message>
    <message>
        <location filename="../gui/ui/frmlistsubtitles.ui" line="43"/>
        <source>&lt;html&gt;&lt;head/&gt;&lt;body&gt;&lt;p&gt;Select the subtitles which you want to download for video file&lt;/p&gt;&lt;p&gt;&lt;span style=&quot; font-weight:600;&quot;&gt;film.avi&lt;/span&gt;&lt;/p&gt;&lt;/body&gt;&lt;/html&gt;</source>
        <translation>&lt;html&gt;&lt;head/&gt;&lt;body&gt;&lt;p&gt;Z poniższej listy wybierz napisy, które chcesz dopasować do pliku&lt;/p&gt;&lt;p&gt;&lt;span style=&quot; font-weight:600;&quot;&gt;film.avi&lt;/span&gt;&lt;/p&gt;&lt;/body&gt;&lt;/html&gt;</translation>
    </message>
    <message>
        <location filename="../gui/ui/frmlistsubtitles.ui" line="53"/>
        <source>Download subtitles</source>
        <translation>Pobierz napisy</translation>
    </message>
    <message>
        <location filename="../gui/ui/frmlistsubtitles.ui" line="60"/>
        <source>Do not download subtitles for this video</source>
        <translation>Nie pobieraj napisów dla tego pliku z filmem</translation>
    </message>
    <message>
        <location filename="../gui/ui/frmlistsubtitles.ui" line="67"/>
        <source>Subtitles &lt;b&gt;likely matching&lt;/b&gt; with your video highlighted with green.</source>
        <translation>Na zielono podświetlono napisy &lt;b&gt;prawdopodobnie&lt;/b&gt; pasujące do Twojego filmu.</translation>
    </message>
    <message>
        <location filename="../gui/ui/frmlistsubtitles.ui" line="77"/>
        <source>Subtitles &lt;b&gt;likely not matching&lt;/b&gt; with your video highlighted with red.</source>
        <translation>Na czerwono podświetlono napisy &lt;b&gt;prawdopodobnie&lt;/b&gt; niepasujące do Twojego filmu.</translation>
    </message>
    <message>
        <location filename="../gui/src/forms/frmlistsubtitles.cpp" line="37"/>
        <source>Select the subtitles which you want to download for video file&lt;br&gt;&lt;br&gt;&lt;b&gt;%1&lt;/b&gt;</source>
        <translation>Z poniższej listy wybierz napisy, które chcesz dopasować do pliku&lt;br&gt;&lt;br&gt;&lt;b&gt;%1&lt;/b&gt;</translation>
    </message>
    <message>
        <location filename="../gui/src/forms/frmlistsubtitles.cpp" line="90"/>
        <source>No subtitle selected</source>
        <translation>Nie wybrano napisów</translation>
    </message>
    <message>
        <location filename="../gui/src/forms/frmlistsubtitles.cpp" line="91"/>
        <source>You need to select a subtitle from the list!</source>
        <translation>Musisz wybrać napisy z listy!</translation>
    </message>
</context>
<context>
    <name>frmNapiProjektConfig</name>
    <message>
        <location filename="../gui/ui/napiprojekt/frmnapiprojektconfig.ui" line="26"/>
        <source>QNapi - NapiProjekt configuration</source>
        <translation>QNapi - Konfiguracja NapiProjekt</translation>
    </message>
    <message>
        <location filename="../gui/ui/napiprojekt/frmnapiprojektconfig.ui" line="32"/>
        <source>Login:</source>
        <translation>Nazwa użytkownika:</translation>
    </message>
    <message>
        <location filename="../gui/ui/napiprojekt/frmnapiprojektconfig.ui" line="46"/>
        <source>Password:</source>
        <translation>Hasło:</translation>
    </message>
    <message>
        <location filename="../gui/ui/napiprojekt/frmnapiprojektconfig.ui" line="63"/>
        <source>Use the fast API as the official NapiProjekt client</source>
        <translation>Używaj szybkiego API jako oficjalny klient NapiProjektu</translation>
    </message>
    <message>
        <location filename="../gui/ui/napiprojekt/frmnapiprojektconfig.ui" line="66"/>
        <source>NapiProjekt&apos;s fast API is not rate-limited, but it refuses QNapi by name. With this option QNapi identifies itself as the official NapiProjekt client to use it. Off: QNapi uses the slower, rate-limited endpoint.</source>
        <translation>Szybkie API NapiProjektu nie ma limitu zapytań, ale odrzuca QNapi po nazwie. Z tą opcją QNapi przedstawia się jako oficjalny klient NapiProjektu, aby z niego korzystać. Wyłączona: QNapi używa wolniejszego adresu z limitem zapytań.</translation>
    </message>
    <message>
        <location filename="../gui/ui/napiprojekt/frmnapiprojektconfig.ui" line="86"/>
        <source>Create an account</source>
        <translation>Załóż konto</translation>
    </message>
    <message>
        <location filename="../gui/ui/napiprojekt/frmnapiprojektconfig.ui" line="106"/>
        <source>Cancel</source>
        <translation>Anuluj</translation>
    </message>
    <message>
        <location filename="../gui/ui/napiprojekt/frmnapiprojektconfig.ui" line="113"/>
        <source>Save</source>
        <translation>Zapisz</translation>
    </message>
</context>
<context>
    <name>frmNapisy24Config</name>
    <message>
        <location filename="../gui/ui/napisy24/frmnapisy24config.ui" line="26"/>
        <source>QNapi - Napisy24 configuration</source>
        <translation>QNapi - Konfiguracja Napisy24</translation>
    </message>
    <message>
        <location filename="../gui/ui/napisy24/frmnapisy24config.ui" line="32"/>
        <source>Login:</source>
        <translation>Nazwa użytkownika:</translation>
    </message>
    <message>
        <location filename="../gui/ui/napisy24/frmnapisy24config.ui" line="46"/>
        <source>Password:</source>
        <translation>Hasło:</translation>
    </message>
    <message>
        <location filename="../gui/ui/napisy24/frmnapisy24config.ui" line="76"/>
        <source>Create an account</source>
        <translation>Załóż konto</translation>
    </message>
    <message>
        <location filename="../gui/ui/napisy24/frmnapisy24config.ui" line="96"/>
        <source>Cancel</source>
        <translation>Anuluj</translation>
    </message>
    <message>
        <location filename="../gui/ui/napisy24/frmnapisy24config.ui" line="103"/>
        <source>Save</source>
        <translation>Zapisz</translation>
    </message>
</context>
<context>
    <name>frmOpenSubtitlesConfig</name>
    <message>
        <location filename="../gui/ui/opensubtitles/frmopensubtitlesconfig.ui" line="26"/>
        <source>QNapi - OpenSubtitles configuration</source>
        <translation>QNapi - Konfiguracja OpenSubtitles</translation>
    </message>
    <message>
        <location filename="../gui/ui/opensubtitles/frmopensubtitlesconfig.ui" line="32"/>
        <source>Login:</source>
        <translation>Nazwa użytkownika:</translation>
    </message>
    <message>
        <location filename="../gui/ui/opensubtitles/frmopensubtitlesconfig.ui" line="46"/>
        <source>Password:</source>
        <translation>Hasło:</translation>
    </message>
    <message>
        <location filename="../gui/ui/opensubtitles/frmopensubtitlesconfig.ui" line="76"/>
        <source>Create an account</source>
        <translation>Załóż konto</translation>
    </message>
    <message>
        <location filename="../gui/ui/opensubtitles/frmopensubtitlesconfig.ui" line="96"/>
        <source>Cancel</source>
        <translation>Anuluj</translation>
    </message>
    <message>
        <location filename="../gui/ui/opensubtitles/frmopensubtitlesconfig.ui" line="103"/>
        <source>Save</source>
        <translation>Zapisz</translation>
    </message>
</context>
<context>
    <name>frmOptions</name>
    <message>
        <location filename="../gui/ui/frmoptions.ui" line="14"/>
        <source>QNapi - Settings</source>
        <translation>QNapi - Ustawienia</translation>
    </message>
    <message>
        <location filename="../gui/ui/frmoptions.ui" line="28"/>
        <source>General</source>
        <translation>Ogólne</translation>
    </message>
    <message>
        <location filename="../gui/ui/frmoptions.ui" line="244"/>
        <source>Do not create a copy of existing subtitles. Overwrite them without confirmation.</source>
        <translation>Nie twórz kopii istniejących napisów. Nadpisuj je bez pytania.</translation>
    </message>
    <message>
        <location filename="../gui/ui/frmoptions.ui" line="216"/>
        <source>Preferred subtitles language:</source>
        <translation>Preferowany język napisów:</translation>
    </message>
    <message>
        <location filename="../gui/ui/frmoptions.ui" line="192"/>
        <source>Show icon in the system tray (requires application restart)</source>
        <translation>Pokazuj ikonę w doku systemowym (wymaga ponownego uruchomienia aplikacji)</translation>
    </message>
    <message>
        <location filename="../gui/ui/frmoptions.ui" line="123"/>
        <source>Subtitles file permissions: </source>
        <translation>Zmień uprawnienia do pliku z napisami na: </translation>
    </message>
    <message>
        <location filename="../gui/ui/frmoptions.ui" line="116"/>
        <source>Alternative subtitles language:</source>
        <translation>Zapasowy język napisów:</translation>
    </message>
    <message>
        <location filename="../gui/ui/frmoptions.ui" line="96"/>
        <source>Path to temporary directory:</source>
        <translation>Ścieżka do katalogu tymczasowego:</translation>
    </message>
    <message>
        <location filename="../gui/ui/frmoptions.ui" line="89"/>
        <source>Path to 7z executable:</source>
        <translation>Ścieżka do programu 7z:</translation>
    </message>
    <message>
        <location filename="../gui/ui/frmoptions.ui" line="76"/>
        <source>Do not show any windows while retrieving subtitles in command-line</source>
        <translation>Nie pokazuje żadnych okien przy pobieraniu napisów z menu kontekstowego w menedżerze plików lub przy podaniu plików jako argumenty wiersza poleceń</translation>
    </message>
    <message>
        <location filename="../gui/ui/frmoptions.ui" line="34"/>
        <source>Language code in file names:</source>
        <translation>Kod języka w nazwach plików:</translation>
    </message>
    <message>
        <location filename="../gui/ui/frmoptions.ui" line="41"/>
        <source>Media servers such as Jellyfin, Plex and Emby find external subtitles by the language code in their name (Movie.pol.srt)</source>
        <translation>Serwery multimediów, takie jak Jellyfin, Plex i Emby, rozpoznają zewnętrzne napisy po kodzie języka w nazwie (Film.pol.srt)</translation>
    </message>
    <message>
        <location filename="../gui/ui/frmoptions.ui" line="45"/>
        <source>None (Movie.srt)</source>
        <translation>Brak (Film.srt)</translation>
    </message>
    <message>
        <location filename="../gui/ui/frmoptions.ui" line="50"/>
        <source>Two letters (Movie.pl.srt)</source>
        <translation>Dwie litery (Film.pl.srt)</translation>
    </message>
    <message>
        <location filename="../gui/ui/frmoptions.ui" line="55"/>
        <source>Three letters (Movie.pol.srt)</source>
        <translation>Trzy litery (Film.pol.srt)</translation>
    </message>
    <message>
        <location filename="../gui/ui/frmoptions.ui" line="82"/>
        <source>Silent batch mode</source>
        <translation>Cichy tryb wsadowy</translation>
    </message>
    <message>
        <location filename="../gui/ui/frmoptions.ui" line="251"/>
        <source>User interface language:</source>
        <translation>Język interfejsu użytkownika:</translation>
    </message>
    <message>
        <location filename="../gui/ui/frmoptions.ui" line="259"/>
        <location filename="../gui/src/forms/frmoptions.cpp" line="452"/>
        <source>Auto-detected based on system language (%1)</source>
        <translation>Wykryty automatycznie z ustawień systemu (%1)</translation>
    </message>
    <message>
        <location filename="../gui/ui/frmoptions.ui" line="264"/>
        <source>English</source>
        <comment>en</comment>
        <translation>Angielski</translation>
    </message>
    <message>
        <location filename="../gui/ui/frmoptions.ui" line="273"/>
        <source>Italian</source>
        <comment>it</comment>
        <translation>Włoski</translation>
    </message>
    <message>
        <location filename="../gui/ui/frmoptions.ui" line="282"/>
        <source>Polish</source>
        <comment>pl</comment>
        <translation>Polski</translation>
    </message>
    <message>
        <location filename="../gui/ui/frmoptions.ui" line="302"/>
        <source>UI Language change will be applied on next application launch</source>
        <translation>Zmiana języka nastąpi przy następnym uruchomieniu aplikacji</translation>
    </message>
    <message>
        <location filename="../gui/ui/frmoptions.ui" line="310"/>
        <source>Subtitles download engines</source>
        <translation>Moduły pobierania napisów</translation>
    </message>
    <message>
        <location filename="../gui/ui/frmoptions.ui" line="316"/>
        <source>Files processed at the same time:</source>
        <translation>Liczba plików przetwarzanych jednocześnie:</translation>
    </message>
    <message>
        <location filename="../gui/ui/frmoptions.ui" line="319"/>
        <source>The services block requests that arrive one after another but let several at once through, so a whole season is much faster in parallel. Only used when no list of subtitles has to be shown, or when it may be shown for several files at once.</source>
        <translation>Serwisy blokują zapytania wysyłane jedno po drugim, ale przepuszczają kilka naraz, więc cały sezon pobiera się równolegle znacznie szybciej. Używane, gdy nie trzeba pokazywać listy napisów albo gdy można ją pokazać dla kilku plików naraz.</translation>
    </message>
    <message>
        <location filename="../gui/ui/frmoptions.ui" line="336"/>
        <source>Available engines to download subtitles from:</source>
        <translation>Dostępne moduły pobierania napisów:</translation>
    </message>
    <message>
        <location filename="../gui/ui/frmoptions.ui" line="349"/>
        <source>Subtitles download policy:</source>
        <translation>Polityka pobierania napisów:</translation>
    </message>
    <message>
        <location filename="../gui/ui/frmoptions.ui" line="363"/>
        <source>Always show a list of available subtitles</source>
        <translation>Zawsze pokazuj listę dostępnych napisów</translation>
    </message>
    <message>
        <location filename="../gui/ui/frmoptions.ui" line="368"/>
        <source>Show a list of available subtitles only if needed</source>
        <translation>Pokazuj listę dostępnych napisów tylko wtedy, gdy potrzeba</translation>
    </message>
    <message>
        <location filename="../gui/ui/frmoptions.ui" line="373"/>
        <source>Always obtain first matching subtitles and never show a list</source>
        <translation>Zawsze pobieraj pierwsze pasujące napisy i nigdy nie pokazuj listy</translation>
    </message>
    <message>
        <location filename="../gui/ui/frmoptions.ui" line="406"/>
        <source>Move up</source>
        <translation>Przesuń do góry</translation>
    </message>
    <message>
        <location filename="../gui/ui/frmoptions.ui" line="423"/>
        <source>Subtitles search policy:</source>
        <translation>Wyszukiwanie napisów:</translation>
    </message>
    <message>
        <location filename="../gui/ui/frmoptions.ui" line="433"/>
        <source>Move down  </source>
        <translation>Przesuń na dół  </translation>
    </message>
    <message>
        <location filename="../gui/ui/frmoptions.ui" line="447"/>
        <source>Information</source>
        <translation>Informacje</translation>
    </message>
    <message>
        <location filename="../gui/ui/frmoptions.ui" line="461"/>
        <source>Configure</source>
        <translation>Konfiguruj</translation>
    </message>
    <message>
        <location filename="../gui/ui/frmoptions.ui" line="476"/>
        <source>Stop the search after finding a matching subtitles (faster)</source>
        <translation>Przerwij wyszukiwanie po znalezieniu pasujących napisów (szybciej)</translation>
    </message>
    <message>
        <location filename="../gui/ui/frmoptions.ui" line="481"/>
        <source>Search all available subtitles (slower)</source>
        <translation>Wyszukaj wszystkie dostępne napisy (wolniej)</translation>
    </message>
    <message>
        <location filename="../gui/ui/frmoptions.ui" line="486"/>
        <source>Find all subtitles, also in alternative language (slowest)</source>
        <translation>Wyszukaj wszystkie napisy, także w zapasowym języku (najwolniej)</translation>
    </message>
    <message>
        <location filename="../gui/ui/frmoptions.ui" line="495"/>
        <source>Subtitles post-processing</source>
        <translation>Przetwarzanie napisów</translation>
    </message>
    <message>
        <location filename="../gui/ui/frmoptions.ui" line="501"/>
        <source>Enable post-processing for downloaded subtitles</source>
        <translation>Włącz przetwarzanie pobranych napisów</translation>
    </message>
    <message>
        <location filename="../gui/ui/frmoptions.ui" line="515"/>
        <source>Character encoding:</source>
        <translation>Kodowanie znaków:</translation>
    </message>
    <message>
        <location filename="../gui/ui/frmoptions.ui" line="529"/>
        <location filename="../gui/ui/frmoptions.ui" line="740"/>
        <source>Keep the original</source>
        <translation>Pozostaw oryginalne</translation>
    </message>
    <message>
        <location filename="../gui/ui/frmoptions.ui" line="534"/>
        <source>Convert (settings below)</source>
        <translation>Konwertuj (ustawienia poniżej)</translation>
    </message>
    <message>
        <location filename="../gui/ui/frmoptions.ui" line="539"/>
        <source>Replace diacritic characters with their ASCII equivalents</source>
        <translation>Zamień znaki diakrytyczne na ich odpowiedniki ASCII</translation>
    </message>
    <message>
        <location filename="../gui/ui/frmoptions.ui" line="576"/>
        <source>From:</source>
        <translation>Z:</translation>
    </message>
    <message>
        <location filename="../gui/ui/frmoptions.ui" line="629"/>
        <source>To:</source>
        <translation>Na:</translation>
    </message>
    <message>
        <location filename="../gui/ui/frmoptions.ui" line="699"/>
        <source>Show all available character encodings</source>
        <translation>Pokaż wszystkie dostępne kodowania znaków</translation>
    </message>
    <message>
        <location filename="../gui/ui/frmoptions.ui" line="726"/>
        <source>Subtitles format:</source>
        <translation>Format napisów:</translation>
    </message>
    <message>
        <location filename="../gui/ui/frmoptions.ui" line="748"/>
        <source>Subtitles extension:</source>
        <translation>Rozszerzenie napisów:</translation>
    </message>
    <message>
        <location filename="../gui/ui/frmoptions.ui" line="762"/>
        <location filename="../gui/src/forms/frmoptions.cpp" line="301"/>
        <source>Default</source>
        <translation>Domyślne</translation>
    </message>
    <message>
        <location filename="../gui/ui/frmoptions.ui" line="788"/>
        <source>Do not add information about QNapi while converting between formats</source>
        <translation>Nie dodawaj informacji o QNapi przy konwersji pomiędzy formatami</translation>
    </message>
    <message>
        <location filename="../gui/ui/frmoptions.ui" line="798"/>
        <source>Attempt to automatically detect encoding</source>
        <translation>Próbuj automatycznie wykrywać kodowanie źródłowe</translation>
    </message>
    <message>
        <location filename="../gui/ui/frmoptions.ui" line="808"/>
        <source>Remove subtitles lines that contain the following words:</source>
        <translation>Usuń z pliku z napisami linie, które zawierają następujące słowa:</translation>
    </message>
    <message>
        <location filename="../gui/ui/frmoptions.ui" line="827"/>
        <source>AI subtitles</source>
        <translation>Napisy AI</translation>
    </message>
    <message>
        <location filename="../gui/ui/frmoptions.ui" line="833"/>
        <source>Generate subtitles with AI when no service has them</source>
        <translation>Generuj napisy przy pomocy AI, gdy żaden serwis ich nie ma</translation>
    </message>
    <message>
        <location filename="../gui/ui/frmoptions.ui" line="840"/>
        <source>The audio is transcribed with whisper.cpp and translated into the preferred subtitles language by a local language model (llama.cpp). It takes several minutes per file and works best on a graphics card. Empty paths mean the default files next to QNapi.</source>
        <translation>Dźwięk jest przepisywany przez whisper.cpp i tłumaczony na preferowany język napisów przez lokalny model językowy (llama.cpp). Trwa to kilka minut na plik i działa najlepiej na karcie graficznej. Puste ścieżki oznaczają domyślne pliki obok QNapi.</translation>
    </message>
    <message>
        <location filename="../gui/ui/frmoptions.ui" line="850"/>
        <source>ffmpeg:</source>
        <translation>ffmpeg:</translation>
    </message>
    <message>
        <location filename="../gui/ui/frmoptions.ui" line="860"/>
        <location filename="../gui/ui/frmoptions.ui" line="883"/>
        <location filename="../gui/ui/frmoptions.ui" line="906"/>
        <location filename="../gui/ui/frmoptions.ui" line="929"/>
        <location filename="../gui/ui/frmoptions.ui" line="952"/>
        <source>...</source>
        <translation>...</translation>
    </message>
    <message>
        <location filename="../gui/ui/frmoptions.ui" line="873"/>
        <source>whisper-cli:</source>
        <translation>whisper-cli:</translation>
    </message>
    <message>
        <location filename="../gui/ui/frmoptions.ui" line="896"/>
        <source>Speech recognition model:</source>
        <translation>Model rozpoznawania mowy:</translation>
    </message>
    <message>
        <location filename="../gui/ui/frmoptions.ui" line="919"/>
        <source>llama-server:</source>
        <translation>llama-server:</translation>
    </message>
    <message>
        <location filename="../gui/ui/frmoptions.ui" line="942"/>
        <source>Translation model (GGUF):</source>
        <translation>Model tłumaczący (GGUF):</translation>
    </message>
    <message>
        <location filename="../gui/ui/frmoptions.ui" line="965"/>
        <source>Language spoken in the movie:</source>
        <translation>Język mówiony w filmie:</translation>
    </message>
    <message>
        <location filename="../gui/ui/frmoptions.ui" line="975"/>
        <source>Extra llama-server options:</source>
        <translation>Dodatkowe opcje llama-server:</translation>
    </message>
    <message>
        <location filename="../gui/ui/frmoptions.ui" line="982"/>
        <source>Appended to llama-server&apos;s command line and override its defaults, e.g. -ngl 20 for a model larger than the graphics card&apos;s memory, or --jinja for a model whose chat template is not recognised</source>
        <translation>Dopisywane do wiersza poleceń llama-server, zastępują jego ustawienia domyślne, np. -ngl 20 dla modelu większego niż pamięć karty graficznej albo --jinja dla modelu, którego szablon czatu nie jest rozpoznawany</translation>
    </message>
    <message>
        <location filename="../gui/ui/frmoptions.ui" line="989"/>
        <source>Lines per translation request:</source>
        <translation>Linii na jedno zapytanie tłumaczenia:</translation>
    </message>
    <message>
        <location filename="../gui/ui/frmoptions.ui" line="1006"/>
        <source>Translation guidelines ({source} and {target} stand for the language names):</source>
        <translation>Wskazówki dla tłumaczenia ({source} i {target} oznaczają nazwy języków):</translation>
    </message>
    <message>
        <location filename="../gui/ui/frmoptions.ui" line="1016"/>
        <source>Restore default</source>
        <translation>Przywróć domyślne</translation>
    </message>
    <message>
        <location filename="../gui/ui/frmoptions.ui" line="1042"/>
        <source>Restore defaults</source>
        <translation>Przywróć domyślne</translation>
    </message>
    <message>
        <location filename="../gui/ui/frmoptions.ui" line="1062"/>
        <source>Cancel</source>
        <translation>Anuluj</translation>
    </message>
    <message>
        <location filename="../gui/ui/frmoptions.ui" line="1069"/>
        <source>Save</source>
        <translation>Zapisz</translation>
    </message>
    <message>
        <location filename="../gui/src/forms/frmoptions.cpp" line="73"/>
        <source>Detect automatically</source>
        <translation>Wykryj automatycznie</translation>
    </message>
    <message>
        <location filename="../gui/src/forms/frmoptions.cpp" line="99"/>
        <source>Select a file</source>
        <translation>Wybierz plik</translation>
    </message>
    <message>
        <location filename="../gui/src/forms/frmoptions.cpp" line="173"/>
        <source>Specify the path for 7z executable</source>
        <translation>Wskaż ścieżkę do programu 7z</translation>
    </message>
    <message>
        <location filename="../gui/src/forms/frmoptions.cpp" line="178"/>
        <source>Invalid path</source>
        <translation>Niepoprawna ścieżka</translation>
    </message>
    <message>
        <location filename="../gui/src/forms/frmoptions.cpp" line="179"/>
        <source>Defined path to 7z executable is invalid. If you can not locate 7z executable, try installing p7zip-full package.</source>
        <translation>&quot;Wskazana przez Ciebie ścieżka do programu 7z jest niepoprawna. Jeśli nie możesz odnaleźć programu 7z, spróbuj zainstalować pakiet p7zip-full.</translation>
    </message>
    <message>
        <location filename="../gui/src/forms/frmoptions.cpp" line="193"/>
        <source>Specify temporary directory</source>
        <translation>Wskaż katalog tymczasowy</translation>
    </message>
    <message>
        <location filename="../gui/src/forms/frmoptions.cpp" line="227"/>
        <source>Warning</source>
        <translation>Ostrzeżenie</translation>
    </message>
    <message>
        <location filename="../gui/src/forms/frmoptions.cpp" line="228"/>
        <source>At least one subtitles engine must remain active!</source>
        <translation>Przynajmniej jeden moduł pobierania musi pozostać aktywny!</translation>
    </message>
    <message>
        <location filename="../gui/src/forms/frmoptions.cpp" line="295"/>
        <source>%1 subtitles engine info</source>
        <translation>Informacje o module pobierania napisów %1</translation>
    </message>
    <message>
        <location filename="../gui/src/forms/frmoptions.cpp" line="307"/>
        <source>Default (%1)</source>
        <translation>Domyślne (%1)</translation>
    </message>
    <message>
        <location filename="../gui/src/forms/frmoptions.cpp" line="607"/>
        <source>All tools and models were found.</source>
        <translation>Znaleziono wszystkie narzędzia i modele.</translation>
    </message>
    <message>
        <location filename="../gui/src/forms/frmoptions.cpp" line="616"/>
        <source>Not found:</source>
        <translation>Nie znaleziono:</translation>
    </message>
</context>
<context>
    <name>frmProgress</name>
    <message>
        <location filename="../gui/ui/frmprogress.ui" line="29"/>
        <source>QNapi - Downloading subtitles...</source>
        <translation>QNapi - Pobieranie napisów...</translation>
    </message>
    <message>
        <location filename="../gui/ui/frmprogress.ui" line="76"/>
        <source>Downloading the subtitles...</source>
        <translation>Pobieranie napisów do filmu...</translation>
    </message>
    <message>
        <location filename="../gui/ui/frmprogress.ui" line="101"/>
        <source>Cancel</source>
        <translation>Anuluj</translation>
    </message>
    <message>
        <location filename="../gui/src/forms/frmprogress.cpp" line="93"/>
        <source>Can not find p7zip!</source>
        <translation>Brak programu p7zip!</translation>
    </message>
    <message>
        <location filename="../gui/src/forms/frmprogress.cpp" line="94"/>
        <source>The path to the program p7zip is incorrect!</source>
        <translation>Ścieżka do programu p7zip jest nieprawidłowa!</translation>
    </message>
    <message>
        <location filename="../gui/src/forms/frmprogress.cpp" line="100"/>
        <source>Invalid temporary directory!</source>
        <translation>Nieprawidłowy katalog tymczasowy!</translation>
    </message>
    <message>
        <location filename="../gui/src/forms/frmprogress.cpp" line="101"/>
        <source>Unable to write to the temporary directory! Check your settings.</source>
        <translation>Nie można pisać do katalogu tymczasowego! Sprawdź swoje ustawienia.</translation>
    </message>
    <message>
        <location filename="../gui/src/forms/frmprogress.cpp" line="107"/>
        <source>No files!</source>
        <translation>Brak plików!</translation>
    </message>
    <message>
        <location filename="../gui/src/forms/frmprogress.cpp" line="108"/>
        <source>Can&apos;t download subtitles as no movie files specified!</source>
        <translation>Nie wskazano filmów do pobrania napisów!</translation>
    </message>
    <message>
        <location filename="../gui/src/forms/frmprogress.cpp" line="142"/>
        <source>QNapi - generating subtitles with AI...</source>
        <translation>QNapi - generowanie napisów przy pomocy AI...</translation>
    </message>
    <message>
        <location filename="../gui/src/forms/frmprogress.cpp" line="143"/>
        <source>QNapi - downloading subtitles (%1/%2)</source>
        <translation>QNapi - pobieranie napisów (%1/%2)</translation>
    </message>
    <message>
        <location filename="../gui/src/forms/frmprogress.cpp" line="146"/>
        <source>QNapi - downloading subtitles...</source>
        <translation>QNapi - pobieranie napisów...</translation>
    </message>
    <message>
        <location filename="../gui/src/forms/frmprogress.cpp" line="179"/>
        <source>Critical error!</source>
        <translation>Błąd krytyczny!</translation>
    </message>
    <message>
        <location filename="../gui/src/forms/frmprogress.cpp" line="186"/>
        <source>AI subtitles</source>
        <translation>Napisy AI</translation>
    </message>
    <message>
        <location filename="../gui/src/forms/frmprogress.cpp" line="206"/>
        <source>QNapi</source>
        <translation>QNapi</translation>
    </message>
    <message>
        <location filename="../gui/src/forms/frmprogress.cpp" line="207"/>
        <source>Do you want to cancel subtitles downloading?</source>
        <translation>Czy chcesz przerwać pobieranie napisów?</translation>
    </message>
    <message>
        <location filename="../gui/src/forms/frmprogress.cpp" line="212"/>
        <source>Finishing the tasks...</source>
        <translation>Kończenie zadań...</translation>
    </message>
</context>
<context>
    <name>frmScan</name>
    <message>
        <location filename="../gui/ui/frmscan.ui" line="20"/>
        <source>QNapi - Directory scanning</source>
        <translation>QNapi - Skanowanie katalogów</translation>
    </message>
    <message>
        <location filename="../gui/ui/frmscan.ui" line="30"/>
        <source>Select directory to search for video files:</source>
        <translation>Wskaż katalog do przeszukania:</translation>
    </message>
    <message>
        <location filename="../gui/ui/frmscan.ui" line="63"/>
        <location filename="../gui/src/forms/frmscan.cpp" line="172"/>
        <source>Scan</source>
        <translation>Skanuj</translation>
    </message>
    <message>
        <location filename="../gui/ui/frmscan.ui" line="83"/>
        <source>Filter file extensions:</source>
        <translation>Filtr rozszerzeń plików:</translation>
    </message>
    <message>
        <location filename="../gui/ui/frmscan.ui" line="94"/>
        <source>*.avi *.asf *.divx *.mkv *.mov *.mp4 *.mpeg *.mpg *.ogm *.rm *.rmvb *.wmv</source>
        <translation>*.avi *.asf *.divx *.mkv *.mov *.mp4 *.mpeg *.mpg *.ogm *.rm *.rmvb *.wmv</translation>
    </message>
    <message>
        <location filename="../gui/ui/frmscan.ui" line="99"/>
        <source>*.*</source>
        <translation>*.*</translation>
    </message>
    <message>
        <location filename="../gui/ui/frmscan.ui" line="114"/>
        <source>Skip files whose names contain:</source>
        <translation>Pomiń pliki zawierające w nazwie:</translation>
    </message>
    <message>
        <location filename="../gui/ui/frmscan.ui" line="121"/>
        <source>EN dubbing</source>
        <translation>PL dubbing</translation>
    </message>
    <message>
        <location filename="../gui/ui/frmscan.ui" line="130"/>
        <source>Skip video files which already have subtitles</source>
        <translation>Podczas wyszukiwania pomijaj pliki, do których istnieją już napisy</translation>
    </message>
    <message>
        <location filename="../gui/ui/frmscan.ui" line="142"/>
        <source>Found video files:</source>
        <translation>Znalezione pliki z filmami:</translation>
    </message>
    <message>
        <location filename="../gui/ui/frmscan.ui" line="180"/>
        <source>Select all</source>
        <translation>Zaznacz wszystko</translation>
    </message>
    <message>
        <location filename="../gui/ui/frmscan.ui" line="187"/>
        <source>Uncheck all</source>
        <translation>Odznacz wszystko</translation>
    </message>
    <message>
        <location filename="../gui/ui/frmscan.ui" line="194"/>
        <source>Invert selection</source>
        <translation>Odwróć zaznaczenie</translation>
    </message>
    <message>
        <location filename="../gui/ui/frmscan.ui" line="214"/>
        <location filename="../gui/src/forms/frmscan.cpp" line="128"/>
        <source>Cancel</source>
        <translation>Anuluj</translation>
    </message>
    <message>
        <location filename="../gui/ui/frmscan.ui" line="224"/>
        <source>Download subtitles</source>
        <translation>Pobierz napisy</translation>
    </message>
    <message>
        <location filename="../gui/ui/frmscan.ui" line="235"/>
        <source>Follow symbolic links</source>
        <translation>Podążaj za dowiązaniami symbolicznymi</translation>
    </message>
    <message>
        <location filename="../gui/src/forms/frmscan.cpp" line="108"/>
        <source>Select the folder to scan</source>
        <translation>Wskaż katalog do skanowania</translation>
    </message>
    <message>
        <location filename="../gui/src/forms/frmscan.cpp" line="129"/>
        <source>Scanning directories...</source>
        <translation>Skanowanie katalogów...</translation>
    </message>
    <message>
        <location filename="../gui/src/forms/frmscan.cpp" line="146"/>
        <source>Interrupting directory scanning...</source>
        <translation>Przerywanie skanowania katalogów...</translation>
    </message>
    <message>
        <location filename="../gui/src/forms/frmscan.cpp" line="166"/>
        <source>Scanning directory &lt;b&gt;%1&lt;/b&gt;...</source>
        <translation>Skanowanie katalogu &lt;b&gt;%1&lt;/b&gt;...</translation>
    </message>
    <message>
        <location filename="../gui/src/forms/frmscan.cpp" line="175"/>
        <source>Select the videos you wish to download subtitles for.</source>
        <translation>Zaznacz filmy, do których chcesz pobrać napisy.</translation>
    </message>
    <message>
        <location filename="../gui/src/forms/frmscan.cpp" line="176"/>
        <source>No video files found.</source>
        <translation>Nie znaleziono plików z filmami.</translation>
    </message>
</context>
<context>
    <name>frmSummary</name>
    <message>
        <location filename="../gui/ui/frmsummary.ui" line="29"/>
        <source>QNapi - subtitles download summary</source>
        <translation>QNapi - podsumowanie pobierania napisów</translation>
    </message>
    <message>
        <location filename="../gui/ui/frmsummary.ui" line="45"/>
        <source>Downloaded subtitles for 5 video files</source>
        <translation>Pobrano napisy dla 5 filmów</translation>
    </message>
    <message>
        <location filename="../gui/ui/frmsummary.ui" line="61"/>
        <source>Could not find the subtitles for 3 video files</source>
        <translation>Nie pobrano napisów dla 3 filmów</translation>
    </message>
    <message>
        <location filename="../gui/ui/frmsummary.ui" line="108"/>
        <source>OK</source>
        <translation>OK</translation>
    </message>
    <message>
        <location filename="../gui/src/forms/frmsummary.cpp" line="87"/>
        <source>Subtitles downloaded for %1 %2</source>
        <translation>Pobrano napisy dla %1 %2</translation>
    </message>
    <message>
        <location filename="../gui/src/forms/frmsummary.cpp" line="89"/>
        <location filename="../gui/src/forms/frmsummary.cpp" line="92"/>
        <source>files</source>
        <translation>plików</translation>
    </message>
    <message>
        <location filename="../gui/src/forms/frmsummary.cpp" line="89"/>
        <location filename="../gui/src/forms/frmsummary.cpp" line="92"/>
        <source>file</source>
        <translation>pliku</translation>
    </message>
    <message>
        <location filename="../gui/src/forms/frmsummary.cpp" line="90"/>
        <source>Could not download subtitles for %1 %2</source>
        <translation>Nie pobrano napisów dla %1 %2</translation>
    </message>
</context>
</TS>
