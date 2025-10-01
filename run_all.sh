#!/bin/bash
# Skripta koja pokrece server i 4 klijenta za Hellapagos projekat

# Putanje do izvršnih fajlova
SERVER="./server/build/Desktop_Qt_6_7_3-Debug/server"
CLIENT="./hellapagos/build/Desktop_Qt_6_7_3-Debug/hellapagos"

# Pokretanje servera u pozadini
echo "Pokrećem server..."
$SERVER &
SERVER_PID=$!

# Malo čekanje da server startuje
sleep 1

# Pokretanje 4 klijenta
for i in {1..4}; do
    echo "Pokrećem klijenta $i..."
    $CLIENT &
done

# Čekanje da server završi
wait $SERVER_PID
echo "Server je završio. Svi klijenti su zatvoreni."
