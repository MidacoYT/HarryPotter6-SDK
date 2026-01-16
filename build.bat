@echo off
echo Compilation du Harry Potter SDK - Ultimate Edition...
g++ -o HarryPotterSDK.exe main.cpp HarryPotterSDK.cpp -lpsapi
if %ERRORLEVEL% == 0 (
    echo Compilation reussie!
    echo.
    echo Pour lancer le SDK:
    echo 1. Lancez le jeu Harry Potter 6 (hp6.exe)
    echo 2. Executez HarryPotterSDK.exe
    echo.
    echo Controles Ultimate:
    echo - SHIFT: Speedhack continu (maintenir)
    echo - A: Voler vers le haut (maintenir)
    echo - W: SpeedHack unique (0.5 unite selon rotation)
    echo - F1: Sauvegarder position
    echo - F2: Charger position sauvegardee
    echo - B: Blink (teleportation)
    echo - V: Vol diagonal
    echo - G/T/R: Taille (Geant/Minuscule/Normal)
    echo - X: Quitter
    echo.
    echo Test Ultimate: SHIFT + ESPACE = Vol super rapide!
    echo            F1 -> Explorer -> F2 pour retour rapide!
) else (
    echo Erreur de compilation!
)
pause
