#include "CANBus.h"
#include "ECU.h"

#include <iostream>
#include <limits>

using namespace std;

void printHeader()
{
    cout << "\n============================================================\n";
    cout << "              CAN BUS PROTOCOL SIMULATOR\n";
    cout << "============================================================\n";
}

void printMenu()
{
    cout << "\nCONTROLS\n";
    cout << "  1  Run one simulation cycle\n";
    cout << "  2  Run live dashboard (10 cycles)\n";
    cout << "  3  Show all logged CAN frames\n";
    cout << "  4  Filter frames by CAN ID\n";
    cout << "  5  Show bus statistics\n";
    cout << "  6  Demonstrate bit-by-bit arbitration\n";
    cout << "  7  Show current ECU values\n";
    cout << "  8  Exit\n";
}

void waitForEnter()
{
    cout << "\nPress ENTER to return to the menu...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();
}

int main()
{
    CANBus bus;

    EngineECU engine;
    ABSECU abs;
    DashboardECU dashboard;

    bus.addNode(&engine);
    bus.addNode(&abs);
    bus.addNode(&dashboard);

    int choice = 0;

    while (true)
    {
        printHeader();

        cout << "\nNETWORK STATUS\n";
        cout << "  Nodes connected : " << bus.nodeCount() << "\n";
        cout << "  CAN bitrate     : 500 kbps\n";
        cout << "  Protocol        : Classical CAN (simulated)\n";
        cout << "  Termination     : 120 ohm at each bus end (real CAN)\n";

        printMenu();

        cout << "\nSelect option: ";

        if (!(cin >> choice))
        {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "\nInvalid input. Please enter a number from 1 to 8.\n";
            continue;
        }

        switch (choice)
        {
            case 1:
            {
                cout << "\n============================================================\n";
                cout << "                 SIMULATION CYCLE\n";
                cout << "============================================================\n";

                bus.runCycle(true);
                waitForEnter();
                break;
            }

            case 2:
            {
                cout << "\n============================================================\n";
                cout << "                 LIVE CAN DASHBOARD\n";
                cout << "============================================================\n";

                for (int i = 1; i <= 10; ++i)
                {
                    cout << "\n-------------------- CYCLE " << i
                         << " --------------------\n";
                    bus.runCycle(true);
                }

                waitForEnter();
                break;
            }

            case 3:
            {
                cout << "\n============================================================\n";
                cout << "                 LOGGED CAN FRAMES\n";
                cout << "============================================================\n";

                bus.analyzer().printAll();
                waitForEnter();
                break;
            }

            case 4:
            {
                unsigned int id;

                cout << "\nEnter CAN ID in hexadecimal.\n";
                cout << "Example: 100\n";
                cout << "CAN ID: 0x";

                cin >> hex >> id;
                cin >> dec;

                bus.analyzer().printById(static_cast<uint16_t>(id));
                waitForEnter();
                break;
            }

            case 5:
            {
                cout << "\n============================================================\n";
                cout << "                 BUS STATISTICS\n";
                cout << "============================================================\n";

                bus.analyzer().printStatistics();
                waitForEnter();
                break;
            }

            case 6:
            {
                cout << "\n============================================================\n";
                cout << "              BIT-BY-BIT CAN ARBITRATION\n";
                cout << "============================================================\n";

                bus.demonstrateArbitration();
                waitForEnter();
                break;
            }

            case 7:
            {
                cout << "\n============================================================\n";
                cout << "                 CURRENT ECU VALUES\n";
                cout << "============================================================\n";

                bus.printECUStatus();
                waitForEnter();
                break;
            }

            case 8:
            {
                cout << "\nExiting CAN Bus Simulator...\n";
                cout << "Thank you!\n\n";
                return 0;
            }

            default:
            {
                cout << "\nInvalid option. Please select 1 to 8.\n";
                waitForEnter();
                break;
            }
        }
    }

    return 0;
}
