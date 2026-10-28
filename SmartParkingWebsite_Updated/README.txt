SMART PARKING LOT MANAGEMENT WEBSITE (C BACKEND)

Technologies: C backend + Node.js local web server + HTML/CSS/JavaScript frontend.
The parking operations and data structure logic are in parking.c. Node.js serves the webpage and passes API actions to the compiled C program.

WINDOWS + VS CODE SETUP
1. Install GCC using MSYS2: https://www.msys2.org/
2. Open MSYS2 UCRT64 terminal and install GCC:
   pacman -S --needed mingw-w64-ucrt-x86_64-gcc
3. Add C:\msys64\ucrt64\bin to Windows PATH and restart VS Code.
4. Install Node.js LTS from https://nodejs.org/ (choose the LTS version).
5. Open the SmartParkingWebsite folder in VS Code.
6. Open Terminal > New Terminal and compile the C backend:
   gcc parking.c -o parking.exe
7. Start the local web server:
   node server.js
8. Open your browser at:
   http://localhost:3000

IMPORTANT: Keep the terminal running while using the website. Press Ctrl+C to stop it.

FEATURES
- Live summary cards for total, occupied, available and waiting vehicles
- 12 clickable parking slot cards
- Add vehicle form and duplicate checks
- Vehicle exit action
- FIFO waiting queue; next waiting vehicle gets the freed slot
- Vehicles page with search
- Persistent data stored in parking_data.txt

DSA CONCEPTS
- Array of slots
- Queue (FIFO) for waiting vehicles
- Linear search to find duplicate/parked vehicles
- Structures to store parking records
- File handling for persistence

If `gcc` or `node` is not recognized, install the relevant software and reopen VS Code. If the C program fails to compile, share the exact compiler error.
